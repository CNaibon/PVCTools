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
        SplitFA(argc, argv);
    }
    else if (cmd == "SegmentFA")
    {
        SegmentFA(argc, argv);
    }
    else if (cmd == "SplitBAM")
    {
        SplitBAM(argc, argv);
    }
    else if (cmd == "SegmentBAM")
    {
        SegmentBAM(argc, argv);
    }
    else if (cmd == "Submit")
    {
        Submit(argc, argv);
    }
    else if (cmd == "StitchVCF")
    {
        StitchVCF(argc, argv);
    }
    else if (cmd == "SmallFA")
    {
        SmallFA(argc, argv);
    }
    else if (cmd == "GetVCF")
    {
        SplitFA(argc, argv);
        SegmentFA(argc, argv);
        SplitBAM(argc, argv);
        SegmentBAM(argc, argv);
        Submit(argc, argv);
        SmallFA(argc, argv);
    }
    else
    {
        std::cout << "Error: Error Parameters." << std::endl;
        std::cout << "Parameters should be 'SplitFA' 'SegmentFA' 'SplitBAM' 'SegmentBAM' 'Submit' 'StitchVCF' or 'SmallFA'." << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
