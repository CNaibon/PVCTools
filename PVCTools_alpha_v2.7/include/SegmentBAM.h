//
// Created by liujiajun on 2017/4/21.
//

#ifndef PVCTOOLS_ALPHA_V2_0_SEGMENTBAM_H
#define PVCTOOLS_ALPHA_V2_0_SEGMENTBAM_H

#include "main.h"
#include "Environment.h"

using std::string;
void Modify(char *buffer, long addresses_number);
int Sam_Address_Modify(char *file_name, long address_count);
int SegmentBAM(int argc, char *argv[]);

#endif //PVCTOOLS_ALPHA_V2_0_SEGMENTBAM_H
