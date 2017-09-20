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

int getheader(char *work_path, char *file_name, const char *samtools_path)
{
    char ShellCommand[CMD_NUM];
    char TmpName[CMD_NUM];
    snprintf(TmpName, sizeof(TmpName), "%s/header.bam", work_path);
    snprintf(ShellCommand, sizeof(ShellCommand), "%s view -H %s > %s", samtools_path, file_name, TmpName);
    system(ShellCommand);
    return 0;
}

int SplitBAM(int argc,char *argv[])
{
    long StartTime = time((time_t*)NULL);
    printf("SplitBAM start time = %ld\n", StartTime);

    vector<string> SampleName;
    string PATH_SAMTOOLS;
    GetToolsPath(argv[0], PATH_SAMTOOLS, "-samtools");
    string PATH_BAMUTIL;
    GetToolsPath(argv[0], PATH_BAMUTIL, "-bamUtil");
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
            if (PathWork[strlen(PathWork) - 1] == '/') PathWork[strlen(PathWork) - 1] = '\0';
        }
        if (cmd == "-bam")
        {
            snprintf(PathBAM, sizeof(PathBAM), "%s", argv[i + 1]);
            if (PathBAM[strlen(PathBAM) - 1] == '/') PathBAM[strlen(PathBAM) - 1] = '\0';
        }
        if (cmd == "-I") ImportBAM = true;
    }

    snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/sample", PathWork);
    system(ShellCommand);
    snprintf(ShellCommand, sizeof(ShellCommand), "touch %s/bamlist", PathWork);
    system(ShellCommand);

    //Import BAM list. If you need to customize the list, you should modify the [bamlist], fill in the need to split the BAM file
    if (ImportBAM == false)
    {
        FILE *fp_bamw;
        char BAMFile[CMD_NUM];
        snprintf(ShellCommand, sizeof(ShellCommand), "%s/bamlist", PathWork);
        if ((fp_bamw = fopen(ShellCommand, "w")) == NULL)
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
            fputs(BAMFile, fp_bamw);
            fputs("\n", fp_bamw);
        }
        closedir(dir);
        fclose(fp_bamw);
    }

    ifstream fp_bam;
    string Buffer;
    string strbuff;
    snprintf(ShellCommand, sizeof(ShellCommand), "%s/bamlist", PathWork);
    fp_bam.open(ShellCommand,ios::in);
    getline(fp_bam, Buffer);
    while (!fp_bam.eof())
    {
        if (Buffer.size() != 0)
        {
            int i = Buffer.rfind('.');
            strbuff = Buffer.substr(0,i);
            SampleName.push_back(strbuff.c_str());
            snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/sample/%s", PathWork, strbuff.c_str());
            system(ShellCommand);
        }
        getline(fp_bam, Buffer);
    }
    fp_bam.close();

    snprintf(ShellCommand, sizeof(ShellCommand), "sort %s/bamlist -o %s/bamlist", PathWork, PathWork);
    system(ShellCommand);

    printf("The sample was split according to the chromosome...\n");

//Parallel Computing
#pragma omp parallel for
    for (int k = 0; k < (int)SampleName.size(); ++k)//The sample is split by chromosome.
    {
        char Command[CMD_NUM];
        snprintf(Command, sizeof(Command), "%s splitChromosome --in %s/%s.bam --out %s/sample/%s/%s_ --bamout", PATH_BAMUTIL.c_str(), PathBAM, SampleName[k].c_str(), PathWork, SampleName[k].c_str(),SampleName[k].c_str());
        system(Command);
    }
    printf("The sample was split by chromosome resolution.\n");

    snprintf(ShellCommand, sizeof(ShellCommand), "%s/%s.bam", PathBAM, SampleName[0].c_str());
    getheader(PathWork, ShellCommand, PATH_SAMTOOLS.c_str());

    long FinishTime = time((time_t*)NULL);
    printf("SplitBAM finish time = %ld\n", FinishTime);
    long RunningTime = FinishTime - StartTime;
    printf("SplitBAM running time = %ld\n", RunningTime);
    return 0;
}