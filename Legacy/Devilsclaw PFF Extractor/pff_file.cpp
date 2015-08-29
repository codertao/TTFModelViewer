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

#include <iostream>
#include <iomanip>
#include <fstream>
#include "pff_file.h"
using namespace std;
using std::fstream;

namespace pff
{
   //////////////////////////////////////////////
   //Initialization and Deinitialization code: //
   //////////////////////////////////////////////

  //initialize pff_file , pff_footer , pff_header
   pff_file::pff_file() : pff_footer(), pff_header(), pff_seg()
   {
   }

   pff_file::~pff_file()
   {
      file.close();
   }

   void pff_file::SetPFFType(int pfftype)
   {
      PFFTYPE = pfftype;
   }

   ///////////////////////////
   //PFF Manipulation Code: //
   ///////////////////////////

  //open a pff file.
   bool pff_file::open(const char* filename)
   {
    //open pff file
      file.open(filename,fstream::in | fstream::out
#ifdef WIN32
            | ios_base::_Nocreate
#endif
            | fstream::binary);

    //check status of file or errors`
      if(!file.fail() && file.is_open())
      {
      //check file size
         if(file.seekg(0,fstream::end))
         {
        //set it back to the star of the file.
            ReadHeader();
            ReadFooter();
            ReadFiles();
         }
      //if the file has a size of 0 return false
         else return false;

      }
    //if file failed or was not open return false.
      else return false;

    //if everything went ok return true.
      return true;
   }

  //close pff file.
   void pff_file::close()
   {
      file << flush;
      file.close();
   }

  //force the data to save pff now.
   bool pff_file::save()
   {
      file << flush;
      return true;
   }

  //create a new pff file. fail if pff file already exist
   bool pff_file::create(const char* filename,bool overwrite)
   {
    //try opening the file
      file.open(filename,fstream::in | fstream::out
#ifdef WIN32
            | ios_base::_Noreplace | ios_base::_Nocreate
#endif
            | fstream::binary);

      file.close();
      if(file.fail() || overwrite) //check to see if there were any errors while opening it.
      {
         file.clear();
      //if there were errors good the file didnt exist
      //now we try to create it
         file.open(filename,fstream::in | fstream::out
#ifdef WIN32
               | ((overwrite)? 0 : ios_base::_Noreplace)
#endif
               | fstream::binary | fstream::trunc);
         if(!file.fail() && file.is_open())
         {
        //Create the pff header
            GenerateHeaderByID(PFFTYPE);
            WriteHeader();
            WriteFooter();
            file.close();
            file.clear();
         }
      //if there was an error or if the file is not open then return false and close file stream.
         else
         {
            file.close();
            return false;
         }
      }
    //if the file exist then return false.
      else return false;
    //if all went well then return true.
      return true;
   }

  //TODO: Make insert look for other deleted files and see if it can hold the new file.
  //also make it check same file name if it will fit. if it finds space have it update
  //the dead space for new size and location.
   bool pff_file::InsertFile(char* sFile)
   {
      size_t seg_counter = 0;
      for(;seg_counter < file_segs.size();seg_counter++)
      {
         if(ParseFullName(file_segs[seg_counter].GetFileName(),sFile))
         {
            file_segs[seg_counter].SetDeleted(true);
            file_segs[seg_counter].SetFileName("<DEADSPACE>");
            break;
         }
      }

      ifstream xfile(sFile,fstream::in | fstream::binary);
      if(!xfile.fail())
      {
         pff_seg temp_seg;
         temp_seg.SetDeleted(0);
         temp_seg.SetFileLocation(GetFileListOffset());
         xfile.seekg(0,fstream::end);
         temp_seg.SetFileSize(xfile.tellg());
         temp_seg.SetPackedDate(time(0));
         temp_seg.SetFileName(sFile);
         if(PFFTYPE >= PFF3)
         {
            temp_seg.SetModifiedDate(0);
            if(PFFTYPE >= PFF4)
            {
               temp_seg.SetCompressionLevel(0);
            }
         }

         file_segs.push_back(temp_seg);

         file.seekp(GetFileListOffset());

         SetNumberOfFiles(GetNumberOfFiles() + 1);
         SetFileListOffset(GetFileListOffset() + temp_seg.GetFileSize());

         xfile.seekg(0,fstream::beg);
         for(int counter = 0; counter < temp_seg.GetFileSize();counter++)
         {
            file.put(xfile.get());
         }

      } else return false;
      WriteHeader();
      WriteFiles();
      WriteFooter();
      xfile.close();
      return true;
   }

  //This tells the game the file is not supposed to be used
  //Marks it to be removed my the OptimizePFF code.
   void pff_file::MarkDeleted(char* sFile)
   {
      if(sFile && strlen(sFile))
      {
         for(size_t seg_counter = 0;seg_counter < file_segs.size();seg_counter++)
         {
            if(ParseName(file_segs[seg_counter].GetFileName(),sFile))
            {
               file_segs[seg_counter].SetDeleted(true);
               file_segs[seg_counter].SetFileName("<DEADSPACE>");
          //break;
            }
         }
         WriteFiles();
      }
   }

  //Create a new pff that has the marked deleted file removed from it.
  //then deletes the original and puts the new one in its place.
   bool pff_file::OptimizePFF(char* sFile)
   {

      for(vector<pff_seg>::iterator seg = file_segs.begin();!file_segs.empty() && seg != file_segs.end();seg++)
      {
         if(seg->GetDeleted())
         {
            file_segs.erase(seg);
            seg = file_segs.begin();
         }
      }

      SetFileListOffset(GetSizeOfHeader());
      SetNumberOfFiles(0);

      fstream xfile(sFile,fstream::in | fstream::out  | fstream::binary | fstream::trunc);
      if(!xfile.fail())
      {
         WriteHeader(xfile);

         if(!file_segs.empty())
         {
        //xfile.seekg(0,fstream::beg);
            for(size_t seg_counter = 0;seg_counter < file_segs.size();seg_counter++)
            {
               file.seekg(file_segs[seg_counter].GetFileLocation());
               file_segs[seg_counter].SetFileLocation(xfile.tellp());
               for(int counter = 0; counter < file_segs[seg_counter].GetFileSize();counter++)
               {
                  xfile.put(file.get());
               }
               SetNumberOfFiles(GetNumberOfFiles() + 1);
            }
            WriteFiles(xfile);
         }
         WriteHeader(xfile);
         WriteFooter(xfile);
      } else return false;
      xfile.close();
      return true;
   }

   void pff_file::PrintFileInfo(size_t filenumber)
   {
      cout << "File: " << file_segs[filenumber].GetFileName();
      cout << "     \tSize: " << file_segs[filenumber].GetFileSize();
      time_t ttime = file_segs[filenumber].GetPackedDate();
      cout << "\tPacked Date: " << ctime(&ttime);
   }

  //This list files in the pff based off a search pattern
   void pff_file::ListFiles(char* pattern)
   {
      if(pattern && strlen(pattern))
      {
         for(size_t i = 0; i < file_segs.size();i++)
         {
            if((pattern[0] == '.' && strlen(pattern) == 1)|| ParseName(file_segs[i].GetFileName(),pattern))
            {
               if(!file_segs[i].GetDeleted())
               {
                  PrintFileInfo(i);
               }
            }
         }
      }
   }

  //this extracts files from a pff based off a search patter and director
   void pff_file::ExtractFiles(char* pattern, char* directory)
   {
      fstream xfile;
      for(size_t seg_counter = 0;seg_counter < file_segs.size();seg_counter++)
      {
         if(((pattern[0] == '.' && strlen(pattern) == 1)|| ParseName(file_segs[seg_counter].GetFileName(),pattern)) && !file_segs[seg_counter].GetDeleted())
         {
            if(dbglevel.GetDebugLevel() >= 1)
            {
               PrintFileInfo(seg_counter);
            }

            if(!(directory && strlen(directory)))
            {
               xfile.open(file_segs[seg_counter].GetFileName(),fstream::binary | fstream:: out | fstream::trunc);
            }
            else
            {
               string stmp;
               stmp = directory;
#ifdef LINUX
               stmp += "/";
#elif WIN32
               stmp += "\\";
#endif
               stmp += file_segs[seg_counter].GetFileName();
               xfile.open(stmp.data(),fstream::binary | fstream:: out | fstream::trunc);
            }
            if(!xfile.fail())
            {
               xfile.seekp(0,fstream::beg);
               file.seekg(file_segs[seg_counter].GetFileLocation(),fstream::beg);
               for(int file_counter = 0;file_counter < file_segs[seg_counter].GetFileSize();file_counter++)
               {
                  xfile.put(file.get());
               }
            } else cout << "Could not create " << file_segs[seg_counter].GetFileName() << "!" << endl;
            xfile.close();
         }
      }
   }

   ////////////////////////////
   //STRING COMPARISON CODE: //
   ////////////////////////////

  //this does a partial look if what you looking for is in any part of the string it returns true.
   bool pff_file::ParseName(char* StringInput,char* StringLookFor)
   {
      size_t strSize = strlen(StringInput);
      size_t str2Size = strlen(StringLookFor);

      for(size_t i = 0; i < strSize;i++)
      {
         for(size_t i2 = 0; i2 < str2Size;i2++)
         {
            if(i2+1 == str2Size && tolower(StringInput[i+i2]) == tolower(StringLookFor[i2]))return true;
            else if(tolower(StringInput[i+i2]) != tolower(StringLookFor[i2]))break;
         }
      }

      return false;
   }

  //this returns only if the entire string matches.
   bool pff_file::ParseFullName(char* StringInput,char* StringLookFor)
   {
      size_t strSize = strlen(StringInput);
      size_t str2Size = strlen(StringLookFor);

      if(strSize == str2Size)
      {
         for(size_t i = 0; i < strSize;i++)
         {
            if(tolower(StringInput[i]) != tolower(StringLookFor[i]))return false;
         }
      }else return false;

      return true;
   }

   /////////////////////////////
   //BASIC PFF HANDLING CODE: //
   /////////////////////////////

  //read the pff header data and if its a supported format
   bool pff_file::ReadHeader(fstream &fs)
   {
      int tint = 0;
      fs.seekg(0,fstream::beg);

      fs.read(((char*)(&tint)),4);
      SetSizeOfHeader(tint);

      char* tchar = new char[5];
      tchar[4] = 0;
      fs.read(tchar,4);
      SetPFFID(tchar);

      fs.read(((char*)(&tint)),4);
      SetNumberOfFiles(tint);

      fs.read(((char*)(&tint)),4);
      SetFileSegmentSize(tint);

      fs.read(((char*)(&tint)),4);
      SetFileListOffset(tint);

      if(dbglevel.GetDebugLevel() >= 2)
      {
         pff_header::print();
      }

    //check to see if it fits any of the supported formats
      if(!strcmp(tchar,"PFF3") && GetSegSizeByPffID(PFF2) == GetFileSegmentSize())PFFTYPE = PFF2;
      else if(!strcmp(tchar,"PFF3") && GetSegSizeByPffID(PFF3) == GetFileSegmentSize())PFFTYPE = PFF3;
      else if(!strcmp(tchar,"PFF4") && GetSegSizeByPffID(PFF4) == GetFileSegmentSize())PFFTYPE = PFF4;
      else return false;

      if(fs.fail())return false;
      else return true;
   }

  //Write the pff header return false if errors
   bool pff_file::WriteHeader(fstream &fs)
   {
      int tint = 0;
      if(dbglevel.GetDebugLevel() >= 2)
      {
         pff_header::print();
      }

    //Set file to start so i can write the header
      fs.seekp(0,fstream::beg);

      tint = GetSizeOfHeader();
      fs.write(((char*)(&tint)),4);

      fs.write(GetPFFID(),4);
      tint = GetNumberOfFiles();

      fs.write(((char*)(&tint)),4);
      tint = GetFileSegmentSize();

      fs.write(((char*)(&tint)),4);
      tint = GetFileListOffset();

      fs.write(((char*)(&tint)),4);
      if(fs.fail())return false;
      else return true;
   }

  //Read All File entries into a vector
   void pff_file::ReadFiles(fstream &fs)
   {
      int tint = 0;
      char tchar[0x10] = {0};

      fs.seekg(GetFileListOffset(),fstream::beg);
      for(int i = 0;i < GetNumberOfFiles();i++)
      {
         pff_seg temp_seg;
         fs.read(((char*)(&tint)),4);
         temp_seg.SetDeleted(tint);

         fs.read(((char*)(&tint)),4);
         temp_seg.SetFileLocation(tint);

         fs.read(((char*)(&tint)),4);
         temp_seg.SetFileSize(tint);

         fs.read(((char*)(&tint)),4);
         temp_seg.SetPackedDate(((time_t)tint));

         fs.read(tchar,0x10);
         temp_seg.SetFileName(tchar);

         if(PFFTYPE > PFF2)
         {
            fs.read(((char*)(&tint)),4);
            temp_seg.SetModifiedDate(tint);
            if(PFFTYPE > PFF3)
            {
               fs.read(((char*)(&tint)),4);
               temp_seg.SetCompressionLevel(tint);
            }
         }
         file_segs.push_back(temp_seg);
      }
   }

  //Write vector entries to a file
   void pff_file::WriteFiles(fstream &fs)
   {
      int tint = 0;
      char tchar[0x10] = {0};

      fs.seekg(GetFileListOffset(),fstream::beg);
      for(size_t i = 0;i < file_segs.size();i++)
      {
         tint = file_segs[i].GetDeleted();
         fs.write(((char*)(&tint)),4);

         tint = file_segs[i].GetFileLocation();
         fs.write(((char*)(&tint)),4);

         tint = file_segs[i].GetFileSize();
         fs.write(((char*)(&tint)),4);

         tint = (int)file_segs[i].GetPackedDate();
         fs.write(((char*)(&tint)),4);

         char* temp_char = file_segs[i].GetFileName();
         fs.write(temp_char,0x10);

         if(PFFTYPE > PFF2)
         {
            tint = file_segs[i].GetModifiedDate();
            fs.write(((char*)(&tint)),4);
            if(PFFTYPE > PFF3)
            {
               tint = file_segs[i].GetCompressionLevel();
               fs.write(((char*)(&tint)),4);
            }
         }
      }
   }

  //Read the pff footer data
   bool pff_file::ReadFooter(fstream &fs)
   {
      int tint = 0;
      fs.seekg((GetFileListOffset() + (GetFileSegmentSize() * GetNumberOfFiles())),fstream::beg);
      fs.read(((char*)(&tint)),4);
      SetIP(tint);
      fs.read(((char*)(&tint)),4);
      SetReserved(tint);

      char* tchar = new char[4];
      fs.read(tchar,4);
      SetKingTag(tchar);

      if(dbglevel.GetDebugLevel() >= 2)
      {
         pff_footer::print();
      }

      if(fs.fail())return false;
      else return true;
   }

  //Write pff footer return false if error
   bool pff_file::WriteFooter(fstream &fs)
   {
      int tint = 0;
      if(dbglevel.GetDebugLevel() >= 2)
      {
         pff_footer::print();
      }

    //Write the footer info
      fs.seekp((GetFileListOffset() + (GetFileSegmentSize() * GetNumberOfFiles())),fstream::beg);
      tint = GetIP();
      fs.write(((char*)(&tint)),4);

      tint = GetReserved();
      fs.write(((char*)(&tint)),4);

      fs.write(GetKingTag(),4);
      if(fs.fail())return false;
      else return true;
   }

  //Private Functions so i dont have to pass a parameter
   bool pff_file::ReadHeader() {return ReadHeader (file);}
   bool pff_file::WriteHeader(){return WriteHeader(file);}
   void pff_file::ReadFiles()  {return ReadFiles  (file);}
   void pff_file::WriteFiles() {return WriteFiles (file);}
   bool pff_file::ReadFooter() {return ReadFooter (file);}
   bool pff_file::WriteFooter(){return WriteFooter(file);}
}
