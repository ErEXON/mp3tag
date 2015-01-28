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

#include "ID3V2_Extended.h"
#include "ID3V2_Header.h"
#include <iostream>
#include <stdint.h>
#include <cstdio>


void ID3V2_Extended::Get_Extended (std::fstream& file)
{
 if (Flag==2) // Checks whether Extended Header is Present or Not
  {
   unsigned char memory[4];
    file.read((char*)memory ,4); // Header Extended Size Must Go Here
    file.read((char*)&Extended_flags,2); // Store The Extended Flags (CRC of Tag)
    file.read((char*)&Padding_size,4); // Store the Padding Size i.e. Size Without The Frames and Headers

    Extended_size =(memory[3] & 0xFF) |
                   ((memory[2] & 0xFF) << 7 ) |
                   ((memory[1] & 0xFF) << 14 ) |
                   ((memory[0] & 0xFF) << 21 );
  }
}

void ID3V2_Extended:: Show_Extended (void)
{
 if (Flag==2)
  {
    std::cout<<"Extended Tag Size = "<<float(Extended_size)/1000<<" (KB) KiloBytes or "<<Extended_size<<" Bytes"<<std::endl;
    std::cout<<"Tag CRC = "<<Extended_flags<<" , Tag Padding Size = "<< Padding_size <<" Bytes"<<std::endl;
  }

}
