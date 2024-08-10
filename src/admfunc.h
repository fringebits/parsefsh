/* Copyright 2013 Bernhard R. Fischer, 2048R/5C5FFD47 <bf@abenteuerland.at>
 *
 * This file is part of Parseadm.
 *
 * Parseadm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * Parseadm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Parseadm. If not, see <http://www.gnu.org/licenses/>.
 */

/*! 
 *  
 *
 *  @author Bernhard R. Fischer
 */

#include <inttypes.h>


#define MAX_FAT_BLOCKLIST 240
#define FAT_SIZE 0x200

#define ADM_EPOCH ((time_t) 631062000L+3600)
#define ADM_LON_SCALE 11930463.0783    //<! tolerance 1.016E-5 - -6.299E-6
#define ADM_LAT_SCALE ADM_LON_SCALE
#define ADM_DEPTH_D 1067808470.8490566037
#define ADM_DEPTH_K 22137.4773584905
#define ADM_DEPTH(x) (((x) - ADM_DEPTH_D) / ADM_DEPTH_K)
#define ADM_DEPTH_NA 0x69045951


#ifdef __GNUC__
#define PACK(__Declaration__) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK(__Declaration__)                                                  \
  __pragma(pack(push, 1)) __Declaration__ __pragma(pack(pop))
#endif

PACK(struct adm_header_t
{
   char xor_byte;
   char null0[7];
   uint8_t ver_major;         //<! 0x008 map's major version
   uint8_t ver_minor;
   uint8_t upd_month;
   uint8_t upd_year;
   int16_t null1;
   int8_t mapsource;          //<! 0x00e 0: Garmin map, 1 Mapsource map
   char checksum;             //<! 0x00f
   char sig[7];               //<! 0x010 "DSKIMG\0"
   char b02_0;                //<! 0x017 always 2
   uint16_t sec0;
   uint16_t head0;
   uint16_t cyl0;
   char null2[18];
   char data[9];              //<! 0x030 unknown bytes, all 0 in IMG
   uint16_t creat_year;       //<! 0x039 creation year
   uint8_t creat_month;
   uint8_t creat_day;
   uint8_t creat_hour;
   uint8_t creat_min;
   uint8_t creat_sec;
   uint8_t fat_phys_block;    //<! 0x040 physical block # of FAT
   char ident[7];             //<! 0x041 "GARMIN\0"
   char null3;
   char map_desc[20];
   uint16_t head1;
   uint16_t sec1;
   uint8_t blocksize_e1;
   uint8_t blocksize_e2;
   uint16_t q;
   char map_name[31];
});

PACK(struct adm_partition_t
{
   char boot;
   uint8_t start_head;
   uint8_t start_sec;
   uint8_t start_cyl;
   uint8_t type;
   uint8_t end_head;
   uint8_t end_sec;
   uint8_t end_cyl;
   uint32_t rel_secs;
   uint32_t num_secs;
});

PACK(struct adm_fat_t
{
   char subfile;           //<! 1 in real subfiles
   char sub_name[8];
   char sub_type[3];
   uint32_t sub_size;
   uint16_t next_fat;      /*<! not sure if this starts 1 byte later,
                             this is 0 in the first block and increments by 256
                             in each following block. */
   char y[14];
   uint16_t blocks[1];     // declare at least ONE 'block'
});

//! name field
#define DESC_TYPE_NAME 0x2c
//! number of data elements (trackpoints)
#define DESC_TYPE_NUMTP 0x2d
//! start address of first data element
#define DESC_TYPE_DATA_START 0x30
//! latitude
#define DESC_TYPE_LAT 0xf4
//! longitude
#define DESC_TYPE_LON 0xf5
//! timestamp
#define DESC_TYPE_TSTAMP 0xf6
//! depth
#define DESC_TYPE_DEPTH 0xf7
//! temperature
#define DESC_TYPE_TEMP 0xf9

PACK(struct adm_descriptor_t
{
   uint8_t type;              //<! descriptor type
   char a;                 //<! unknown, always 1
   uint16_t size;          //<! number of bytes of described element
});

PACK(struct adm_trk_header
{
   uint16_t hl;            //<! 0x000 common header length, = 0
   uint32_t len;           //<! 0x002 total length (including this header + trk_header2 + all trackpoints)
   int32_t a;
   char b;
   int32_t c;
   int16_t d;
   uint32_t len1;          //<! 0x011 len - 15
   uint32_t start_hdr_tbl;          //!< pointer to header descriptor table
   uint32_t hdr_tbl_entries;        //!< number of entries in header descriptor table
   uint32_t start_data_desc_tbl;    //!< pointer to data descriptor table
   uint32_t data_desc_tbl_entries;  //!< number of entries in data descriptor table
   uint32_t start_hdr;              //!< pointer to first header
   uint32_t e;                      //!< how many blocks of data? usually 1?
});

PACK(struct adm_trk_trailer_t
{
   uint16_t a;             //!< always 0x0001
   uint32_t b;             //!< always 0x0000000a
   uint32_t c;             //!< unknown, checksum?
});

PACK(struct adm_track_point_t
{
   int32_t lat;         /*<! latitude and longitude linearly scaled by
                          ADM_LAT_SCALE and ADM_LON_SCALE */
   int32_t lon;
   int32_t timestamp;   /*<! timestamp starting at ADM_EPOCH after the epoch of
                          1.1.1970 */
   int32_t depth;       /*<! scaled depth, could be 2 int16_t fields as well
                          with the second one being the depth */
   char d;              //<! 0 or 1 at first point
   int32_t tempr;       //<! temperature
});