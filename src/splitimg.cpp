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
 * along with Parsefsh. If not, see <http://www.gnu.org/licenses/>.
 */
#include "splitimg.h"
#include <cstdio>
#include "helpers.h"
#include "admfunc.h"

int write_subfile(const void* fbase, const adm_fat_t* af, const char* dir, unsigned blocksize)
{
    char name[2048]; //strlen(dir) + 14];  #bugbug

    //int i,
    //    fat_cntl;      // fat block counter
    //size_t sub_size,  // number of bytes not yet written
    //    wsize;     // number of bytes which should be written by write()

    snprintf(name, sizeof(name), "%s\\%.*s.%.*s",
        dir, (int)sizeof(af->sub_name), af->sub_name, (int)sizeof(af->sub_type), af->sub_type);

    //if ((outfd = open(name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR  | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)) == -1)
    //   return -1;

    auto outfd = std::fopen(name, "w+");
    if (outfd == nullptr)
    {
        return -1;
    }

    size_t sub_size = af->sub_size;
    int fat_cnt = 0;
    for (fat_cnt = 0;;)
    {
        int i;

        for (i = 0; i < MAX_FAT_BLOCKLIST && af->blocks[i] != 0xffff; i++)
        {
#ifdef DEBUG
            vlog("block[%d] = 0x%04x\n", i, af->blocks[i]);
#endif

            auto wsize = sub_size > blocksize ? blocksize : sub_size;

            auto len = fwrite(
                BYTE_OFFSET(void, fbase, blocksize * af->blocks[i]),
                1,
                wsize,
                outfd);
            //if ((len = write(outfd, fbase + blocksize * af->blocks[i], wsize)) < (int)wsize)

            if (len < (int)wsize)
            {
                if (len == -1)
                    perror("write()"), exit(1);
                vlog("write() truncated, %d < %ld\n", (int)len, (int)wsize);
                break;
            }

            sub_size -= len;
        }

        fat_cnt++;

        // if block list of FAT was full increment to next FAT
        if (i >= MAX_FAT_BLOCKLIST)
        {
            //af = (void*)af + FAT_SIZE;
            af = (adm_fat_t*)((uint8_t*)af + FAT_SIZE);

            // check if next FAT belongs to same file
            if (af->next_fat)
                continue;
        }

        // break loop in all other cases
        break;
    }

    if (fclose(outfd) == -1)
        perror("fclose()");

    return fat_cnt;
}

int SplitImage(const std::vector<uint8_t>& buffer, const std::string outputPath)
{
   char ts[64];
   struct tm tm;
   int fd = 0;
   int blocksize;
   int fat_cnt;

   const void* fbase = &buffer[0];

   auto ah = static_cast<const adm_header_t*>(fbase);
   memset(&tm, 0, sizeof(tm));
   tm.tm_year = ah->creat_year - 1900;
   tm.tm_mon = ah->creat_month;
   tm.tm_mday = ah->creat_day;
   tm.tm_hour = ah->creat_hour;
   tm.tm_min = ah->creat_min;
   tm.tm_sec = ah->creat_sec;
   strftime(ts, sizeof(ts), "%Y-%m-%dT%H:%M:%SZ", &tm);

   blocksize = 1 << (ah->blocksize_e1 + ah->blocksize_e2);

   printf("signature = %s\nidentifier = %s\ncreation date = %s\n"
         "updated = %d/%d\nblock size = %d\nmap desc = %.*s\n"
         "version = %d.%d\nfat physical block = %d\n",
         ah->sig, ah->ident, ts, ah->upd_month + 1,
         ah->upd_year + (ah->upd_year >= 0x63 ? 1900 : 2000),
         blocksize, (int) sizeof(ah->map_desc), ah->map_desc,
         ah->ver_major, ah->ver_minor, ah->fat_phys_block);

   //af = fbase + ah->fat_phys_block * 0x200 + 0x200;
   auto af = BYTE_OFFSET(adm_fat_t, fbase, ah->fat_phys_block * 0x200 + 0x200);

   for (; af->subfile; )
   {
      if (!af->next_fat)
      {
         printf("subfile = %d, subname = %.*s, subtype = %.*s, size = %d, nextfat = %d\n",
               af->subfile, (int) sizeof(af->sub_name), af->sub_name,
               (int) sizeof(af->sub_type), af->sub_type, af->sub_size, af->next_fat);

         if ((fat_cnt = write_subfile(fbase, af, outputPath.c_str(), blocksize)) == -1)
         {
            perror("write_subfile()"), exit(1);
         }

         af = BYTE_OFFSET(adm_fat_t, af, fat_cnt * FAT_SIZE);
      }
      else
      {
         vlog("BUG!\n");
       }
   }

   return 0;
}

