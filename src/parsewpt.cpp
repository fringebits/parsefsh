
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>

#include "parsewpt.h"
#include "admfunc.h"
#include "helpers.h"

// parsewpt

// header 7 * 16 - 3


// waypoint (4 * 16 + 6) = 70b??
// char[10] name
// char[20] description
// uint8_t  symbol
// byte[40] unknown (always A4??)

// 109 bytes
PACK(struct wpt_header_t
{
    uint8_t  null0[32];
    uint16_t count;
    uint8_t  null1[75];
});

// 71 bytes
PACK(struct wpt_waypoint_t
{
     char     name[10];
     char     description[20];
     uint8_t  symbol;
     uint8_t  body[40];
});

namespace {

//void output_node(const adm_track_point_t *tp)
//{
//   char ts[TBUFLEN] = "";
//   double tempr, depth;
//   struct tm *tm;
//   time_t t;
//
//   t = tp->timestamp + ADM_EPOCH;
//   if ((tm = gmtime(&t)) != NULL)
//      strftime(ts, sizeof(ts), "%Y-%m-%dT%H:%M:%SZ", tm);
//
//   if (tp->tempr != ADM_DEPTH_NA)
//      //tempr = (double) tp->tempr / 1E7;
//      tempr = tp->tempr / ADM_LON_SCALE;
//   else
//      tempr = NAN;
//
//   if (tp->depth != ADM_DEPTH_NA)
//      depth = ADM_DEPTH(tp->depth);
//   else
//      depth = NAN;
//
//   printf("%s,%.4f,%.4f,%.1f,%.1f\n",
//         ts, tp->lat / ADM_LAT_SCALE, tp->lon / ADM_LON_SCALE, depth / 100, tempr);
//}

static long decode_int(void *ptr, int size)
{
   long val = 0;
   for (int i = size - 1; i >= 0; i--)
   {
      val <<= 8;
      val |= *((unsigned char*) ptr + i);
   }
   return val;
}

void parse_wpt(wpt_header_t* th)
{
   //printf("<!--\nheader descriptor table: %d, at offset 0x%0x\n"
   //      "data descriptor table: %d, at offset 0x%0x\n"
   //      "header: 0x%0x\n",
   //      th->hdr_tbl_entries, th->start_hdr_tbl,
   //      th->data_desc_tbl_entries, th->start_data_desc_tbl,
   //      th->start_hdr);

    th->count = swap_uint16(th->count);

    for (auto ii = 0 ; ii < th->count; ii++)
    {
        auto wpt = BYTE_OFFSET(wpt_waypoint_t, th, (ii+1) * sizeof(wpt_header_t));
        printf("%2d. sym=%d, name=[%s], desc=[%s]\n", ii, wpt->symbol, wpt->name, wpt->description);
    }
}

}

void ParseWpt(const std::vector<uint8_t>& buffer)
{
    auto header = const_cast<wpt_header_t*>(reinterpret_cast<const wpt_header_t*>(&buffer[0]));
    parse_wpt(header);
}