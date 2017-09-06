//
// Created by liujiajun on 2017/4/21.
//

#ifndef PVCTOOLS_ALPHA_V2_0_SEGMENTBAM_H
#define PVCTOOLS_ALPHA_V2_0_SEGMENTBAM_H

#include "main.h"
#include "Environment.h"

using std::string;

int Modify(string &buffer, long addresses_number, const char *chr_name, long LN);
int Sam_Address_Modify(char *file_name, long address_count, const char *chr_name, long LN);
int ParallelBAM(int argc, char *argv[]);
int SegmentBAM(int argc, char *argv[]);

#endif //PVCTOOLS_ALPHA_V2_0_SEGMENTBAM_H
