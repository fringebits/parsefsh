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