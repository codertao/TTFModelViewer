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

#ifndef PFF_HEADER_H
#define PFF_HEADER_H

#include "pff_seg.h"
#include "debug_level.h"
namespace pff
{
  #define PFF1 0
  #define PFF2 1
  #define PFF3 2
  #define PFF4 3


  class pff_header
  {
    public:
      pff_header(int hsize = 0x14,char* id = "PFF3",int files = 0,int fsize = 0x20, int floc = 0);
      ~pff_header();
      void  SetSizeOfHeader(int size);
      int   GetSizeOfHeader();
      void  SetPFFID(char* id);
      const char* GetPFFID();
      void  SetNumberOfFiles(int files);
      int   GetNumberOfFiles();
      void  SetFileSegmentSize(int size);
      int   GetFileSegmentSize();
      void  SetFileListOffset(int offset);
      int   GetFileListOffset();
      void  GenerateHeaderByID(int id);
      void  print();
    private:
      int  SizeOfHeader;  //Size of the PFF header
      const char* PFFID;      //Stores file identifier PFF
      int  NumOfFiles;    //Stores the number of files in the pff
      int  FileSegSize;   //Stores segment size of the file descriptor
      int  FileListLoc;   //Stores the location of the first file in the PFF
  };
}
#endif
