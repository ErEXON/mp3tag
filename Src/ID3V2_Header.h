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


#include <cstdio>
#include <stdint.h>
#include <fstream>
#include <iostream>

#ifndef ID3V2_HEADER_H
#define ID3V2_HEADER_H

/**
ID3 Header :-

The first three bytes of an MP# (Music/Mpeg File) Consist of Three Letters i.e. -->'I' 'D' '3' in order to show that the file is consisting
ID3v2 tag into it.

The next byte (i.e. 4th Byte) Consist of ID3 version (v2.x where x = 2,3 or 4) e.g. 4th Byte = 3 => Version =ID3v2."3"

The next byte (i.e. 5th Byte) Consist of ID3 Revision No. i.e. 2.x.y where y = revision No.
(Most of the times this byte is left empty for the future purposes)

The next byte is flag byte (byte no. 6). Flag byte shows information about extended tags and other information (format = abc00000 )
where a,b,c represents the corresponding flags.

The next four bytes shows information about the SIZE (combined/total) of the tag but each byte's most significant bit (MSB - that is, the 7 bit)
is set to zero and is ignored.
The remaining 28 bits represent the actual value .An integer encoded in this way is known as a synchronization safe integer
(See comments below for more information).

**/

class ID3V2_Header
{
    public:
            int Is_present; //Checks For ID3v2 Tag
            void Get_Header(std::fstream& file); //Function To Write Information into The Members
            void Show_Header(void); //Function To Show The Information After Writing
            uint32_t Get_Size (void);
    protected:
            unsigned char Header_id[3]; // Here We Set The Header ID upto 3 Bytes in order to Store The Word "I" "D" "3".
            uint8_t Version;  // We set the size and type of unsigned integer of length 8 bits to the Major Version of ID3v2
            uint8_t Revision; // We set the size and type of unsigned integer of length 8 bits to the Revision number of ID3v2
            unsigned char Header_flags; // Flags are in %abc00000 Form , Therefore We indicate one flag byte, we use unsigned
                                        // characters so they can take values 0 - 255 and not -128 up to 127
            int Flag; // Checks For ID3v2 Flags And Set Flags Accordingly
            uint32_t Header_size; // We indicate the size by 4 bytes = 32 bits (Synchronization Safe Integer,See Below)
};

#endif // ID3V2_HEADER_H

/** Please Must Read The Documentation Of ID3 Tag at www.id3.org for detailed information about bit pattern of ID3.

  ---------------------------------------------------------------------------------------
  |ID3v2-Tag|ID3v2-Extended Tag |Lyrics v3 |Audio Data | ID3v1 extended-Tag | ID3v1-Tag | <----- MP3 File
  ---------------------------------------------------------------------------------------

Synchronization Safe Integer (SIZE WITH A TWIST):-

Every MPEG file e.g. MP3 file consists of music/audio data But, this audio data is kept somewhere in between of ID3v2 and ID3v1 Tag.
This Audio Data is recognized by our music player (Generally legacy players) if and only if it Starts with a bit
pattern of 0xFF 0xFF (in hex) or 11111111 11111111 (in binary).

Now let's assume, we have our tag data in such a way that it forms a pattern such as 11111111 1111111 therefore this will provide a false signal
to our music player and as a result, our music player will try to play this tag information instead of music data and Hence our Music Player Might
Get Crashed.
            Now , as Stated above "Every MSB is Set to Zero" therefore we will not be able to get our tag data in 11111111 11111111 pattern
instead we get 01111111 01111111 and hence any incompatible Music Player will Skip this data until it found the bit pattern of FFFF and Safely
plays the Music Data without any false signaling.

**/

