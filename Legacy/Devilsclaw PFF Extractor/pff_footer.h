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

#ifndef PFF_FOOTER_H
#define PFF_FOOTER_H

#ifdef WIN32
#include <winsock2.h>
#pragma comment(lib,"WS2_32.lib")
#endif

#ifdef LINUX
#include <netdb.h>
#include <arpa/inet.h>
extern int h_errno;
#endif

#include <iostream>
using namespace std;

namespace pff
{
  class pff_footer
  {
    public:
      pff_footer();
      ~pff_footer();
      void SetIP(int ip);
      int GetIP();
      void SetReserved(int r);
      int GetReserved();
      const char* GetKingTag();
      void SetKingTag(const char* k);
      bool SetSystemsIP();
      void print();

    private:
      int SystemIP;         //Stores the current system ip
      int Reserved;         //Stores Nothing that i know of
      const char* KINGTag;     //Stores the KING tag
  };
}
#endif
