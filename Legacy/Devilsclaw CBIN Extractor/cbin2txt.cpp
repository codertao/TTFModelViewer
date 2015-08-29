/***************************************************************************
 *   Copyright (C) 2007 by Neil Hellfeldt                                  *
 *   devilsclaw@devilsclaws.net                                            *
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
#include <stdio.h>
#include <iomanip>
#include <string>
#include <limits>
#include <cmath>
#include <bitset>
#include <sstream>

#define OUT_EXTENTION ".txt"

using namespace std;

#ifdef WIN32
	FILE* _fopen(const char* file,const char* mode)
	{
		FILE* filep = 0;
		fopen_s(&filep,file,mode);
		return filep;
	}
#else
	#define _fopen fopen
#endif

//CBINID is the first 4 bytes of the file and if it contains CBIN in them its a CBIN file.
//TextDataOffset is a pointer to where in the file the null terminated string data is stored
//TextDataSize tells you the total size of the string data.
//TotalItems tells you the total numbe of items there are in all groups in the file.
//EncryptionKey is the value used to encrypt this file and we use it to decrypt.
struct cbin_header
{
  unsigned char CBINID[4];
  unsigned int  TextDataOffset;
  unsigned int  TextDataSize;
  unsigned int  TotalItems;
  unsigned int  EncyptionKey;
};

//GroupID uses the TextData to tell you what text it uses.
//TotalItems tell you the total number of items in this group.
struct cbin_group
{
  unsigned int GroupID;
  unsigned int TotalItems;
};

//ItemID uses the TextData to tell you what text it uses.
//TotalItemParameters lets you know how many Parameters a item has.
struct cbin_item
{
  unsigned int ItemID;
  unsigned int TotalItemParameters;
};

//Parameter can be int,float,string.
//ParameterType lets you know the type of data Parameter is.
//If Parameter is string type it uses the TextData to look up what text it uses.
struct cbin_parameter
{
  unsigned int Parameter;
  unsigned int ParameterType;
};

//speed wise this is greate but also not as flexible as a bitset collection or queue
template<typename T>
    T rol ( T input,int pos ) //bit wise role to the left since c++ does not have one
{
  return ( input<<pos ) | ( input >> ( ( CHAR_BIT*sizeof ( T ) )-pos ) & ( ~ ( T ( -1 ) << pos ) ) );
}

template<typename T>
    T ror ( T input,int pos ) //bit wise role to the right since c++ does not have one
{
  return ( input>>pos ) | ( input << ( ( CHAR_BIT*sizeof ( T ) )-pos ) & ( ~ ( T ( -1 ) << pos ) ) );
}

/*
AND OPERATION VALUES
Binary                           Hex
10000000000000000000000000000000 0x80000000 //clip all but signed bit
01111111100000000000000000000000 0x7F800000 //clip all but exponent bits 23 - 30
00000000011111111111111111111111 0x007FFFFF //clip all but significand bits 0 - 22
00000000000000000000000000000001 0x00000001 //clip everything except the first bit
*/

//unsigned base to the power of an exponent
unsigned int upow(unsigned int x,unsigned int y)
{
  unsigned int ret = x;
  if(y == 0) return 1; //anything by the power of 0 equals 1 even 0
  if(x == 0) return 0; //0 to the power of anything other then 0 is 0

  for(unsigned int counter = 0; counter < y - 1;++counter)
  {
    ret *= x;
  }
  return ret;
}


//intel IEEE-754 float to stdlib float which is compatible with cout and printf
float iflt2sflt(int value)
{
  //sin stands for signed as in possitive or negative value 1 = neg 0 = pos
  //exp stands for exponet with the 32bit float the exponent is subtracted by 127 for find the exponent value
  //sig stands for significand and is the value used for calculating the Mantissa
  bool sin = (value & 0x80000000) >> 31;
  int  exp = ((value & 0x7F800000) >> 23) - 127;
  int  sig = (value & 0x007FFFFF) << 8;

  //the 32bit float assumes that the Mantissa starts with the base value of 1.0
  float m = 1.0; //m stands for Mantissa

  //counter used for calculating the Mantissa
  int counter = 0;

  //loop until all placement valuse have been added up from the significand into the Mantissa
  for(counter = 0; counter <= 23;++counter)
  {
    //check to see if this placement of the significand needs to be calculated and added up
    //if the 32nd bit has the bit set to 1 then we need to do calculations on it if its 0
    //then there is no point since adding 0 to anything has no affect in the outcome
    if((sig & 0x80000000))
    {
      //cout << upow(2,counter) << endl;
      m += (1.0 / upow(2,counter));
    }
    sig <<= 1; //shit the significand to the left by one so we can test the next bit on the next loop.
  }
  //cout << m;

  //we check to see if the exponent is negative
  if((exp & 0x80000000) >> 31)
  {
    //cout << upow(2,(-1*exp)) << " ";
    //make exp possitive by multiplying by -1
    //then 2 to the power of exp
    //once all do we do 2 to the power of absolute exp divided into the Mantissa
    m /= upow(2,(-1*exp));
  }
  else
  {
    //if its positive then we do 2 to the power of exp * Mantissa
    m *= upow(2,(exp));
  }

//   //debug code used to make sure all bits are from raw data
//   bitset<(8*sizeof(int))> flt(value);
//   cout << "count: " << flt.size() << endl;
//   for(int counter = flt.size() - 1;counter >= 0;--counter)
//   {
//     cout << flt[counter];
//   }
//   cout << endl;

//   //this idea wont work with how i was thinking but it is a good example of how to bitwise operate on floats
//   cout << "precision: " << precision << endl;
//   if(precision)
//   {
//     *(int*)(&m) >>= 23 - precision;
//     *(int*)(&m) <<= 23 - precision;
//   }

  //make the float negative if needed.
  if(sin) m *= -1;

  //return a float value that is compatible with stdlib
  return  m;
}


//used to validate the file format and make sure its the cbin format
// bool cbin_validate( unsigned char* buffer )
// {
//   if((reinterpret_cast<const int*>(buffer))[0] == ((int*)('N','I','B','C'))[0])
//   {
//     return true;
//   }
//   return false;
// };


//used to decrypt the file before we can proccess its raw data.
void cbin_decrypt(unsigned char* buffer,unsigned int size,size_t key)
{
  //if key has something then we need to decrypt
  if(buffer && size && key)
  {
    //decrypt the buffer data
    for(size_t counter = 0;counter < size;++counter)
    {
      key = rol ( key,7 );
      buffer[counter] ^= key;
    }
  }
}

//used to make sure no errors happend on the stream
bool checkstream(FILE* f)
{
  if(feof(f))
  {
    //printf("Exited with end of file\n");
    return false;
  }
  else if(ferror(f))
  {
    //printf("Exited with %s\n",(perror(f)));
    return false;
  }
  return true;
}

size_t get_filesize(FILE* input_file)
{
  //use seek to find the file size and check to see if there were any errors while trying to send it to end of file.
  if((fseek(input_file,0,SEEK_END)))
  {
    return 0;
  }

  //store the file size into filesize
  size_t filesize = ftell(input_file);
  return filesize;
}


//this is the main function in processing the cbin file and converting it to text
int convert_cbin2text(FILE* ifile, size_t filesize,char* filename)
{
  string output = "";

  //set the file back to the begining of the file
  rewind(ifile);
  cbin_header cHeader; //allocate some memory for the header info.
  memset(&cHeader,0,sizeof(cbin_header)); //initialize cHeader to 0

  //TODO: add in check to see if the file is atleast the same size of the header or bigger.

  //read the header info from file and test to see if we actually read the entire header
  //NOTE: read progresses the file possition on its own so after this any reading is past the header.
  if((fread(&cHeader,sizeof(unsigned char),sizeof(cbin_header),ifile)) != sizeof(cbin_header))
  {
    return 1;
  }

  //check to see if its a CBIN valid file.
  if(!(cHeader.CBINID[0] == 'C' && cHeader.CBINID[1] == 'B' && cHeader.CBINID[2] == 'I' && cHeader.CBINID[3] == 'N'))
  {
    return 1;
  }

  //allocate memory for the rest of the file which will be read into memory soon.
  unsigned char* filebuff = (unsigned char*)malloc(filesize - sizeof(cbin_header));

  //check to see if we manage to allocate the memory we needed.
  if(!filebuff)
  {
    return 1;
  }

  //read the rest of the file into memory. check to see if we read all the intended bytes we wanted.
  //NOTE: after this we only work with the file in memory
  if((fread(filebuff,sizeof(unsigned char),filesize - sizeof(cbin_header),ifile)) != (filesize - sizeof(cbin_header)))
  {
    return 1;
  }

  //decrypt the file that is in memory to a usable form we know.
  cbin_decrypt(filebuff,filesize - sizeof(cbin_header),cHeader.EncyptionKey);

  //grab the total number of groups for the filebuffer
  size_t totalGroups = ((int*)filebuff)[0];

  //create a temp pointer to the memory buffer so we can free memory with original pointer.
  unsigned char* tfilebuff = filebuff + sizeof(int);

  //calculate the new memory buffer size with out the header and store that into buffsize.
  //size_t buffsize = filesize - (sizeof(cbin_header) + sizeof(int));

  //TODO: Make sure buffer size is not in the negatives.

  //TODO: need to do some pre processing so there is only one memory allocation for the groups,items and parameters

  //allocate space for all the groups we need.
  cbin_group* cGroups = (cbin_group*)tfilebuff;

  //update tfilebuff possition to items location.
  tfilebuff += sizeof(cbin_group) * totalGroups;

  //allocate memory for the index's of the items and the parameters
  //since im building a 3D array so I can supports groups,items and parameters.
  //the index's are exactly like it sounds when im done processing the data it
  //will point to an item or a parameter.
  cbin_item** cItems = (cbin_item**)calloc(totalGroups,sizeof(int));
  cbin_parameter*** cParameters = (cbin_parameter***)calloc(totalGroups,sizeof(int));
  if(!cItems || !cParameters)
  {
    return 1;
  }

  //the index's for the groups are being calculated here and filling them in so when
  //im needing to pull the info for them its easy
  for(size_t gc = 0; gc < totalGroups;++gc)
  {
    cItems[gc] = (cbin_item*)tfilebuff; //point current index to the current group location

    //make cure this group has items other wise the possition does not need to be moved.
    if(cGroups[gc].TotalItems)
    {
      tfilebuff += (cGroups[gc].TotalItems * sizeof(cbin_item)) + sizeof(cbin_item); //adjust the group possition base off how many items it has + 1 more since they have a empty item in each group
      cParameters[gc] = (cbin_parameter**)calloc(cGroups[gc].TotalItems,sizeof(int));
    }
  }

  size_t gc = 0; //group counter
  size_t ic = 0; //item counter
  //size_t pc = 0; //parameter counter

  //loop through all groups
  for(gc = 0; gc < totalGroups;++gc)
  {
//      cout << " Group: " << gc+1;
//      cout << " Items: " << cGroups[gc].TotalItems << endl;

    //loop through all items if this group has any.
    for(ic = 0; ic < cGroups[gc].TotalItems;++ic)
    {
//       cout << " Item: " << ic+1;
//       cout << " Params: " << cItems[gc][ic].TotalItemParameters << endl;

      //point current parameter index to the proper location
      cParameters[gc][ic] = (cbin_parameter*)tfilebuff;

      //update to the next parameter possition

      //make sure this item actually has parameters if not no point in moving the possition
      if(cItems[gc][ic].TotalItemParameters)
      {
        tfilebuff += cItems[gc][ic].TotalItemParameters * sizeof(cbin_parameter);
      }
    }
  }

  //allocate memory pointer for all the string values in the file.
  unsigned char** strValues = (unsigned char**)calloc(cHeader.TotalItems,sizeof(int));
  if(!strValues)
  {
    return 1;
  }

  size_t sc = 0; //string counter
  //build the pointer index table to the strings
  for(sc = 0; sc < cHeader.TotalItems;++sc)
  {
    strValues[sc] = tfilebuff;
    //cout << strValues[sc] << endl;
    while(*tfilebuff)tfilebuff++;
    tfilebuff++;
  }

  string tempstr = "";
  size_t uc = 0; //uppercase counter
  for(gc = 0; gc < totalGroups;++gc)
  {
    //make groups uppercase
    for(uc = 0;strValues[cGroups[gc].GroupID-1][uc];++uc)
    {
      if(strValues[cGroups[gc].GroupID-1][uc] >= 'a' && strValues[cGroups[gc].GroupID-1][uc] <= 'z')
        strValues[cGroups[gc].GroupID-1][uc] -= 0x20;
    }

    //display current group in all uppercase
    //cout << "[" <<strValues[cGroups[gc].GroupID-1] << "]" <<endl;
    output += "[" + ((string)((char*)strValues[cGroups[gc].GroupID-1])) + "]" + "\n";
    bool comment_trigger = false;
    //loop through all the groups items
    for(ic = 0; ic < cGroups[gc].TotalItems;++ic)
    {
      //display current item for group.
      //cout <<strValues[cItems[gc][ic].ItemID-1] << " = ";
      output += ((string)((char*)strValues[cItems[gc][ic].ItemID-1])) + " = ";

      //display all of the current items parameters
      for(sc = 0; sc < cItems[gc][ic].TotalItemParameters;++sc)
      {
        stringstream tempstream;
        tempstr = "";
        //display current parameter based off type
        switch(cParameters[gc][ic][sc].ParameterType)
        {
          case 1: //type 1 is int
            //cout << (int)cParameters[gc][ic][sc].Parameter;
            tempstream << (int)cParameters[gc][ic][sc].Parameter;
            tempstream >> tempstr;
            output += tempstr;
            break;
          case 2: //type 2 is IEEE-754 float
            //cout << iflt2sflt(cParameters[gc][ic][sc].Parameter);
            tempstream << iflt2sflt(cParameters[gc][ic][sc].Parameter);
            tempstream >> tempstr;
            output += tempstr;
            break;

          //NOTE: there is no type 3
          case 4: //type 4 is a string
            //cout << strValues[cParameters[gc][ic][sc].Parameter-1];
            if(strValues[cParameters[gc][ic][sc].Parameter-1][0] == '/' &&
                strValues[cParameters[gc][ic][sc].Parameter-1][1] == '/')
            {
              if(output.size())
              {
                if(output[output.size()-1] == ',')
                  output[output.size()-1] = '\t';
              }
              comment_trigger = true;
            }
            output += ((string)((char*)strValues[cParameters[gc][ic][sc].Parameter-1]));
            break;
          default:
            //display an error message it should never make it here
            cout << "Error wrong parameter type: " << cParameters[gc][ic][sc].ParameterType << endl;
            return 1;
        }
        //output the , if its not the last parameter for this item
        //if(sc+1 != cItems[gc][ic].TotalItemParameters)cout << ',';
        if(!comment_trigger)
          if(sc+1 != cItems[gc][ic].TotalItemParameters)output += ",";
      }
      //cout << endl; //end line for item
      output += "\n"; //end line for item
      comment_trigger = false;
    }
    //cout << endl; //end line for group
    output += "\n"; //end line for group
  }

  tempstr = (string)filename + OUT_EXTENTION;
  FILE* tmpfile = _fopen(tempstr.c_str(),"w+");
  if(tmpfile)
  {
    fwrite(output.c_str(),1,output.size(),tmpfile);
  }
  else
  {
    cout << "Could not creat " << tempstr << " returning with error." << endl;
    return 1;
  }
  fclose(tmpfile);
  return 0;
}

//the main program fucntion
int main(int argc, char *argv[])
{
  //setting the counter to 1 because the first parameter passed to the program is the program name.
  int counter;
  FILE* hfile = 0;
  size_t filesize = 0;

  for(counter = 1; counter != argc;++counter)
  {
    cout << "Opening " << argv[counter] << endl;
    //open file that has to be there
    hfile = _fopen(argv[counter],"r+b");
    if(hfile)
    {
      filesize = get_filesize(hfile);
      if(filesize)
      {
        //check to see if any errors have happend sofar and if there is a file size.
        if(!((ferror(hfile)) || !filesize))
        {
          if(!convert_cbin2text(hfile,filesize,argv[counter]))
          {
            cout << "Finished processing " << argv[counter] << endl;
            cout << "Outputed File:  " << argv[counter] << OUT_EXTENTION << endl;
          }
          else
          {
            cout << "Error while processing " << argv[counter] << endl;
          }
        }
        else
        {
          cout << "There was an error with " << argv[counter] << endl;
        }
      }
      else
      {
        cout << argv[counter] << " was zero in size." << endl;
      }
    }
    else
    {
      cout << "Could not open " << argv[counter] << endl;
    }
    fclose(hfile);
    hfile = 0;
  }
}
