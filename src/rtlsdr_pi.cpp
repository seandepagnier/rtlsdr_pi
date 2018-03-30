/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  rtlsdr Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2015 by Sean D'Epagnier                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 */


#include <wx/process.h>

#ifdef __unix__
#include <sys/stat.h>
#endif

#ifdef BUILTIN_RTLAIS
#include "rtl-sdr-misc/rtl_ais.h"
#endif

#include "rtlsdr_pi.h"
#include "rtlsdrDialog.h"
#include "rtlsdrPrefs.h"
#include "icons.h"

static void KillProcess(wxProcess *process)
{
    if(!process)
        return;

    int pid = process->GetPid();
    wxThread::Sleep(25);
    if(wxProcess::Exists(pid))
        wxProcess::Kill(pid);
    wxThread::Sleep(25);
    if(wxProcess::Exists(pid))
        wxProcess::Kill(pid, wxSIGKILL);
}

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new rtlsdr_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}

#ifdef __WIN32__ // on windows the utility is distributed with the plugin
#define PATH() *GetpSharedDataLocation() + _T("plugins\\rtlsdr_pi\\bin\\")
#else
#define PATH() wxString()
#endif

//---------------------------------------------------------------------------------------------------------
//
//    rtlsdr PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

void rtlsdr_pi::OnTestTerminate(wxProcessEvent& event)
{
    for(int i=0; i<PROCESS_COUNT; i++)
        if(event.GetPid() == TestPid[i])
            have_processes[i] = event.GetExitCode() != -1;
}


rtlsdr_pi::rtlsdr_pi(void *ppimgr)
    : opencpn_plugin_18(ppimgr), m_bNeedStart(false), m_Process1(NULL), m_Process2(NULL),
      m_prtlsdrDialog(NULL)

{
    // Create the PlugIn icons
    initialize_images();

    for(int i=0; i<PROCESS_COUNT; i++) {
        TestPid[i] = 0;
        have_processes[i] = false;
    }

    // detect helper programs
    const wxString ProcessNames[] = {_T("rtl_ais"), _T("rtl_fm"), _T("soft_fm"),
                                     _T("aisdecoder"), _T("ais_rx"), _T("rtl_adsb"), _T("aplay")};
    for(int i=0; i<PROCESS_COUNT; i++) {
        // pass -h because we are only testing the binary exists in the path

        wxProcess *process = wxProcess::Open(PATH() + ProcessNames[i] + _T(" -h"));
        if (process) {
            TestPid[i] = process->GetPid();
            process->Connect(wxEVT_END_PROCESS, wxProcessEventHandler
                (rtlsdr_pi::OnTestTerminate), NULL, this);
        }
    }

#ifdef BUILTIN_RTLAIS
    context = NULL;
#endif    
}

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

int rtlsdr_pi::Init(void)
{
      AddLocaleCatalog( _T("opencpn-rtlsdr_pi") );

      //    Get a pointer to the opencpn display canvas, to use as a parent for the POI Manager dialog
      m_parent_window = GetOCPNCanvasWindow();

      //    And load the configuration items
      LoadConfig();

      m_leftclick_tool_id  = InsertPlugInTool(_T(""), _img_rtlsdr, _img_rtlsdr, wxITEM_NORMAL,
            _("rtlsdr"), _T(""), NULL,
             RTLSDR_TOOL_POSITION, 0, this);

      m_prtlsdrDialog = NULL;

      /* periodically check for updates from computation thread */
      m_Timer.Connect(wxEVT_TIMER, wxTimerEventHandler
                      ( rtlsdr_pi::OnTimer ), NULL, this);
      m_Timer.Start(1000);

      return (WANTS_TOOLBAR_CALLBACK    |
              INSTALLS_TOOLBAR_TOOL     |
              WANTS_PREFERENCES         |
              WANTS_CONFIG
           );
}

bool rtlsdr_pi::DeInit(void)
{
    if(m_Process1)
        m_Process1->Disconnect(wxEVT_END_PROCESS, wxProcessEventHandler
                              ( rtlsdr_pi::OnTerminate ), NULL, this);

    if(m_Process2) {
        m_Process2->Disconnect(wxEVT_END_PROCESS, wxProcessEventHandler
                              ( rtlsdr_pi::OnTerminate ), NULL, this);
        Stop();
    }


    //    Record the dialog position
    if (NULL != m_prtlsdrDialog)
    {
        wxPoint p = m_prtlsdrDialog->GetPosition();
        SetrtlsdrDialogX(p.x);
        SetrtlsdrDialogY(p.y);
        
        m_prtlsdrDialog->Close();
        delete m_prtlsdrDialog;
        m_prtlsdrDialog = NULL;
    }
    SaveConfig();
    
    RemovePlugInTool(m_leftclick_tool_id);
    
    return true;
}

int rtlsdr_pi::GetAPIVersionMajor()
{
      return MY_API_VERSION_MAJOR;
}

int rtlsdr_pi::GetAPIVersionMinor()
{
      return MY_API_VERSION_MINOR;
}

int rtlsdr_pi::GetPlugInVersionMajor()
{
      return PLUGIN_VERSION_MAJOR;
}

int rtlsdr_pi::GetPlugInVersionMinor()
{
      return PLUGIN_VERSION_MINOR;
}

wxBitmap *rtlsdr_pi::GetPlugInBitmap()
{
    return new wxBitmap(_img_rtlsdr->ConvertToImage().Copy());
}

wxString rtlsdr_pi::GetCommonName()
{
      return _T("rtlsdr");
}


wxString rtlsdr_pi::GetShortDescription()
{
      return _("rtlsdr PlugIn for OpenCPN");
}

wxString rtlsdr_pi::GetLongDescription()
{
      return _("rtlsdr PlugIn for OpenCPN\n\
This plugin automatically controls external command-line tools for software radio.\n\
There are several options for AIS data, as well as ADS-b FM radio and vhf audio.\
");
}

int rtlsdr_pi::GetToolbarToolCount(void)
{
      return 1;
}

void rtlsdr_pi::SetColorScheme(PI_ColorScheme cs)
{
      if (NULL == m_prtlsdrDialog)
            return;
      DimeWindow(m_prtlsdrDialog);
}

void rtlsdr_pi::RearrangeWindow()
{
      if (NULL == m_prtlsdrDialog)
            return;

      SetColorScheme(PI_ColorScheme());

      m_prtlsdrDialog->Fit();
}

void rtlsdr_pi::OnToolbarToolCallback(int id)
{
    if(!m_prtlsdrDialog)
    {
        m_prtlsdrDialog = new rtlsdrDialog(*this, m_parent_window);
        m_prtlsdrDialog->m_cbEnabled->SetValue(m_bEnabled);
        m_prtlsdrDialog->Move(wxPoint(m_rtlsdr_dialog_x, m_rtlsdr_dialog_y));
    }

    RearrangeWindow();
    m_prtlsdrDialog->Show(!m_prtlsdrDialog->IsShown());

    wxPoint p = m_prtlsdrDialog->GetPosition();
    m_prtlsdrDialog->Move(0,0);        // workaround for gtk autocentre dialog behavior
    m_prtlsdrDialog->Move(p);
}

void rtlsdr_pi::ProcessInputStream( wxInputStream *in )
{
    int c;
//    in->FlushBuffer();
    while(in->CanRead() && (c = in->GetC()) != wxEOF) {
        wxString s((wxChar)c);

        if(c == '\n') {
            if(m_prtlsdrDialog && m_prtlsdrDialog->IsShown())
                m_prtlsdrDialog->m_tMessages->AppendText(m_sLastMessage);

            switch(m_Mode) {
            case AIS:
                if(m_sLastMessage.StartsWith(_T("!AIVDM"))) {
                    PushNMEABuffer(m_sLastMessage);
                    m_AISCount++;
                }
                break;
            case ADSB:
                /* need to decode ADSB messages, and be able to plot */
                break;
            default:
                break;
            }

            m_sLastMessage.clear();
        }
        else
            m_sLastMessage += s;
    }
}

void rtlsdr_pi::OnTimer( wxTimerEvent & )
{
#ifdef BUILTIN_RTLAIS
    if(context) {
        if(rtl_ais_isactive(context)) {
            const char *str;
            while((str = rtl_ais_next_message(context))) {
                wxString msg = str;
                if(m_prtlsdrDialog && m_prtlsdrDialog->IsShown())
                    m_prtlsdrDialog->m_tMessages->AppendText(msg);
                
                if(msg.StartsWith(_T("!AIVDM"))) {
                    PushNMEABuffer(msg);
                    m_AISCount++;
                }
            }
        } else
            Stop();
    }
#endif
    if(!m_Process2 || !m_bEnabled)
        return;

    if(m_Mode == FM || m_Mode == VHF)
        return;

    /* manually pipe the data */
    if(m_Process1) {
        wxInputStream *in = m_Process1->GetInputStream();
        wxOutputStream *out = m_Process2->GetOutputStream();

        char buffer[16384];
        while(in->CanRead()) {
            in->Read(buffer, sizeof buffer);
            int size = in->LastRead();
            out->Write(buffer, size);
        }
    }

    ProcessInputStream(m_Process2->GetInputStream());
    ProcessInputStream(m_Process2->GetErrorStream());
}

void rtlsdr_pi::ReportErrorStream(wxProcess *process)
{
    if(!m_prtlsdrDialog)
        return;

    wxInputStream *in = process->GetErrorStream();

    char buffer[16384];
    while(in->CanRead()) {
        in->Read(buffer, sizeof buffer);
        int size = in->LastRead();
        buffer[size] = 0;
        m_prtlsdrDialog->m_tMessages->AppendText(wxString::FromUTF8(buffer));
    }
    m_prtlsdrDialog->m_tMessages->AppendText(_T("\n"));
}

void rtlsdr_pi::OnTerminate(wxProcessEvent& event)
{
    bool handle = false;

    if(m_Process1 && event.GetPid() == m_Process1->GetPid()) {
        ReportErrorStream(m_Process1);
        m_Process1 = NULL;
        handle = true;
    }
    
    if(m_Process2 && event.GetPid() == m_Process2->GetPid()) {
        ReportErrorStream(m_Process2);
        m_Process2 = NULL;
    
        if(event.GetExitCode()) {
            if(m_prtlsdrDialog)
                m_prtlsdrDialog->m_tMessages->AppendText(wxDateTime::Now().Format() + _T(": ")
                                                         + _("Execution failed") + _T("\n"));
        }
        handle = true;
    }

    if(handle) {
        Stop();

        if(m_bNeedStart)
            Start();
        else
            Disable();
    }
}

void rtlsdr_pi::Disable()
{
    m_bEnabled = false;
    if(m_prtlsdrDialog)
        m_prtlsdrDialog->m_cbEnabled->SetValue(m_bEnabled);
}

double VHFFrequencyMHZ(int channel, bool WX)
{
    if(WX)
        switch(channel) {
        case 1: return 162.550;
        case 2: return 162.400;
        case 3: return 162.475;
        case 4: return 162.425;
        case 5: return 162.450;
        case 6: return 162.500;
        case 7: return 162.525;
        default: return 0;
        }
            
    if(channel >= 0 && channel <= 28)
        return 156 + (double)channel*.05;
    if(channel >= 60 && channel <= 88)
        return 156.025 + (double)(channel-60)*.05;
    return 0;
}

wxString PlayFM(double frequency, int samplerate, int outputrate, int squelch)
{
    if(frequency == 0)
        return _("Invalid FM frequency");
    return wxString::Format(_T("rtl_fm -r %dk -s %dk -f %.1fM -l %d"),
                            samplerate, outputrate, frequency, squelch);
}

void rtlsdr_pi::Restart()
{
    Stop();
    if(!m_bEnabled)
        return;

    if(m_Process1 || m_Process2)
        m_bNeedStart = true;
    else
        Start();
}

void rtlsdr_pi::Start()
{
    m_bNeedStart = false;
    m_sLastMessage.clear();

    m_command1 = _T("");

    switch(m_Mode) {
    case AIS:
#ifdef BUILTIN_RTLAIS
        if(m_AISProgram == _T("builtin rtl_ais")) {
            struct rtl_ais_config config;
            rtl_ais_default_config(&config);
            config.ppm_error = m_AISError;
            config.custom_ppm = 1;
            context = rtl_ais_start(&config);
            if(context) {
                if(m_prtlsdrDialog)
                    m_prtlsdrDialog->m_tMessages->AppendText(_("Started builtin rtl_ais") + _T("\n"));
            } else {
                if(m_prtlsdrDialog)
                    m_prtlsdrDialog->m_tMessages->AppendText(_("failed to start builtin rtl_ais") + _T("\n") +
                                                             _("is an rtlsdr device available?\n") + _T("\n"));
                Disable();
            }
            return;
        } else
#endif            
            if(m_AISProgram == _T("rtl_ais")) {
            m_command2 = PATH() + wxString::Format(_T("rtl_ais -n -p %d ") + m_P1args,
                                          m_AISError);
        } else if(m_AISProgram == _T("rtl_fm")) {
            m_command1 = PATH() + wxString::Format(_T("rtl_fm -f 161975000 -p %d -s 48k ")
                                                   + m_P1args, m_AISError);
            m_command2 = PATH() + _T("aisdecoder -h 127.0.0.1 -p 10110 -a file -c mono -d -f /dev/stdin "
                 + m_P2args);
        } else if(m_AISProgram == _T("soft_fm")) {
            m_command1 = PATH() + wxString::Format(_T("soft_fm -f 161975000 -p %d -s 48k ")
                                                   + m_P1args, m_AISError);
            m_command2 = PATH() + _T("aisdecoder -h 127.0.0.1 -p 10110 -a file -c mono -d -f /dev/stdin "
                 + m_P2args);
        } else if(m_AISProgram == _T("ais_rx")) {
            m_command2 = PATH() + wxString::Format(_T("ais_rx -d -r %d -e %d ") + m_P1args,
                                          m_AISSampleRate*1000, m_AISError);
        }
        break;
    case ADSB:
        m_command2 = PATH() + wxString::Format(_T("rtl_adsb"));
        break;
    case FM:
        m_command1 = PlayFM(m_dFMFrequency, 48, 250, 0);
        m_command2 = _T("aplay -r 48k -f S16_le -t raw -c 1");
        break;
    case VHF:
        m_command1 = PlayFM(VHFFrequencyMHZ(m_iVHFChannel, m_bVHFWX), 12, 12, m_iVHFSquelch);
        m_command2 = _T("aplay -r 12k -f S16_le -t raw -c 1");
        break;
    default:
        m_command2 = _("Unknown mode");
    }

    if(m_command1.size()) {
        if((m_Process1 = wxProcess::Open(m_command1)))
            m_Process1->Connect(wxEVT_END_PROCESS, wxProcessEventHandler
                                ( rtlsdr_pi::OnTerminate ), NULL, this);

            if(m_prtlsdrDialog)
                m_prtlsdrDialog->m_tMessages->AppendText(_("Executing: ") + m_command1 + _T("\n"));
        else {
            wxMessageDialog mdlg(m_parent_window, _("Failed to open: ") + m_command1,
                                 _("rtlsdr"), wxOK | wxICON_ERROR);
            mdlg.ShowModal();
            return;
        }
    } else
        m_Process1 = NULL;

    if((m_Process2 = wxProcess::Open(m_command2))) {
        m_Process2->Connect(wxEVT_END_PROCESS, wxProcessEventHandler
                            ( rtlsdr_pi::OnTerminate ), NULL, this);
        if(m_prtlsdrDialog)
            m_prtlsdrDialog->m_tMessages->AppendText(_("Executed: ") + m_command2 + _T("\n"));
    } else {
        wxMessageDialog mdlg(m_parent_window, _("Failed to open: ") + m_command2,
                             _("rtlsdr"), wxOK | wxICON_ERROR);
        mdlg.ShowModal();
    }
}

void rtlsdr_pi::Stop()
{
#ifdef BUILTIN_RTLAIS
    if(context) {
        rtl_ais_cleanup(context);
        context = NULL;

        if(m_prtlsdrDialog)
            m_prtlsdrDialog->m_tMessages->AppendText(_("Stopped builtin rtl_ais") + _T("\n"));

        Disable();
    }
#endif
    if(m_Process1) {
        wxProcess *process = m_Process1;
        m_Process1 = NULL;
        KillProcess(process);

        if(m_prtlsdrDialog)
            m_prtlsdrDialog->m_tMessages->AppendText(_("Stopped: ") + m_command1 + _T("\n"));
    }

    if(m_Process2) {
        wxProcess *process = m_Process2;
        m_Process2 = NULL;
        KillProcess(process);

        if(m_prtlsdrDialog)
            m_prtlsdrDialog->m_tMessages->AppendText(_("Stopped: ") + m_command2 + _T("\n"));
    }
}

void rtlsdr_pi::ShowPreferencesDialog( wxWindow* parent )
{
    rtlsdrPrefs *dialog = new rtlsdrPrefs( *this, parent );
    
    dialog->m_rbAIS->SetValue(m_Mode == AIS);
    for(unsigned int i = 0; i<dialog->m_cAISProgram->GetCount(); i++)
        if(dialog->m_cAISProgram->GetString(i).Contains(m_AISProgram))
           dialog->m_cAISProgram->SetSelection(i);

    dialog->m_tP1args->SetValue(m_P1args);
    dialog->m_tP2args->SetValue(m_P2args);

    dialog->m_sAISSampleRate->SetValue(m_AISSampleRate);
    dialog->m_sAISError->SetValue(m_AISError);

    dialog->m_rbADSB->SetValue(m_Mode == ADSB);
    dialog->m_cbADSBPlot->SetValue(m_bADSBPlot);

    dialog->m_rbFM->SetValue(m_Mode == FM);
    dialog->m_tFMFrequency->SetValue(wxString::Format(_T("%.1f"), m_dFMFrequency));

    dialog->m_rbVHF->SetValue(m_Mode == VHF);
    dialog->m_tVHFChannel->SetValue(wxString::Format(_T("%d"), m_iVHFChannel));
    dialog->m_sVHFSquelch->SetValue(wxString::Format(_T("%d"), m_iVHFSquelch));
    dialog->m_cbVHFWX->SetValue(m_bVHFWX);

    wxCommandEvent d;
    dialog->OnAISProgram(d);
    
    dialog->Fit();
    wxColour cl;
    GetGlobalColor(_T("DILG1"), &cl);
    dialog->SetBackgroundColour(cl);
    
    if(dialog->ShowModal() == wxID_OK)
    {
        int mode = m_Mode;
        if(dialog->m_rbAIS->GetValue())
            mode = AIS;
        else if(dialog->m_rbADSB->GetValue())
            mode = ADSB;
        else if(dialog->m_rbFM->GetValue())
            mode = FM;
        else if(dialog->m_rbVHF->GetValue())
            mode = VHF;

        wxString AISProgram = dialog->m_cAISProgram->GetString(dialog->m_cAISProgram->GetSelection());
        wxString AISPrograms[] = {_T("rtl_ais"), _T("rtl_fm"), _T("soft_fm"), _T("ais_rx")};

        if(!AISProgram.Contains("builtin"))
        for(unsigned int i=0; i < (sizeof AISPrograms) / (sizeof *AISPrograms); i++)
            if(AISProgram.Contains(AISPrograms[i]))
               AISProgram = AISPrograms[i];
                
        wxString P1args = dialog->m_tP1args->GetValue();
        wxString P2args = dialog->m_tP2args->GetValue();
        int AISSampleRate = dialog->m_sAISSampleRate->GetValue();
        int AISError = dialog->m_sAISError->GetValue();

        m_bADSBPlot = dialog->m_cbADSBPlot->GetValue();

        double FMFrequency;
        dialog->m_tFMFrequency->GetValue().ToDouble(&FMFrequency);

        long VHFChannel;
        dialog->m_tVHFChannel->GetValue().ToLong(&VHFChannel);
        int VHFSquelch = dialog->m_sVHFSquelch->GetValue();
        bool VHFWX = dialog->m_cbVHFWX->GetValue();

        bool restart =
            m_Mode != mode ||
            (mode == AIS && (m_AISProgram != AISProgram ||
                             m_P1args != P1args ||
                             m_P2args != P2args ||
                             m_AISSampleRate != AISSampleRate ||
                             m_AISError != AISError)) ||
            (mode == FM && m_dFMFrequency != FMFrequency) ||
            (mode == VHF && (m_iVHFChannel != VHFChannel ||
                             m_iVHFSquelch != VHFSquelch ||
                             m_bVHFWX != VHFWX));

        m_Mode = (rtlsdrMode)mode;

        m_AISProgram = AISProgram;
        m_P1args = P1args;
        m_P2args = P2args;
        m_AISSampleRate = AISSampleRate;
        m_AISError = AISError;

        m_dFMFrequency = FMFrequency;

        m_iVHFChannel = VHFChannel;
        m_iVHFSquelch = VHFSquelch;
        m_bVHFWX = VHFWX;

        if(restart)
            Restart();

        SaveConfig();
    }
    delete dialog;
}

bool rtlsdr_pi::LoadConfig(void)
{
    wxFileConfig *pConf = GetOCPNConfigObject();

    if(!pConf)
        return false;

    pConf->SetPath ( _T( "/Settings/rtlsdr" ) );

    m_rtlsdr_dialog_x =  pConf->Read ( _T ( "DialogPosX" ), 20L );
    m_rtlsdr_dialog_y =  pConf->Read ( _T ( "DialogPosY" ), 20L );

    pConf->Read ( _T ( "Enabled" ), &m_bEnabled, false);
    int mode;
    pConf->Read ( _T ( "Mode" ), &mode, 0 );
    m_Mode = (rtlsdrMode)mode;

    m_AISProgram = pConf->Read ( _T ( "AISProgram" ), _T("aisdecoder") );
    m_P1args = pConf->Read ( _T ( "P1args" ), _T("") );
    m_P2args = pConf->Read ( _T ( "P2args" ), _T("") );
    m_AISSampleRate = pConf->Read ( _T ( "AISSampleRate" ), 256 );
    m_AISError = pConf->Read ( _T ( "AISError" ), 50 );

    pConf->Read ( _T ( "ADSBPlot" ), &m_bADSBPlot, 1 );

    pConf->Read ( _T ( "FMFrequency" ), &m_dFMFrequency, 104.4 );

    pConf->Read ( _T ( "VHFChannel" ), &m_iVHFChannel, 16 );
    pConf->Read ( _T ( "VHFSquelch" ), &m_iVHFSquelch, 30 );
    pConf->Read ( _T ( "VHFWX" ), &m_bVHFWX, false );

    if(m_bEnabled)
        Start();

    return true;
}

bool rtlsdr_pi::SaveConfig(void)
{
    wxFileConfig *pConf = GetOCPNConfigObject();

    if(!pConf)
        return false;

    pConf->SetPath ( _T ( "/Settings/rtlsdr" ) );

    pConf->Write ( _T ( "DialogPosX" ),   m_rtlsdr_dialog_x );
    pConf->Write ( _T ( "DialogPosY" ),   m_rtlsdr_dialog_y );

    pConf->Write ( _T ( "Enabled" ), m_bEnabled );
    pConf->Write ( _T ( "Mode" ), (int)m_Mode );

    pConf->Write ( _T ( "AISProgram" ), m_AISProgram );
    pConf->Write ( _T ( "P1args" ), m_P1args );
    pConf->Write ( _T ( "P2args" ), m_P2args );
    pConf->Write ( _T ( "AISSampleRate" ), m_AISSampleRate );
    pConf->Write ( _T ( "AISError" ), m_AISError );

    pConf->Write ( _T ( "ADSBPlot" ), m_bADSBPlot );

    pConf->Write ( _T ( "FMFrequency" ), m_dFMFrequency );

    pConf->Write ( _T ( "VHFChannel" ), m_iVHFChannel );
    pConf->Write ( _T ( "VHFSquelch" ), m_iVHFSquelch );
    pConf->Write ( _T ( "VHFWX" ), m_bVHFWX );

    return true;
}
