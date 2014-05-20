/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  rtlsdr Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2013 by Sean D'Epagnier                                 *
 *   sean at depagnier dot com                                             *
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


#include <wx/wx.h>
#include <wx/process.h>

#ifdef __unix__
#include <sys/stat.h>
#endif

#include "rtlsdr_pi.h"
#include "rtlsdrDialog.h"
#include "rtlsdrPrefs.h"
#include "icons.h"

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new rtlsdr_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}

//---------------------------------------------------------------------------------------------------------
//
//    rtlsdr PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------


rtlsdr_pi::rtlsdr_pi(void *ppimgr)
    : opencpn_plugin_18(ppimgr), m_bNeedStart(false), m_Process1(NULL), m_Process2(NULL)

{
    // Create the PlugIn icons
    initialize_images();
}

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

int rtlsdr_pi::Init(void)
{
      AddLocaleCatalog( _T("opencpn-rtlsdr_pi") );

      // Set some default private member parameters
      m_rtlsdr_dialog_x = 0;
      m_rtlsdr_dialog_y = 0;

      ::wxDisplaySize(&m_display_width, &m_display_height);

      //    Get a pointer to the opencpn display canvas, to use as a parent for the POI Manager dialog
      m_parent_window = GetOCPNCanvasWindow();

      //    Get a pointer to the opencpn configuration object
      m_pconfig = GetOCPNConfigObject();

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
Read rtlsdr nmea messages from gr-ais ais_rx.py script. \n\
Support ADS-b FM radio and vhf\n\
Eventually version will need to link with gnu radio directly.\n\
\n\
The rtlsdr plugin was written by Sean D'Epagnier\n\
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

void rtlsdr_pi::OnTerminate(wxProcessEvent& event)
{
    if(m_Process2 && event.GetPid() == m_Process2->GetPid()) {
        if(event.GetExitCode()) {
            wxString msg = _("Execution failed");
            wxMessageDialog mdlg(m_parent_window, msg + _T(": \"") + m_command2 + _T("\""),
                                 _("rtlsdr"), wxOK | wxICON_ERROR);
            mdlg.ShowModal();
        }

        m_Process2 = NULL;
        if(m_bNeedStart)
            Start();
    }
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

wxString PlayFM(double frequency, int samplerate, int outputrate)
{
    if(frequency == 0)
        return _("Invalid FM frequency");
    return wxString::Format(_T("rtl_fm -r %dk -s %dk -f %.1fM"),
                            samplerate, outputrate, frequency);
}

void rtlsdr_pi::Restart()
{
    Stop();
    if(!m_bEnabled)
        return;

    if(m_Process2)
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
        switch(m_AISProgram) {
        case AISDECODER:
            m_command1 = wxString::Format(_T("rtl_fm -f 161975000 -p %d -s 48k"),
                                          m_AISError);
            m_command2 = _T("aisdecoder -h 127.0.0.1 -p 10110 -a file -c mono -d -f /dev/stdin");
            break;
        case AIS_RX:
            m_command2 = wxString::Format(_T("ais_rx -d -r %d -e %d"),
                                          m_AISSampleRate*1000, m_AISError);
            break;
        } break;
    case ADSB:
        m_command2 = wxString::Format(_T("rtl_adsb"));
        break;
    case FM:
        m_command1 = PlayFM(m_dFMFrequency, 48, 250);
        m_command2 = _T("aplay -r 48k -f S16_le -t raw -c 1");
        break;
    case VHF:
        m_command1 = PlayFM(VHFFrequencyMHZ(m_iVHFChannel, m_bVHFWX), 12, 12);
        m_command2 = _T("aplay -r 12k -f S16_le -t raw -c 1");
        break;
    default:
        m_command2 = _("Unknown mode");
    }

    if(m_command1.size()) {
        if((m_Process1 = wxProcess::Open(m_command1)))
            m_Process1->Connect(wxEVT_END_PROCESS, wxProcessEventHandler
                                ( rtlsdr_pi::OnTerminate ), NULL, this);
        else {
            wxMessageDialog mdlg(m_parent_window, _("Failed to open: ") + m_command1,
                                 _("rtlsdr"), wxOK | wxICON_ERROR);
            mdlg.ShowModal();
            return;
        }
    } else
        m_Process1 = NULL;

    if((m_Process2 = wxProcess::Open(m_command2)))
        m_Process2->Connect(wxEVT_END_PROCESS, wxProcessEventHandler
                            ( rtlsdr_pi::OnTerminate ), NULL, this);
    else {
        wxMessageDialog mdlg(m_parent_window, _("Failed to open: ") + m_command2,
                             _("rtlsdr"), wxOK | wxICON_ERROR);
        mdlg.ShowModal();
    }
}

static void KillProcess(wxProcess *process)
{
    if(!process)
        return;

    int pid = process->GetPid();
    wxProcess::Kill(pid);
    wxThread::Sleep(25);
    if(wxProcess::Exists(pid))
        wxProcess::Kill(pid, wxSIGKILL);
}

void rtlsdr_pi::Stop()
{
#if 0
#ifdef __unix__
    system("killall -9 rtl_fm > /dev/null");
    system("killall -9 aplay > /dev/null");
    wxThread::Sleep(1000);
#endif
#endif

    KillProcess(m_Process1);

    wxProcess *process = m_Process2;
    m_Process2 = NULL;
    KillProcess(process);
}

void rtlsdr_pi::ShowPreferencesDialog( wxWindow* parent )
{
    rtlsdrPrefs *dialog = new rtlsdrPrefs( *this, parent );
    
    dialog->m_rbAIS->SetValue(m_Mode == AIS);
    dialog->m_cAISProgram->SetSelection(m_AISProgram);
    dialog->m_sAISSampleRate->Enable((int)m_AISProgram);
    dialog->m_sAISSampleRate->SetValue(m_AISSampleRate);
    dialog->m_sAISError->SetValue(m_AISError);

    dialog->m_rbADSB->SetValue(m_Mode == ADSB);
    dialog->m_cbADSBPlot->SetValue(m_bADSBPlot);

    dialog->m_rbFM->SetValue(m_Mode == FM);
    dialog->m_tFMFrequency->SetValue(wxString::Format(_T("%.1f"), m_dFMFrequency));

    dialog->m_rbVHF->SetValue(m_Mode == VHF);
    dialog->m_tVHFChannel->SetValue(wxString::Format(_T("%d"), m_iVHFChannel));
    dialog->m_cbVHFWX->SetValue(m_bVHFWX);
    
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

        aisProgram AISProgram = (aisProgram)dialog->m_cAISProgram->GetSelection();
        int AISSampleRate = dialog->m_sAISSampleRate->GetValue();
        int AISError = dialog->m_sAISError->GetValue();

        m_bADSBPlot = dialog->m_cbADSBPlot->GetValue();

        double FMFrequency;
        dialog->m_tFMFrequency->GetValue().ToDouble(&FMFrequency);

        long VHFChannel;
        dialog->m_tVHFChannel->GetValue().ToLong(&VHFChannel);
        bool VHFWX = dialog->m_cbVHFWX->GetValue();

        bool restart =
            m_Mode != mode ||
            (mode == AIS && (m_AISProgram != AISProgram ||
                             m_AISSampleRate != AISSampleRate ||
                             m_AISError != AISError)) ||
            (mode == FM && m_dFMFrequency != FMFrequency) ||
            (mode == VHF && (m_iVHFChannel != VHFChannel || m_bVHFWX != VHFWX));

        m_Mode = (rtlsdrMode)mode;

        m_AISProgram = AISProgram;
        m_AISSampleRate = AISSampleRate;
        m_AISError = AISError;

        m_dFMFrequency = FMFrequency;

        m_iVHFChannel = VHFChannel;
        m_bVHFWX = VHFWX;

        if(restart)
            Restart();

        SaveConfig();
    }
    delete dialog;
}

bool rtlsdr_pi::LoadConfig(void)
{
      wxFileConfig *pConf = m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T( "/Settings/rtlsdr" ) );

            m_rtlsdr_dialog_x =  pConf->Read ( _T ( "DialogPosX" ), 20L );
            m_rtlsdr_dialog_y =  pConf->Read ( _T ( "DialogPosY" ), 20L );

            pConf->Read ( _T ( "Enabled" ), &m_bEnabled, false);
            int mode;
            pConf->Read ( _T ( "Mode" ), &mode, 0 );
            m_Mode = (rtlsdrMode)mode;

            m_AISProgram = (aisProgram)pConf->Read ( _T ( "AISProgram" ), 0L );
            m_AISSampleRate = pConf->Read ( _T ( "AISSampleRate" ), 256 );
            m_AISError = pConf->Read ( _T ( "AISError" ), 50 );

            pConf->Read ( _T ( "ADSBPlot" ), &m_bADSBPlot, 1 );

            pConf->Read ( _T ( "FMFrequency" ), &m_dFMFrequency, 104.4 );

            pConf->Read ( _T ( "VHFChannel" ), &m_iVHFChannel, 16 );
            pConf->Read ( _T ( "VHFWX" ), &m_bVHFWX, false );

            if(m_bEnabled)
                Start();

            return true;
      } else
            return false;
}

bool rtlsdr_pi::SaveConfig(void)
{
      wxFileConfig *pConf = m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T ( "/Settings/rtlsdr" ) );

            pConf->Write ( _T ( "DialogPosX" ),   m_rtlsdr_dialog_x );
            pConf->Write ( _T ( "DialogPosY" ),   m_rtlsdr_dialog_y );

            pConf->Write ( _T ( "Enabled" ), m_bEnabled );
            pConf->Write ( _T ( "Mode" ), (int)m_Mode );

            pConf->Write ( _T ( "AISProgram" ), m_AISProgram );
            pConf->Write ( _T ( "AISSampleRate" ), m_AISSampleRate );
            pConf->Write ( _T ( "AISError" ), m_AISError );

            pConf->Write ( _T ( "ADSBPlot" ), m_bADSBPlot );

            pConf->Write ( _T ( "FMFrequency" ), m_dFMFrequency );

            pConf->Write ( _T ( "VHFChannel" ), m_iVHFChannel );
            pConf->Write ( _T ( "VHFWX" ), m_bVHFWX );

            return true;
      }
      else
            return false;
}
