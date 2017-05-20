cmake_minimum_required(VERSION 3.7)
project(PVCTools_alpha_v2_5)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        include/Environment.h
        include/main.h
        include/SegmentBAM.h
        include/SegmentFA.h
        include/SmallFA.h
        include/SplitBAM.h
        include/SplitFA.h
        include/StitchVCF.h
        include/Submit.h
        include/WriteScript.h
        src/childnode/Environment.cpp
        src/childnode/main.cpp
        src/childnode/SegmentBAM.cpp
        src/childnode/SegmentFA.cpp
        src/childnode/SmallFA.cpp
        src/childnode/SplitBAM.cpp
        src/childnode/SplitFA.cpp
        src/childnode/StitchVCF.cpp
        src/childnode/Submit.cpp
        src/headnode/Environment.cpp
        src/headnode/WriteScript.cpp
        config
        Makefile)

add_executable(PVCTools_alpha_v2_5 ${SOURCE_FILES})