//
// Created by liujiajun on 2017/4/21.
//

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Calculate the small chromosome FA files.
//Command: ./SmallFA <-w WorkPath>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SmallFA.h"
#include "Environment.h"

using namespace std;

int SmallFA(int argc, char *argv[])
{
    long StartTime = time((time_t*)NULL);
    printf("start time = %ld\n", StartTime);

    string  PATH_SAMTOOLS;
    GetToolsPath(argv[0], PATH_SAMTOOLS, "-samtools");
    string PATH_BCFTOOLS;
    GetToolsPath(argv[0], PATH_BCFTOOLS, "-bcftools");
    string PATH_GATK;
    GetToolsPath(argv[0], PATH_GATK, "-gatk");
    string PATH_FREEBAYES;
    GetToolsPath(argv[0], PATH_FREEBAYES, "-freebayes");

    vector<string> ChrName;
    vector<string> SampleName;
    //Command string.
    char ShellCommand[CMD_NUM];
    string Buffer;
    char PathWork[CMD_NUM];
    char Queue[CMD_NUM] = "normal";
    char Span[CMD_NUM] = "20";
    string Tool = "samtools";
    string Parameters = " ";

    for (int i = 0; i < argc; i++)
    {
        string cmd = argv[i];
        if (cmd == "-w")
        {
            snprintf(PathWork, sizeof(PathWork), "%s", argv[i + 1]);
            if (PathWork[strlen(PathWork) - 1] == '/') PathWork[strlen(PathWork) - 1] = '\0';
        }
        if (cmd == "-q") snprintf(Queue, sizeof(Queue), "%s", argv[i + 1]);
        if (cmd == "-Span") snprintf(Span, sizeof(Span), "%s", argv[i + 1]);
        if (cmd == "-T") Tool = argv[i + 1];
        if (cmd == "-P") Parameters = argv[i + 1];
    }
    snprintf(ShellCommand, sizeof(ShellCommand), "ls -al %s/fa | grep '^-' | grep '.fa$' | awk '{print $9}' > %s/smalllist_tmp", PathWork, PathWork);
    system(ShellCommand);
    snprintf(ShellCommand, sizeof(ShellCommand), "sort %s/smalllist_tmp %s/falist %s/falist | uniq -u > %s/smalllist" , PathWork, PathWork, PathWork, PathWork);
    system(ShellCommand);
    snprintf(ShellCommand, sizeof(ShellCommand), "rm %s/smalllist_tmp" , PathWork);
    system(ShellCommand);
    snprintf(ShellCommand, sizeof(ShellCommand), "sort %s/smalllist -o %s/smalllist", PathWork, PathWork);
    system(ShellCommand);

    string strbuff;
    //Import BAM list, if you need to customize the list, you should modify the [bamlist], fill in the need to split the BAM file
    ifstream fp_bam;
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
        }
        getline(fp_bam, Buffer);
    }
    fp_bam.close();

    //Import FA list, if you need to customize the list, you should modify the [falist], fill in the need to split the FA file
    snprintf(ShellCommand, sizeof(ShellCommand), "%s/smalllist", PathWork);
    ifstream fp_small;
    fp_small.open(ShellCommand,ios::in);
    getline(fp_small, Buffer);
    while (!fp_small.eof())
    {
        if (Buffer.size() != 0)
        {
            int i = Buffer.rfind('.');
            strbuff = Buffer.substr(0,i);
            ChrName.push_back(strbuff.c_str());
        }
        getline(fp_small, Buffer);
    }
    fp_small.close();

    //Create the relevant directory.
    snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/out", PathWork);
    system(ShellCommand);
    snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/out/smallFA", PathWork);
    system(ShellCommand);
    snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/err", PathWork);
    system(ShellCommand);
    snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/err/smallFA", PathWork);
    system(ShellCommand);
    snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/sub_script", PathWork);
    system(ShellCommand);
    snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/sub_script/smallFA", PathWork);
    system(ShellCommand);
    snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/vcf", PathWork);
    system(ShellCommand);
    snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/vcf/Final_Result", PathWork);
    system(ShellCommand);

#pragma omp parallel for
    for (int i = 0; i < (int)ChrName.size(); i++)
    {
        char Command[CMD_NUM];
        //Write the commit script.
        FILE *fp_sh;
        snprintf(Command, sizeof(Command), "%s/sub_script/smallFA/%s_%s.sh", PathWork, ChrName[i].c_str(), Tool.c_str());
        if ((fp_sh = fopen(Command, "w")) == NULL)
            exit(-1);
        snprintf(Command, sizeof(Command), "#BSUB -q %s\n", Queue);
        fputs(Command, fp_sh);
        snprintf(Command, sizeof(Command), "#BSUB -J %s_%s\n", ChrName[i].c_str(), Tool.c_str());
        fputs(Command, fp_sh);
        snprintf(Command, sizeof(Command), "#BSUB -o %s/out/smallFA/%s_%s.out\n", PathWork, ChrName[i].c_str(), Tool.c_str());
        fputs(Command, fp_sh);
        snprintf(Command, sizeof(Command), "#BSUB -e %s/err/smallFA/%s_%s.err\n", PathWork, ChrName[i].c_str(), Tool.c_str());
        fputs(Command, fp_sh);
        snprintf(Command, sizeof(Command), "#BSUB -n 1\n");
        fputs(Command, fp_sh);
        snprintf(Command, sizeof(Command), "#BSUB -R \"span[ptile=%s]\"\n", Span);
        fputs(Command, fp_sh);

        if (Tool == "samtools")
        {
            snprintf(Command, sizeof(Command), "%s mpileup -u -t DP,AD,ADF %s ", PATH_SAMTOOLS.c_str(), Parameters.c_str());
            fputs(Command, fp_sh);
            snprintf(Command, sizeof(Command), "-f %s/fa/%s.fa ", PathWork, ChrName[i].c_str());
            fputs(Command, fp_sh);
            //Write a number of sample small copies.
            for (int n = 0; n < (int)SampleName.size(); n++)
            {
                snprintf(Command, sizeof(Command), "touch %s/sample/%s/%s_%s.bam", PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str());
                system(Command);

                snprintf(Command, sizeof(Command), "%s/sample/%s/%s_%s.bam ", PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str());
                fputs(Command, fp_sh);
            }
            snprintf(Command, sizeof(Command), "| %s call -vmO v -o %s/vcf/Final_Result/%s.var.flt.vcf", PATH_BCFTOOLS.c_str(), PathWork, ChrName[i].c_str());
            fputs(Command, fp_sh);
        }
        else if (Tool == "gatk")
        {
            snprintf(Command, sizeof(Command), "%s faidx %s/fa/%s.fa\n", PATH_SAMTOOLS.c_str(), PathWork, ChrName[i].c_str());
            fputs(Command, fp_sh);

            snprintf(Command, sizeof(Command), "java -jar /gpfs01/home/jingjing/software/GATK/picard-tools-1.119/CreateSequenceDictionary.jar ");
            fputs(Command, fp_sh);

            snprintf(Command, sizeof(Command), "R= %s/fa/%s.fa O= %s/fa/%s.dict\n", PathWork, ChrName[i].c_str(), PathWork, ChrName[i].c_str());
            fputs(Command, fp_sh);

            snprintf(Command, sizeof(Command), "java -jar %s -T HaplotypeCaller %s ", PATH_GATK.c_str(), Parameters.c_str());
            fputs(Command, fp_sh);
            snprintf(Command, sizeof(Command), "-R %s/fa/%s.fa -nct 1 ", PathWork, ChrName[i].c_str());
            fputs(Command, fp_sh);
            for (int n = 0; n < (int)SampleName.size(); n++)
            {
                snprintf(Command, sizeof(Command), "touch %s/sample/%s/%s_%s.bam", PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str());
                system(Command);

                snprintf(Command, sizeof(Command), "-I %s/sample/%s/%s_%s.bam ", PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str());
                fputs(Command, fp_sh);
            }
            snprintf(Command, sizeof(Command), "-o %s/vcf/Final_Result/%s.vcf", PathWork, ChrName[i].c_str());
            fputs(Command, fp_sh);
        }
        else if (Tool == "freebayes")
        {
            snprintf(Command, sizeof(Command), "/usr/bin/time -f \"%%E\" ");
            fputs(Command, fp_sh);
            snprintf(Command, sizeof(Command), "%s --strict-vcf %s ", PATH_FREEBAYES.c_str(), Parameters.c_str());
            fputs(Command, fp_sh);
            snprintf(Command, sizeof(Command), "-f %s/fa/%s.fa ", PathWork, ChrName[i].c_str());
            fputs(Command, fp_sh);
            for (int n = 0; n < (int)SampleName.size(); n++)
            {
                snprintf(Command, sizeof(Command), "touch %s/sample/%s/%s_%s.bam", PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str());
                system(Command);

                snprintf(Command, sizeof(Command), "-b %s/sample/%s/%s_%s.bam ", PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str());
                fputs(Command, fp_sh);
            }
            snprintf(Command, sizeof(Command), "-v %s/vcf/Final_Result/%s.vcf", PathWork, ChrName[i].c_str());
            fputs(Command, fp_sh);
        }

        snprintf(Command, sizeof(Command), "\n%s JudgeVCF -w %s -C %ld -N %s -S small", argv[0], PathWork, long(ChrName.size()), ChrName[i].c_str());
        fputs(Command, fp_sh);
        fclose(fp_sh);
        // Submit.
        snprintf(Command, sizeof(Command), "bsub < %s/sub_script/smallFA/%s_%s.sh", PathWork, ChrName[i].c_str(), Tool.c_str());
        system(Command);
    }

    long FinishTime = time((time_t*)NULL);
    printf("finish time = %ld\n", FinishTime);
    long RunningTime = FinishTime - StartTime;
    printf("running time = %ld\n", RunningTime);

    return 0;
}