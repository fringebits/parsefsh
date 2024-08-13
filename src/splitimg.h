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
//#include <stdio.h>
//#include <stdlib.h>
////#include <unistd.h>
//#include <errno.h>
//#include <time.h>
//#include <string.h>
//#include <sys/types.h>
////#include <sys/mman.h>
//#include <sys/stat.h>
//#include <fcntl.h>

//#include "admfunc.h"

#include <vector>
#include <string>

int SplitImage(const std::vector<uint8_t>& buffer, const std::string outputPath);
//int write_subfile(const void* fbase, const adm_fat_t* af, const char* dir, unsigned blocksize);