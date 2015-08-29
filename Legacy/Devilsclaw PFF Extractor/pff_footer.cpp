/***************************************************************************
 *   Copyright (C) 2007 by Neil Hellfeldt   *
 *   devilsclaw@devilsclaws.net   *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "pff_footer.h"

namespace pff
{
  //Constructor Set the const value on intialization
  pff_footer::pff_footer()
  {
    KINGTag = "KING";
    //Default set the footers ip to this systems ip
    if(!SetSystemsIP())
    {
      SystemIP = 0;
    }
    Reserved = 0; //this needs to be 0 since its never actually used.
  }

  //Deconstructor
  pff_footer::~pff_footer()
  {
  }

  //Sets the Footers IP.
  void pff_footer::SetIP(int ip)
  {
    SystemIP = ip;
  }

  //Get Footers IP.
  int pff_footer::GetIP()
  {
    return SystemIP;
  }

  //Added so i can manipulate if i want to
  void pff_footer::SetReserved(int r)
  {
    Reserved = r;
  }

  //need this for when i write the footer info
  int pff_footer::GetReserved()
  {
    return Reserved;
  }

  //get the kink tag
  const char* pff_footer::GetKingTag()
  {
    return KINGTag;
  }

  //this should never be needed but they might change the file format someday.
  void pff_footer::SetKingTag(const char* k)
  {
    KINGTag = k;
  }

  //Use Current systems ip.
  bool pff_footer::SetSystemsIP()
  {
    //allocate some memory for the host name of this machine.
    char hostname[256] = {0};

    //fill hostname with this systems host name.
    if(!gethostname(hostname,sizeof(hostname)))
    {
      //based off the host name get this systems ip.
      hostent* host = gethostbyname(hostname);
      if(host)
      {
        //Move the ip of this system into SystemIP
        memcpy(&SystemIP,host->h_addr_list[0],sizeof(long)); //Assign the systems ip.

      //Return False if we could not get the host info based off host name.
      }else return false;
    //Return False if we could not get host name.
    }else return false;

    //if all went well return true.
    return true;
  }

  void pff_footer::print()
  {
    int ip = GetIP();
    cout << "IP:          " << (int)((unsigned char*)(&ip))[0] << "." << (int)((unsigned char*)(&ip))[1] << "." << (int)((unsigned char*)(&ip))[2] << "." << (int)((unsigned char*)(&ip))[3] <<endl;
    cout << "RESERVED:    " << GetReserved() << endl;
    cout << "KING TAG:    " << GetKingTag()[0] << GetKingTag()[1] << GetKingTag()[2] << GetKingTag()[3] << endl;
  }
}
