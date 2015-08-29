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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "debug_level.h"
#include "pff_file.h"
using namespace pff;

#include "cmdh.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <fstream>
#include <cstdlib>
using namespace std;


#ifdef WIN32
#include <direct.h>
//#pragma comment(lib,"cmdh_build.lib")
//#pragma comment(lib,"pffh_build.lib")
#endif

#ifdef LINUX
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/errno.h>
#endif

#include "cmdh.h"
extern CmdLineH cmdh;
extern pff_file pffh;
extern debug_level dbglevel;
extern char STR_VERSION[];

void cmd_verbose(void* Input)
{
  dbglevel.SetDebugLevel(1);
  if(dbglevel.GetDebugLevel() >= 2)cout << "Verbose: on" << endl;
}

void cmd_debug(void* Input)
{
  dbglevel.SetDebugLevel(2);
  cout << "DEBUG: on" << endl;
}

void cmd_version(void* Input)
{
  cout << "Version: " << STR_VERSION << endl;
}

void cmd_help(void* Input)
{
  cout << "Usage: pff_new -xaimcvVlzZdD [pff] [directory('s)] {files}\n";
  cout << "\n";
  cout << "  -v, --verbose      Enables verbose output." << endl;
  cout << "      --debug        Enables debug output." << endl;
  cout << "  -V, --version      Displays version info." << endl;
  cout << "  -h, --help         Displays this message." << endl;
  cout << "  -l, --list         Outputs a list of files in the PFF." << endl;
  cout << "                     eg. pff_new -l example.pff file1 file2" << endl << endl;
  cout << "  -t, --pfftype      Lets you set the PFF type(PFF2,PFF3,PFF4)." << endl;
  cout << "                     eg. pff_new -t PFF3 -c example.pff" << endl << endl;
  cout << "  -c, --create       Creates a PFF." << endl;
  cout << "                     eg. pff_new -c example.pff" << endl << endl;
  //cout << "  -C, --compress    Compresses files going into the PFF(PFF4 ONLY)." << endl;
  cout << "  -i, --insert       Tells the program what files to pack into the PFF." << endl;
  cout << "                     eg. pff_new -i example.pff file1 file2" << endl << endl;
  //cout << "  -Z                Compresses all the files currently in the PFF." << endl;
  cout << "  -o, --outdir       Tells the program where to extract the files to." << endl;
  cout << "                     eg. pff_new -o -x example.pff outdir files" << endl << endl;
  //cout << "  -d, --decompress  Decompress files on extraction." << endl;
  cout << "  -x, --extract      Extract files from the PFF." << endl;
  cout << "                     eg. pff_new -x example.pff file1 file2" << endl << endl;
  //cout << "  -D                Decompress all files while in the PFF." << endl;
  cout << "  -n, --nologo       Disables the Devilsclaw Logo" << endl;
  //cout << "  -M, --merge        Merge two pff files into one." << endl;
  cout << "  -m, --mark-deleted Marks a file as deleted. with out removing it." << endl;
  cout << "                     eg. pff_new -m example.pff file1 file2" << endl << endl;
  cout << "  -O, --optimize     Deletes all files marked to be deleted permanently." << endl;
  cout << "                     eg. pff_new -O example.pff" << endl << endl;
  cout << "Usage Examples:" << endl;
  cout << "     Create and insert: pff_new -ci file.pff file.pff insert_file.dat" << endl;
}

void cmd_list(void* Input)
{
  if(Input)
  {
    char** tmpInput = (char**)Input;
    pffh.open(tmpInput[0]);
    char* tmp = cmdh.GetFirstFile();
    if(tmp)
    {
      do
      {
        pffh.ListFiles(tmp);
        tmp = cmdh.GetNextFile();
      }while(tmp);
    }
  }
}

void cmd_pfftype(void* Input)
{
  char** tmpInput = (char**)Input;
  //pffh.ParseFullName("PFF1",tmpInput[0])pffh.SetPFFType(PFF1);
  if(pffh.ParseFullName("PFF2",tmpInput[0]))pffh.SetPFFType(PFF2);
  else if(pffh.ParseFullName("PFF3",tmpInput[0]))pffh.SetPFFType(PFF3);
  else if(pffh.ParseFullName("PFF4",tmpInput[0]))pffh.SetPFFType(PFF4);
  else pffh.SetPFFType(PFF3);
}

void cmd_create(void* Input)
{
  if(Input)
  {
    char** tmpInput = (char**)Input;
    char* outdir = cmdh.GetOutDirecotry();
    if(outdir)
    {
      cout << "OUT DIR: " << endl;
#ifdef LINUX
      mkdir(outdir,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#else
#ifdef WIN32
      mkdir(outdir);
#endif
#endif
    }

    if(!pffh.create(tmpInput[0],false))
    {
      char yn = 0;
      cout << "Either Pff already exists or failed to open\n"
          << "Would you like to try to force it? (y/n):" << flush;
      cin >> yn;
      if(yn == 'y')
      {
        if(!pffh.create(tmpInput[0],true))
        {
          cout << "Was not able to force it." << endl;
        }
      }
    }
  }
}

// void cmd_compress(void* Input)
// {
// }

void cmd_insert(void* Input)
{
  if(Input)
  {
    char** tmpInput = (char**)Input;
    pffh.open(tmpInput[0]);
    char* tmp = cmdh.GetFirstFile();
    if(tmp)
    {
      do
      {
        pffh.InsertFile(tmp);
        tmp = cmdh.GetNextFile();
      }while(tmp);
    }
  }
}

// void cmd_remove(void* Input)
// {
//   if(Input)
//   {
//     char** tmpInput = (char**)Input;
//     pffh.open(tmpInput[0]);
//     char* tmp = cmdh.GetFirstFile();
//     if(tmp)
//     {
//       do
//       {
//         pffh.RemoveFile(tmp);
//         tmp = cmdh.GetNextFile();
//       }while(tmp);
//     }
//   }
// }

void cmd_markdeleted(void* Input)
{
  if(Input)
  {
    char** tmpInput = (char**)Input;
    pffh.open(tmpInput[0]);
    char* tmp = cmdh.GetFirstFile();
    if(tmp)
    {
      do
      {
        pffh.MarkDeleted(tmp);
        tmp = cmdh.GetNextFile();
      }while(tmp);
    }
  }
}

// void cmd_compall(void* Input)
// {
//
// }

void cmd_outdir(void* Input)
{
  cmdh.SetDirectories(OUTDIR_ONLY);
}

// void cmd_decompress(void* Input)
// {
// }

void cmd_extract(void* Input)
{
  if(Input)
  {
    char** tmpInput = (char**)Input;
    pffh.open(tmpInput[0]);
    char* tmp = cmdh.GetFirstFile();
    if(tmp)
    {
      char* outdir = cmdh.GetOutDirecotry();
      if(outdir)
      {
#ifdef LINUX
        mkdir(outdir,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#else
#ifdef WIN32
        mkdir(outdir);
#endif
#endif
      }

      do
      {
        pffh.ExtractFiles(tmp,outdir);
        tmp = cmdh.GetNextFile();
      }while(tmp);
    }
  }
}

// void cmd_decompall(void* Input)
// {
// }

void cmd_optimize(void* Input)
{
  if(Input)
  {
    char** tmpInput = (char**)Input;
    string stmp = tmpInput[0];
    stmp += "XXXXXX";
    char* tmp = new char[stmp.size()];
    stmp.copy(tmp,stmp.size(),0);
    pffh.open(tmpInput[0]);
    pffh.OptimizePFF(tmp);
    pffh.close();
    remove(tmpInput[0]);
    rename(tmp,tmpInput[0]);
    //pffh.close();
  }
}

void cmd_merge(void* Input)
{
  if(Input)
  {
    char** tmpInput = (char**)Input;
  }
}
