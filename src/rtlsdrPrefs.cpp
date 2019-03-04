/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  rtlsdr Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2019 by Sean D'Epagnier                                 *
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
#include <wx/progdlg.h>

#include "rtlsdr_pi.h"
#include "rtlsdrPrefs.h"

rtlsdrPrefs::rtlsdrPrefs( rtlsdr_pi &_rtlsdr_pi, wxWindow* parent)
    : rtlsdrPrefsBase( parent ), m_rtlsdr_pi(_rtlsdr_pi)
{
    int pindex = 0;
#ifndef BUILTIN_RTLAIS
        m_cAISProgram->Delete(pindex);
#else        
        pindex++;
#endif        

    if(!m_rtlsdr_pi.have_processes[rtlsdr_pi::RTL_AIS])
        m_cAISProgram->Delete(pindex);
    else
        pindex++;

    if(!m_rtlsdr_pi.have_processes[rtlsdr_pi::RTL_FM] ||
       !m_rtlsdr_pi.have_processes[rtlsdr_pi::AISDECODER])
        m_cAISProgram->Delete(pindex);
    else
        pindex++;

    if(!m_rtlsdr_pi.have_processes[rtlsdr_pi::SOFT_FM] ||
       !m_rtlsdr_pi.have_processes[rtlsdr_pi::AISDECODER])
        m_cAISProgram->Delete(pindex);
    else
        pindex++;

    if(!m_rtlsdr_pi.have_processes[rtlsdr_pi::AIS_RX])
        m_cAISProgram->Delete(pindex);
    else
        pindex++;

    if(pindex == 0)
        m_rbAIS->Disable();

    if(!m_rtlsdr_pi.have_processes[rtlsdr_pi::DUMP1090])
        m_rbADSB->Disable();

    if(!m_rtlsdr_pi.have_processes[rtlsdr_pi::RTL_FM] ||
       !m_rtlsdr_pi.have_processes[rtlsdr_pi::APLAY]) {
        m_rbFM->Disable();
        m_rbVHF->Disable();
    }
}

void rtlsdrPrefs::OnAISProgram( wxCommandEvent& event )
{
    m_sAISSampleRate->Enable((bool)event.GetSelection());

    wxString AISProgram = m_cAISProgram->GetString(m_cAISProgram->GetSelection());
    bool pipe = AISProgram.Contains(_T("|"));
    m_stP2args->Show(pipe);
    m_tP2args->Show(pipe);

    m_stP1args->SetLabel(AISProgram.BeforeFirst('|'));
    m_stP2args->SetLabel(AISProgram.AfterFirst('|'));

    Fit();
}

void rtlsdrPrefs::OnAutoCalibrate( wxCommandEvent& event )
{
    wxProgressDialog progress(_("rtlsdr error calibration"), _T(""), 256, this,
                              wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME | wxPD_CAN_ABORT);
    m_rtlsdr_pi.Stop();

    int i;
    for(i = 0; i < 96; i+=4) {
        m_rtlsdr_pi.m_AISError = i;
        m_rtlsdr_pi.m_AISCount = 0;
        m_rtlsdr_pi.Start();

        for(int c = 0; c<6 && !m_rtlsdr_pi.m_AISCount; c++) {
            wxThread::Sleep(2500);
            if(!progress.Update(i*3/2+c))
                goto abort;
        }

        m_rtlsdr_pi.Stop();

        if(m_rtlsdr_pi.m_AISCount)
            goto havecount;
    }

    for(i = -6; i > -96; i-=4) {
        if(!progress.Update(64 - i ))
            break;

        m_rtlsdr_pi.m_AISError = i;
        m_rtlsdr_pi.m_AISCount = 0;
        m_rtlsdr_pi.Start();

        for(int c = 0; c<6 && !m_rtlsdr_pi.m_AISCount; c++) {
            wxThread::Sleep(2500);
            if(!progress.Update(64-i*3/2+c))
                goto abort;
        }

        m_rtlsdr_pi.Stop();
        
        if(m_rtlsdr_pi.m_AISCount)
            goto havecount;
    }

havecount:

    {
    int e = i;
    int pos = 128;
    do {
        i--;
        m_rtlsdr_pi.m_AISError = i;
        m_rtlsdr_pi.m_AISCount = 0;
        m_rtlsdr_pi.Start();

        for(int c = 0; c<6 && !m_rtlsdr_pi.m_AISCount; c++) {
            wxThread::Sleep(3500);
            if(!progress.Update(pos++))
                goto abort;
            if(pos == 256) pos = 255;
        }

        m_rtlsdr_pi.Stop();
    } while(i > -128 && m_rtlsdr_pi.m_AISCount);

    do {
        e++;
        m_rtlsdr_pi.m_AISError = e;
        m_rtlsdr_pi.m_AISCount = 0;
        m_rtlsdr_pi.Start();

        for(int c = 0; c<6 && !m_rtlsdr_pi.m_AISCount; c++) {
            wxThread::Sleep(3500);
            if(!progress.Update(pos++))
                goto abort;
            if(pos == 256) pos = 255;
        }

        m_rtlsdr_pi.Stop();
    } while(e < 128 && m_rtlsdr_pi.m_AISCount);

    m_sAISError->SetValue((i + e)/2);

    m_rtlsdr_pi.Restart();
    
    return;
    }

abort:
    m_rtlsdr_pi.Stop();
    {
        wxMessageDialog mdlg(this, _("No AIS messages received\nCalibration Failed"),
                         _("rtlsdr error calibration"),
                         wxOK | wxICON_ERROR);
        mdlg.ShowModal();
        return;
    }
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
The error value must be set to properly calibrate the radio dongle.\n\n\
To perform calibration, the autocalibrate takes some time, but will scan \
many error values searching for AIS signals.  This requires fairly frequent \
AIS traffic to work.\n\n\
Alternately if gnuradio is installed, you may use the included script \
and either look for peaks on ais traffic, or use a known frequency \
(for example, vhf channel 8 is ") + wxString::Format(_T("%.2f MHZ"), VHFFrequencyMHZ(8))
+ _(") and transmit on this frequency to note how far the peak is \
from the true frequency in khz.  This is the error value needed to calibrate the dongle.\n\n\
Once working, you should be able to see bursts (ais data) from the A and B\n\
in the filtered output, and receive ais messages (if any are available)\n"),
                         _("rtlsdr"), wxOK);
    mdlg.ShowModal();
}

void rtlsdrPrefs::OnInformation( wxCommandEvent& event )
{
    wxString basemsg = _("\
This plugin integrates opencpn with the rtlsdr project to turn DVB dongles into \
a SDR receiver. see:  http://sdr.osmocom.org/trac/wiki/rtl-sdr\n\n\
The author is using the r820t type dvb-t dongle with a standard vhf antenna, \
but many others can work.  The antenna coax cable is spliced to the dvb-t \
cable, both inner and outer connections must be made.\n\n\
rtl_ais can efficiently recieve channel A and B simultaneously\n\
rtl_fm/softfm and aisdecoder can only receive channel A or B\n\
ais_rx (gnuradio) receives both channels A and B but uses much more cpu.");

    wxMessageDialog mdlg(this,
#ifdef __WIN32__
	_("\
Under Windows Operating system, you must first run the utility program 'zadig'\
to install the needed usb driver once the device is plugged in.\n\
You can find this program at http://zadig.akeo.ie\n\n") +
#endif
                             basemsg, _("rtlsdr"), wxOK);
    mdlg.ShowModal();
}

void rtlsdrPrefs::OnAboutAuthor( wxCommandEvent& event )
{
    wxLaunchDefaultBrowser(_T(ABOUT_AUTHOR_URL));
}
