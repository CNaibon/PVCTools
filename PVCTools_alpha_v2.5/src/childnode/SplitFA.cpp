//
// Created by liujiajun on 2017/4/21.
//

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//The source gene sequence fa is split into small portions according to the included chromosomes
//		all.fa ------> all_Chr01.fa all_Chr02.fa ………….all_ChrN.fa
//
//Command:  ./SplitFA	<-w WorkPath> <-fa FAPath>
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SplitFA.h"
#include "Environment.h"

using namespace std;

//The FA file is divided into chromosomes and obtained after cutting the chromosome name.
void Fa_Chr(char *workpath,char *fa_path)
{
    FILE *fp;
    if((fp = fopen(fa_path, "r")) == NULL)
        exit(-1);
    char *Line = NULL;
    size_t Len = FILE_LINE;
    char FileName[CMD_NUM];
    char ShellCommand[CMD_NUM];
    //Create the FA directory.
    snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/fa", workpath);
    system(ShellCommand);
    getline(&Line, &Len, fp);
    //Extract the sequence of each chromosome
    while (!feof(fp))
    {
        strncpy(FileName,Line + 1 ,sizeof(FileName) - 4);
        for (int i = 0; i < (int)strlen(FileName); i++)
        {
            if(FileName[i] == ' ' || FileName[i] == '\t' || FileName[i] == '\n')
            {
                FileName[i] = '\0';
                break;
            }
        }
        strncat(FileName, ".fa", sizeof(FileName) - strlen(FileName));
        snprintf(ShellCommand, sizeof(ShellCommand), "touch %s", FileName);
        system(ShellCommand);

        FILE *fp_tmp;
        if((fp_tmp = fopen(FileName, "w")) == NULL)
            exit(-1);

        fputs(Line, fp_tmp);
        getline(&Line, &Len, fp);
        while (strstr(Line, ">") == NULL && feof(fp) == 0)
        {
            fputs(Line, fp_tmp);
            getline(&Line, &Len, fp);
        }
        fclose(fp_tmp);
        snprintf(ShellCommand, sizeof(ShellCommand), "mv %s %s/fa/", FileName, workpath);
        system(ShellCommand);
    }
    fclose(fp);
}

int SplitFA(int argc,char *argv[])
{
    long StartTime = time((time_t*)NULL);
    printf("start time = %ld\n", StartTime);

    char ShellCommand[CMD_NUM];

    char PathWork[CMD_NUM];
    char PathFA[CMD_NUM];

    for (int i = 0; i < argc; i++)
    {
        string cmd = argv[i];
        if (cmd == "-w")
        {
            snprintf(PathWork, sizeof(PathWork), "%s", argv[i + 1]);
        }
        if (cmd == "-fa")
        {
            snprintf(PathFA, sizeof(PathFA), "%s", argv[i + 1]);
        }
    }

    //Generates an empty [falist] for import.
    snprintf(ShellCommand, sizeof(ShellCommand), "touch %s/falist", PathWork);
    system(ShellCommand);

    //Cut the FA file.
    Fa_Chr(PathWork, PathFA);

    long FinishTime = time((time_t*)NULL);
    printf("finish time = %ld\n", FinishTime);
    long RunningTime = FinishTime - StartTime;
    printf("running time = %ld\n", RunningTime);

    return 0;
}

