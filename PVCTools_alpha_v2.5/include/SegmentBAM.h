//
// Created by liujiajun on 2017/4/21.
//

#ifndef PVCTOOLS_ALPHA_V2_0_SEGMENTBAM_H
#define PVCTOOLS_ALPHA_V2_0_SEGMENTBAM_H

#include "main.h"
#include "Environment.h"

using std::string;
int Modify(char *buffer, long addresses_number, char *chr_name, string tool, long LN);
int Sam_Address_Modify(char *file_name, long address_count, char *chr_name, string tool, long LN);
int SegmentBAM(int argc, char *argv[]);

#endif //PVCTOOLS_ALPHA_V2_0_SEGMENTBAM_H
