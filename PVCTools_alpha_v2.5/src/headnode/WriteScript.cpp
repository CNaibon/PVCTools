//
// Created by liujiajun on 2017/4/21.
//

/////////////////////////////////////////////////////////////////////////////
//Write the commit script to the node.
//Command: ./Writescript $Command $WORKPATH $PARAMETERS .... ($QUEUE $CPU $SPAN)
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Environment.h"

using namespace std;

int main(int argc, char *argv[])
{

    // Verify at least one arg.
    if (argc == 1)
    {
        std::cout << " SplitFA              - The source gene sequence FA file is split into small portions according to the included chromosomes." << std::endl;
        std::cout << " SegmentFA            - Each FA file is segmented into small portions according to the split number." << std::endl;
        std::cout << " SplitBAM             - The source ratio of the sample BAM file is split into small portions according to the included chromosome." << std::endl;
        std::cout << " SegmentBAM           - Each BAM file is segmented into small portions according to the split number." << std::endl;
        std::cout << " Submit               - Submit and calculate VCF results." << std::endl;
        std::cout << " SmallFA              - Calculate the small chromosome FA files." << std::endl;
        std::cout << " GetVCF               - Integration 'SplitFA' 'SegmentFA' 'SplitBAM' 'SegmentBAM' and 'Submit' steps." << std::endl;
        std::cout << " Environment          - Setting environment variable." << std::endl;
        std::cout << std::endl;
    }
    else if (argc == 2)
    {
        std::string cmd = argv[1];
        if (cmd == "SplitFA" )
        {
            std::cout << "\t./PVCTools SplitFA <-w WorkPath> <-fa FAPath> [-q Queue] [-cpu CPU] [-span Span]" << std::endl;
            std::cout << "\tRequired Parameters:" << std::endl;
            std::cout << "\t\t-w               : Working directory path for using to store the generated files." << std::endl;
            std::cout << "\t\t-fa              : The path of the original FA file."<< std::endl;
            std::cout << "\tOptional Parameters:" << std::endl;
            std::cout << "\t\t-q               : The queue you want to run the task (Default queue: normal)." << std::endl;
            std::cout << "\t\t-cpu             : The number of CPUs you want to allocate for running the task (Default value: 1)." << std::endl;
            std::cout << "\t\t                   Ideal value: The number of samples." << std::endl;
            std::cout << "\t\t                   BUT considering the CPU resources,this value should be less then the queue CPU maximum." << std::endl;
            std::cout << "\t\t-span            : The maximum number of the CPU used on each node (Default value: 20)." << std::endl;
            std::cout << std::endl;
        }
        else if (cmd == "SegmentFA")
        {
            std::cout << "\t./PVCTools SegmentFA <-w WorkPath> <-n SplitNumber> [-lm Size] [-q Queue] [-cpu CPU] [-span Span]" << std::endl;
            std::cout << "\tRequired Parameters:" << std::endl;
            std::cout << "\t\t-w               : Working directory path for using to store the generated files." << std::endl;
            std::cout << "\t\t-n               : The number of divisions." << std::endl;
            std::cout << "\tOptional Parameters:" << std::endl;
            std::cout << "\t\t-lm              : Minimize the size(MB) of files to be cut(default: Cut all the FA files imported from [falist])." << std::endl;
            std::cout << "\t\t-q               : The queue you want to run the task (Default queue: normal)." << std::endl;
            std::cout << "\t\t-cpu             : The number of CPUs you want to allocate for running the task (Default value: 1)." << std::endl;
            std::cout << "\t\t                   Ideal value: The number of samples." << std::endl;
            std::cout << "\t\t                   BUT considering the CPU resources,this value should be less then the queue CPU maximum." << std::endl;
            std::cout << "\t\t-span            : The maximum number of the CPU used on each node (Default value: 20)." << std::endl;
            std::cout << "\tTips:" << std::endl;
            std::cout << "\tYou may need to customize the FA file list that you want to import in [falist]." << std::endl;
            std::cout << "\tRunning again will delete the previous data." << std::endl;
            std::cout << std::endl;
        }
        else if (cmd == "SplitBAM")
        {
            std::cout << "\t./PVCTools SplitBAM <-w WorkPath> <-bam BAMPath> [-I] [-q Queue] [-cpu CPU] [-span Span]" << std::endl;
            std::cout << "\tRequired Parameters:" << std::endl;
            std::cout << "\t\t-w               : Working directory path for using to store the generated files." << std::endl;
            std::cout << "\t\t-bam             : The path of the original comparison sample BAM files group." << std::endl;
            std::cout << "\tOptional Parameters:" << std::endl;
            std::cout << "\t\t-I               : Will import [bamlist](By default, do not import, all of the BAM files in the target path will be calculated)." << std::endl;
            std::cout << "\t\t-q               : The queue you want to run the task (Default queue: normal)." << std::endl;
            std::cout << "\t\t-cpu             : The number of CPUs you want to allocate for running the task (Default value: 1)." << std::endl;
            std::cout << "\t\t                   Ideal value: The number of samples." << std::endl;
            std::cout << "\t\t                   BUT considering the CPU resources,this value should be less then the queue CPU maximum." << std::endl;
            std::cout << "\t\t-span            : The maximum number of the CPU used on each node (Default value: 20)." << std::endl;
            std::cout << std::endl;
        }
        else if (cmd == "SegmentBAM")
        {
            std::cout << "\t./PVCTools SegmentBAM <-w WorkPath> <-n SplitNumber> [-q Queue] [-cpu CPU] [-span Span]" << std::endl;
            std::cout << "\tRequired Parameters:" << std::endl;
            std::cout << "\t\t-w               : Working directory path for using to store the generated files." << std::endl;
            std::cout << "\t\t-n               : The number of divisions." << std::endl;
            std::cout << "\tOptional Parameters:" << std::endl;
            std::cout << "\t\t-q               : The queue you want to run the task (Default queue: normal)." << std::endl;
            std::cout << "\t\t-cpu             : The number of CPUs you want to allocate for running the task (Default value: 1)." << std::endl;
            std::cout << "\t\t                   Ideal value: The number of samples." << std::endl;
            std::cout << "\t\t                   BUT considering the CPU resources,this value should be less then the queue CPU maximum." << std::endl;
            std::cout << "\t\t-span            : The maximum number of the CPU used on each node (Default value: 20)." << std::endl;
            std::cout << "\tTips:" << std::endl;
            std::cout << "\tYou may need to customize the FA/BAM file list that you want to import in [falist]." << std::endl;
            std::cout << "\tRunning again will delete the previous data." << std::endl;
            std::cout << std::endl;
        }
        else if (cmd == "Submit")
        {
            std::cout << "\t./PVCTools Submit <-w WorkPath> <-n SplitNumber> [-T Tools] [-q Queue] [-cpu CPU] [-span Span]" << std::endl;
            std::cout << "\tRequired Parameters:" << std::endl;
            std::cout << "\t\t-w               : Working directory path for using to store the generated files." << std::endl;
            std::cout << "\t\t-n               : The number of divisions." << std::endl;
            std::cout << "\tOptional Parameters:" << std::endl;
            std::cout << "\t\t-T               : The tool you want to use to run the task (Default tool: samtools)." << std::endl;
            std::cout << "\t\t                   Optional tools : [samtools] [gatk] [freebayes]." << std::endl;
            std::cout << "\t\t-q               : The queue you want to run the task (Default queue: normal)." << std::endl;
            std::cout << "\tTips:" << std::endl;
            std::cout << "\tYou may need to customize the FA file list that you want to import in [falist]." << std::endl;
            std::cout << std::endl;
        }
        else if (cmd == "SmallFA")
        {
            std::cout << "\t./PVCTools SmallFA <-w WorkPath> [-T Tools] [-q Queue] [-cpu CPU] [-span Span]" << std::endl;
            std::cout << "\tRequired Parameters:" << std::endl;
            std::cout << "\t\t-w               : Working directory path for using to store the generated files." << std::endl;
            std::cout << "\tOptional Parameters:" << std::endl;
            std::cout << "\t\t-T               : The tool you want to use to run the task (Default tool: samtools)." << std::endl;
            std::cout << "\t\t                   Optional tools : [samtools] [gatk] [freebayes]." << std::endl;
            std::cout << "\t\t-q               : The queue you want to run the task (Default queue: normal)." << std::endl;
            std::cout << "\t\t-cpu             : The number of CPUs you want to allocate for running the task (Default value: 1)." << std::endl;
            std::cout << "\t\t                   Ideal value: The number of samples." << std::endl;
            std::cout << "\t\t                   BUT considering the CPU resources,this value should be less then the queue CPU maximum." << std::endl;
            std::cout << "\t\t-span            : The maximum number of the CPU used on each node (Default value: 20)." << std::endl;
            std::cout << std::endl;
        }
        else if (cmd == "GetVCF")
        {
            std::cout << "\t./PVCTools GetVCF <-w WorkPath> <-fa FAPath> <-bam BAMPath> <-n SplitNumber> [-lm Size] [-I] [-T Tools] [-q Queue] [-cpu CPU] [-span Span]" << std::endl;
            std::cout << "\tRequired Parameters:" << std::endl;
            std::cout << "\t\t-w               : Working directory path for using to store the generated files." << std::endl;
            std::cout << "\t\t-fa              : The path of the original FA file."<< std::endl;
            std::cout << "\t\t-n               : The number of divisions." << std::endl;
            std::cout << "\t\t-bam             : The path of the original comparison sample BAM files group." << std::endl;
            std::cout << "\tOptional Parameters:" << std::endl;
            std::cout << "\t\t-lm              : Minimize the size(MB) of files to be cut(default: Cut all the FA files imported from [falist])." << std::endl;
            std::cout << "\t\t-I               : Will import [bamlist](By default, do not import, all of the BAM files in the target path will be calculated)." << std::endl;
            std::cout << "\t\t-T               : The tool you want to use to run the task (Default tool: samtools)." << std::endl;
            std::cout << "\t\t                   Optional tools : [samtools] [gatk] [freebayes]." << std::endl;
            std::cout << "\t\t-q               : The queue you want to run the task (Default queue: normal)." << std::endl;
            std::cout << "\t\t-cpu             : The number of CPUs you want to allocate for running the task (Default value: 1)." << std::endl;
            std::cout << "\t\t                   Ideal value: The number of samples." << std::endl;
            std::cout << "\t\t                   BUT considering the CPU resources,this value should be less then the queue CPU maximum." << std::endl;
            std::cout << "\t\t-span            : The maximum number of the CPU used on each node (Default value: 20)." << std::endl;
            std::cout << std::endl;
        }
        else if (cmd == "Environment")
        {
            std::cout << "\t./PVCTools Environment [-samtools PATH] [-bcftools PATH] [-bamUtil PATH]" << std::endl;
            std::cout << "\tOptional Parameters:" << std::endl;
            std::cout << "\t\t-samtools        : Setting samtools path." << std::endl;
            std::cout << "\t\t-bcftools        : Setting bcftools path." << std::endl;
            std::cout << "\t\t-bamUtil         : Setting bamUtil path." << std::endl;
            std::cout << std::endl;
            PrintEvmt(argv[0]);
        }
        else
        {
            std::cout << "Error: Error Parameters." << std::endl;
            std::cout << "Parameters should be 'SplitFA' 'SegmentFA' 'SplitBAM' 'SegmentBAM' 'Submit' or 'SmallFA'." << std::endl;
            std::cout << std::endl;
        }
    }
    else
    {
        std::string cmd = argv[1];
        if (cmd == "SplitFA" || cmd == "SegmentFA" || cmd == "SplitBAM" || cmd == "SegmentBAM" || cmd == "Submit" || cmd == "SmallFA" || cmd == "GetVCF")
        {
            char ShellCommand[CMD_NUM];
            char Queue[CMD_NUM] = "normal";
            char Cpu[CMD_NUM] = "1";
            char Span[CMD_NUM] = "20";
            char PathWork[CMD_NUM];

            for (int i = 0; i < argc; i++)
            {
                cmd = argv[i];
                if (cmd == "-w") sprintf(PathWork, "%s", argv[i + 1]);
                if (cmd == "-q") sprintf(Queue, "%s", argv[i + 1]);
                if (cmd == "-cpu") sprintf(Cpu, "%s", argv[i + 1]);
                if (cmd == "-span") sprintf(Span, "%s", argv[i + 1]);
            }
            char CurrentPath[CMD_NUM];
            //Get the current directory.
            snprintf(CurrentPath, sizeof(CurrentPath), "%s", argv[0]);
            int Count = 0;
            for (int j = int(strlen(CurrentPath)) - 1; j > 0; j--)
            {
                if (CurrentPath[j] == '/')
                {
                    Count ++;
                    if (Count == 2) CurrentPath[j] = '\0';
                }
            }
            if (Count < 2) snprintf(CurrentPath, sizeof(CurrentPath), ".");
            FILE *fp_sh;
            sprintf(ShellCommand, "%s/run.sh", CurrentPath);
            if ((fp_sh = fopen(ShellCommand, "w")) == NULL) exit(-1);
            //Write the relevant parameters of the script.
            sprintf(ShellCommand, "#BSUB -q %s\n", Queue);
            fputs(ShellCommand, fp_sh);
            sprintf(ShellCommand, "#BSUB -J %s\n", argv[1]);
            fputs(ShellCommand, fp_sh);
            sprintf(ShellCommand, "#BSUB -o %s/%s.out\n", PathWork, argv[1]);
            fputs(ShellCommand, fp_sh);
            sprintf(ShellCommand, "#BSUB -e %s/%s.err\n", PathWork, argv[1]);
            fputs(ShellCommand, fp_sh);
            sprintf(ShellCommand, "#BSUB -n %s\n", Cpu);
            fputs(ShellCommand, fp_sh);
            sprintf(ShellCommand, "#BSUB -R \"span[ptile=%s]\"\n", Span);
            fputs(ShellCommand, fp_sh);
            sprintf(ShellCommand, "#BSUB -a openmpi\n\n\n");
            fputs(ShellCommand, fp_sh);
            //mpirun.lsf
            sprintf(ShellCommand, "%s/bin/./main", CurrentPath);
            char tmp[CMD_NUM];
            for (int i = 1; i < argc; i++)
            {
                sprintf(tmp, " %s", argv[i]);
                strcat(ShellCommand, tmp);
            }
            fputs(ShellCommand, fp_sh);
            fclose(fp_sh);
            //Submit the script.
            sprintf(ShellCommand, "bsub < %s/run.sh", CurrentPath);
            system(ShellCommand);
        }
        else if(cmd == "Environment") SetEvmt(argc, argv);
        else
        {
            std::cout << "Error: Error Parameters." << std::endl;
            std::cout << "Please check again." << std::endl;
            std::cout << std::endl;
        }
    }

    return 0;
}
