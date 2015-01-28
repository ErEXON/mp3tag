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


#include <string>
#include <stdint.h>
#include <fstream>
#ifndef ALL_FRAMES_H
#define ALL_FRAMES_H

class Unique_file_id
{
    protected:
            char* Owner_identifier;
            char identifier[8];
    public:
            void Get_unique_id (int size,std::fstream&);
            void Set_unique_id (int size,std::fstream&);
};

class Text_frame
{
    protected:
            uint8_t Text_encoding;
            char* Information;
    public:
            Text_frame();
            void Get_text_frame(int size,std::fstream&);
            void Set_text_frame(int size,std::fstream&,char* arg,char*arg2,int,unsigned int );
            void Create_Text_Frame(std::fstream&,char* arg,char* arg2,char* Frame_name,int,unsigned int );
};

class UD_text_frame
{
    protected:
            uint8_t UD_text_encoding;
            uint8_t Flags[2];
            uint32_t Split_Size[4];
    public:
            UD_text_frame();
            void Get_UD_text_frame (int size,std::fstream&);
            void Create_UD_text_frame(std::fstream& file,int Version_info,char* Description,char* UD_text,unsigned int Header_Size);

};

class Url_link_frame
{
    protected:
            std::string Url;
    public:
            void Get_url_link_frame (int size,std::fstream&);
            void Set_url_link_frame (int size,std::fstream&);
};

class Unknown_Frame
{
    protected:
                uint32_t Size_to_skip;
    public:
                void Skip_this_frame (int size,std::fstream&);

};

class Skip_Silently
{
    protected:
                uint32_t Size_to_skip;
    public:
                void Skip_this_frame_silently (int size,std::fstream&);

};

class UD_url_frame
{
    protected:
            uint8_t UD_url_encoding;
            char* UD_url_description;
            char* Url;
    public:
            UD_url_frame();
            void Get_UD_url_frame (int size,std::fstream&);
            void Set_UD_url_frame (int size,std::fstream&);
};

class Music_cd_frame
{
    protected:
            uint32_t Mcdi_header;
            uint64_t Bytes_per_track;
            uint64_t lead_out;
    public:
            void Get_music_cd_frame (int size,std::fstream&);
            void Set_music_cd_frame (int size,std::fstream&);

};

class Event_timing_frame
{
    protected:
            uint8_t Time_format;
            uint8_t Event_type;
    public:
            void Get_event_timing (int size,std::fstream&);
            void Set_event_timing (int size,std::fstream&);

};

class Mpeg_lookup_table
{
    protected:
            uint16_t Mpeg_frame_ref;
            char Byte_ref[3];
            char Milli_ref[3];
            uint8_t Bit_byte_dev;
            uint8_t Bit_milli_dev;
    public:
            void Get_mpeg_lookup(int size,std::fstream&);
            void Set_mpeg_lookup(int size,std::fstream&);

};

class Sync_tempo_codes
{
    protected:
            uint8_t Time_stamp;
    public:
            void Get_sync_tempo (int size,std::fstream&);

};

class Private_frame
{
    std::string owner;
public:
    void Get_private_frame (int size,std::fstream&);

};

class Unsync_lyrics_text
{
    protected:
            uint8_t Text_encoding;
            char Language[4];
            std::string description;
            std::string lyrics;
    public:
            Unsync_lyrics_text();
            void Get_unsync_lyrics (int size,std::fstream&,int Lyrics_Flag,char* Destination); // Lyrics Flag Shows Whether To Save Lyrics Into File Or Show Onto Screen


};

class Sync_lyrics_text
{
    protected:
            uint8_t Text_encoding;
            char Language[4];
            uint8_t Time_stamp;
            uint8_t Content_type;
            std::string lyrics;
    public:
            Sync_lyrics_text();
            void Get_sync_lyrics (int size,std::fstream&);

};

class Comments
{
    protected:
            uint8_t Text_encoding;
            char Language[4];
            std::string Description;
            std::string Comment;
    public:
            Comments();
            void Get_comment_frame(int size,std::fstream&);
            void Set_comment_frame(int size,std::fstream&);

};

class Attached_Picture
{
    protected:
            uint8_t Text_encoding;
            std::string MIME;
            uint8_t Picture_type;
            std::string Description;
    public:
            Attached_Picture();
            void Get_picture_frame(int size,std::fstream&,int Cover_flag,char* Destination = NULL);

};

class General_enc_object
{
    protected:
            uint8_t Text_encoding;
            std::string MIME;
            std::string Filename;
            std::string Description;
    public:
            void Get_enc_objet(int size,std::fstream&);

};

class Play_counter
{
    protected:
            char counter[4];
    public:
            void Get_play_counter(int size,std::fstream&);

};

class Popularimeter
{
    protected:
            std::string User_email;
            uint8_t rating;
    public:
            void Get_popularimeter(int size,std::fstream&);

};

class Audio_encryption
{
    protected:
            std::string Owner_id;
            uint8_t prev_start,prev_length;
    public:
            void Get_audio_enc (int size,std::fstream&);

};

#endif // ALL_FRAMES_H
