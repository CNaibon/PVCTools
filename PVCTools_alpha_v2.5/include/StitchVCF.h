//
// Created by liujiajun on 2017/4/21.
//

#ifndef PVCTOOLS_ALPHA_V2_0_STITCHVCF_H
#define PVCTOOLS_ALPHA_V2_0_STITCHVCF_H

#include "main.h"
#include "Environment.h"
#include <string>

using std::string;

void VCF_Modify(string &buffer, long addresses_number);
int VCF_Link(char *tarfile, char *formfile, long add_count);
int StitchVCF(int argc,char *argv[]);

#endif //PVCTOOLS_ALPHA_V2_0_STITCHVCF_H
