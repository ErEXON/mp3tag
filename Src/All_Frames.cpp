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


#include "All_Frames.h"
#include <iostream>
#include <cstring>
#include <stdexcept>


void Text_frame::Get_text_frame(int size, std::fstream& file)
{
    file.read((char*)&Text_encoding,1); //Read Encoding Byte of Frame

    while(size-1!=0) // Size+1 coz Actual size is Differ From 1 To Seek Size
    {
        std::cout<<(char)file.get();
        size--;
    }
}

Text_frame::Text_frame()
:Text_encoding(0)
{}

void Text_frame::Set_text_frame(int size, std::fstream& file,char* arg,char*arg2,int Version_info,unsigned int Header_Size)
{
   Information = arg; //Set Information = Input Argument
   std::cout<<"Updating..."<<" , Applying Version 2."<<Version_info;
   int32_t String_Len = strlen(Information); //Get String Size
   int Zero = 0;
   file.write(reinterpret_cast<const char *>(&Zero),1); //Set Encoding Bit = 00 i.e. UTF -8 or Ansi
    uint32_t Tell = file.tellg(); // Get Current Cursor Position Of File
    if((String_Len<size)&&((size-String_Len)<=10)) //If String Size is Less than The Previous String then Overwrite The Previous One in Order To Enhance The Performance.
    {
        file.write(Information,String_Len); //Write New information into Current File
        size = size-String_Len; // Examin The Remaning Size
        --size; // remove the Encoding Bit
            while(size!=0)  //Pad Remaining Data (Previous String) By Space
            {
                file.write(" ",1);
                size--;
            }
            std::cout<<"\nFrame Updated Sucessfully"<<std::endl;
    }
    else //If String size is Greater Than The Previous One Then Perform The Following Performance
        {
            char* buffer = new char[Tell];  //Create A Buffer Of memory upto Curr Cursor position
            std::fstream file_1,file_2;
            file_1.open(arg2,std::ios_base::binary |std::ios_base::in); //Open The Curr Processing File in Read Mode

            file_2.open("file.mp3",std::ios_base::binary|std::ios_base::out); //Create A Temporary File

            file_1.read(buffer,Tell);   //Put Value in Buffer upto current position
            file_2.write((char*)buffer,Tell);   //Save/Write Buffer Data in Temporary File
            file_2.write((char*)Information,String_Len); //Write Input String in Temporary File
            String_Len += 7;    //Increase The Size by 7 Length /// This Will Help in Seeking Back To Size Bytes
            file_2.seekg(-String_Len,std::ios_base::cur); // Seek To Size Byte(s) (4*0xxxxxxx)
            String_Len -= 7;    //Get The Orignal Length of String
            String_Len++; //Increase size by One in Order To Save Space For Unicoded/Ansi Encoded String Recognisation
            uint8_t Split_Size[4];  // 4 Byte Array in Order to store the size of Information
                if(Version_info == 3)   //Version 2.3 , Therefore Extract Simple Size From the String Size
                 {
                    Split_Size[3]=String_Len;
                    Split_Size[2]=String_Len>>8;
                    Split_Size[1]=String_Len>>16;
                    Split_Size[0]=String_Len>>24;
                 }
                else if(Version_info == 4)  //Version 2.4 , Therefore Extract Synchronisation Safe Integer From the String Size
                    {
                        Split_Size[3]=(String_Len & 0x0000007F);
                        Split_Size[2]=(String_Len>>7 & 0x0000007F);
                        Split_Size[1]=(String_Len>>14 & 0x0000007F);
                        Split_Size[0]=(String_Len>>21 & 0x0000007F);
                    }
                    else
                        {
                            std::cout<<"\nUnsupported Tag or ID3 Version Type \n";
                            return;
                        }
                        file_2.write(reinterpret_cast<const char *>(&Split_Size),4);//Write The Integer into Binary File instead of writing it As An Ansi String
                        String_Len--;   //Revert Back To Orignal Length
                        String_Len += 3;    //Increase The Length By 3 in order To Seek at The End Of the Input String i.e 2bytes For Flags and 1 Byte for Encoding
                        file_2.seekg(String_Len,std::ios_base::cur);//Seek To End Of The Temp File
                        file_1.seekg(size,std::ios_base::cur);//seek to the Next Frame (By Skipping The Size Of Current Frame) in Orignal File
                        Tell = file_1.tellg();  //Get The Position Upto Current Position
                        Tell--; // Decrement Seek Because Seek Will Return Curr Byte Position Starting from 0
                        file_1.seekg(0,std::ios_base::end); //Seek To The End Of The Orignal File
                        int Result,Tell2 = file_1.tellg(); //Save Complete File Size in Tell2
                        Result = Tell2 - Tell; //Now Just Put The Remaining Bytes Position in Result
                        file_1.seekg(Tell,std::ios_base::beg); //Seek Orignal File Upto Previously Saved Position (saved in Tell)
                        delete [] buffer;// Empty the buffer
                        buffer = new char[Result]; //Again reserve space in buffer
                        file_1.read(buffer,Result); //read remaining Data of Orignal File (i.e.Data After The Altered Frame )
                        file_2.write(buffer,Result); //write Buffer Data into File
                        file_2.seekg(6,std::ios_base::beg);//Time To Write Complete Tag Information
                        String_Len-=3; // According to Line No. 93 (In Order To Get The Orignal Size Of String)
                        String_Len = (String_Len +1) -size; //Total Change in Length of Frame (size = previous Size of Frame [Including Encoding Bit])
                        String_Len = String_Len + Header_Size; // Total Size = Previous Header Size + Total Change In Length Of Frame.

                Split_Size[3]=(String_Len & 0x0000007F);
                Split_Size[2]=(String_Len>>7 & 0x0000007F);
                Split_Size[1]=(String_Len>>14 & 0x0000007F);
                Split_Size[0]=(String_Len>>21 & 0x0000007F);

                    file_2.write(reinterpret_cast<const char *>(&Split_Size),4);
                    delete [] buffer; // Free Up The buffer in order to Prevent Memory Leakage
                    file_2.close(); //Close Files And Save Up Temporary File
                    file_1.close();

            std::fstream file1;
            file1.open("file.mp3",std::ios_base::binary |std::ios_base::in);//Now Finally Open The Modified File and overwrite the Previous File.
            file1.seekg(0,std::ios_base::end);
            uint32_t Tell = file1.tellg();
            file1.seekg(0,std::ios_base::beg);
            buffer = new char[Tell];
            file1.read(buffer,Tell);

            if(!file1.is_open())
                std::cout<<"Error\n";
            file.seekg(0,std::ios_base::beg);
            if(!file.is_open())
                {
                    throw std::runtime_error("\nFatal Error Unable To Write Destination File\n");
                }
            file.write((char*)buffer,Tell);
            delete [] buffer;
            file.close();
            file1.close();
    //Here We Go...
            std::cout<<"\nFrame Updated Sucessfully "<<std::endl;
    }
}

void Text_frame::Create_Text_Frame(std::fstream& file,char* arg,char*arg2,char*Frame_name,int Version_info,unsigned int Header_Size)
{
    Information = arg;
    int32_t String_Len = strlen(Information); //Get String Size
    file.seekg(-10,std::ios_base::cur); //seeking data -10 bytes because Last Frame Reading Sets our pointer 10 bytes Far from EOFrame
    uint32_t Tell = file.tellg(); // Get Current Cursor Position Of File
    char* buffer = new char[Tell];  //Create A Buffer Of memory upto Curr Cursor position
    std::fstream file_1,file_2;
    file_1.open(arg2,std::ios_base::binary |std::ios_base::in); //Open The Curr Processing File in Read Mode
    file_2.open("file.mp3",std::ios_base::binary|std::ios_base::out); //Create A Temporary File
    file_1.read((char*)buffer,Tell);
    file_2.write((char*)buffer,Tell);
    String_Len++; // Text Data + 1 Encoding Byte
    file_2.write((char*)Frame_name,4);
    uint8_t Split_Size[4];  // 4 Byte Array in Order to store the size of Information
    if(Version_info == 3)   //Version 2.3 , Therefore Extract Simple Size From the String Size
    {
        Split_Size[3]=String_Len;
        Split_Size[2]=String_Len>>8;
        Split_Size[1]=String_Len>>16;
        Split_Size[0]=String_Len>>24;
    }
    else if(Version_info == 4)  //Version 2.4 , Therefore  convert size into  Synchronisation Safe Integer From the String Size
    {
        Split_Size[3]=(String_Len & 0x0000007F);
        Split_Size[2]=(String_Len>>7 & 0x0000007F);
        Split_Size[1]=(String_Len>>14 & 0x0000007F);
        Split_Size[0]=(String_Len>>21 & 0x0000007F);
    }
    else
    {
        std::cout<<"\n Unsupported Tag or ID3 Version Type \n";
        return;
    }
    file_2.write(reinterpret_cast<const char *>(&Split_Size),4);//Write The Integer into Binary File instead of writing it As An Ansi String
    String_Len--;   //Revert Back To Orignal Length
    int Flags_Data = 0;
    file_2.write(reinterpret_cast<const char *>(&Flags_Data),3);// No Flag Set And Also Set Encoding Bit = $00 , i.e. UTF -8 / Ansi
    file_2.write((char*)Information,String_Len);
    Tell = file_1.tellg();  //Get The Position of The Orignal File Upto The Last Byte (From Where We Left The file)
    Tell--; // Decrement Seek Because Seek Will Return Curr Byte Position Starting from 0
    file_1.seekg(0,std::ios_base::end); //Seek To The End Of The Orignal File
    int Result,Tell2 = file_1.tellg(); //Save Complete File Size in Tell2
    Result = Tell2 - Tell; //Now Just Put The Remaining Bytes Position in Result
    file_1.seekg(Tell,std::ios_base::beg); //Seek Orignal File Upto Previously Saved Position (saved in Tell)
    delete [] buffer;// Empty the buffer
    buffer = new char[Result]; //Again reserve space in buffer
    file_1.read(buffer,Result); //read remaining Data of Orignal File (i.e.Data From The Orignal ID3 Tag End)
    file_2.write(buffer,Result); //write Buffer Data into File
    file_2.seekg(6,std::ios_base::beg);//Time To Write Complete Tag Information
    String_Len+=11; // Total Change in Size = 10 bytes For ID3 Header + 1 Encoding Byte + String Length
    String_Len = String_Len + Header_Size;  // Total Size = Previous Header Size + Total Change in Size

        Split_Size[3]=(String_Len & 0x0000007F);
        Split_Size[2]=(String_Len>>7 & 0x0000007F);
        Split_Size[1]=(String_Len>>14 & 0x0000007F);
        Split_Size[0]=(String_Len>>21 & 0x0000007F);

    file_2.write(reinterpret_cast<const char *>(&Split_Size),4);
    delete [] buffer; // Free Up The buffer in order to Prevent Memory Leakage
    file_2.close(); //Close Files And Save Up Temporary File
    file_1.close();

    std::fstream file1;
    file1.open("file.mp3",std::ios_base::binary |std::ios_base::in);//Now Finally Open The Modified File and overwrite the Previous File.
    file1.seekg(0,std::ios_base::end);
    Tell = file1.tellg();
    file1.seekg(0,std::ios_base::beg);
    buffer = new char[Tell];
    file1.read(buffer,Tell);

    if(!file1.is_open())
    std::cout<<"Error\n";
    file.seekg(0,std::ios_base::beg);
    if(!file.is_open())
    {
        throw std::runtime_error("\nError: Unable To Write Destination File\n");
    }
    file.write((char*)buffer,Tell);
    delete [] buffer;
    file.close();
    file1.close();
    //Here We Go...
    std::cout<<"\nFrame Created Sucessfully "<<std::endl;

}

void UD_text_frame::Get_UD_text_frame(int size,std::fstream& file)
{
    file.read((char*)&UD_text_encoding,1);
    std::cout<<"Description :";
    unsigned char info = 'A';
    while((size-1)!=0)
    {
        info = file.get();
        std::cout<<info;
        size--;
        if((info&0xFF)==0)
        break;
    }
        std::cout<<"\nUser Defined Text : ";
        while((size-1)!=0)
        {
            std::cout<<(char)file.get();
            size--;
        }
        std::cout<<"\n";

}

UD_text_frame::UD_text_frame()
:UD_text_encoding(0)
{}

void UD_text_frame::Create_UD_text_frame(std::fstream& file,int Version_info,char* Description,char* UD_text,unsigned int Header_Size)
{
 uint32_t Des_size = strlen(Description),UD_text_size = strlen(UD_text);
 file.write("TXXX",4);
 file.write(reinterpret_cast<const char *>(Flags),2);
 uint32_t String_Len = Des_size+UD_text_size+2;
   if(Version_info == 3)   //Version 2.3 , Therefore Extract Simple Size From the String Size
    {
        Split_Size[3]=String_Len;
        Split_Size[2]=String_Len>>8;
        Split_Size[1]=String_Len>>16;
        Split_Size[0]=String_Len>>24;
    }
   else if(Version_info == 4)  //Version 2.4 , Therefore convert size into Synchronisation Safe Integer From the String Size
    {
        Split_Size[3]=(String_Len & 0x0000007F);
        Split_Size[2]=(String_Len>>7 & 0x0000007F);
        Split_Size[1]=(String_Len>>14 & 0x0000007F);
        Split_Size[0]=(String_Len>>21 & 0x0000007F);
    }
    else
    {
        std::cout<<"\n Unsupported Tag or ID3 Version Type \n";
        return;
    }
 file.write(reinterpret_cast<const char *>(&Split_Size),4);
 file.write(reinterpret_cast<const char *>(&UD_text_encoding),1); //Set Encoding Byte = 0 i.e.  Ansi Format
 file.write(Description,Des_size);
 file.write(reinterpret_cast<const char *>(&UD_text_encoding),1); //Set Zero immediately After Description showing end of description string
 file.write(UD_text,UD_text_size);
 file.seekg(6,std::ios_base::beg); //Get Back To ID3 Header
std::cout<<"Header size= "<<Header_Size<<std::endl;
 String_Len = String_Len + Header_Size+ 10;  // Total Size = Previous Header Size + Total Change in Size + 10 Bytes of Frame Header

        Split_Size[3]=(String_Len & 0x0000007F);    // Encoding Synchronisation Safe Integer
        Split_Size[2]=(String_Len>>7 & 0x0000007F);
        Split_Size[1]=(String_Len>>14 & 0x0000007F);
        Split_Size[0]=(String_Len>>21 & 0x0000007F);
 file.write(reinterpret_cast<const char *>(&Split_Size),4);
 file.close(); //Finally Close the File

}


void Url_link_frame::Get_url_link_frame(int size,std::fstream& file)
{
    while(size!=0)
    {
      std::cout<<(char)file.get();
      size--;
    }

}

UD_url_frame::UD_url_frame()
:UD_url_encoding(0)
{}

void UD_url_frame::Get_UD_url_frame(int size,std::fstream& file)
{
    file.read((char*)&UD_url_encoding,1);
    unsigned char info = 'A';

    std::cout<<"Description :";
    while(info!='\0')
    {
        info = file.get();
        std::cout<<info;
    }

    std::cout<<"\nUser Defined URL Link : ";
    while(size+1!=0)
    {
        std::cout<<(char)file.get();
        size--;
    }

}

void Popularimeter::Get_popularimeter(int size,std::fstream& file)
{
    unsigned char info;
    while(size!=0)
    {
        info = file.get();
        std::cout<<info;
        size--;
        if((info&0xFF)==0)
        break;
    }
    file.read((char*)&rating,1);
    std::cout<<"\nRatings : "<<(int)rating<<" Out of 255";
    size--;
    if(size!=0&&size>0)
    {
    std::cout<<"\nPlay Counter : This Song is Played ";
    {
      while(size!=0&&size>0)
       {
         info = file.get();
         std::cout<<(int)info;
         size--;
       }
    }
    std::cout<<" Times";
    }

}

Attached_Picture::Attached_Picture()
:Text_encoding(0)
{}

void Attached_Picture::Get_picture_frame(int size,std::fstream& file,int Cover_flag,char* Destination)
{
 file.read((char*)&Text_encoding,1);
 size--;
 unsigned char info='A';
int j=0;
 while((info&0xFF)!=0)
 {
    info = file.get();
    std::cout<<info;
    size--;
    ++j;
 }
 std::cout<<"\nPicture Type : ";
 file.read((char*)&Picture_type,1);
 int type = (Picture_type&0xFF);
    switch(type)
    {
        case 0: std::cout<<"Other Type";
                break;
        case 1: std::cout<<"32x32 Pixels 'file icon'";
                break;
        case 2: std::cout<<"Other File Icon";
                break;
        case 3: std::cout<<"Cover(Front)";
                break;
        case 4: std::cout<<"Cover(Back)";
                break;
        case 5: std::cout<<"Leaflet Page";
                break;
        case 6: std::cout<<"Media (e.g. label side of CD)";
                break;
        case 7: std::cout<<"Lead Artist/Lead Performer/Soloist";
                break;
        case 8: std::cout<<"Artist/Performer";
                break;
        case 9: std::cout<<"Conductor";
                break;
        case 10: std::cout<<"Band/Orchestra";
                break;
        case 11: std::cout<<"Composer";
                break;
        case 12: std::cout<<"Lyrics/Text Writer";
                break;
        case 13: std::cout<<"Recording Location";
                break;
        case 14: std::cout<<"During Location";
                break;
        case 15: std::cout<<"During Performance";
                break;
        case 16: std::cout<<"Movie/Video Screen Capture";
                break;
        case 17: std::cout<<"A Bright Coloured Fish";
                break;
        case 18: std::cout<<"Illustration";
                break;
        case 19: std::cout<<"Band/Artist Logotype";
                break;
        case 20: std::cout<<"Publisher/Studio Logotype";
                break;
        default: std::cout<<"Unknown Picture Type";
                break;
    }
 size--;
 std::cout<<"\nPicture Description : ";
 info = 'A';
 while((info&0xFF)!=0)
 {
    info = file.get();
    std::cout<<info;
    size--;

 }

 std::ofstream Cover;
 if (Cover_flag==0)
 {
    if(j==11) //==> Image/jpeg
        Cover.open("Cover.jpg", std::ios_base::binary |std::ios_base::out|std::ios_base::ate);
    else//==>Image/png
        Cover.open("Cover.png", std::ios_base::binary |std::ios_base::out|std::ios_base::ate);
 }
 else
 Cover.open(Destination, std::ios_base::binary |std::ios_base::out|std::ios_base::ate);

 while(size!=0)
 {
    info = file.get();
    Cover.write((char*)&info,1);
    size--;
 }
 Cover.close();

}

Unsync_lyrics_text::Unsync_lyrics_text()
:Text_encoding(0)
{}

void Unsync_lyrics_text::Get_unsync_lyrics(int size,std::fstream& file,int Lyrics_Flag,char* Destination)
{
    file.read((char*)&Text_encoding,1);
    size--;
    unsigned char info{'A'};
    file.read((char*)Language,3);
    Language[3]='\0';

    if(((Language[0]&0xFF)|(Language[1]&0xFF)|(Language[2]&0xFF))==0)
    std::cout<<"\nLyrics Language : English";

    else if(Language[0]=='X'&&Language[1]=='X'&&Language[2]=='X')
        std::cout<<"\nLyrics Language : Unknown";

    else
    std::cout<<"\nLyrics Language : "<<Language;

    size=size-3;
    std::cout<<"\nContent Discription : ";
    while((info&0xFF)!=0)
    {
        info = file.get();
        std::cout<<info;
        size--;
    }
    if(Lyrics_Flag==0)
    {
    std::cout<<"\n\n-------------------- Lyrics --------------------\n";
    while(size!=0)
    {
      std::cout<<(char)file.get();
      size--;
    }
    std::cout<<"\n------------ End Of Lyrics ------------\n";
    }
    else
    {
        std::ofstream Lyrics;
        char info;
    Lyrics.open(Destination, std::ios_base::out|std::ios_base::ate);
    while(size!=0)
    {
    info = (char)file.get();
    Lyrics.write((char*)&info,1);
    size--;
    }
 Lyrics.close();

    }
}

Sync_lyrics_text::Sync_lyrics_text()
:Text_encoding(0)
{}

Comments::Comments()
:Text_encoding(0)
{}

void Comments::Get_comment_frame(int size,std::fstream& file)
{
    file.read((char*)&Text_encoding,1);
    size--;
    file.read((char*)Language,3);
    Language[3]='\0';

    if(((Language[0]&0xFF)|(Language[1]&0xFF)|(Language[2]&0xFF))==0)
    std::cout<<"\n\nComment Language : English";

    else if(Language[0]=='X'&&Language[1]=='X'&&Language[2]=='X')
        std::cout<<"\nComment Language : Unknown";

    else
    std::cout<<"\nComment Language : "<<Language;
    size = size-3;
     unsigned char info;
    std::cout<<"\nShort Description : ";
    while(size!=0)
    {
        info = file.get();
        std::cout<<info;
        size--;
        if((info&0xFF)==0)
        break;
    }
    std::cout<<"\nActual Comment : ";
    while(size!=0)
    {
        std::cout<<(char)file.get();
        size--;
    }
    std::cout<<"\n";
}

void Private_frame::Get_private_frame(int size,std::fstream& file)
{
    unsigned char info;
    while(size!=0)
    {
        info = file.get();
        std::cout<<info;
        size--;
        if((info&0xFF)==0)
        break;
    }
    std::cout<<"(Private Frame) : ";
    while(size!=0)
 {
    std::cout<<file.get();
    size--;
 }

}

void Unknown_Frame::Skip_this_frame (int size,std::fstream& file)
{
    Size_to_skip = size;
    file.seekg(size,std::ios_base::cur);
    std::cout<<"Skipping Frame.... ";
}

void Skip_Silently::Skip_this_frame_silently(int size,std::fstream&  file)
{
    Size_to_skip = size;
    file.seekg(size,std::ios_base::cur);
}

