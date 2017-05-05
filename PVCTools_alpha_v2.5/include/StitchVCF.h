//
// Created by liujiajun on 2017/4/21.
//

#ifndef PVCTOOLS_ALPHA_V2_0_STITCHVCF_H
#define PVCTOOLS_ALPHA_V2_0_STITCHVCF_H

#include "main.h"
#include "Environment.h"

int VCF_Link(char *tarfile, char *formfile, const char *ChrName, long add_count);

void VCF_Modify(char *buffer, const char *chrname, int chrname_len, long addresses_number);

int StitchVCF(int argc,char *argv[]);

#endif //PVCTOOLS_ALPHA_V2_0_STITCHVCF_H
