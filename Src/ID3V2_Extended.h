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
#include <iostream>
#include <stdint.h>
#include <cstdio>

#ifndef ID3V2_EXTENDED_H
#define ID3V2_EXTENDED_H

/*
ID3 Extended Header:-
If The Flag Byte in Main Header is Having A Bit Pattern of 01000000 Then an Extended Header is Present in File.
The extended header contains information that is not vital to the correct parsing of the tag information, hence the extended header is optional.

First 4 Bytes = Extended header size = 4bytes = 32 bits
Next 2 Bytes = Extended Flags	= %x0000000 00000000 , If this flag is set four bytes of CRC-32 data is appended to the extended header. The CRC should be calculated before unsynchronisation on the data between the extended header and the padding, i.e. the frames and only the frames.
Next 4 Bytes = Size of padding	$xx xx xx xx = The 'Size of padding' is simply the total tag size excluding the frames and the headers, in other words the padding
*/
class ID3V2_Extended : protected ID3V2_Header
{
    public:
            void Get_Extended(std::fstream& file); //Function To Write Information into The Members
            void Show_Extended(void); //Function To Show The Information After Writing
    private:
            uint32_t Extended_size; // Extended Header Size (Synchronisation Safe Integer)
            uint16_t Extended_flags; // Flags Of Extended Header Which Represents whether A 32 bit CRC Data is Present or Not
            uint32_t Padding_size; // Padding Size , Here We Indicated This Size is 4bytes = 32bits in An Single Integer of 4 Bytes.
};

#endif // ID3V2_EXTENDED_H
