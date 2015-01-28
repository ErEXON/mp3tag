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


#include "Frame_Header.h"
#include "All_Frames.h"
#include "ID3V2_Header.h"
#include <cstring>

const char* Frame_list[]{ // List Of All Possible Frames in ID3v2.3/4
"AENC", //Audio encryption
"APIC", //Attached picture
"COMM", //Comments
"COMR", //Commercial frame
"ENCR", //Encryption method registration
"EQUA", //Equalization
"ETCO", //Event timing codes
"GEOB", //General encapsulated object
"GRID", //Group identification registration
"IPLS", //Involved people list
"LINK", //Linked information
"MCDI", //Music CD identifier
"MLLT", //MPEG location lookup table
"OWNE", //Ow999nership frame
"PRIV", //Private frame
"PCNT", //Play counter
"POPM", //Popularimeter
"POSS", //Position synchronization frame
"RBUF", //Recommended buffer size
"RVAD", //Relative volume adjustment
"RVRB", //Reverb
"SYLT", //Synchronized lyric/text
"SYTC", //Synchronized tempo codes
"TALB", //Album/Movie/Show title
"TBPM", //BPM (beats per minute)
"TCOM", //Composer
"TCON", //Content type
"TCOP", //Copyright message
"TDAT", //Date
"TDLY", //Playlist delay
"TENC", //Encoded by
"TEXT", //Lyricist/Text writer
"TFLT", //File type
"TIME", //Time
"TIT1", //Content group description
"TIT2", //Title/song name/content description
"TIT3", //Subtitle/Description refinement
"TKEY", //Initial key
"TLAN", //Language(s)
"TLEN", //Length
"TMED", //Media type
"TOAL", //Original album/movie/show title
"TOFN", //Original filename
"TOLY", //Original lyricist(s)/text writer(s)
"TOPE", //Original artist(s)/performer(s)
"TORY", //Original release year
"TOWN", //File owner/licensee
"TPE1", //Lead performer(s)/Soloist(s)
"TPE2", //Band/orchestra/accompaniment
"TPE3", //Conductor/performer refinement
"TPE4", //Interpreted, remixed, or otherwise modified by
"TPOS", //Part of a set
"TPUB", //Publisher
"TRCK", //Track number/Position in set
"TRDA", //Recording dates
"TRSN", //Internet radio station name
"TRSO", //Internet radio station owner
"TSIZ", //Size
"TSRC", //ISRC (international standard recording code)
"TSSE", //Software/Hardware and settings used for encoding
"TYER", //Year
"TXXX", //User defined text information frame
"UFID", //Unique file identifier
"USER", //Terms of use
"USLT", //Unsynchronized lyric/text transcription
"WCOM", //Commercial information
"WCOP", //Copyright/Legal information
"WOAF", //Official audio file webpage
"WOAR", //Official artist/performer webpage
"WOAS", //Official audio source webpage
"WORS", //Official Internet radio station homepage
"WPAY", //Payment
"WPUB", //Publishers official webpage
"WXXX", //User defined URL link frame
"ASPI", //Audio seek point //-------------------NEW ID3V2.4 FRAMES-----------------------//
"EQU2",// Equalization
"RVA2",// Relative volume adjustment
"SEEK",// Seek frame
"SIGN",// Signature frame
"TDEN", //Encoding time
"TDOR", //Original release time
"TDRC", //Recording time
"TDRL", //Release time
"TDTG", //Tagging time
"TIPL", //Involved people list
"TMCL", //Musician credits list
"TMOO", //Mood
"TPRO", //Produced notice
"TSOA", //Album sort order
"TSOP", //Performer sort order
"TSOT", //Title sort order
"TSST", //Set subtitle
"TCMP", //Non Standard Itunes Frame
};

enum{ // Assigning Values (Default) To every Frame Name in Order To Increase Readability
AENC,
APIC,
COMM,
COMR,
ENCR,
EQUA,
ETCO,
GEOB,
GRID,
IPLS,
LINK,
MCDI,
MLLT,
OWNE,
PRIV,
PCNT,
POPM,
POSS,
RBUF,
RVAD,
RVRB,
SYLT,
SYTC,
TALB,
TBPM,
TCOM,
TCON,
TCOP,
TDAT,
TDLY,
TENC,
TEXT,
TFLT,
TIME,
TIT1,
TIT2,
TIT3,
TKEY,
TLAN,
TLEN,
TMED,
TOAL,
TOFN,
TOLY,
TOPE,
TORY,
TOWN,
TPE1,
TPE2,
TPE3,
TPE4,
TPOS,
TPUB,
TRCK,
TRDA,
TRSN,
TRSO,
TSIZ,
TSRC,
TSSE,
TYER,
TXXX,
UFID,
USER,
USLT,
WCOM,
WCOP,
WOAF,
WOAR,
WOAS,
WORS,
WPAY,
WPUB,
WXXX,
ASPI,
EQU2,
RVA2,
SEEK,
SIGN,
TDEN,
TDOR,
TDRC,
TDRL,
TDTG,
TIPL,
TMCL,
TMOO,
TPRO,
TSOA,
TSOP,
TSOT,
TSST,
TCMP // Non Standrad Itunes Frame (Self Tested)
};

/**
Function Name = Get_Version ()
Returns =   ID3 Version present in file
Arguments = No Arguments
**/

int Frame_Header::Get_Version(void) //Get Version Information From File
    {
                return (Version);
    }

/**
Function Name = Get_Header_Size ()
Returns =   Total Header Size of Current Frame
Arguments = No Arguments
**/

unsigned int Frame_Header::Get_Header_Size(void)
{
            return (Header_size);
}

/**
Function Name = Get_Frame()
Returns =    Nothing
Arguments = file
file = source file pointer

Description = Reads complete frame header (10 bytes) by desynchronising the Synchronisation safe integer (v2.4) if any.

**/

void Frame_Header:: Get_Frame(std::fstream& file) // Get/Read Frame Header
{
   unsigned char memory[4];

    file.read((char*)&Frame_id,4); // Frame ID Goes Here i.e. TALB,TART,TCOM,COMM etcetera etcetra.
    Frame_id[4]='\0';
    file.read((char*)memory ,4);   // Reads Data into Memory Array
    if(Get_Version()==4)        // ==> if Version = 4 then Decode Synchronisation Safe Integer
    {
    Frame_size =  (memory[3] & 0xFF) |
                  ((memory[2] & 0xFF) << 7 ) |
                  ((memory[1] & 0xFF) << 14 ) |
                  ((memory[0] & 0xFF) << 21 );
    }
    else
    {                       // Else No Further Need To Desynchronise Because Fames Are Not Synchronised
    Frame_size =  (memory[3] & 0xFF) |
                  ((memory[2] & 0xFF) << 8 ) |
                  ((memory[1] & 0xFF) << 16 ) |
                  ((memory[0] & 0xFF) << 22 );
    }
    file.read((char*)Frame_flags,2);

}

/**
Function Name = Create_Text_Frame ()
Returns =  Nothing
Arguments = file,arg,arg2,Frame_name

file =  source file pointer
arg =   Source File location (required in order to keep track of Source file size,header-size etc.)
arg2 =  Value that is to be written into new frame
Frame_name =    Name of the frame that is to be created

Description = Creates Frame by Calling the Create_Text_Frame() member function of Text_frame Class
**/


void Frame_Header::Create_Text_Frame(std::fstream& file,char* arg,char* arg2,char* Frame_name)
    {
        Text_frame TEXT;
        int Version_info = Get_Version();
        unsigned int Header_Size = Get_Header_Size();
        TEXT.Create_Text_Frame(file,arg,arg2,Frame_name,Version_info,Header_Size);
        std::cout<<"\n";
    }

/**
Function Name = Skip_Frame ()
Returns =    Nothing
Arguments = file

file = current Source file pointer

Description = Skips a particular frame viz not known or implemented
**/


void Frame_Header::Skip_Frame(std::fstream& file)
    {
        Unknown_Frame SKP;
        SKP.Skip_this_frame(Frame_size,file);
        std::cout<<"\n";
    }

/**
Function Name = Skip_Silent ()
Returns =    Nothing
Arguments = file

file = current Source file pointer

Description = Skips a particular frame whenever required (for internal Use Only).
**/


void Frame_Header::Skip_Silent(std::fstream& file)
    {
        Skip_Silently SKPSL;
        SKPSL.Skip_this_frame_silently(Frame_size,file);
    }

/**
Function Name = Get_Text ()
Returns =    Nothing
Arguments = file

file = Current Source file pointer

Description = show text of current frame onto screen
**/

void Frame_Header::Get_Text (std::fstream& file)
    {
        Text_frame text;
        text.Get_text_frame(Frame_size , file);
        std::cout<<"\n";
    }

/**
Function Name = Get_Rating ()
Returns =    Nothing
Arguments = file

file = Current file pointer

Description = Shows frame ratings onto screen

**/

void Frame_Header::Get_Rating (std::fstream& file)
    {
        Popularimeter POP;
        POP.Get_popularimeter(Frame_size,file);
        std::cout<<"\n";

    }


/**
Function Name = Get_UDText()
Returns =    Nothing
Arguments = file

file = Current Source file pointer

Description = Shows User Defined Frame Details onto screen
**/

void Frame_Header::Get_UDText(std::fstream& file)
    {
        UD_text_frame UDT;
        UDT.Get_UD_text_frame(Frame_size,file);
        std::cout<<"\n";
    }

/**
Function Name = Priv_Frame ()
Returns = Nothing
Arguments = file

file = current source file pointer

Description = Shows private frame (if any) onto screen
**/

void Frame_Header::Priv_Frame(std::fstream& file)
    {
        Private_frame PVT;
        PVT.Get_private_frame(Frame_size,file);
        std::cout<<"\n";
    }


/**
Function Name = Get_CoverArt()
Returns =    Nothing
Arguments = file,[Destination]

file =  Current Source file pointer
Destination = Destination file in which cover will get saved (optional) e.g. /home/media/my-stuff/cover.jpg

Description = if No Destination file is specified then, cover will automatically get saved in IDTE Directory
**/

void Frame_Header::Get_CoverArt(std::fstream& file,int Cover_flag,char* Destination)
    {
        Attached_Picture PCT;
        PCT.Get_picture_frame(Frame_size,file,Cover_flag,Destination);
        std::cout<<"\n";

    }
/**
Function Name = Set_Text ()
Returns = Nothing
Arguments = file,arg,arg2

file = currrent source file pointer
arg = Value that is to be assign to the currently processing frame
arg2 = Location of the source Music file.

Description = Sets/Adds/Modify the text frame accordingly
**/


void Frame_Header::Set_Text(std::fstream& file,char* arg,char*arg2)
    {
        Text_frame text;
        int Version_info = Get_Version();
        unsigned int Header_Size = Get_Header_Size();
        text.Set_text_frame(Frame_size,file,arg,arg2,Version_info,Header_Size);
        std::cout<<"\n";
    }


/**
Function Name = Get_USlyrics()
Returns = nothing
Arguments = file,lyrics_flag,[Destination]

file = Current Source file pointer
Lyrics_Flag = 0 or 1 ,Indicates whether show lyrics onto screen or save into file.
Destination = Destination file in which lyrics will get saved (Optional if Flag =0)

Description =  Lyrics will get saved into Destination file iff lyrics flag =1.

**/

void Frame_Header::Get_USlyrics(std::fstream& file,int Lyrics_Flag,char* Destination)
    {
        Unsync_lyrics_text USL;
        USL.Get_unsync_lyrics(Frame_size,file,Lyrics_Flag,Destination);
        std::cout<<"\n";
    }

/**
Function Name = Get_Url()
Returns = Nothing
Arguments = file

file = Current Source file pointer

Description = Shows URl link onto screen (if any)
**/

void Frame_Header::Get_Url(std::fstream& file)
    {
        Url_link_frame URLF;
        URLF.Get_url_link_frame(Frame_size,file);
        std::cout<<"\n";
    }


/**
Function Name = Get_Comment()
Returns =    Nothing
Arguments = file

file = Current Source file pointer

Description = Shows Comments onto  screen
**/

void Frame_Header::Get_Comment(std::fstream& file)
    {
        Comments COMN;
        COMN.Get_comment_frame(Frame_size,file);
        std::cout<<"\n";
    }


/**
Function Name = Show_Frame()
Returns =  Nothing
Arguments = file,End

file = Current Source file pointer
End = Flag, in order to indicate end of all frames

Description = This function compare current ID3 Frame-ID with all 93 standrad Frame-IDs and print details onto screen by calling
the corresponding FrameID function (if any match Found) else Set End =1 indicating End of All Frames.
**/

void  Frame_Header:: Show_Frame(std::fstream& file,int* End)
    {

 int i; // Make it Local For This Function in order to use below

for(i=0;i<93;i++)
 {
    if(strcmp((char*)Frame_id,Frame_list[i])==0)
    break;
 }

if(i==93) // Well, No Match is Found
    {
        std::cout<<"\nTag Processing Finished Sucessfully :)\n";
        *End =1; // Terminate The Loop By Setting End =1 (Global Variable)
    }
    else // A Match is Found
    {
switch(i) // Jump To Label\Match According To Value of i
        {
        case AENC:Skip_Frame(file);
                   break;
        case APIC: std::cout<<"Picture MIME Type : ";
                   Get_CoverArt(file,0);
                   break;
        case COMM: Get_Comment(file);
                   break;
        case COMR: std::cout<<"Seller Information : ";
                   Skip_Frame(file);
                  break;
        case ENCR: std::cout<<"Encrypted by : ";
                    Skip_Frame(file);
                  break;
        case EQU2:
        case EQUA: std::cout<<"Equilisation : ";
                  Skip_Frame(file);
                  break;
        case ETCO: std::cout<<"Event Timming Codes : ";
                 Skip_Frame(file);
                 break;
        case GEOB: std::cout<<"Encapsulated Object : ";
                 Skip_Frame(file);
                 break;
        case GRID: std::cout<<"Groutp Identification Frame : ";
                 Skip_Frame(file);
                 break;
        case TMCL:
        case TIPL:
        case IPLS: std::cout<<"Credits/Involved People : ";
                   Get_Text(file);
                 break;
        case LINK: std::cout<<"Linked Information : ";
                 Skip_Frame(file);
                 break;
        case MCDI: std::cout<<"Music CD Identifier is Found in Tag (Check Your Binary Data MCDI).\nFor More Information Please Visit http://www.cddb.com in Order to Identify Your CD in Database\n";
                 Skip_Frame(file);
                 break;
        case MLLT: std::cout<<"MPEG Locaion Lookup Table : ";
                 Skip_Frame(file);
                 break;
        case OWNE: std::cout<<"Transaction/Bill Proof : ";
                 Skip_Frame(file);
                 break;
        case PRIV:
                 Priv_Frame(file);
                 break;
        case PCNT: std::cout<<"Play Counter : ";
                 Skip_Frame(file);
                 break;
        case POPM: std::cout<<"Song Rating By : ";
                   Get_Rating(file);
                    break;
        case POSS:  std::cout<<"Position Synchronization : ";
                 Skip_Frame(file);
                 break;
        case RBUF:  std::cout<<"Buffer Size (Recommended) : ";
                 Skip_Frame(file);
                 break;
        case RVA2:
        case RVAD: std::cout<<"Relative Volume Adjustments (Normailization) : ";
                 Skip_Frame(file);
                 break;
        case RVRB: std::cout<<"Reverb : ";
                 Skip_Frame(file);
                 break;
        case SYLT: std::cout<<"=============== Synchronised Lyrics =============== :";
                 Skip_Frame(file);
                 break;
        case SYTC: std::cout<<"Synchronised Tempo : ";
                 Skip_Frame(file);
                 break;
        case TALB: std::cout<<"Album : ";
                  Get_Text(file);
                  break;
        case TBPM: std::cout<<"Beats/Minute : ";
                  Get_Text(file);
                  break;
        case TCOM: std::cout<<"Composer : ";
                  Get_Text(file);
                  break;
        case TCON: std::cout<<"Genre : ";
                  Get_Text(file);
                  break;
        case TCOP: std::cout<<"Copyright : ";
                  Get_Text(file);
                  break;
        case TDAT: std::cout<<"Date : ";
                  Get_Text(file);
                  break;
        case TDLY: std::cout<<"Playlist Delay : ";
                  Get_Text(file);
                  break;
        case TENC: std::cout<<"Encoded By : ";
                  Get_Text(file);
                  break;
        case TEXT: std::cout<<"Lyrics/Text Writer : ";
                  Get_Text(file);
                  break;
        case TFLT: std::cout<<"File Type : ";
                  Get_Text(file);
                  break;
        case TIME: std::cout<<"Recording Time : ";
                  Get_Text(file);
                  break;
        case TIT1: std::cout<<"Content Group Description : ";
                  Get_Text(file);
                  break;
        case TIT2: std::cout<<"Title : ";
                  Get_Text(file);
                  break;
        case TIT3: std::cout<<"Subtitle/Description Refinement : ";
                  Get_Text(file);
                  break;
        case TKEY: std::cout<<"Music Key (Start With) : ";
                  Get_Text(file);
                  break;
        case TLAN: std::cout<<"Audio Language : ";
                  Get_Text(file);
                  break;
        case TLEN: std::cout<<"Song Length (Milliseconds) : ";
                  Get_Text(file);
                  break;
        case TMED: std::cout<<"Media Type : ";
                  Get_Text(file);
                  break;
        case TOAL: std::cout<<"Source title : ";
                  Get_Text(file);
                  break;
        case TOFN: std::cout<<"Orignal Filename : ";
                  Get_Text(file);
                  break;
        case TOLY: std::cout<<"Orignal Lyricist/Text Writer : ";
                  Get_Text(file);
                  break;
        case TOPE: std::cout<<"Orignal Artist/Performer : ";
                  Get_Text(file);
                  break;
        case TDOR:
        case TORY: std::cout<<"Orignal Release Year : ";
                  Get_Text(file);
                  break;
        case TOWN: std::cout<<"File Owner/Licensee : ";
                  Get_Text(file);
                  break;
        case TPE1: std::cout<<"Artist : ";
                  Get_Text(file);
                  break;
        case TPE2: std::cout<<"Band/Orchestra/Album Artist : ";
                  Get_Text(file);
                  break;
        case TPE3: std::cout<<"Conductor : ";
                  Get_Text(file);
                  break;
        case TPE4: std::cout<<"Remixed By : ";
                  Get_Text(file);
                  break;
        case TPOS: std::cout<<"Disc Number : ";
                  Get_Text(file);
                  break;
        case TPUB: std::cout<<"Publisher : ";
                  Get_Text(file);
                  break;
        case TRCK: std::cout<<"Track Number : ";
                  Get_Text(file);
                  break;
        case TRDA: std::cout<<"Recording Dates : ";
                  Get_Text(file);
                  break;
        case TRSN: std::cout<<"Internet Radio Station : ";
                  Get_Text(file);
                  break;
        case TRSO: std::cout<<"Internet Radio Station Owner : ";
                  Get_Text(file);
                  break;
        case TSIZ: std::cout<<"Audio Size (Bytes) : ";
                  Get_Text(file);
                  break;
        case TSRC: std::cout<<"ISRC (International Standrad recording Code) : ";
                  Get_Text(file);
                  break;
        case TSSE: std::cout<<"Audio Encoder Settings : ";
                  Get_Text(file);
                  break;
        case TDRC:
        case TYER: std::cout<<"Year : ";
                  Get_Text(file);
                  break;
        case TXXX: std::cout<<"\nUser Specified Text ";
                  Get_UDText(file);
                   break;
        case UFID: std::cout<<"Unique File Identification : ";
                  Skip_Frame(file);
                  break;
        case USER: std::cout<<"Terms Of Use : ";
                  Skip_Frame(file);
                  break;
        case USLT: std::cout<<"\n============== Unsynchronised Lyrics ==============";
                 Get_USlyrics(file,0);
                  break;
        case WCOM: std::cout<<"Commercial Information Link : ";
                 Get_Url(file);
                  break;
        case WCOP: std::cout<<"Copyright/Legal Information Link : ";
                 Get_Url(file);
                  break;
        case WOAF: std::cout<<"Official audio file webpage : ";
                 Get_Url(file);
                  break;
        case WOAR: std::cout<<"Official artist/performer webpage : ";
                 Get_Url(file);
                  break;
        case WOAS: std::cout<<"Official audio source webpage : ";
                 Get_Url(file);
                  break;
        case WORS: std::cout<<"Official internet radio station homepage : ";
                 Get_Url(file);
                  break;
        case WPAY: std::cout<<"Payment Site : ";
                 Get_Url(file);
                  break;
        case WPUB: std::cout<<"Publishers official webpage : ";
                 Get_Url(file);
                  break;
        case WXXX: std::cout<<"User Defined URl link : ";
                  break;
        case TCMP: std::cout<<"ITunes (Nonstandrad Frame TCMP) : ";
                  Get_Text(file);
                  break;

       }
    }
}

/**
Function Name = Search_frame()
Returns = Nothing
Arguments = file,End,Frame_name,arg,arg2

file = Current source file pointer
End = Flag, in order to indicate that end of all frames is reached
Frame_name = Particular Frame Name to Search for e.g. "TPE2"
arg = Value passed to Frame name e.g. "OneRepublic"
arg2 = Filename of the source music file

Description = Search for paticular frame and invoke the corresponding function for altering that frame
              if the frame is unavailable then, a new frame will be created

**/

void Frame_Header::Search_frame(std::fstream& file,int* End,char* Frame_name,char* arg,char* arg2)
{
  int i; // Make it Local For This Function in order to use below

for(i=0;i<93;i++)
 {
    if(strcmp((char*)Frame_id,Frame_list[i])==0)
    break;
 }
    if(i==93)
    {
        std::cout<<" not Available in File!\nCreating One...";
        Create_Text_Frame(file,arg,arg2,Frame_name);
        *End = 1;
    }
   else
   {
        if(strcmp((char*)Frame_id,Frame_name)==0)
        {
           Set_Text(file,arg,arg2);
            *End = 1;
        }
        else
        {
           Skip_Silent(file);
        }
   }

}


/**
Function Name = Search_only_frame()
Returns = Nothing
Arguments = file,Frame_name,End,Destination

file = Current source file pointer
End = Flag, in order to end of all frame is reached
Frame_name  = particular frame to search for e.g. "TALB"
Destination = Destination file in which Inforormation will get saved

Description = Search_only_frame will search for particular frame and stores information of that frame into destination file

**/

void Frame_Header::Search_only_frame(std::fstream& file,char* Frame_name,int* End,char*Destination)
{

int i; // Make it Local For This Function in order to use below

for(i=0;i<93;i++)
 {
    if(strcmp((char*)Frame_id,Frame_list[i])==0)
    break;
 }
    if(i==93)
    {
        std::cout<<"\n";
        *End = 1;
    }
   else
   {
        if(strcmp((char*)Frame_id,Frame_name)==0)
        {
            if(strcmp((char*)Frame_id,"APIC")==0)
            {
                std::cout<<"Picture MIME Type : ";
                Get_CoverArt(file,1,Destination);
            }
            else if(strcmp((char*)Frame_id,"USLT")==0)
            {
                Get_USlyrics(file,1,Destination);
            }
            else
            {
                Get_Text(file);
                file.seekg(10,std::ios_base::beg);
            }
            *End = 1;
        }
        else
        {
           Skip_Silent(file);
        }
   }

}
/**
Function Name = Get_End ()
returns = Nothing
Arguments = file,End

file = Current Source file pointer
End = Flag Pointer, in order to show the end of frame is reached

Description =  This function will set the file pointer at the very end of the frame

**/

void Frame_Header::Get_End(std::fstream& file,int* End)
{
int i; // Make it Local For This Function in order to use below

for(i=0;i<93;i++)
 {
    if(strcmp((char*)Frame_id,Frame_list[i])==0)
    break;
 }
    if(i==93)
        {
            *End = 1;
        }
        else
            {
                Skip_Silent(file);
            }

}
/**
Function Name = Add_UDText()
Returns = Nothing
Arguments = file,Description,UD_Text

file = Current File pointer (Genrerally at the end of Tag)
Description = Description of User Specified Text
UD_Text = User Defined Text

**/

void Frame_Header::Add_UDText(std::fstream& file,char* Description,char* UD_Text)
{
    int End = 0;
    while(End!=1)
    {
        Get_End(file,&End);
    }
    file.seekg(-10,std::ios_base::cur);
    UD_text_frame UDF;
    int version = Get_Version();
    unsigned int hdr_size = Get_Header_Size();
    UDF.Create_UD_text_frame(file,version,Description,UD_Text,hdr_size);
    file.close();
}
