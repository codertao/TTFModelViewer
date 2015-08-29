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

#ifndef PFF_FILE_H
#define PFF_FILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include "pff_seg.h"
#include "pff_header.h"
#include "pff_footer.h"
#include "debug_level.h"
using namespace std;

namespace pff
{
  class pff_file : public pff_footer, pff_header, pff_seg
  {
    public:
      pff_file();
      ~pff_file();
      bool create(const char* filename,bool overwrite);
      bool open(const char* filename);
      void close();
      bool save();
      bool WriteHeader(fstream &fs);
      bool ReadHeader(fstream &fs);
      bool WriteFooter(fstream &fs);
      bool ReadFooter(fstream &fs);
      void ReadFiles(fstream &fs);
      void WriteFiles(fstream &fs);
      void ExtractFiles(char* pattern, char* directory);
      void ListFiles(char* pattern);
      bool ParseName(char* StringInput,char* StringLookFor);
      bool ParseFullName(char* StringInput,char* StringLookFor);
      bool InsertFile(char* sFile);
      bool OptimizePFF(char* sFile);
      void SetPFFType(int pfftype);
      void MarkDeleted(char* pattern);
    private:
      fstream file;
      debug_level dbglevel;
      int PFFTYPE;
      vector<pff_seg> file_segs;
      bool WriteHeader();
      bool ReadHeader();
      bool WriteFooter();
      bool ReadFooter();
      void ReadFiles();
      void WriteFiles();
      void PrintFileInfo(size_t filenumber);
  };
}
#endif
