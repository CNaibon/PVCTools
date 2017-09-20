//
// Created by liujiajun on 2017/4/21.
//

#ifndef PVCTOOLS_ALPHA_V2_0_SPLITBAM_H
#define PVCTOOLS_ALPHA_V2_0_SPLITBAM_H

#include "main.h"
#include "Environment.h"

int getheader(char *work_path, char *file_name, const char *samtools_path);
int SplitBAM(int argc,char *argv[]);

#endif //PVCTOOLS_ALPHA_V2_0_SPLITBAM_H
