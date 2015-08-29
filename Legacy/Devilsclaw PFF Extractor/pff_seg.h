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

#ifndef PFF_SEG_H
#define PFF_SEG_H

#include <ctime>
#include <vector>
using namespace std;
#include "pff_header.h"

namespace pff
{
  class pff_seg
  {
    public:
      pff_seg();
      ~pff_seg();
      void   SetDeleted(int d);
      int    GetDeleted();
      void   SetFileLocation(int l);
      int    GetFileLocation();
      void   SetFileSize(int s);
      int    GetFileSize();
      void   SetPackedDate(time_t t);
      time_t GetPackedDate();
      void   SetFileName(char* f);
      char*  GetFileName();
      void   SetModifiedDate(int d);
      int    GetModifiedDate();
      void   SetCompressionLevel(int c);
      int    GetCompressionLevel();
      int    GetSegSizeByPffID(int pffid);

    private:
      int    Deleted;    //delete trigger 1 = deleted, 0 = Not deleted
      int    FileLocation;    //File Offset
      int    FileSize;      //File Size
      time_t PackedDate;      //Date the file was first packed into pff
      char   FileName[0x10];//File Name
      int    ModifiedDate;   //Repack Date
      int    CompressionLevel;     //Compression Level;
  };
}
#endif
