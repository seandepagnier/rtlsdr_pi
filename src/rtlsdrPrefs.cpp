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
    wxProcess::Open(_T("gnuradio-companion gr-ais.grc")/*, wxEXEC_SYNC*/);
    m_rtlsdr_pi.Restart();
}

void rtlsdrPrefs::OnInfo( wxCommandEvent& event )
{
    wxMessageDialog mdlg(this, _("\
Because the software radio is not a precision device, \
The error value must be set to properly calibrate the radio dongle.\n\
To perform calibration, launch \"gnuradio-companion gr-ais.grc\"\
(On a netbook first enable only the lower block or the top two take the\
whole screen and you cannot see the third block)\
Set the frequency to a known vhf frequency, \
(for example, channel 8 is ") + wxString::Format(_T("%.2f MHZ"), VHFFrequencyMHZ(8))
+ _(") then transmit with vhf transmitter on this channel and note how far the peak is \
from the true frequency in khz, this is the error value needed to calibrate the dongle.\n\
Once working, you should be able to see bursts (ais data) from the A and B\n\
in the filtered outputsChannel blocks once enabled\n"),
                         _("rtlsdr"), wxOK);
    mdlg.ShowModal();
}
