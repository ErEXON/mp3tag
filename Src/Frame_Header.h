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
#include "All_Frames.h"

#ifndef FRAME_HEADER_H
#define FRAME_HEADER_H

/**
Frame Header:
As the tag consists of a tag header and a tag body with one or more frames, all the frames consists of a frame header followed by one or more fields containing the actual information.

The First Four Bytes (3 Bytes in v2.2) Of A Frame Consist of The Frame Header ID i.e. TALB (Album), TART (Artist) etc.The frame ID is made out of the capital letters A-Z and 0-9
Next Four Bytes Consist Of Frame Size (Excluding Frame Header) ,This Size Might Be In Sync Safe Integer or It Might Be In Simple intergers.
After Size ,The Frame header Consists of Frame Flags of two Bytes and therefore Completes The Frame Header
**/

class Frame_Header : public ID3V2_Header
{
    public:
            void Get_End(std::fstream& file,int*);
            void Get_Frame(std::fstream& file); //Read Data into frame
            void Show_Frame(std::fstream& file,int* ); //Shows The Frame onto Console Output
            void Search_frame(std::fstream& file,int* ,char*,char*,char*); // Search For A Particular Specified frame and Take Actions Accordingly
            void Search_only_frame(std::fstream& ,char* ,int*,char* Destination = NULL); // Search For A Particular Frame And Print It Accordingly
            void Get_Text(std::fstream& file); //Get Text Frame
            void Get_Rating(std::fstream& file); //Get Popularitymeter Frame
            void Get_UDText(std::fstream& file); // Get User Defined Text Frame
            void Add_UDText(std::fstream& file,char* Description,char* UDText);
            void Get_CoverArt(std::fstream& file,int Cover_flag,char* Destination = NULL); // Get Cover Art Frame
            void Get_USlyrics(std::fstream& file,int Lyrics_Flag,char*Destination = NULL); // Get Unsynchronised Lyrics Text
            void Get_Url(std::fstream& file); // Get URL
            void Get_Comment(std::fstream& file); // Get Comment Field
            void Priv_Frame(std::fstream& file);// Get Private Frame
            void Skip_Frame(std::fstream& file); // Skips The Unknown Or unimplemented Frame
            void Skip_Silent(std::fstream& file); // Silently Skips The Frame (The Process is Hidden From User)
            void Set_Text(std::fstream& file,char*,char*); //Set Text Frame
            void Create_Text_Frame(std::fstream& file,char*,char*,char*);//Create a New Text Frame if No Such Frame Exists in File
    protected:
            unsigned char Frame_id[5]; //4 character For ID and One For \0
            uint32_t Frame_size;   //4 Byte integer in order to store Frame Size
            unsigned char Frame_flags[2]; //2 Bytes Flags Therefore An Array Of Two Characters
            int Get_Version(void); // Gets The ID3 Version From Previous Inherited ID3V2 Class
            unsigned int Get_Header_Size(void); //Get The Size Of The Complete Tag
};

#endif // FRAME_HEADER_H

/**
Note :- Generally it is not Possible to Implement All the Frames, Because Players Such As Itunes,Winamp etc. has their Own non Standradized Frames. Therefore Some Of The Frames Aren't Possibly Implemented.

Also , The Synchronised Scheme is Not Used in ID3v2.3,2.2 Frames But, Instead Used in ID3v2.4 Frames.However All Headers And Extended headers Of ID3V2 Make Use Of Synchronisation Scheme.

**/
