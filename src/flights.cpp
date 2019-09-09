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

#include <vector>

#include "flights.h"

/*
     Maintain a list of flights from dump1090 <server> 30003
*/

BEGIN_EVENT_TABLE(Flights, wxEvtHandler)
    EVT_SOCKET(-1, Flights::OnSocketEvent)
END_EVENT_TABLE()


Flights::Flights() {
    sock.SetEventHandler(*this);
    sock.SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
    sock.Notify(true);
    sock.SetTimeout(1);
    
}        


void Flights::connect(wxString host)
{
    if(host.empty())
        host = "localhost";
            
    int port = 30003; /* default port */

    wxIPV4address addr;
    addr.Hostname(host);
    addr.Service(port);
    sock.Connect(addr, false);
}

void Flights::disconnect() {
    sock.Close();
};


void Flights::OnSocketEvent(wxSocketEvent& event)
{
    switch(event.GetSocketEvent())
    {
        case wxSOCKET_CONNECTION:
            flights.clear();
            sock_buffer.clear();
            break;

        case wxSOCKET_LOST:
            disconnect();
            break;

        case wxSOCKET_INPUT:
        {
    #define RD_BUF_SIZE    8192
            std::vector<char> data(RD_BUF_SIZE+1);
            event.GetSocket()->Read(&data.front(),RD_BUF_SIZE);
            if(!event.GetSocket()->Error())
            {
                size_t count = event.GetSocket()->LastCount();
                if(count && count < RD_BUF_SIZE)
                {
                    data[count]=0;
                    sock_buffer += (&data.front());
                }
            }

            for(;;) {
                int line_end = sock_buffer.find_first_of("\n");
                if(line_end <= 0)
                    break;
                std::string line = sock_buffer.substr(0, line_end);
                int c = -1, n=0, index;
                FlightInfo info;
                for(index = 0; n < (int)line.length(); index++) {
                    n = c+1;
                    c = line.find(",", n);
                    if(c == -1)
                        break;
                    if(c == n)
                        continue;
                    std::string sub = line.substr(n, c - n);
                    const char *cstr = sub.c_str();
                    switch(index) {
                    case 0: if(sub != "MSG") goto ignore_message; break;
                    case 1: info.msg_type = strtol(cstr, 0, 10); break;
                    case 2: /* 111 */ break;
                    case 3: /* 11111 */ break;
                    case 4:
                        info.hex = strtol(cstr, 0, 16);
                        if(flights.find(info.hex) != flights.end())
                            info = flights[info.hex];
                    break;
                    case 5: /* 111111 */ break;
                    case 6: info.datetime.ParseISODate(sub); break;
                    case 7: // message reception time and date
                    {
                        wxDateTime time;
                        time.ParseISOTime(sub);
                        if(info.datetime.IsValid() && time.IsValid()) {
                            info.datetime.SetHour(time.GetHour());
                            info.datetime.SetMinute(time.GetMinute());
                            info.datetime.SetSecond(time.GetSecond());
                        }
                    } break;
                    case 8: // time and date when dump1090 processed (don't care)
                    case 9:
                        break;
                    case 10: info.Name = sub; break;
                    case 11: info.Altitude = strtod(cstr, 0) / 3.28; break;
                    case 12: info.Speed = strtol(cstr, 0, 10); break;
                    case 13: info.Hdg = strtol(cstr, 0, 10); break;
                    case 14: info.Lat = strtod(cstr, 0); break;
                    case 15: info.Lon = strtod(cstr, 0); info.position_age = wxDateTime::UNow(); break;
                    case 16: // vertical rate
                    case 17: // Squawk
                    case 18: // Squawk Changing Alert flag
                    case 19: // Squawk Changing Alert flag
                    case 20: // Squawk Ident flag (if we have it)
                        break;
                    case 21: // On The Ground Flag
                        info.on_the_ground = sub == "0" ? false : true;
                    default: break;
                    }
                }

                if(info.on_the_ground || info.Speed == 0)
                    goto ignore_message;

                if(index>20) {
                    // add / update flight
                    info.age = wxDateTime::UNow();
                    info.messages++;
                    flights[info.hex] = info;
                }

            ignore_message:

                if(line_end > (int)sock_buffer.size())
                    sock_buffer.clear();
                else
                    sock_buffer = sock_buffer.substr(line_end+1);                
            }

            // remove flights older than 5 minutes
            wxDateTime now = wxDateTime::Now();
            for(std::map<int, FlightInfo>::iterator i = flights.begin(); i != flights.end(); i++)
                if((now - i->second.age).GetSeconds() > 60*5) {
                    flights.erase(i);
                    break;
                }
        } break;
    default:;
    }
}
