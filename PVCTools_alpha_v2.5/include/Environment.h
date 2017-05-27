//
// Created by liujiajun on 2017/4/23.
//

#ifndef PVCTOOLS_ALPHA_V2_0_ENVIRONMENT_H
#define PVCTOOLS_ALPHA_V2_0_ENVIRONMENT_H

#include "main.h"

#define FILE_LINE 16383
#define CMD_NUM 2048
using std::string;

int PrintEvmt(const char *dir);
int SetToolsPath(const char *dir, const char *order, const char *path);
int GetToolsPath(const char *dir, string &path, const char *order);
int SetEvmt(int argc, char *argv[]);

#endif //PVCTOOLS_ALPHA_V2_0_ENVIRONMENT_H
