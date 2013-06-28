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

#ifndef _RTLSDRPI_H_
#define _RTLSDRPI_H_

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include <wx/fileconf.h>

#define     PLUGIN_VERSION_MAJOR    0
#define     PLUGIN_VERSION_MINOR    8

#define     MY_API_VERSION_MAJOR    1
#define     MY_API_VERSION_MINOR    8

#include "../../../include/ocpn_plugin.h"

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

#define RTLSDR_TOOL_POSITION    -1          // Request default positioning of toolbar tool

class rtlsdrDialog;
class wxProcessEvent;

enum rtlsdrMode {AIS, ADSB, FM, VHF};

class rtlsdr_pi : public opencpn_plugin_18, public wxEvtHandler
{
public:
      rtlsdr_pi(void *ppimgr);

//    The required PlugIn Methods
      int Init(void);
      bool DeInit(void);

      int GetAPIVersionMajor();
      int GetAPIVersionMinor();
      int GetPlugInVersionMajor();
      int GetPlugInVersionMinor();
      wxBitmap *GetPlugInBitmap();
      wxString GetCommonName();
      wxString GetShortDescription();
      wxString GetLongDescription();

      int GetToolbarToolCount(void);

      void OnToolbarToolCallback(int id);

//    Optional plugin overrides
      void SetColorScheme(PI_ColorScheme cs);

//    Other public methods
      void SetrtlsdrDialogX    (int x){ m_rtlsdr_dialog_x = x;};
      void SetrtlsdrDialogY    (int x){ m_rtlsdr_dialog_y = x;}

      void OnRtlsdrDialogClose();
      void Restart();
      void Start();
      void Stop();

      void ShowPreferencesDialog( wxWindow* parent );

      wxWindow         *m_parent_window;

      bool m_bEnabled;

      rtlsdrMode m_Mode;
      int m_AISSampleRate, m_AISError;
      bool m_bADSBPlot;
      double m_dFMFrequency;
      int m_iVHFChannel;
      bool m_bVHFWX;

      wxFileConfig     *m_pconfig;

private:
      void OnTimer( wxTimerEvent & );
      void OnTerminate(wxProcessEvent&);

      bool              LoadConfig(void);
      bool              SaveConfig(void);

      bool m_bNeedStart;

      wxTimer            m_Timer;
      wxProcess         *m_Process;
      wxString           m_command;
      rtlsdrDialog      *m_prtlsdrDialog;

      wxString m_sLastMessage;

      int               m_rtlsdr_dialog_x, m_rtlsdr_dialog_y;
      int               m_display_width, m_display_height;

      int               m_leftclick_tool_id;

      void              RearrangeWindow();
};

double VHFFrequencyMHZ(int channel, bool WX=false);

#endif
