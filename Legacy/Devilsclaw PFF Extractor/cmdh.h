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

#ifndef CMDH_H
#define CMDH_H


#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <fstream>
#include <cstdlib>
#include "debug_level.h"
using namespace std;

#define INDIR_ONLY 1
#define OUTDIR_ONLY 2
#define ALL_DIRS 3

struct argcv
{
  unsigned int argc;
  char** argv;
};

struct CommandDB
{
  char*     cmdPrimary;     //store commands like --help
  char*     cmdSecondary;   //stores commands like -h
  int       numParams;      //number of params
  int       ParamPos;     //param possition start
  int       cmdPriority;    //cmd pririty
  int       cmdGroup;     //cmd group
  bool      cmdEnabled;     //cmd Enabled
  void *      cmdRef;       //Reference Address to function for command
};

class CmdLineH
{
  public:
    CmdLineH();
    ~CmdLineH();

    bool  InitializeCommandHandler(int argc,char *argv[]);
    bool  AddCommand(char * cmdPrimary,char * cmdSecondary,int cmdGroup,int cmdPriority,int numParams,void * cmdRef);
    bool  CompareCommands();
    bool  RunCommand(int cmdGroup);
    char* GetExecName();
    char* GetFirstFile();
    char* GetNextFile();
    void  SetDirectories(int Dirs);
    char* GetInDirecotry();
    char* GetOutDirecotry();
    void  Param_1_Enabled();

  private:
    debug_level dbglevel;
    //bool VERBOSE;
    //bool DEBUG;
    bool INDIR;
    bool OUTDIR;
    bool PARAM_1_INCLUDED;

  //This is needed for everything to work
    int ArgC;     //holds the number of items in the command line (executable name)+(commands)+(parameters)
    char** ArgV;    //similar to how the params db is built this points to a index that points to the offsets of what ever is passed to the program

  //This is everything needed to build the proper cmd database
    CommandDB* cmdDB; //After all said and done this is the Commands database
    int TotalCmds;    //this holds the total number of commands actually added to the database
    int TotalCmdParams; //this holds the number of parameters are needed all together for the commands

  //This is everthing needed to build the proper params database
    char** paramsDB;  //After all said and done this is the Params Data Base
    int TotalParams;  //this holds the total number of paramaters passed
    int curntparam;

    int  BindParams();
    int  GetParamReq();

    int  GetMaxPriority();
    bool CreateDBs();
};

#endif
