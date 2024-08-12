// Parsefsh.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "src/splitimg.h"
#include "helpers.h"

int main(int argc, char **argv)
{
   char ts[64];
   struct tm tm;
   int fd = 0;
   int blocksize;
   void *fbase;
   int fat_cnt;

   auto path = std::string(argv[1]);
   std::vector<uint8_t> buffer;

   readBinaryFileToBuffer(path, buffer);

   fbase = &buffer[0];

   auto ah = static_cast<adm_header_t*>(fbase);
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

        auto folder = getFolderNameFromPath(path);

         if ((fat_cnt = write_subfile(fbase, af, folder.c_str(), blocksize)) == -1)
            perror("write_subfile()"), exit(1);

         af = BYTE_OFFSET(adm_fat_t, af, fat_cnt * FAT_SIZE);
      }
      else
      {
         vlog("BUG!\n");
       }
   }

   return 0;
}

