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

//TODO: Implement code that will clean up the command handle when finished with it.
#include "cmdh.h"

#ifdef WIN32
#include <windows.h>
#endif

#ifdef ISDLL
  //standard windows dll entry point
BOOL APIENTRY DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{
  if (fdwReason==DLL_PROCESS_ATTACH)
  {
    return true;
  }

  else if (fdwReason==DLL_PROCESS_DETACH)
  {
    return true;
  }
  return false;
}
#endif

//Constructor
CmdLineH::CmdLineH()
{
  //this makes sure everything is initialized even if it does not need to be.
  ArgC = 0;
  ArgV = 0;
  CommandDB* cmdDB = 0;
  TotalCmds = 0;
  TotalCmdParams = 0;
  paramsDB = 0;
  TotalParams = 0;
  INDIR = false;
  OUTDIR = false;
  curntparam = 0;
  PARAM_1_INCLUDED = false;
}

//Deconstructor
CmdLineH::~CmdLineH()
{
}

//this says that even the first parameter is to be
//included in the scan since we might use this for no commandling parsing
void CmdLineH::Param_1_Enabled()
{
  PARAM_1_INCLUDED = true;
}

//this sets up what directories you plan on taking in.. input ouput or both.
void CmdLineH::SetDirectories(int Dirs)
{
  //this code check to see what type of directories your going to use.
  //if any or all are chosen then set either INDIR, OUTDIR or both to true.
  if(Dirs == INDIR_ONLY || Dirs == ALL_DIRS){INDIR = true;}
  if(Dirs == OUTDIR_ONLY || Dirs == ALL_DIRS){OUTDIR = true;}
}

//this gets the input directory..
char* CmdLineH::GetInDirecotry()
{
  //even though the above tells us that the program wants to use indir we check to see if it really does
  //if it does then reutrn the first item in the file list and assume it to be the directory..
  if(INDIR)return(paramsDB[TotalCmdParams]);
  else return(0); //if not wanted then return 0
}

//this gets the out put directory.
char* CmdLineH::GetOutDirecotry()
{
  //now the out directory can have two possitions. it can be the first file possition or the second depending on if
  //indir was wanted also if not then pass the first possition if indir was wanted also then assume second possition
  if(INDIR)return(paramsDB[TotalCmdParams+1]);
  else if(OUTDIR) return(paramsDB[TotalCmdParams]);
  else return(0); //if not wanted then return 0
}

//this code gets the first file in the file list
char* CmdLineH::GetFirstFile()
{
  //if debug was toggled on then we want to show this info
  if(dbglevel.GetDebugLevel() >= 2)
  {
    cout << "total cmd params: " << TotalCmdParams << endl;
    cout << "total params: " << TotalParams << endl;
  }

  //if total params is greater then the parameters needed by the commands then assume its the file list
  if(TotalParams > TotalCmdParams)
  {
    curntparam = TotalCmdParams; //adjust the possition of the params db
    if(INDIR)curntparam += 1; //adjust based off if indir was wanted
    if(OUTDIR)curntparam += 1; //adjust based off if outdir was wanted
    return((char*)paramsDB[curntparam]); //now that all calcs are done assume this possition is the file list and return it
  }
  else return(0); //if totalparams are smaller then total cmds then assume that we have all the params we need and there is no file list
}

//this code gets the next file in the file list
char* CmdLineH::GetNextFile()
{

  //now we need to test based off postion not by the number of params so we adjust by -1
  //then we test to see if total params is still bigger then current params.
  if(TotalParams-1 > curntparam)
  {
    curntparam++; //adjust to new param which we consider to be a file name
    return((char*)paramsDB[curntparam]); //retur file name..
  }
  else return(0); //if curntparams is not smaller return 0
}

//runs a command by group and prority and pesses the parameter to it.
bool CmdLineH::RunCommand(int cmdGroup)
{
  //we have to default to false or we can get a false possitive problem
  bool ACommandWasExecuted = false;

  int MaxPriority = GetMaxPriority(); //get the max priority possible in any given group..

  for(int i = 0; i <= MaxPriority || i == 0;i++) //now we test all commands with its possible priority..
  {
    for(int i2 = 0; i2 < TotalCmds;i2++) //we loop until we test all the commands in the database
    {
      if(cmdDB[i2].cmdPriority == i) //we check to see if the current command has the right priority..
      {
        if(cmdDB[i2].cmdGroup == cmdGroup && cmdDB[i2].cmdEnabled == true) //we check to see if the command belongs to ths group and if its actually enabled..
        {
          if(cmdDB[i2].cmdRef != 0) //we check to see if there is a function we need to call for this command..
          {
            //typedef void(__cdecl *CCMD)(); Works in windows only
            typedef void(*CCMD)(void * Input);  //works in both windows and linux.. this is used so we can call on the needed function
            CCMD CallCMD = (CCMD)cmdDB[i2].cmdRef; //update CallCMD to the function pointer of the function we want to call
            if(cmdDB[i2].numParams != 0)CallCMD(paramsDB+(cmdDB[i2].ParamPos)); //we check to see if the function wants anything passsed to it
            else CallCMD(0); //if the function wants nothing passed to it we pass 0
          }
          ACommandWasExecuted = true; //set this state to true because the job was completed the way we wanted it to..
        }
      }
    }
  }
  return(ACommandWasExecuted); //return the final result and that is if any command was ran with in a group..
}

//This function setups some stuff that the command line handler needs
bool CmdLineH::InitializeCommandHandler(int argc,char *argv[])
{
  ArgC = argc; //this is passes the int main argc to the class and it holds the number of sections the command line has
  ArgV = argv; //this is passes the int main argv to the class and it holds the pointers of all the sections of the command line
  if(!CreateDBs()) //we request that that the databases get created if not give error message
  {
    cout << "Unable to create initial databases." << endl;
    //printf("Unable to create initial databases.\n");
    return false;
  }

  TotalParams = BindParams(); //we request that all the parameters get bound to a data base
  if(TotalParams == -1) //if it cant then we display and error message.
  {
    cout << "Unable to bind params to there database." << endl;
    //printf("Unable to bind params to there database.\n");
    return false;
  }
  return true;
}

bool CmdLineH::CreateDBs()
{
  //allocate initial command database it gets re-adjusted later
  cmdDB = (CommandDB*)calloc(1,sizeof(CommandDB));
  if(!cmdDB)
  {
    cout << "Unable to allocate initial memory for cmdDB" << endl;
    //printf("Unable to allocate initial memory for cmdDB\n");
    return false;
  }

  //allocate initial params database it gets re-adjusted later
  paramsDB = (char**)calloc(1,sizeof(int));
  if(!paramsDB)
  {
    cout << "Unable to allocate initial memory for paramsDB" << endl;
    //printf("Unable to allocate initial memory for paramsDB\n");
    return false;
  }

  return true;
}

int CmdLineH::BindParams()
{
  int NumParams = 0;

  //the reason for this test is because the exe name would be included as a param if it was not here
  if(NumParams < ArgC && ArgC > 1)
  {
    int i = 1;
    if(PARAM_1_INCLUDED)i = 0;
    //loop for testing and finding params
    for(;i < ArgC;i++)
    {
      //check to see if all the requirements of a param are met
      if((strlen(ArgV[i])) >= 1 && *ArgV[i] != '-')
      {
        //dynamically adjust params index table
        paramsDB = (char**)realloc(paramsDB,((NumParams+1) * sizeof(int)));
        if(!paramsDB) //realloc will return 0 if it could not alloc this is to test that
        {
          cout << "Unable to move or allocate more memory for paramsDB" << endl;
          //printf("Unable to move or allocate more memory for paramsDB\n");
          return(-1);
        }
        //cout << "Param:\t" << ArgV[i] << endl;
        paramsDB[NumParams] = ArgV[i];
        NumParams++; //increment number of params

      }
    }
  }
  return(NumParams); //if everything goes as planned return true
}

//this function adds a command to the data base
bool CmdLineH::AddCommand(char * cmdPrimary,char * cmdSecondary,int cmdGroup,int cmdPriority,int numParams,void * cmdRef)
{
  //enlarge command database as needed
  cmdDB = (CommandDB*)realloc(cmdDB,(TotalCmds+1) * (sizeof(CommandDB)));
  if(!cmdDB)
  {
    cout << "Unable to move or allocate more memory for cmdDB" << endl;
    //printf("Unable to move or allocate more memory for cmdDB\n");
    return false;
  }
  else
  {
    char* buffer; //temp memory buffer address holder

    //if primary not equal to 0 then run code
    if(cmdPrimary != 0)
    {
      //allocate memory and point cmdPrimary to the address then copy text into buffer
      buffer = (char*)calloc(1,(strlen(cmdPrimary))+1);
      if(buffer == 0) //checking to see if we were not able to allocate memory
      {
        cout << "Unable to allocate memory for cmdPrimary" << endl;
        //printf("Unable to allocate memory for cmdPrimary\n");

        return false;
      }
      else //if memory allocation was good update cmd data base and compy the command sting into its place
      {
        cmdDB[TotalCmds].cmdPrimary = buffer;
        strcpy(buffer,cmdPrimary);
      }
    }else cmdDB[TotalCmds].cmdPrimary = cmdPrimary;

    //if secondary not equal to 0 then run code
    if(cmdSecondary != 0)
    {
      //allocate memory and point cmdSecondary to the address then copy text into buffer
      buffer = (char*)calloc(1,(strlen(cmdSecondary))+1);
      if(buffer == 0) //checking to see if we were not able to allocate memory
      {
        cout << "Unable to allocate memory for cmdSecondary" << endl;
        //printf("Unable to allocate memory for cmdSecondary\n");
        return false;
      }
      else //if memory allocation was good update cmd data base and compy the command sting into its place
      {
        cmdDB[TotalCmds].cmdSecondary = buffer;
        strcpy(buffer,cmdSecondary);
      }

    }else cmdDB[TotalCmds].cmdSecondary = cmdSecondary;

    //update the rest of the command info for this command
    cmdDB[TotalCmds].cmdEnabled = false;
    cmdDB[TotalCmds].ParamPos = false;
    cmdDB[TotalCmds].cmdGroup = cmdGroup;  //put this commands group number into place
    cmdDB[TotalCmds].cmdPriority = cmdPriority; //set this commands prority for its self in its group
    cmdDB[TotalCmds].cmdRef = cmdRef; //set the pointer to the function to run if the command is ran
    cmdDB[TotalCmds].numParams = numParams; //pas the number of params command has to use

    TotalCmds++;  //incrament the number of commands
  }
  return true;
}


//TODO: maybe optimize the code and maybe make it check first against the command db and not require - and -- for indevidual commands
//might still need the - for string together commands
//this checked to see if any commands were passed and why way the command was passed //returns 0 if failed
bool CmdLineH::CompareCommands()
{
  int CurrentPramPos = 0; //we have to initialize to 0
  bool cmd_passed = false; //this has to be false first so we dont get any false positives

  //if ArgC is not greater then one then there is not point in running the code at all
  //there will always be at least one because the exe name is included in the count
  if(ArgC > 1)
  {
    //run code until we have test everything that was passed to the program
    for(int cmdpos = 1;cmdpos < ArgC;cmdpos++)
    {

      int CmdLen = (int)strlen(ArgV[cmdpos]);

      //test for commands that start with --
      if(*ArgV[cmdpos] == '-' && (*(&ArgV[cmdpos])+1)[0] == '-' && CmdLen > 2)
      {
        //TODO: make this check both primary and seconday there is no point in making it restrict -- to seconday only
        for(int i = 0; i < TotalCmds;i++) //run code until we have tested current passed param with all commands that are in the db
        {
          if(cmdDB[i].cmdSecondary != 0) //check to see if there is a seconday command
          {
            if(strcmp(cmdDB[i].cmdSecondary, ArgV[cmdpos]) == 0) //check to see if the seconday command matches this parameter
            {
              if(!cmdDB[i].cmdEnabled) //check to see if the command is not enabled yet
              {
                cmdDB[i].cmdEnabled = true;
                cmd_passed = true;
                if(cmdDB[i].numParams != 0) //check to see if there are any parameters
                {
                  cmdDB[i].ParamPos = CurrentPramPos; //update this command paramater pointer
                  for(int parampos = 0;parampos < cmdDB[i].numParams;parampos++)CurrentPramPos++; //if it takes more then param then update pos to next commands starting param
                }
                break;
              }
              //TODO: some commands might want to be ran twice like opening two diff files.. might want to remove this or add in a check for special commands
              else //if command is already enabled give message
              {
                cout << "Repeat Parameter: " << ArgV[cmdpos] <<endl;
                //printf("Repeat Parameter: %s\n",ArgV[cmdpos]);
                return(0);
              }
            }else cmd_passed = false;
          }else cmd_passed = false;
        }
        if(!cmd_passed)//if the checking did not pass anything then we have to give an error message
        {
          cout << "Error Bad Parameter: " << ArgV[cmdpos] <<endl;
          //printf("Error Bad Parameter: %s\n",ArgV[cmdpos]);
          return(0);
        }
      }

      //test for comands that start with - but not string togeth commands
      else if(*ArgV[cmdpos] == '-' && (*(&ArgV[cmdpos])+1)[0] != '-' && CmdLen == 2)
      {
        for(int i = 0; i < TotalCmds;i++)
        {
          if(cmdDB[i].cmdPrimary != 0)
          {
            if(strcmp(cmdDB[i].cmdPrimary, ArgV[cmdpos]) == 0)
            {
              if(!cmdDB[i].cmdEnabled)
              {
                cmdDB[i].cmdEnabled = true;
                cmd_passed = true;
                if(cmdDB[i].numParams != 0)
                {
                  cmdDB[i].ParamPos = CurrentPramPos;
                  for(int parampos = 0;parampos < cmdDB[i].numParams;parampos++)CurrentPramPos++;
                }
                break;
              }
              else
              {
                cout << "Repeat Parameter: " << ArgV[cmdpos] <<endl;
                //printf("Repeat Parameter: %s\n",ArgV[cmdpos]);
                return(0);
              }
            }else cmd_passed = false;
          }else cmd_passed = false;
        }
        if(!cmd_passed)
        {
          cout << "Error Bad Paramater: " << ArgV[cmdpos] <<endl;
          //printf("Error Bad Parameter: %s\n",ArgV[cmdpos]);
          return(0);
        }
      }
      //test for commands that start with - and are string together
      else if(*ArgV[cmdpos] == '-' && (*(&ArgV[cmdpos])+1)[0] != '-' && CmdLen > 2)
      {
        char testchar;
        for(int i2 = 0;i2 < CmdLen-1;i2++)
        {
          testchar = (*(&ArgV[cmdpos])+i2+1)[0];
          for(int i = 0; i < TotalCmds;i++)
          {
            if(cmdDB[i].cmdPrimary != 0)
            {
              if(cmdDB[i].cmdPrimary[1] == testchar)
              {
                if(!cmdDB[i].cmdEnabled)
                {
                  cmdDB[i].cmdEnabled = true;
                  cmd_passed = true;
                  if(cmdDB[i].numParams != 0)
                  {
                    cmdDB[i].ParamPos = CurrentPramPos;
                    for(int parampos = 0;parampos < cmdDB[i].numParams;parampos++)CurrentPramPos++;
                  }
                  break;
                }
                else
                {
                  cout << "Repeat Parameter: " << testchar <<" in " << ArgV[cmdpos] <<endl;
                  //printf("Repeat Parameter: %s %s\n",testchar,ArgV[cmdpos]);
                  return(0);
                }
              }else cmd_passed = false;
            }else cmd_passed = false;
          }
          if(!cmd_passed)
          {
            cout << "Error Bad Paramater: " << testchar <<" in " << ArgV[cmdpos] << endl;
            //printf("Error Bad Parameter: %s %s\n",testchar,ArgV[cmdpos]);
            return(0);
          }
        }
      }
    }
  }else return false;

  //this stores the number the command of params the commands need
  TotalCmdParams = GetParamReq();
  return(true);
}

//this command check for the largets number passed to the priorities list and returns that value
int CmdLineH::GetMaxPriority()
{
  size_t MaxPriority = 0; //we have to initialize this to 0 since windows does not clean alocated memory..
  size_t CurrentPriority = 0; //we dont have to initialize to 0 but i want to..

  if(TotalCmds >= 1) //I tryed != 0 but it gave false possitives so i decided to make it greater or equal to 1
  {
    for(int i = 0;i < TotalCmds;i++) //loop until we have ran through all the commands in the command db
    {
      CurrentPriority = cmdDB[i].cmdPriority; //grab the current commands priority..
      if(CurrentPriority > MaxPriority)MaxPriority = CurrentPriority; //test it against the last one. if its larger then put current ito maxpriority
    }
  }
  return((int)MaxPriority); //return the maximum priority..
}

//this returns a char pointer to the program that called this
char * CmdLineH::GetExecName()
{
  return ArgV[0];
}

//this gets the number of required parameters for the commands passed
int CmdLineH::GetParamReq()
{
  int ParamsN = 0; //we have to initialize to 0

  for(int i = 0; i < TotalCmds;i++) //run code until we have added up all the params needed
  {
    if(cmdDB[i].cmdEnabled && cmdDB[i].numParams >= 1) //check to see if the command is enabled if it is check to see if it needs params
    {
      ParamsN += cmdDB[i].numParams; //if params are needed add them to the number of params needed
    }
  }
  return(ParamsN); // return the total params needed
}


//convert windows getcommandline string to std standard command line stuff
/*
char** ConCMDL2STD(char* strin)
{
  int strsize = (int)strlen(strin);
  int* command_db = (int*) calloc(1,sizeof(int));
  int command_num = 0;
  for(int i = 0; i < strsize;i++)
  {
    if(strin[i] == '"')
    {
      //TODO: need to add new possition to the db
      command_db = (int*) realloc(command_db,command_num*sizeof(int));
      if(!command_db)
      {
        cout << "could not allocate memory for the command db builder" << endl;
        return(0);
      }
      strin[i] = 0;
      command_db[i] = (int)&strin[i+1];
      for(;i < strsize;i++)
      {
        if(strin[i] == '"')
        {
          strin[i] = 0;
          i++;
          break;
        }
        else if(i+1 > strsize)return(0);
      }
    }
    else if(strin[i] == 0x20)
    {
      strin[i] = 0;
      command_db[i] = (int)&strin[i+1];

    }
    else if(i == 0)
    {
      command_db[i] = (int)&strin[i];
    }
  }
}
*/
