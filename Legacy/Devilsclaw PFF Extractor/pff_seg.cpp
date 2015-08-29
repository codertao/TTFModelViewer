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

#include "pff_seg.h"

namespace pff
{
  pff_seg::pff_seg()
  {
    Deleted = 0;
    FileLocation = 0;
    FileSize = 0;
    PackedDate = 0;
    for(int i = 0;i < 0;i++)FileName[i]=0;
    ModifiedDate = 0;
    CompressionLevel = 0;
  }

  pff_seg::~pff_seg()
  {
  }

  void pff_seg::SetDeleted(int d)
  {
    Deleted = d;
  }

  int pff_seg::GetDeleted()
  {
    return Deleted;
  }

  void pff_seg::SetFileLocation(int l)
  {
    FileLocation = l;
  }

  int pff_seg::GetFileLocation()
  {
    return FileLocation;
  }

  void pff_seg::SetFileSize(int s)
  {
    FileSize = s;
  }

  int pff_seg::GetFileSize()
  {
    return FileSize;
  }

  void pff_seg::SetPackedDate(time_t t)
  {
    PackedDate = t;
  }

  time_t pff_seg::GetPackedDate()
  {
    return PackedDate;
  }

  void pff_seg::SetFileName(char* f)
  {
    size_t size = strlen(f);
    for(size_t i = 0;i < 0x10;i++)
    {
      if(i < size)
      {
        FileName[i] = f[i];
      }
      else FileName[i] = 0;
    }
  }

  char* pff_seg::GetFileName()
  {
    return FileName;
  }

  void pff_seg::SetModifiedDate(int d)
  {
    ModifiedDate = d;
  }

  int pff_seg::GetModifiedDate()
  {
    return ModifiedDate;
  }

  void pff_seg::SetCompressionLevel(int c)
  {
    CompressionLevel = c;
  }

  int pff_seg::GetCompressionLevel()
  {
    return CompressionLevel;
  }

  int pff_seg::GetSegSizeByPffID(int pffid)
  {
    switch(pffid)
    {
      //case PFF1:
      //  return 0;
      //  break;
      case PFF2:
        return 0x20;
        break ;
      case PFF3:
        return 0x24;
        break;
      case PFF4:
        return 0x28;
        break;
	  default:
		  return 0;
    }
  }
}
