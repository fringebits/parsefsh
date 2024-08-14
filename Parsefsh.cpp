// Parsefsh.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "src/splitimg.h"
#include "src/parsetrk.h"
#include "src/parsewpt.h"
#include "src/helpers.h"
#include "src/helix/cmdline.h"

#include "src/helix/util_filename.h"

int main(int argc, char **argv)
{
    cmdline::parser a;

    a.add<std::string>("input", 'i', "input", true, "");
    a.add<std::string>("output", 'o', "output", false, "."); // default is the current path

    a.parse_check(argc, argv);

    // read the input file
    auto path = a.get<std::string>("input");
    std::vector<uint8_t> buffer;
    readBinaryFileToBuffer(path, buffer);

    auto ext = util::string::tolower(util::Filename::GetExtension(path, false));
    
    if (ext == "adm")
    {
        SplitImage(buffer, a.get<std::string>("output"));
        return 0;
    }

    if (ext == "agp")
    {
        // to be implemented
    }

    if (ext == "bdy")
    {
        // to be implemented
    }

    if (ext == "rte")
    {
        // to be implemented
    }

    if (ext == "trk")
    {
        ParseTrack(buffer);
        return 0;
    }

    if (ext == "wpt")
    {
        ParseWpt(buffer);
        return 0;
    }

    return -1;

}