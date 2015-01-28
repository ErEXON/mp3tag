/*
 * IDTE - ID3 Tag Editor by Rajat kosh
 * Copyright (C) 2013-14 rEX_ON_FiRE
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */


#include "ID3V2_Header.h"

/**
Function Name = Get_Header()
returns = Nothing
Arguments = file

file = Source file pointer (Very front of the file)

Description = This function reads ID3v2 Header and set flags accordingly

**/

void ID3V2_Header::Get_Header(std::fstream& file)
{
    file.seekg(0); //Sets The Pointer To Points At The Begining Of The File
    //Let's Get The Below Process According To The ID3v2 Standrad
    file.read((char*)Header_id ,3);             // Read 3 Bytes in Header i.e. "I" "D" "3"
    file.read((char*)&Version,1);             // Reads The Next 1 byte actually 8-bits i.e. ID3 Version in The Version Member
    file.read((char*)&Revision,1);            // Reads Next 1 Byte actually 8-bits i.e. ID3 Revision No. in The Revision Member
    file.read((char*)&Header_flags,1);         // Reads Header Flags
    unsigned char memory[4];
    file.read((char*)memory,sizeof(Header_size));  //Reads 4 Bytes (Sync Safe Form) in Memory.

    Header_size =  (memory[3] & 0xFF) |         //Finally Desynchronise The Synch. Safe Integer i.e.0xxxxxxx 0xxxxxxx ......
                  ((memory[2] & 0xFF) << 7 ) |  // Size    =  0xxxxxxx
                  ((memory[1] & 0xFF) << 14 ) | //          +        0xxxxxxx
                  ((memory[0] & 0xFF) << 21 );  //          +               0xxxxxxx
                                                //          +                      0xxxxxxx
                                                //__________________________________________
                                                // Size    =  xxxxxxxxxxxxxxxxxxxxxxxxxxxx      where + = OR Operation

if ((Header_id[0]=='I')and(Header_id[1]=='D')and(Header_id[2]=='3')) // Checks If Header ID  is I D 3
Is_present = 1;
else
Is_present = 0;

}

/**
Function Name = Show_Header()
Returns = Nothing
Arguments = No Arguments

Description = This function will print out ID3v2 Header information onto screen
**/

void ID3V2_Header::Show_Header(void)
{
    std::cout<<" "<<std::endl;
    if ((Header_id[0]=='I')and(Header_id[1]=='D')and(Header_id[2]=='3')) // Checks If Header ID  is I D 3
    {
        std::cout<<"ID3v2."<<(int)Version<<"."<<(int)Revision<<" is Present in File"<<std::endl; // Prints Version Onto screen
        std::cout<<"Total Size of Tag = "<<float(float(Header_size)/1024)<<" kilobytes(KB) or "<<Header_size<<" Bytes"<<std::endl;
        Is_present = 1; // Well set Present Flag To 1
        if((int)Version==4) //Now Set Flags According to the in Standrad
        Flag=1;
        else if((Header_flags&0x4F)==64) // Set Flag =1 indicating that Frames Contain Unsynchronysed Data Size information
        Flag=1;
        else if((Header_flags&0x8F)==128) //Sets Flag =2 indicating that File Contains Extended Header
            Flag=2;
                else
                    Flag=0; // Sets Flag =0 indicating No Extended Header and Unsynchronisation

    }
    else
    {
        std::cout<<"File Has No Known ID3v2 Tags"<<std::endl; // If Header ID is Not I D 3 Then File Doesn't Contain Any ID3 Tag
        Is_present = 0; //Set Present Flag To Zero
    }

}

/**
Function Name = Get_Size()
returns = 32-bit unsigned integer containing size of the Tag
Arguments = No Arguments

Descrition = This function will return size of the tag iff ID3 tag is present in file, else it will return 0

**/

uint32_t ID3V2_Header::Get_Size(void)
{
    if (Is_present == 1)
        return Header_size;
    else
        return 0;
}
