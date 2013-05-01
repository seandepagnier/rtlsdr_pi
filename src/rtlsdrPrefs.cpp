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
#include "rtlsdrPrefs.h"

rtlsdrPrefs::rtlsdrPrefs( rtlsdr_pi &_rtlsdr_pi, wxWindow* parent)
    : rtlsdrPrefsBase( parent ), m_rtlsdr_pi(_rtlsdr_pi)
{
}

void rtlsdrPrefs::OnLaunchGnuRadioCompanion( wxCommandEvent& event )
{
    m_rtlsdr_pi.Stop();
    wxProcess::Open(_T("gnuradio-companion gr-ais.grc"), wxEXEC_SYNC);
    m_rtlsdr_pi.Restart();
}

void rtlsdrPrefs::OnInfo( wxCommandEvent& event )
{
    wxMessageDialog mdlg(this, _("\
Because the software radio is not a precision device,\n\
The error value must be set to properly calibrate the radio dongle\n\
Run \"gnuradio-companion gr-ais.grc\" and first enable only the lower block.\n\
Set the frequency to a known vhf frequency, and transmit, and note\n\
how far the peak is from the true frequency in khz, this is the error value\n\
Once working, you should be able to see bursts (ais data) from the A and B\n\
Channel blocks once enabled\n"),
                         _("rtlsdr"), wxOK);
    mdlg.ShowModal();
}
