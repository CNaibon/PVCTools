//
// Created by liujiajun on 2017/4/21.
//
#include <iostream>
#include "SegmentBAM.h"
#include "SegmentFA.h"
#include "SplitBAM.h"
#include "SplitFA.h"
#include "StitchVCF.h"
#include "Submit.h"
#include "SmallFA.h"

int main(int argc, char *argv[])
{
    std::string cmd = argv[1];
    if (cmd == "SplitFA" )
    {
        SplitFA(argc - 1, argv + 1);
    }
    else if (cmd == "SegmentFA")
    {
        SegmentFA(argc - 1, argv + 1);
    }
    else if (cmd == "SplitBAM")
    {
        SplitBAM(argc - 1, argv + 1);
    }
    else if (cmd == "SegmentBAM")
    {
        SegmentBAM(argc - 1, argv + 1);
    }
    else if (cmd == "Submit")
    {
        Submit(argc - 1, argv + 1);
    }
    else if (cmd == "StitchVCF")
    {
        StitchVCF(argc - 1, argv + 1);
    }
    else if (cmd == "SmallFA")
    {
        SmallFA(argc - 1, argv + 1);
    }
    else if (cmd == "GetVCF")
    {
        SplitFA(argc - 1, argv + 1);
        SegmentFA(argc - 1, argv + 1);
        SplitBAM(argc - 1, argv + 1);
        SegmentBAM(argc - 1, argv + 1);
        Submit(argc - 1, argv + 1);
    }
    else
    {
        std::cout << "Error: Error Parameters." << std::endl;
        std::cout << "Parameters should be 'SplitFA' 'SegmentFA' 'SplitBAM' 'SegmentBAM' 'Submit' 'StitchVCF' or 'SmallFA'." << std::endl;
        std::cout << std::endl;
    }

    return true;
}
