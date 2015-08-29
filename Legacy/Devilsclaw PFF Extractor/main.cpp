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

#include <iostream>
#include <cstdlib>
#include "pff_file.h"
#include "cmdh.h"
#include "cmds.h"
#include "debug_level.h"
using namespace std;
using namespace pff;

//Global variables
//Its normally not good to use globals but i cant find any other way for this right now
char STR_VERSION[] = "v0.0.0.1";
CmdLineH cmdh;
pff_file pffh;
debug_level dbglevel;

int main(int argc, char *argv[])
{
  cmdh.InitializeCommandHandler(argc,argv);

  //Add commands to the command DB
  //Short, Long, Group, Priority, Parameters, function pointer
  cmdh.AddCommand("-v","--verbose"     ,0,3,0,(void*)&cmd_verbose);    //Verbose Command
  cmdh.AddCommand(   0,"--debug"       ,0,2,0,(void*)&cmd_debug);      //debug Command
  cmdh.AddCommand("-V","--version"     ,0,1,0,(void*)&cmd_version);    //Version Command
  cmdh.AddCommand("-o","--outdir"      ,0,0,0,(void*)&cmd_outdir);     //where to put the extracted files

  cmdh.AddCommand("-h","--help"        ,1,1,0,(void*)&cmd_help);       //Help Command

  cmdh.AddCommand("-l","--list"        ,2,0,1,(void*)&cmd_list);       //List Files

  cmdh.AddCommand("-t","--pfftype"     ,3,0,1,(void*)&cmd_pfftype);    //PFF Type
  cmdh.AddCommand("-c","--create"      ,3,1,1,(void*)&cmd_create);     //Create PFF
  cmdh.AddCommand("-i","--insert"      ,3,3,1,(void*)&cmd_insert);     //add file to pff //TODO: Make it so that insert file can be ran with create pff
  cmdh.AddCommand("-O","--optimize"    ,3,4,1,(void*)&cmd_optimize);   //clean the pff file

  cmdh.AddCommand("-x","--extract"     ,4,2,1,(void*)&cmd_extract);    //extract file from pff

  cmdh.AddCommand("-m","--mark-deleted",5,0,1,(void*)&cmd_markdeleted);//merge two pffs into one

  cmdh.AddCommand("-n","--nologo"      ,10,1,0,0);                     //dont show my header

  //IGNORE FOR NOW:
  //to be added
  //cmdh.AddCommand("-Z",0              ,4,1,1,(void*)&cmd_compall);    //Compress All files in pff
  //cmdh.AddCommand("-d","--decompress" ,5,1,0,(void*)&cmd_decompress); //decompress files while extracting
  //cmdh.AddCommand("-D",0              ,6,0,0,(void*)&cmd_decompall);  //decompress all files in pff
  //cmdh.AddCommand("-r","--remove"     ,7,0,1,(void*)&cmd_remove);      //merge two pffs into one
  //cmdh.AddCommand("-C","--compress"   ,3,2,1,(void*)&cmd_compress);   //Compress files
  //cmdh.AddCommand("-M","--merge"       ,3,4,1,(void*)&cmd_merge);      //merge two pffs into one


   //TODO: if possible move this type of code into the command handling code.
   //IDEA: first sort out by group , then prority and finally check to see what commands run with everything
   //and then run the commands. name the command RunCommands or something like that
  bool ccmd = false;
  if(cmdh.CompareCommands()) //check to see if any thing passed is not really a valid command
  {
    if(!cmdh.RunCommand(10))cout << "Devilsclaw's PFF Utility\n";
    cmdh.RunCommand(0); //verbose command is currently associated with group 0 and we are checking to see if the command was passed
     //this code runs commands by there group number. and keeps hierachy among the command groups and exits when appropreit.
    if(ccmd == false && (ccmd = cmdh.RunCommand(1)) == true){return(1);}
    else if(ccmd == false && (ccmd = cmdh.RunCommand(2)) == true){return(1);}
    else if(ccmd == false && (ccmd = cmdh.RunCommand(3)) == true){return(1);}
    else if(ccmd == false && (ccmd = cmdh.RunCommand(4)) == true){return(1);}
    else if(ccmd == false && (ccmd = cmdh.RunCommand(5)) == true){return(1);}
     //else if(ccmd == false && (ccmd = cmdh.RunCommand(6)) == true){return(1);}
     //else if(ccmd == false && (ccmd = cmdh.RunCommand(8)) == true){return(1);}
    else {return(0);}

  }
  else cmd_help(0);

  return 0;
}
