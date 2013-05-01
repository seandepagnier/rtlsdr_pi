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
 *   the Free Software Foundation; either version 2 of the License, or     *
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


#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include "wx/process.h"

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
    : opencpn_plugin_18(ppimgr), m_Process(NULL)
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
      return _img_rtlsdr;
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
Eventual version will need to link with gnu radio directly.\n\
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
            m_prtlsdrDialog->m_cbEnabled->SetValue(m_Enabled);
            m_prtlsdrDialog->Move(wxPoint(m_rtlsdr_dialog_x, m_rtlsdr_dialog_y));
      }

      RearrangeWindow();
      m_prtlsdrDialog->Show(!m_prtlsdrDialog->IsShown());

      wxPoint p = m_prtlsdrDialog->GetPosition();
      m_prtlsdrDialog->Move(0,0);        // workaround for gtk autocentre dialog behavior
      m_prtlsdrDialog->Move(p);
}

void rtlsdr_pi::OnTimer( wxTimerEvent & )
{
    if(!m_Process || !m_Enabled)
        return;

    int c;
    wxInputStream *in = m_Process->GetInputStream();
    while(in->CanRead() && (c = in->GetC()) != wxEOF) {
        wxString s((wxChar)c);
        if(m_prtlsdrDialog)
            m_prtlsdrDialog->m_tMessages->AppendText(s);

        if(c == '\n') {
            if(m_sLastNmeaMessage.StartsWith(_T("!AIVDM")))
                PushNMEABuffer(m_sLastNmeaMessage);

            m_sLastNmeaMessage.clear();
        }
        else
            m_sLastNmeaMessage += s;
    }
}

bool rtlsdr_pi::LoadConfig(void)
{
      wxFileConfig *pConf = m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T( "/Settings/rtlsdr" ) );

            m_rtlsdr_dialog_x =  pConf->Read ( _T ( "DialogPosX" ), 20L );
            m_rtlsdr_dialog_y =  pConf->Read ( _T ( "DialogPosY" ), 20L );

            pConf->Read ( _T ( "Enabled" ), &m_Enabled, false);
            m_SampleRate = pConf->Read ( _T ( "SampleRate" ), 256 );
            m_Error = pConf->Read ( _T ( "Error" ), 50 );

            if(m_Enabled)
                RestartGrAis();
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

            pConf->Write ( _T ( "Enabled" ), m_Enabled );
            pConf->Write ( _T ( "SampleRate" ), m_SampleRate );
            pConf->Write ( _T ( "Error" ), m_Error );

            return true;
      }
      else
            return false;
}

void rtlsdr_pi::RestartGrAis()
{
    StopGrAis();
    if(m_Enabled)
        m_Process = wxProcess::Open(wxString::Format(_T("ais_rx.py -d -r %d -e %d "),
                                                     m_SampleRate*1000, m_Error));
}

void rtlsdr_pi::StopGrAis()
{
    if(m_Process) {
        int pid = m_Process->GetPid();
        wxProcess::Kill(pid);
        wxThread::Sleep(10);
        if(wxProcess::Exists(pid))
            wxProcess::Kill(pid, wxSIGKILL);
    }
    m_Process = NULL;
}

void rtlsdr_pi::ShowPreferencesDialog( wxWindow* parent )
{
    rtlsdrPrefs *dialog = new rtlsdrPrefs( *this, parent );
    
    dialog->m_sSampleRate->SetValue(m_SampleRate);
    dialog->m_sError->SetValue(m_Error);
    
    dialog->Fit();
    wxColour cl;
    GetGlobalColor(_T("DILG1"), &cl);
    dialog->SetBackgroundColour(cl);
    
    if(dialog->ShowModal() == wxID_OK)
    {
        int sampleRate = dialog->m_sSampleRate->GetValue();
        int error = dialog->m_sError->GetValue();
        if(m_SampleRate != sampleRate || m_Error != error) {
            m_SampleRate = sampleRate;
            m_Error = error;
            RestartGrAis();
        }
        SaveConfig();
    }
    delete dialog;
}
