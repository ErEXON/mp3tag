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

/*
* This Source Code is Compiled using gcc with FLAGS -Wall -fexceptions -g -pedantic -std=c++11 -Wextra
* Your Compiler must follow the C++11 Standrad in order to build the Binary From this Source Code
*/

#include <iomanip>  //For Printing in specific order
#include <cstring>  //For String Operation
#include <cstdio>
#include <cstdlib>
#include <cstdio>
#include <stdexcept>    //For handling exceptions (well i haven't use most of them)
#include "ID3V2_Header.h"   //Main Header of Id3v2
#include "ID3V2_Extended.h" //Extended Header of Id3v2
#include "Frame_Header.h"   //Frame Header

#include <unistd.h>

#define Sleep(x) usleep(x*1000)

int End{0}; // Global Variable in order to indicate the End Of All Frames , Initiallized to zero.
char* Orignal_Music_file = NULL; //Pointer Containing the Source file Location , Initially NULL.

void Search_n_set(char*,char*,char*); //A function who will search for the particular Frame (Passed as an Argument)
                                      // and Set/Changes Its Fields Accordingly.

void Read_ID3v1(std::fstream&); //Function For Reading ID3v1 Tags.

uint32_t Get_Tag_Size(char* argv); //Function who will return the complete tag size (Including Header 10 Bytes)
bool split_file (uint32_t size,char* filelocation); //Function for splitting up files into tag file and music file
bool join (char* destination);  //Function for joining the splitted files back into orignal file.

int main (int argc,char*argv[])
{
if(argc==1) //If No Argument is Supplied To IDTE then, By default it will Execute this area
{
    std::cout<<"\nMP3 Command line v1.0 By Rajat Kosh \nCopyright (c)Team IDTE 2013-15\nWith Support of All Frames Of ID3v1 v1.1 v2.3 v2.4 and Lyrics v3 (Ansi and UTF-8 Only)\n";
    std::cout<<"\nUsage : mp3tag [Options] <Music Files>\nUse mp3tag --h For Detailed Help Information"<<std::endl;
    std::cout<<"Options:"<<std::endl;
    std::cout<<"--artist <artist name>"<<std::setw(25)<<"Set/Add Artist Name"<<std::endl;
    std::cout<<"--album <album name>"<<std::setw(26)<<"Set/Add Album Name"<<std::endl;
    std::cout<<"--track <track number>"<<std::setw(24)<<"Set/Add Track Name"<<std::endl;
    std::cout<<"--title <song title>"<<std::setw(26)<<"Set/Add Song Title"<<std::endl;
    std::cout<<"--year <released year>"<<std::setw(23)<<"Set/Add Song Year"<<std::endl;
    std::cout<<"--genre <song genre>"<<std::setw(26)<<"Set/Add Song Genre"<<std::endl;
    std::cout<<"\nSymbols such as <> {} [] () should not be included while providing command line arguments"<<std::endl;
    std::cout<<"these Symbols are just for seperation purpose."<<std::endl;
}
if(argc>4)  //if More no.s of arguments are provided to IDTE (i.e. 5 or more) then split up the file into two parts in order to increase its efficiency
{
    Orignal_Music_file = argv[argc-1];  //Save Current Source file location (for future use)

    uint32_t new_size = Get_Tag_Size(argv[argc-1]); //Get The Current File Complete Tag Size

    if(split_file(new_size,argv[argc-1]))   //Split up the file into two parts such that "tag_file.mp3" contains tag data Only
                                           //and "music_file.mp3" Contains the music data only
        argv[argc-1]=(char*)"tag_file.mp3"; //Set the source file = tag_file.mp3
    else                                    //If Splitting isn't possible then exit
        {
            std::cout<<"Unable to Create temporary file for Saving Tag Data"<<std::endl;
            return 0;
        }
}

for(int i=1;i<argc;i=i+2)   //Loop Everything ,in Case user will Provide Multiple Arguments at same Time (Some Of The Arguments Are Not yet Implemented)
{
    if(strcmp(argv[i],"--h")==0) //User Selects For Help (Code Followed By //NI Are Not Yet Implemented)
    {

    std::cout<<"\nMP3 Command line v1.0 By Rajat Kosh \nCopyright (c)Team IDTE 2013-15\nWith Support of All Frames Of ID3v1 v1.1 v2.3 v2.4 and Lyrics v3 (Ansi and UTF-8 Only)\n";
    std::cout<<"\nUsage : mp3tag [Options] <Music Files>"<<std::endl;
    std::cout<<"Options:"<<std::endl;
    std::cout<<"--artist <Artist Name>"<<std::setw(45)<<"Set/Add Artist Name"<<std::endl;
    std::cout<<"--album <Album Name>"<<std::setw(46)<<"Set/Add Album Name"<<std::endl;
    std::cout<<"--track <Track Number>"<<std::setw(44)<<"Set/Add Track Name"<<std::endl;
    std::cout<<"--title <Song Title>"<<std::setw(46)<<"Set/Add Song Title"<<std::endl;
    std::cout<<"--year <Released Year>"<<std::setw(43)<<"Set/Add Song Year"<<std::endl;
    std::cout<<"--genre <Song Genre>"<<std::setw(46)<<"Set/Add Song Genre"<<std::endl;
    std::cout<<"--composer <Composer Name>"<<std::setw(43)<<"Set/Add Song Composer"<<std::endl;
    std::cout<<"--publisher <Publisher Name>"<<std::setw(42)<<"Set/Add Song Publisher"<<std::endl;
    std::cout<<"--album-artist <Album Artist Name>"<<std::setw(39)<<"Set/Add Song Album Artist"<<std::endl;
    std::cout<<"--copyright <Copyright Message>"<<std::setw(42)<<"Set/Add Copyright Message"<<std::endl;
    std::cout<<"--text-frame <Frame ID> {Value}"<<std::setw(63)<<"Set/Add Particular Text Frame (Excluding TXXX)"<<std::endl;
    std::cout<<"--rating {email} [rating(Out of 255)]"<<std::setw(27)<<"Set Song Ratings"<<std::endl; //NI
    std::cout<<"--comment {Description} [lng] (Actual Text)"<<std::setw(25)<<"Add Comments To Song"<<std::endl; //NI
    std::cout<<"--picture {Description} [Type] (Picture File)"<<std::setw(24)<<"Add Cover Art To Song"<<std::endl; //NI
    std::cout<<"--utext {Description} [Text]"<<std::setw(50)<<"Set User Specified text Frame"<<std::endl; //NI
    std::cout<<"--url {Description} [URL]"<<std::setw(51)<<"Set User Specified URL Frame"<<std::endl; //NI
    std::cout<<"--get-cover <Filename>"<<std::setw(52)<<"Save Cover Art To Filename"<<std::endl;
    std::cout<<"--get-details"<<std::setw(62)<<"Prints Every Detail of Song"<<std::endl;
    std::cout<<"--get-ISRC"<<std::setw(79)<<"Get International Standard Recording Code"<<std::endl; //NI
    std::cout<<"--show-lyrics"<<std::setw(60)<<"Prints Lyrics Onto Screen"<<std::endl;
    std::cout<<"--get-lyrics <Filename>"<<std::setw(48)<<"Save Lyrics in Filename"<<std::endl;
    std::cout<<"--v1"<<std::setw(63)<<"Save ID3v1 Tag Only"<<std::endl;  //NI
    std::cout<<"--v2"<<std::setw(73)<<"Save ID3v2 Tag Only (default)"<<std::endl; //NI
    std::cout<<"--v23"<<std::setw(73)<<"Save Both ID3v1 and ID3v2 Tags"<<std::endl; //NI
    std::cout<<"--version"<<std::setw(70)<<"Show Available ID3 Tags in File"<<std::endl; //NI
    std::cout<<"--list-picture-types"<<std::setw(54)<<"List All The Picture Types"<<std::endl;
    std::cout<<"--list-all-frames"<<std::setw(62)<<"List All The Standrad Frame IDs"<<std::endl;
    std::cout<<"--list-media-types"<<std::setw(50)<<"List All Media Types"<<std::endl;
    std::cout<<"--list-event-types"<<std::setw(72)<<"List All Music Synchronisation Event Types"<<std::endl;
    std::cout<<"--list-id3v1-genre"<<std::setw(77)<<"List All ID3v1 Genres (Including Winamp Genres)"<<std::endl;
    std::cout<<"--doc"<<std::setw(66)<<"Show IDTE Documentation"<<std::endl;
    std::cout<<"--play"<<std::setw(105)<<"Play/Preview Music File (Uses inbuilt Bass Engine for Playback)"<<std::endl;
    std::cout<<"\nSymbols such as <> {} [] () should not be included while providing command line arguments"<<std::endl;
    std::cout<<"these Symbols are just for seperation purpose. Batch File Processing is Also Accepted\n"<<std::endl;
    return 0;
    }
    else if(strcmp(argv[i],"--list-id3v1-genre")==0) //User Chose To View All Possible Genres Of ID3v1
    {
        std::cout<<"The following genres are defined in ID3v1:-"
        <<"\n0.Blues\n1.Classic Rock\n2.Country\n3.Dance\n4.Disco\n5.Funk\n6.Grunge\n7.Hip-Hop\n8.Jazz\n9.Metal\n10.New Age\n11.Oldies"
        <<"\n12.Other\n13.Pop\n14.R&B\n15.Rap\n16.Reggae\n17.Rock\n18.Techno\n19.Industrial\n20.Alternative\n21.Ska\n22.DeathMetal\n23.Pranks"
        <<"\n24.Soundtrack\n25.Euro-Techno\n26.Ambient\n27.Trip-Hop\n28.Vocal\n29.Jazz+Funk\n30.Fusion\n31.Trance\n32.Classical\n33.Instrumental"
        <<"\n34.Acid\n35.House\n36.Game\n37.Sound Clip\n38.Gospel\n39.Noise\n40.AlternRock\n41.Bass\n42.Soul\n43.Punk\n44.Space\n45.Meditative"
        <<"\n46.Instrumental Pop\n47.Instrumental Rock\n48.Ethnic\n49.Gothic\n50.Darkwave\n51.Techno-Industrial\n52.Electronic\n53.Pop-Folk"
        <<"\n54.Eurodance\n55.Dream\n56.Southern Rock\n57.Comedy\n58.Cult\n59.Gangsta\n60.Top 40\n61.Christian Rap\n62.Pop/Funk\n63.Jungle"
        <<"\n64.Native American\n65.Cabaret\n66.New Wave\n67.Psychadelic\n68.Rave\n69.Showtunes\n70.Trailer\n71.Lo-Fi\n72.Tribal\n73.Acid Punk"
        <<"\n74.Acid Jazz\n75.Polka\n76.Retro\n77.Musical\n78.Rock & Roll\n79.Hard Rock\n"<<std::endl;
        std::cout<<"The following genres are Winamp extensions:- "
        <<"\n80.Folk\n81.Folk-Rock\n82.National Folk\n83.Swing\n84.Fast Fusion\n85.Bebob\n86.Latin\n87.Revival\n88.Celtic\n89.Bluegrass\n"
        <<"90.Avantgarde\n91.Gothic Rock\n92.Progressive Rock\n93.Psychedelic Rock\n94.Symphonic Rock\n95.Slow Rock\n96.Big Band\n97.Chorus"
        <<"\n98.Easy Listening\n99.Acoustic\n100.Humour\n101.Speech\n102.Chanson\n103.Opera\n104.Chamber Music\n105.Sonata\n106.Symphony\n"
        <<"107.Booty Bass\n108.Primus\n109.Porn Groove\n110.Satire\n111.Slow Jam\n112.Club\n113.Tango\n114.Samba\n115.Folklore\n116.Ballad\n"
        <<"117.Power Ballad\n118.Rhythmic Soul\n119.Freestyle\n120.Duet\n121.Punk Rock\n122.Drum Solo\n123.A capella\n124.Euro-House\n"
        <<"125.Dance Hall"<<std::endl;
    return 0;
    }
    else if(strcmp(argv[i],"--list-picture-types")==0) //User Chose To View All Possible Pictures Type
    {
    std::cout<<"Followings Are The Picture Types With Their Codes:- "
    <<"\n00	Other\n01	32x32 pixels 'file icon' (PNG only)\n02	Other file icon"
    <<"\n03	Cover (front)\n04	Cover (back)\n05	Leaflet page\n06	Media (e.g. lable side of CD)\n"
    <<"07	Lead artist/lead performer/soloist\n08	Artist/performer\n09	Conductor\n0A	Band/Orchestra\n"
    <<"0B	Composer\n0C	Lyricist/text writer\n0D	Recording Location\n0E	During recording\n0F	During performance\n"
    <<"10	Movie/video screen capture\n11	A bright coloured fish\n12	Illustration\n13	Band/artist logotype\n"
    <<"14	Publisher/Studio logotype"<<std::endl;
    return 0;
    }
    else if(strcmp(argv[i],"--list-all-frames")==0) //User Chose To View All Possible Frame Type
    {
     std::cout<<"Generic Frames in ID3v1 and v2:-"
     <<" \nAENC\nAPIC\nCOMM\nCOMR\nENCR\nEQUA\nETCO\nGEOB\nGRID\nIPLS\nLINK\nMCDI\nMLLT"
     <<"\nOWNE\nPRIV\nPCNT\nPOPM\nPOSS\nRBUF\nRVAD\nRVRB\nSYLT\nSYTC\nTALB\nTBPM\nTCOM\n"
     <<"TCON\nTCOP\nTDAT\nTDLY\nTENC\nTEXT\nTFLT\nTIME\nTIT1\nTIT2\nTIT3\nTKEY\nTLAN\nTLEN"
     <<"\nTMED\nTOAL\nTOFN\nTOLY\nTOPE\nTORY\nTOWN\nTPE1\nTPE2\nTPE3\nTPE4\nTPOS\nTPUB\nTRCK"
     <<"\nTRDA\nTRSN\nTRSO\nTSIZ\nTSRC\nTSSE\nTYER\nTXXX\nUFID\nUSER\nUSLT\nWCOM\nWCOP\nWOAF\nWOAR"
     <<"\nWOAS\nWORS\nWPAY\nWPUB\nWXXX\n\nNew Frames in ID3v2.4:-\nASPI\nEQU2\nRVA2\nSEEK\nSIGN\nTDEN"
     <<"\nTDOR\nTDRC\nTDRL\nTDTG\nTIPL\nTMCL\nTMOO\nTPRO\nTSOA\nTSOP\nTSOT\nTSST\nTCMP\n"<<std::endl;
   return 0;
    }
    else if(strcmp(argv[i],"--list media-type")==0) //User Chose To View All Possible Media Type
    {
        std::cout<<"DIG	 	"
        <<"Other digital media\n/A\n 	Analog transfer from media\nANA	Other analog media\n/WAC"
        <<"	Wax cylinder\n/8CA\n 	8-track tape cassette\nCD	CD\n/A	Analog transfer from media\n"
        <<"/DD	DDD\n/AD	ADD\n/AA\n 	AAD\nLD	Laserdisc\n/A\n 	Analog transfer from media\nTT	"
        <<"Turntable records\n/33	33.33 rpm\n/45	45 rpm\n/71	71.29 rpm\n/76	76.59 rpm\n/78	78.26 rpm\n"
        <<"/80\n 	80 rpm\nMD	MiniDisc\n/A\n 	Analog transfer from media\nDAT	DAT\n/A	Analog transfer from media\n"
        <<"/1	standard, 48 kHz/16 bits, linear\n/2	mode 2, 32 kHz/16 bits, linear\n/3	mode 3, 32 kHz/12 bits, nonlinear, low speed\n"
        <<"/4	mode 4, 32 kHz/12 bits, 4 channels\n/5	mode 5, 44.1 kHz/16 bits, linear\n/6\n 	mode 6, 44.1 kHz/16 bits, 'wide track' play\n"
        <<"DCC	DCC\n/A\n 	Analog transfer from media\nDVD	DVD\n/A\n 	Analog transfer from media\nTV	Television\n/PAL	PAL\n/NTSC	NTSC\n"
        <<"/SECAM\n 	SECAM\nVID	Video\n/PAL	PAL\n/NTSC	NTSC\n/SECAM	SECAM\n/VHS	VHS\n/SVHS	S-VHS\n/BETA\n 	BETAMAX\nRAD	Radio\n"
        <<"/FM	FM\n/AM	AM\n/LW	LW\n/MW\n 	MW\nTEL	Telephone\n/I\n 	ISDN\nMC	MC (normal cassette)\n"
        <<"/4	4.75 cm/s (normal speed for a two sided cassette)\n/9	9.5 cm/s\n/I	Type I cassette (ferric/normal)\n"
        <<"/II	Type II cassette (chrome)\n/III	Type III cassette (ferric chrome)\n/IV\n 	Type IV cassette (metal)\nREE	Reel\n"
        <<"/9	9.5 cm/s\n/19	19 cm/s\n38	38 cm/s\n/76	76 cm/s\n/I	Type I cassette (ferric/normal)\n/II	Type II cassette (chrome)"
        <<"\n/III	Type III cassette (ferric chrome)\n/IV\n 	Type IV cassette (metal)"<<std::endl;
    return 0;
    }
    else if(strcmp(argv[i],"--list-event-type")==0) //User Chose To View All Possible Event Types
    {
        std::cout<<"Followings are the Event Time Coding of a song:-"
        <<" \n00	padding (has no meaning)\n01	end of initial silence\n02	intro start\n03	mainpart start\n04	outro start\n"
        <<"05	outro end\n06	verse start\n07	refrain start\n08	interlude start\n09	theme start\n0A	variation start\n0B	key change\n"
        <<"0C	time change\n0D	momentary unwanted noise (Snap, Crackle & Pop)\n0E	sustained noise\n0F	sustained noise end\n10	intro end\n"
        <<"11	mainpart end\n12	verse end\n13	refrain end\n14	theme end\n15-\nDF	reserved for future use\nE0-\n"
        <<"EF	not predefined sync 0-F\nF0-\nFC	reserved for future use\nFD	audio end (start of silence)\nFE	audio file ends\n"
        <<"FF	one more byte of events follows \n(all the following bytes with the value FF have the same function)"<<std::endl;
    return 0;
    }
    else if(strcmp(argv[i],"--doc")==0) //User Chose To View Documentation Of IDTE
    {
        std::cout<<"\nIDTE- ID3 Tag Editor by Rajat kosh\nCopyright (c) 2013-14 rEX_ON_FiRE\n\n"
        <<"IDTE is free software: you can redistribute it and/or modify\n"
        <<"it under the terms of the GNU General Public License as published by\n"
        <<"the Free Software Foundation, either version 3 of the License, or\n"
        <<"(at your option) any later version.\n\n"
        <<"IDTE is distributed in the hope that it will be useful,\n"
        <<"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
        <<"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
        <<"GNU General Public License for more details.\n\n"
        <<"You should have received a copy of the GNU General Public License\n"
        <<"along with IDTE .  If not, see <http://www.gnu.org/licenses/>.\n"
        <<"\n\nReport your Bugs at rajatkosh2153@gmail.com\n";
    return 0;
    }
    else if(strcmp(argv[i],"--artist")==0)  //Change or Add Artist Field
    {
        std::cout<<"\nArtist Frame is ";
        Search_n_set((char*)"TPE1",argv[i+1],argv[argc-1]);
        End = 0;     //Specify That End is not Reached Yet (Helpful in Case, User Provide multiple Arguments)
    }
    else if(strcmp(argv[i],"--album")==0)   //Change or Add Album Field
    {
        std::cout<<"\nAlbum Frame is ";
        Search_n_set((char*)"TALB",argv[i+1],argv[argc-1]);
        End = 0;
    }
    else if(strcmp(argv[i],"--title")==0)   //Change or Add Title Field
    {
        std::cout<<"\nTitle Frame is ";
        Search_n_set((char*)"TIT2",argv[i+1],argv[argc-1]);
        End = 0;

    }
    else if(strcmp(argv[i],"--track")==0)   //Change or Add Track Field
    {
        std::cout<<"\nTrack Frame is ";
        Search_n_set((char*)"TRCK",argv[i+1],argv[argc-1]);
        End = 0;
    }
    else if(strcmp(argv[i],"--genre")==0)   //Change or Add Genre Field
    {
        std::cout<<"\nGenre Frame is ";
        Search_n_set((char*)"TCON",argv[i+1],argv[argc-1]);
        End = 0;
    }
    else if(strcmp(argv[i],"--composer")==0)    //Change or Add Composer Field
    {
        std::cout<<"\nComposer Frame is ";
        Search_n_set((char*)"TCOM",argv[i+1],argv[argc-1]);
        End = 0;
    }
    else if(strcmp(argv[i],"--publisher")==0)   //Change or Add Publisher Field
    {
        std::cout<<"\nPublisher Frame is ";
        Search_n_set((char*)"TPUB",argv[i+1],argv[argc-1]);
        End = 0;
    }
    else if(strcmp(argv[i],"--album-artist")==0)    //Change or Add Album Artist Field
    {
        std::cout<<"\nAlbum Artist Frame is ";
        Search_n_set((char*)"TPE2",argv[i+1],argv[argc-1]);
        End = 0;
    }
    else if(strcmp(argv[i],"--copyright")==0)   //Change or Add Copyright Message
    {
        std::cout<<"\nCopyright Frame is ";
        Search_n_set((char*)"TCOP",argv[i+1],argv[argc-1]);
        End = 0;
    }
    else if(strcmp(argv[i],"--year")==0)    //Change or Add Year Field
    {
        std::cout<<"\nYear Frame is ";
        Search_n_set((char*)"TYER",argv[i+1],argv[argc-1]);
        End = 0;
    }
    else if(strcmp(argv[i],"--get-details")==0) //Print Out Complete Details Onto Screen
    {
        std::fstream file;
        file.open(argv[argc-1], std::ios_base::binary |std::ios_base::in |std::ios_base::out);  //Open The Current File
        if(!file.is_open()) //Check If Opened Successfully
        {
            std::cout<<"Unable To Open File!!!! :("<<std::endl;
            return 0;
        }
        Frame_Header Frame; //Create A Frame Header Object
        Frame.Get_Header(file); //Get The Header
        Frame.Show_Header();    //Show Onto Screen (If Successful Then Show Information else Show Error Message)

        if(Frame.Is_present==0) //If Header is Not Present Then Exit
            return 0;

        ID3V2_Extended Ext_Header;

        Ext_Header.Get_Extended(file); //Check For Extended Header
        Ext_Header.Show_Extended(); //Show Onto Screen If Present

    std::cout<<"\n";

    while(End!=1)   //Loop All The Frames Until End of Frame Is Reached
    {
       Frame.Get_Frame(file);   //Get The Frame
       Frame.Show_Frame(file,&End); //Show Onto Screen
    }
    file.close();   //Close The File
    return 0;       //Exit
    }
    else if(strcmp(argv[i],"--text-frame")==0) //User Chose For Altering Any Text Frame (Excluding TXXX)
    {
        std::cout<<"\nText Frame is ";
        Search_n_set(argv[i+1],argv[i+2],argv[argc-1]);
        End = 0;
    }
    else if(strcmp(argv[i],"--get-cover")==0) //User Chose To save Cover Art Of A File
    {
        std::fstream file;
        file.open(argv[argc-1], std::ios_base::binary |std::ios_base::in |std::ios_base::out);  //Open The File
        if(!file.is_open()) //Checks For Availability
        {
            std::cout<<"Unable To Open File! :("<<std::endl; //Shows The Error Onto Screen
            return 0;
        }
        Frame_Header Frame;
        Frame.Get_Header(file);
        if(Frame.Is_present==0)
            return 0;

        ID3V2_Extended Ext_Header;

        Ext_Header.Get_Extended(file);
        std::cout<<"\nExtracting Cover ...\n";
        while(End!=1)
        {
            Frame.Get_Frame(file);
            Frame.Search_only_frame(file,(char*)"APIC",&End,argv[i+1]);
        }
        return 0;
    }
    else if(strcmp(argv[i],"--get-lyrics")==0)  //Save Lyrics into File
    {
      std::fstream file;
        file.open(argv[argc-1], std::ios_base::binary |std::ios_base::in |std::ios_base::out);  //Open The File
        if(!file.is_open()) //Checks For Availability
        {
            std::cout<<"Unable To Open File! :("<<std::endl; //Shows The Error Onto Screen
            return 0;
        }
        Frame_Header Frame;
        Frame.Get_Header(file);
        if(Frame.Is_present==0)
            return 0;

        ID3V2_Extended Ext_Header;

        Ext_Header.Get_Extended(file);
        std::cout<<"\nExtracting Lyrics From File...\n";
        while(End!=1)
        {
            Frame.Get_Frame(file);
            Frame.Search_only_frame(file,(char*)"USLT",&End,argv[i+1]);
        }
        return 0;
    }
    else if(strcmp(argv[i],"--utext")==0)
    {
       std::cout<<"Will Be Implemented Soon..."<<std::endl;
        return 0;

    }
    else if(argc==2)// If only filename is provided i.e. No More Arguments
    {
        std::fstream file;
        file.open(argv[argc-1], std::ios_base::binary |std::ios_base::in |std::ios_base::out);
        if(!file.is_open())
        {
            std::cout<<"No Files and or Invalid File :("<<std::endl;
            return 0;
        }
        Frame_Header Frame;
        Frame.Get_Header(file);
        Frame.Show_Header();
        if(Frame.Is_present==0)
            return 0;

        ID3V2_Extended Ext_Header;
        Ext_Header.Get_Extended(file);
        Ext_Header.Show_Extended();

    std::cout<<"\nProcessing ID3v2 Tag..\n\n";
        std::cout<<"Album : ";
        while(End!=1)
        {
        Frame.Get_Frame(file);
        Frame.Search_only_frame(file,(char*)"TALB",&End);
        }
        End =0;
        file.seekg(10);
        std::cout<<"Title : ";
        while(End!=1)
        {
        Frame.Get_Frame(file);
        Frame.Search_only_frame(file,(char*)"TIT2",&End);
        }
        End =0;
        file.seekg(10);
        std::cout<<"Track : ";
        while(End!=1)
        {
        Frame.Get_Frame(file);
        Frame.Search_only_frame(file,(char*)"TRCK",&End);
        }
        End =0;
        file.seekg(10);
        std::cout<<"Artist : ";
        while(End!=1)
        {
        Frame.Get_Frame(file);
        Frame.Search_only_frame(file,(char*)"TPE1",&End);
        }
        End =0;
        file.seekg(10);
        std::cout<<"Album Artist : ";
        while(End!=1)
        {
        Frame.Get_Frame(file);
        Frame.Search_only_frame(file,(char*)"TPE2",&End);
        }
        End =0;
        file.seekg(10);
        std::cout<<"Genre : ";
        while(End!=1)
        {
        Frame.Get_Frame(file);
        Frame.Search_only_frame(file,(char*)"TCON",&End);
        }
        End =0;
        file.seekg(10);
        std::cout<<"Year : ";
        while(End!=1)
        {
        Frame.Get_Frame(file);
        Frame.Search_only_frame(file,(char*)"TYER",&End);
        }
        End =0;
        file.seekg(10);
        std::cout<<"Publisher : ";
        while(End!=1)
        {
        Frame.Get_Frame(file);
        Frame.Search_only_frame(file,(char*)"TPUB",&End);
        }
        End =0;
        file.seekg(10);
        std::cout<<"Composer : ";
        while(End!=1)
        {
        Frame.Get_Frame(file);
        Frame.Search_only_frame(file,(char*)"TCOM",&End);
        }
        End =0;

        std::fstream file_2; //Another File pointer For Reading ID3v1 Tags
        file_2.open(argv[argc-1], std::ios_base::binary |std::ios_base::in |std::ios_base::out);
       if(!file_2.is_open())
        std::cout<<"Unable To Read ID3v1 Tag"<<std::endl;
        Read_ID3v1(file_2);
    std::cout<<"\nTag Process Finished Sucessfully..\n\n";

    return 0;
    }
    else if(argc==3)
    {
        std::cout<<"\nError : Wrong Arguments and or File.\nRemember - Arguments are Case Sensitive. Use IDTE --h For More Information"<<std::endl;
        return 0;
    }
    else
    {
      if(argc>4)
        {
            if(join(Orignal_Music_file))
                {
                    std::cout<<"Finalizing file... Done"<<std::endl;
                    return 0;
                }
            else
            std::cout<<"Sorry :( an Error is encountered"<<std::endl;
        }
        else
        return 0;
    }
}
}

/***
* function Name = Search_n_set()
* return Value = Nothing
* Arguments = Frame_name,argv,argv1
* Frame_name = desired frame that is to be changed or added into file e.g. "TCOM" (Composer)
* argv = 1st Argument viz passed by user as a value i.e. frame value of desired frame name e.g. "Linkin Park"
* argv1 = 2nd Argument viz a filename i.e. source file e.g. /home/IDTE/music/powerless.mp3

* Side Effects = if no ID3 Frame and/or header is present in file, then this function will automatically create that frame and/or header
                 for that file
**/

void Search_n_set (char* Frame_name,char*argv,char*argv1)
{
        std::fstream file;
        file.open(argv1, std::ios_base::binary |std::ios_base::in |std::ios_base::out); //Open The File
        if(!file.is_open()) //Check if File is open
            {
                throw std::runtime_error("\nUnable To Open File\n");
            }
        Frame_Header Frame;
        Frame.Get_Header(file); //Get the Header of The ID3 Tag
        if(Frame.Is_present==0) //If there is No ID3 Tag Present in The File
        {
        char ch;
        std::cout<<" Unavailable!\nReason : This File Does Not Contain Any Recognized ID3 Tag into It\n"
        <<"Do You Want to add first ID3 Tag into it? (y/n) : "; //Prompt User in Order To Create ID3 Tag Into File
        std::cin>>ch; //Take Input From User
        /****************************  Write ID3 Header Into Tag if there is no such tag exists in File *****************/

        if(ch=='y'|| ch=='Y')   //If User wants to create ID3 Tag Into File
            {
                std::fstream file_1,file_2; //Create Two Temporary Files object
                file_1.open("file.mp3",std::ios_base::binary|std::ios_base::out);   //Open temporary file for Output Only

                if(!file_1.is_open())   //Check If File is Open
                    {
                        throw std::runtime_error("\nUnable to Create a Temporary File For Storage\n");
                    }

                /*
                                                     _____________________
                Our Motive is to Write 10 Bytes -->  |I|D|3|4|0|0|0|0|0|0|  into temporary file
                */
                file_1.write("ID3",3);  //Write ID3 Header into new Temporary file created Above

                int Version_ID3{4}; //Set Value of ID3 Version = 4 (IDTE Will By-Default Write ID3v2.4 )
                int Zero[6]{};   //Write Version 2.4 (Without Flags) (Create An Array of size = 6. All Initiallized to zero)

                file_1.write(reinterpret_cast<const char *>(&Version_ID3),1); //Write 4 at 3rd Byte indicating that ID3v2.4 is Present
                file_1.write(reinterpret_cast<const char *>(&Zero),6);  //Zero All The Flag Bytes And the Size Bytes.

/**********************************Create a temorary Copy of Our Music File in Dynamic Array******************/

            file.seekg(0,std::ios_base::end);//Seek at the very end Of Orignal File (File containing No Tags into it).
            uint64_t Part1 = file.tellg(); //Store The Current position Of file in Part1.(Part1 = Complete Size of The File).
            char* Arr = new char[Part1];    //Create A Dynamic Array which Can Store the Binary Data upto Part1
            file.seekg(0);// Seek At The Front of The file
            file.read((char*)Arr,Part1);    //read data into dynamic array upto End of our Orignal Music File
            file_1.write((char*)Arr,Part1); // Append/ Write Data into Our Temporary File
            file_1.close();  //Close The Temporary File
            delete [] Arr; //Free Up the Space in order to prevent The Memory Leakage.

/************************************** Begin The Copying Process *****************************************/

            file_2.open("file.mp3",std::ios_base::binary|std::ios_base::out|std::ios_base::in);//Open The Temporary File

            if(!file_2.is_open()) //checks if file opened sucessfully
                std::cout<<"Error : An Unknown Error Occurrred. Error Code = 0x00000007\n";
            Part1 = Part1+10;   //Increase The Size By 10, Since the size of temporary file is 10 Bytes Larger due to addition of ID3 Header
            file_2.seekg(0,std::ios_base::beg);//Move The Pointer To Begining Of File
            Arr = new char[Part1];  //Create New Space
            file_2.read(Arr,Part1); //Copy The Temporary File into Array

            file.seekg(0,std::ios_base::beg);   //Seek At The Front Of our orignal Music File
            if(!file.is_open())
                {
                    throw std::runtime_error("\nError: Unable To Write Destination File\n");
                }
            file.write((char*)Arr,Part1);   //Write Everything From Temporary File into Music File (Overwrite the previous File)
            delete [] Arr;  //Free up the Space
            file_2.close(); //Close The Temporary File
            file.close();   //Close The Music File
        std::cout<<"\nID3 Tag created sucessfully. Now you are good to go..\n\n";
        std::cout<<"Current Frame ";
        Search_n_set (Frame_name,argv,argv1);
        return;
        }
        else
            {
                std::cout<<"\n";
                return ;
            }
        }

        ID3V2_Extended Ext_Header;

        Ext_Header.Get_Extended(file);  //Checks For the extended header (if any)
        Ext_Header.Show_Extended(); //Shows onto screen (if Present)

    while(End!=1)   //Loop Until the desired frame is Obtained
    {
       Frame.Get_Frame(file); //Get Frame
       Frame.Search_frame(file,&End,Frame_name,argv,argv1);  //Checks if current Frame is required one
    }

    file.close();
}

/**
* function name = Read_ID3v1 ()
* return value = Nothing
* arguments = file
* file = Source file pointer
**/
void Read_ID3v1 (std::fstream& file)
{
    char id3v1[128];
        file.seekg(-128,std::ios_base::end);    //Seelk onto last 128 Bytes
        file.read((char*)id3v1,128);    //Copy Everything into an Array Of 128 Characters
for(int i=0;i<128; i++)
  {
      if(i<3)
      {
        if(id3v1[0]!='T'&&id3v1[1]!='A'&&id3v1[2]!='G') //Header ==> T A G
        {
           std::cout<<"No ID3v1.x Tags Are Present"<<std::endl;
          break;
        }
        else
        {
            std::cout<<"\nParsing ID3v1...\n";
            std::cout<<"\nTotal Size of Tag = 128 bytes (Generic)\n";
            i+=3;
        }
      }

      if(i==3)
      {
          std::cout<<"\nTitle: ";
      }
      if(i>=3&&i<33)
      {
          std::cout<<id3v1[i];
      }
      if(i==33)
       std::cout<<"\nArtist: ";

      if(i>=33&&i<63)
      {
       std::cout<<id3v1[i];
      }
      if(i==63)
       std::cout<<"\nAlbum: ";

      if(i>=63&&i<93)
      {
       std::cout<<id3v1[i];
      }
      if(i==93)
       std::cout<<"\nYear: ";

      if(i>=93&&i<97)
      {
       std::cout<<id3v1[i];
      }

      if(i==97)
       std::cout<<"\nComments: ";

      if(i>=97&&i<125)
      {
       std::cout<<id3v1[i];
      }

      if(i==125)
      {
       std::cout<<"\nTrack Number : ";
      }

      if(i>=125&&i<127)
      {
          id3v1[i]=id3v1[i];

       std::cout<<(int)id3v1[i];

      }

      if(i==127)
      {
       std::cout<<"\nGenre Code: ";
       if((int)id3v1[i]<0)
       std::cout<<"Unspecified Genre Code";
       else
        std::cout<<(int)id3v1[i]<<" Use (--list-id3v1-genre) For More information on genre codes";
       std::cout<<"\n";
      }
  }
}

/**
* Function Name = Get_End()
* Return = unsigned integer 32-bit containing size of the complete tag (size of tag + 10 bytes of header)
* Arguments = file

* file = Source file pointer

* Important = Get_Tag_Size returns size of tag using ID3 Header Information.

**/

uint32_t Get_Tag_Size(char* argv)
{
    std::fstream file;
    file.open(argv, std::ios_base::binary |std::ios_base::in |std::ios_base::out); //Open The File
    if(!file.is_open()) //Check if File is open
        {
            throw std::runtime_error("\nUnable To Open File\n");
        }
    Frame_Header Frame;
    Frame.Get_Header(file); //Get the Header of The ID3 Tag
    file.close();
    return (Frame.Get_Size()+10); //Size of Tag + 10 bytes of header Itself
}

/**
* Function Name = split_file()
* Return = boolean value (true or false) True, if files splits sucessfully else False
* Arguments = size, filelocation

* size = 32-bit unsigned integer containing size of the tag viz required to act as a partitioner in order to split the file
        into two parts.
* filelocation = location of source file viz required to split-up into two parts

* Description = This function will split-up file into two parts , Splitting up of file is required in case of large sized
                files because , the continous copying and appending requires alot of data i/o and therefore slows down the
                process of Tagging.

**/

bool split_file (uint32_t size,char* filelocation)
{
    std::fstream file1,file2,file3;
    file1.open(filelocation,std::ios_base::binary|std::ios_base::in);
    if(!file1.is_open())
    {
        std::cout<<"Sorry :( an Error is encountered"<<std::endl;
        return false;
    }
    file1.seekg(0);
    char* buffer = new char[size];
    file1.read(buffer,size);
    file1.seekg(0,std::ios_base::end);
    int size1 = file1.tellg();
    file1.seekg(size);
    int size2 = file1.tellg();
    char* buffer2 = new char[size1-size2];
    file1.read(buffer2,size1-size2);
    file2.open("tag_file.mp3",std::ios_base::binary|std::ios_base::out);
    if(!file1.is_open())
        {
            std::cout<<"Sorry :( an Error is encountered while creating temporary tag file"<<std::endl;
            return false;
        }
    file2.write(buffer,size);
    file3.open("music_file.mp3",std::ios_base::binary|std::ios_base::out);
    if(!file3.is_open())
        {
            std::cout<<"Sorry :( an Error is encountered while creating temporary Music file"<<std::endl;
            return false;
        }
    file3.write(buffer2,size1-size2);
    file2.close();
    file3.close();
    delete []buffer2;
    delete []buffer;
    return true;
}

/**
* Function Name = join ()
* Return = boolean value (true or false)
* Arguments = destination

* destination = destination file location, at which joining takes place

* Description = This function will join the two splitted files (tag_file.mp3 and music_file.mp3) at destination location.

* Important = files must be splitted out before joining takes place
**/

bool join (char* destination)
{
    std::fstream file1,file2,Final;
    file1.open("tag_file.mp3",std::ios_base::binary|std::ios_base::in);
    if(!file1.is_open())
    {
        std::cout<<"Sorry :( an Error is encountered"<<std::endl;
        return false;
    }
    file2.open("music_file.mp3",std::ios_base::binary|std::ios_base::in);
    if(!file1.is_open())
    {
        std::cout<<"Sorry :( an Error is encountered while Parsing Tag"<<std::endl;
        return false;
    }
    file1.seekg(0,std::ios_base::end);
    file2.seekg(0,std::ios_base::end);
    int size1 = file1.tellg();
    int size2 = file2.tellg();
    char* Buff = new char[size1];
    char* Buff2 = new char[size2];
    file1.seekg(0);
    file2.seekg(0);
    file1.read(Buff,size1);
    file2.read(Buff2,size2);
    Final.open(destination,std::ios_base::binary|std::ios_base::out);
    Final.write(Buff,size1);
    Final.write(Buff2,size2);
    Final.close();
    delete []Buff;
    delete []Buff2;
    return true;
}
