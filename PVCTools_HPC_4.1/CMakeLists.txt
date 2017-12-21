cmake_minimum_required(VERSION 3.8)
project(PVCTools_alpha_v3_7__HPC)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        include/Environment.h
        include/JudgeVCF.h
        include/main.h
        include/SegmentBAM.h
        include/SegmentFA.h
        include/SmallFA.h
        include/SplitBAM.h
        include/SplitFA.h
        include/Submit.h
        include/WriteScript.h
        src/childnode/Environment.cpp
        src/childnode/JudgeVCF.cpp
        src/childnode/main.cpp
        src/childnode/SegmentBAM.cpp
        src/childnode/SegmentFA.cpp
        src/childnode/SmallFA.cpp
        src/childnode/SplitBAM.cpp
        src/childnode/SplitFA.cpp
        src/childnode/Submit.cpp
        src/headnode/Environment.cpp
        src/headnode/WriteScript.cpp)

add_executable(PVCTools_alpha_v3_7__HPC ${SOURCE_FILES})