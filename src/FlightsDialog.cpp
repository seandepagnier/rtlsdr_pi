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

#include "rtlsdr_pi.h"

enum {HEX, NAME, HDG, SPD, LAT, LON, ALT, AGE, MSGS};

FlightsDialog::FlightsDialog( Flights &_flights, wxWindow* parent)
    : FlightsDialogBase( parent ), last_view_scale_ppm(0), m_flights(_flights)
{
    m_lFlights->InsertColumn(HEX, "HEX");
    m_lFlights->InsertColumn(NAME, "Name");
    m_lFlights->InsertColumn(HDG, "Heading");
    m_lFlights->InsertColumn(SPD, "Speed");
    m_lFlights->InsertColumn(LAT, "Lat");
    m_lFlights->InsertColumn(LON, "Lon");
    m_lFlights->InsertColumn(ALT, "Altitude");
    m_lFlights->InsertColumn(AGE, "Age");
    m_lFlights->InsertColumn(MSGS, "Messages");

    m_timer.Connect(wxEVT_TIMER, wxTimerEventHandler
                       ( FlightsDialog::OnTimer ), NULL, this);
    m_timer.Start(3000);
}

void FlightsDialog::OnGoto( wxCommandEvent& event )
{
    int index = m_lFlights->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    
    double lat, lon;
    if(index >= 0 &&
       m_lFlights->GetItemText(index, LAT).ToDouble(&lat) &&
       m_lFlights->GetItemText(index, LON).ToDouble(&lon) &&
       last_view_scale_ppm)
         JumpToPosition(lat, lon, last_view_scale_ppm);
    else {
        wxString basemsg = _("Failed to go to position!") + "\n";
        wxMessageDialog mdlg(this, basemsg, _("rtlsdr"), wxOK);
        mdlg.ShowModal();
    }
}

void FlightsDialog::OnTimer( wxTimerEvent & )
{
    if(!IsShown())
        return;

    m_stConnected->SetLabel(m_flights.connected() ? _("Connected") : _("Not Connected"));

    // remove flights not in list
    long index = 0;
    while(index<m_lFlights->GetItemCount()) {
        long hex = 0;
        m_lFlights->GetItemText(index, HEX).ToCLong(&hex, 16);
        std::map<int, FlightInfo>::iterator i;
        for(i = m_flights.flights.begin(); i != m_flights.flights.end(); i++) {
            FlightInfo &info = i->second;
            if(hex == info.hex)
                break;
        }

        if(i != m_flights.flights.end())
            m_lFlights->DeleteItem(index);
        else
            index++;
    }
    
    // update m_lFlights from m_flights
    for(std::map<int, FlightInfo>::iterator i = m_flights.flights.begin(); i != m_flights.flights.end(); i++) {
        FlightInfo &info = i->second;
        for(index=0; index<m_lFlights->GetItemCount(); index++) {
            long hex = 0;
            m_lFlights->GetItemText(index, HEX).ToCLong(&hex, 16);
            if(hex == info.hex)
                break;
        }

        if(index == m_lFlights->GetItemCount()) {
            if(info.Lat==0 && info.Lon==0)
                continue;
            
            wxListItem item;
            index = m_lFlights->InsertItem(m_lFlights->GetItemCount(), item);
        }

        m_lFlights->SetItem(index, HEX, wxString::Format("%x", info.hex));
        m_lFlights->SetItem(index, NAME, info.Name);
        m_lFlights->SetItem(index, HDG, wxString::Format("%.0f", info.Hdg));
        m_lFlights->SetItem(index, SPD, wxString::Format("%.0f", info.Speed));
        m_lFlights->SetItem(index, LAT, wxString::Format("%f", info.Lat));
        m_lFlights->SetItem(index, LON, wxString::Format("%f", info.Lon));
        m_lFlights->SetItem(index, ALT, wxString::Format("%f", info.Altitude));
        m_lFlights->SetItem(index, AGE, wxString::Format("%d", (wxDateTime::Now() - info.age).GetSeconds()));
        m_lFlights->SetItem(index, MSGS, wxString::Format("%d", info.messages));
    }

    for(int i=0; i<MSGS; i++)
        m_lFlights->SetColumnWidth(i, wxLIST_AUTOSIZE);
}
