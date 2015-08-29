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

#include "pff_header.h"
#include "pff_seg.h"
#include <iostream>
using namespace std;

namespace pff
{
  //Set the defaults to the pff3 format or user specified
  pff_header::pff_header(int hsize,char* id,int files,int fsize, int floc)
  {
    SizeOfHeader = hsize;
    PFFID = id;
    NumOfFiles = files;
    FileSegSize = fsize;
    FileListLoc = floc;
  }

  //Deconstructor
  pff_header::~pff_header()
  {
  }


  //Set the size of the header.
  void pff_header::SetSizeOfHeader(int size)
  {
    SizeOfHeader = size;
  }

  //Get the size of the header.
  int pff_header::GetSizeOfHeader()
  {
    return SizeOfHeader;
  }

  //Set the pffs id.
  void pff_header::SetPFFID(char* id)
  {
    PFFID = id;
  }

  //Get the pffs id.
  const char* pff_header::GetPFFID()
  {
    return PFFID;
  }

  //Get the number of files.
  void pff_header::SetNumberOfFiles(int files)
  {
    NumOfFiles = files;
  }

  //Set the number of files.
  int pff_header::GetNumberOfFiles()
  {
    return NumOfFiles;
  }

  //Set the size of the file segments.
  void pff_header::SetFileSegmentSize(int size)
  {
    FileSegSize = size;
  }

  //Get the size of the file segments.
  int pff_header::GetFileSegmentSize()
  {
    return FileSegSize;
  }

  //Set the offset location of the file list.
  void pff_header::SetFileListOffset(int offset)
  {
    FileListLoc = offset;
  }

  //Get the offset location of the file list.
  int pff_header::GetFileListOffset()
  {
    return FileListLoc;
  }

  //Generate Header by the id of the pff you want
  void pff_header::GenerateHeaderByID(int id)
  {
    pff_seg seg;
    switch(id)
    {
      //Still unknow
      //case PFF1:
        //SetPFFID("PFF1");
      //  break;
      case PFF2:
        SetPFFID("PFF3"); //pff2 has the same id but the seg size changes
        SetFileSegmentSize(seg.GetSegSizeByPffID(PFF2));
        break;
      case PFF3:
        SetPFFID("PFF3");
        SetFileSegmentSize(seg.GetSegSizeByPffID(PFF3));
        break;
      case PFF4:
        SetPFFID("PFF4");
        SetFileSegmentSize(seg.GetSegSizeByPffID(PFF4));
        break;

        default: //Defaults to pff3 format
          SetPFFID("PFF3");
          SetFileSegmentSize(seg.GetSegSizeByPffID(PFF3));
    }
    //sofar all pff formats have this same info.
    SetSizeOfHeader(0x14);
    SetNumberOfFiles(0);
    SetFileListOffset(0x14);
  }

  void pff_header::print()
  {
    cout << "HEADER SIZE: " << GetSizeOfHeader() << endl;
    cout << "PFF ID:      " << GetPFFID()[0] << GetPFFID()[1] << GetPFFID()[2] << GetPFFID()[3] << endl;
    cout << "TOTAL FILES: " << GetNumberOfFiles() << endl;
    cout << "SEG SIZE:    " << GetFileSegmentSize() << endl;
    cout << "OFFSET:      " << hex << GetFileListOffset() << endl;
    cout << dec << flush;
  }
}
