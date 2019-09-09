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

#include <wx/socket.h>

#include <list>
#include <map>

struct FlightInfo
{
    FlightInfo() {
        Altitude = Speed = Hdg = Lat = Lon = Signal = 0;
        messages = on_the_ground = 0;
    }
    unsigned int hex;
    enum Mode {S} mode;
    wxDateTime datetime;
    int msg_type; // ?
    wxString Name;
    double Altitude; // in meters
    double Speed; // in knots
    double Hdg; // true north
    double Lat, Lon;
    double Signal;
    int messages;
    int on_the_ground;

    wxDateTime age, position_age;
};

class Flights : public wxEvtHandler
{
public:
    Flights();
    void connect(wxString host);
    void disconnect();
    bool connected() { return sock.IsConnected(); }

    void OnSocketEvent(wxSocketEvent& event);

    std::string         sock_buffer;
    std::map<int, FlightInfo> flights;
    wxSocketClient       sock;

    DECLARE_EVENT_TABLE()
};
