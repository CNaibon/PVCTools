//
// Created by liujiajun on 2017/4/21.
//

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//The source ratio of the sample bam file is divided into small portions according to the included chromosome.
//		S001.merge.bam -------- S001_Chr01.bam S001_Chr02.bam S001_Chr03.bam ...... S001_ChrN.bam
//
//Command:    ./SplitBAM <-w WorkPath> <-bam BAMPath> [-I]
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SplitBAM.h"
#include "Environment.h"

using namespace std;

int SplitBAM(int argc,char *argv[])
{
    long StartTime = time((time_t*)NULL);
    printf("start time = %ld\n", StartTime);

    vector<string> SampleName;

    char PATH_BAMUTIL[CMD_NUM];
    GetToolsPath(PATH_BAMUTIL, "-bamUtil");

    char ShellCommand[CMD_NUM];
    char PathWork[CMD_NUM];
    char PathBAM[CMD_NUM];
    bool ImportBAM = false;

    for (int i = 0; i < argc; i++)
    {
        string cmd = argv[i];
        if (cmd == "-w")
        {
            snprintf(PathWork, sizeof(PathWork), "%s", argv[i + 1]);
            if (PathWork[strlen(PathWork) - 1] == '/')
                PathWork[strlen(PathWork) - 1] = '\0';
        }
        if (cmd == "-bam")
        {
            snprintf(PathBAM, sizeof(PathBAM), "%s", argv[i + 1]);
            if (PathBAM[strlen(PathBAM) - 1] == '/')
                PathBAM[strlen(PathBAM) - 1] = '\0';
        }
        if (cmd == "-I")
        {
            ImportBAM = true;
        }
    }

    snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/sample", PathWork);
    system(ShellCommand);
    snprintf(ShellCommand, sizeof(ShellCommand), "touch %s/bamlist", PathWork);
    system(ShellCommand);

    FILE *fp_bam;
    char *Buffer = NULL;
    size_t Len = FILE_LINE;

    //Import BAM list. If you need to customize the list, you should modify the [bamlist], fill in the need to split the BAM file
    if (ImportBAM == false)
    {
        char BAMFile[CMD_NUM];
        snprintf(ShellCommand, sizeof(ShellCommand), "%s/bamlist", PathWork);
        if ((fp_bam = fopen(ShellCommand, "w")) == NULL)
            exit(-1);
        DIR *dir;
        struct dirent *ptr;
        dir = opendir(PathBAM);
        while ((ptr = readdir(dir)) != NULL)
        {
            if (ptr->d_type == 4) continue;
            strncpy(BAMFile, ptr->d_name, sizeof(BAMFile)-1);
            string Suffix;
            for (int i = (int) strlen(BAMFile) - 1; i > 0; i--)
            {
                if (BAMFile[i] == '.')
                {
                    Suffix = BAMFile + i + 1;
                    break;
                }
            }
            if (Suffix != "bam") continue;
            fputs(BAMFile, fp_bam);
            fputs("\n", fp_bam);
        }
        closedir(dir);
        fclose(fp_bam);
    }
    snprintf(ShellCommand, sizeof(ShellCommand), "%s/bamlist", PathWork);
    if ((fp_bam = fopen(ShellCommand, "r")) == NULL)
        exit(-1);
    getline(&Buffer, &Len, fp_bam);
    while (!feof(fp_bam))
    {
        if (strlen(Buffer) != 0)
        {
            for (int i = (int)strlen(Buffer) - 1; i > 0; i--)
            {
                if (Buffer[i] == '.')
                {
                    Buffer[i] = '\0';
                    break;
                }
            }
            SampleName.push_back(Buffer);
            snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/sample/%s", PathWork, Buffer);
            system(ShellCommand);
        }
        getline(&Buffer, &Len, fp_bam);
    }
    fclose(fp_bam);

    printf("The sample was split according to the chromosome...\n");

//Parallel Computing
#pragma omp parallel for
    for (int k = 0; k < (int)SampleName.size(); ++k)//The sample is split by chromosome.
    {
        char Command[CMD_NUM];
        snprintf(Command, sizeof(Command), "%s splitChromosome --in %s/%s.bam --out %s/sample/%s/%s_ --bamout", PATH_BAMUTIL, PathBAM, SampleName[k].c_str(),PathWork,SampleName[k].c_str(),SampleName[k].c_str());
        system(Command);
    }
    printf("The sample was split by chromosome resolution.\n");


    long FinishTime = time((time_t*)NULL);
    printf("finish time = %ld\n", FinishTime);
    long RunningTime = FinishTime - StartTime;
    printf("running time = %ld\n", RunningTime);
    return 0;
}