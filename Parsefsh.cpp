// Parsefsh.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "src/splitimg.h"
#include "src/parsetrk.h"
#include "src/helpers.h"
#include "src/helix/cmdline.h"

#include "src/helix/util_filename.h"

int main(int argc, char **argv)
{

    cmdline::parser a;

    a.add<std::string>("input", 'i', "input", true, "");
    a.add<std::string>("output", 'o', "output", false, "./");

    a.parse_check(argc, argv);

    // read the input file
    auto path = a.get<std::string>("input");
    std::vector<uint8_t> buffer;
    readBinaryFileToBuffer(path, buffer);

    auto ext = util::string::tolower(util::Filename::GetExtension(path, false));
    
    if (ext == "adm")
    {
    }

    if (ext == "rte")
    {
    }

    if (ext == "trk")
    {
    }

    if (ext == "wpt")
    {
    }

    ParseTrack(&buffer[0], FMT_OSM);

    //splitImage(argc, argv);
}