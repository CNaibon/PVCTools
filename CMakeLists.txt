cmake_minimum_required(VERSION 3.8)
project(Gene_tools)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        PVCTools_alpha_v3.0/include/Environment.h
        PVCTools_alpha_v3.0/include/JudgeVCF.h
        PVCTools_alpha_v3.0/include/main.h
        PVCTools_alpha_v3.0/include/SegmentBAM.h
        PVCTools_alpha_v3.0/include/SegmentFA.h
        PVCTools_alpha_v3.0/include/SmallFA.h
        PVCTools_alpha_v3.0/include/SplitBAM.h
        PVCTools_alpha_v3.0/include/SplitFA.h
        PVCTools_alpha_v3.0/include/Submit.h
        PVCTools_alpha_v3.0/include/WriteScript.h
        PVCTools_alpha_v3.0/src/childnode/Environment.cpp
        PVCTools_alpha_v3.0/src/childnode/JudgeVCF.cpp
        PVCTools_alpha_v3.0/src/childnode/main.cpp
        PVCTools_alpha_v3.0/src/childnode/SegmentBAM.cpp
        PVCTools_alpha_v3.0/src/childnode/SegmentFA.cpp
        PVCTools_alpha_v3.0/src/childnode/SmallFA.cpp
        PVCTools_alpha_v3.0/src/childnode/SplitBAM.cpp
        PVCTools_alpha_v3.0/src/childnode/SplitFA.cpp
        PVCTools_alpha_v3.0/src/childnode/Submit.cpp
        PVCTools_alpha_v3.0/src/headnode/Environment.cpp
        PVCTools_alpha_v3.0/src/headnode/WriteScript.cpp)

add_executable(Gene_tools ${SOURCE_FILES})