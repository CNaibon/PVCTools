//
// Created by liujiajun on 2017/5/22.
//

#ifndef PVCTOOLS_ALPHA_V2_5_JUDGEVCF_H
#define PVCTOOLS_ALPHA_V2_5_JUDGEVCF_H

#include "main.h"
#include "Environment.h"
#include <string>

using std::string;

void VCF_Modify(string &buffer, long addresses_number);
int VCF_Link(char *tarfile, char *formfile, long add_count);
int TotalVCF(const char *CurrentPath, const char *workdir, int judge);
int StitchVCF(int argc,char *argv[]);
int JudgeVCF(int argc, char *argv[]);

#endif //PVCTOOLS_ALPHA_V2_5_JUDGEVCF_H
