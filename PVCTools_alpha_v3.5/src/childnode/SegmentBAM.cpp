//
// Created by liujiajun on 2017/4/21.
//

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//The BAM files has been split in accordance with the chromosome split, then modify the relative address.
//If you need to customize, you should modify the [bamlist] and [falist] in the working directory.
//Command:     ./SegmentBAM <-w WorkPath> <-n SplitNumber>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SegmentBAM.h"
#include "Environment.h"

using namespace std;

int Modify(string &buffer, long addresses_number, const char *chr_name, string tool, long LN)
{
    if (tool != "gatk")
    {
        if (buffer[0] == '@') return 0;
        int count = 0;
        int flag = 0;
        for (int i = 0; i < (int)buffer.size(); i++)
        {
            if (buffer[i] == '\t')
            {
                count++;
                if (count == 3 || count == 7) flag = 1;
            }
            if (flag == 1)
            {
                char *p = &buffer[i + 1];
                char Front[FILE_LINE], Rear[FILE_LINE];
                char Number_Old[CMD_NUM], Number_New[CMD_NUM];
                long Num_Old = atol(p);
                snprintf(Number_Old, sizeof(Number_Old), "%ld", Num_Old);
                int Length_Old = (int)strlen(Number_Old);
                //Modify the starting address.
                long Num_New = Num_Old - addresses_number;
                snprintf(Number_New, sizeof(Number_New), "%ld", Num_New);
                snprintf(Front, (size_t)(i + 2), "%s", buffer.c_str());
                snprintf(Rear, sizeof(Rear), "%s", &buffer[i + 1 + Length_Old]);
                strncat(Front, Number_New, sizeof(Front) - strlen(Front));
                strncat(Front, Rear, sizeof(Front) - strlen(Front));
                buffer = Front;
                flag = 0;
            }
            if (count == 7) return 0;
        }
    } else
    {
        if(buffer.find("@SQ") != string::npos && buffer.find(chr_name) != string::npos)
        {
            int count = 0;
            for (int i = 0; i < (int)buffer.size(); i++)
            {
                if (buffer[i] == '\t') count++;
                if (count == 2)
                {
                    char *p = &buffer[i + 4];
                    char Front[FILE_LINE], Rear[FILE_LINE];
                    char Number_Old[CMD_NUM], Number_New[CMD_NUM];
                    long Num_Old = atol(p);
                    snprintf(Number_Old, sizeof(Number_Old), "%ld", Num_Old);
                    int Length_Old = (int)strlen(Number_Old);
                    snprintf(Number_New, sizeof(Number_New), "%ld", LN);
                    snprintf(Front, (size_t)(i + 5), "%s", buffer.c_str());
                    snprintf(Rear, sizeof(Rear), "%s", &buffer[i + 4 + Length_Old]);
                    strncat(Front, Number_New, sizeof(Front) - strlen(Front));
                    strncat(Front, Rear, sizeof(Front) - strlen(Front));
                    buffer = Front;
                    return 0;
                }
            }
        }
        else if (buffer.find("@SQ") != string::npos) return -1;
        else
        {
            if (buffer[0] == '@') return 0;
            int count = 0;
            int flag = 0;
            for (int i = 0; i < (int)buffer.size(); i++)
            {
                if (buffer[i] == '\t')
                {
                    count++;
                    if (count == 3 || count == 7) flag = 1;
                }
                if (flag == 1)
                {
                    char *p = &buffer[i + 1];
                    char Front[FILE_LINE], Rear[FILE_LINE];
                    char Number_Old[CMD_NUM], Number_New[CMD_NUM];
                    long Num_Old = atol(p);
                    snprintf(Number_Old, sizeof(Number_Old), "%ld", Num_Old);
                    int Length_Old = (int) strlen(Number_Old);
                    //Modify the starting address.
                    long Num_New = Num_Old - addresses_number;
                    snprintf(Number_New, sizeof(Number_New), "%ld", Num_New);
                    snprintf(Front, (size_t)(i + 2), "%s", buffer.c_str());
                    snprintf(Rear, sizeof(Rear), "%s", &buffer[i + 1 + Length_Old]);
                    strncat(Front, Number_New, sizeof(Front) - strlen(Front));
                    strncat(Front, Rear, sizeof(Front) - strlen(Front));
                    buffer = Front;
                    flag = 0;
                }
                if (count == 7) return 0;
            }
        }
    }
    return 0;
}

int Sam_Address_Modify(char *file_name, long address_count, const char *chr_name, string tool, long LN)
{
    ifstream fp_old;
    ofstream fp_new;
    fp_old.open(file_name,ios::in);
    char TmpName[CMD_NUM];
    snprintf(TmpName, sizeof(TmpName), "%s-%d", file_name, (int)getpid());
    fp_new.open(TmpName, ios::out);
    string Buffer;
    getline(fp_old,Buffer);
    while (!fp_old.eof())
    {
        if(Modify(Buffer, address_count, chr_name, tool, LN) != -1) fp_new<<Buffer<<endl;
        getline(fp_old,Buffer);
    }
    fp_old.close();
    fp_new.close();

    remove(file_name);
    rename(TmpName, file_name);
    return 0;
}

int ParallelBAM(int argc, char *argv[])
{

    char chr_name[CMD_NUM];
    char bam_name[CMD_NUM];
    char Command[CMD_NUM];
    int FileNumber;
    string TransBuffer;
    char PathWork[CMD_NUM];
    string PATH_SAMTOOLS;
    string Tool = "samtools";
    GetToolsPath(argv[0], PATH_SAMTOOLS, "-samtools");
    double Reserved = 0;

    for (int i = 0; i < argc; i++)
    {
        string cmd = argv[i];
        if (cmd == "-w")
        {
            snprintf(PathWork, sizeof(PathWork), "%s", argv[i + 1]);
            if (PathWork[strlen(PathWork) - 1] == '/') PathWork[strlen(PathWork) - 1] = '\0';
        }
        if (cmd == "-c") snprintf(chr_name, sizeof(chr_name), "%s", argv[i + 1]);
        if (cmd == "-b") snprintf(bam_name, sizeof(bam_name), "%s", argv[i + 1]);
        if (cmd == "-f") FileNumber = atoi(argv[i + 1]);
        if (cmd == "-T") Tool = argv[i + 1];
        if (cmd == "-R") Reserved = atoi(argv[i + 1]);
    }

    long ReadCount[FileNumber + 1];ReadCount[0] = 0;
    long LN[FileNumber];

    //Gets the length of each line in the fa file.
    snprintf(Command, sizeof(Command), "wc -L %s/fa/%s.fa > %s/sample/%s/%s_%s/tmp", PathWork, chr_name, PathWork, bam_name, bam_name, chr_name);
    system(Command);
    snprintf(Command, sizeof(Command), "%s/sample/%s/%s_%s/tmp", PathWork, bam_name, bam_name, chr_name);
    ifstream fp_sp;
    fp_sp.open(Command,ios::in);
    getline(fp_sp, TransBuffer);
    //sscanf(TransBuffer, "%[^ ]", Command);
    int Maxlen_PreLine = atoi(TransBuffer.c_str());
    fp_sp.close();

    //Statistics read number.
    for (int j = 0; j < FileNumber; j++)
    {
        //Read the number of rows of the current FA file.
        snprintf(Command, sizeof(Command), "wc -l %s/fa/%s/%s_%d.fa > %s/sample/%s/%s_%s/tmp", PathWork, chr_name, chr_name, j, PathWork, bam_name, bam_name, chr_name);
        system(Command);
        snprintf(Command, sizeof(Command), "%s/sample/%s/%s_%s/tmp", PathWork, bam_name, bam_name, chr_name);
        fp_sp.open(Command,ios::in);
        getline(fp_sp, TransBuffer);
        fp_sp.close();
        //Record the number of all reads so far.
        LN[j] = (atol(TransBuffer.c_str()) - 1)*Maxlen_PreLine;
        ReadCount[j + 1] = ReadCount[j] + LN[j]  - (long)((int)(ceil(Reserved / Maxlen_PreLine))*Maxlen_PreLine);
        if(j == FileNumber - 1)
        {
            snprintf(Command, sizeof(Command), "tail -n 1 %s/fa/%s/%s_%d.fa | wc -c > %s/sample/%s/%s_%s/tmp", PathWork, chr_name, chr_name, j, PathWork, bam_name, bam_name, chr_name);
            system(Command);
            snprintf(Command, sizeof(Command), "%s/sample/%s/%s_%s/tmp", PathWork, bam_name, bam_name, chr_name);
            fp_sp.open(Command,ios::in);
            getline(fp_sp, TransBuffer);
            fp_sp.close();
            LN[j] = LN[j] - Maxlen_PreLine + atol(TransBuffer.c_str()) - 1;
            ReadCount[j + 1] = ReadCount[j + 1] - Maxlen_PreLine + atol(TransBuffer.c_str()) - 1;
        }
    }
    snprintf(Command, sizeof(Command), "%s/sample/%s/%s_%s/tmp", PathWork, bam_name, bam_name, chr_name);
    remove(Command);

    //Sort and index.
    snprintf(Command, sizeof(Command), "mkdir -p %s/sample/%s/%s_%s_TemporarySort", PathWork, bam_name, bam_name, chr_name);
    system(Command);

    snprintf(Command, sizeof(Command), "%s sort -T %s/sample/%s/%s_%s_TemporarySort %s/sample/%s/%s_%s.bam > %s/sample/%s/%s_%s_sorted.bam ", PATH_SAMTOOLS.c_str(), PathWork, bam_name, bam_name, chr_name, PathWork, bam_name, bam_name, chr_name, PathWork, bam_name, bam_name, chr_name);
    system(Command);

    snprintf(Command, sizeof(Command), "%s index %s/sample/%s/%s_%s_sorted.bam ", PATH_SAMTOOLS.c_str(), PathWork, bam_name, bam_name, chr_name);
    system(Command);

    snprintf(Command, sizeof(Command), "rm -rf %s/sample/%s/%s_%s_TemporarySort", PathWork, bam_name, bam_name, chr_name);
    system(Command);

    for (int k = 0; k < FileNumber; k++)
    {
        // Extract read in the corresponding interval.
        snprintf(Command, sizeof(Command),
                 "%s view -bh %s/sample/%s/%s_%s_sorted.bam %s:%ld-%ld > %s/sample/%s/%s_%s/%s_%s_%d.bam", PATH_SAMTOOLS.c_str(),
                 PathWork, bam_name, bam_name, chr_name, chr_name,
                 ReadCount[k] + 1, ReadCount[k + 1], PathWork, bam_name,
                 bam_name, chr_name,
                 bam_name, chr_name, k);
        system(Command);

        //The split BAM file into SAM format.
        snprintf(Command, sizeof(Command), "%s view -h %s/sample/%s/%s_%s/%s_%s_%d.bam > %s/sample/%s/%s_%s/%s_%s_%d.sam", PATH_SAMTOOLS.c_str(), PathWork, bam_name,
                 bam_name, chr_name,
                 bam_name, chr_name, k, PathWork, bam_name,
                 bam_name, chr_name,
                 bam_name, chr_name, k);
        system(Command);
        snprintf(Command, sizeof(Command), "%s/sample/%s/%s_%s/%s_%s_%d.bam", PathWork, bam_name,
                 bam_name, chr_name,
                 bam_name, chr_name, k);
        remove(Command);

        //Sam address modified.
        if (k > 0 || Tool == "gatk") Sam_Address_Modify(Command, ReadCount[k] ,chr_name, Tool, LN[k]);

        //Change the modified sam file back to bam format.
        snprintf(Command, sizeof(Command), "%s view -b %s/sample/%s/%s_%s/%s_%s_%d.sam > %s/sample/%s/%s_%s/%s_%s_%d.bam", PATH_SAMTOOLS.c_str(), PathWork, bam_name,
                 bam_name, chr_name,
                 bam_name, chr_name, k, PathWork, bam_name,
                 bam_name, chr_name,
                 bam_name, chr_name, k);
        system(Command);
        snprintf(Command, sizeof(Command), "%s/sample/%s/%s_%s/%s_%s_%d.sam", PathWork, bam_name,
                 bam_name, chr_name,
                 bam_name, chr_name, k);
        remove(Command);
        if(Tool == "gatk")
        {
            snprintf(Command, sizeof(Command), "%s sort %s/sample/%s/%s_%s/%s_%s_%d.bam > %s/sample/%s/%s_%s/%s_%s_%d_tmp.bam", PATH_SAMTOOLS.c_str(), PathWork, bam_name,
                     bam_name, chr_name,
                     bam_name, chr_name, k, PathWork, bam_name,bam_name, chr_name,bam_name, chr_name, k);
            system(Command);
            snprintf(Command, sizeof(Command), "%s/sample/%s/%s_%s/%s_%s_%d.bam", PathWork, bam_name,
                     bam_name, chr_name,
                     bam_name, chr_name, k);
            remove(Command);
            snprintf(Command, sizeof(Command), "mv %s/sample/%s/%s_%s/%s_%s_%d_tmp.bam %s/sample/%s/%s_%s/%s_%s_%d.bam", PathWork, bam_name,
                     bam_name, chr_name,
                     bam_name, chr_name, k, PathWork, bam_name,bam_name, chr_name,bam_name, chr_name, k);
            system(Command);
            snprintf(Command, sizeof(Command), "%s index %s/sample/%s/%s_%s/%s_%s_%d.bam", PATH_SAMTOOLS.c_str(), PathWork, bam_name,
                     bam_name, chr_name,
                     bam_name, chr_name, k);
            system(Command);
        }
    }
    snprintf(Command, sizeof(Command), "%s/sample/%s/%s_%s_sorted.bam", PathWork, bam_name, bam_name, chr_name);
    remove(Command);
    snprintf(Command, sizeof(Command), "%s/sample/%s/%s_%s_sorted.bam.bai", PathWork, bam_name, bam_name, chr_name);
    remove(Command);

    FILE *fp;
    snprintf(Command, sizeof(Command), "%s/SortCount", PathWork);
    if ((fp = fopen(Command, "a+")) == NULL)
    {
        cerr<<"Can't open the SortCount file."<<std::endl;
        exit(-1);
    }
    int fd = fileno(fp);
    flock(fd,LOCK_EX);
    snprintf(Command, sizeof(Command), "%s_%s finished!\n", bam_name, chr_name);
    fputs(Command,fp);
    fclose(fp);
    flock(fd,LOCK_UN);
    return 0;
}

int SegmentBAM(int argc, char *argv[])
{
    long StartTime = time((time_t*)NULL);
    printf("start time = %ld\n", StartTime);

    vector<string> ChrName;
    vector<string> SampleName;
    //Command string.
    char ShellCommand[CMD_NUM];
    string Buffer;
    char PathWork[CMD_NUM];
    char Queue[CMD_NUM] = "fatnode";
    string PATH_SAMTOOLS;
    GetToolsPath(argv[0], PATH_SAMTOOLS, "-samtools");
    string Tool = "samtools";
    int Reserved = 0;
    long TotalNumber = 0;
    
    for (int i = 0; i < argc; i++)
    {
        string cmd = argv[i];
        if (cmd == "-w")
        {
            snprintf(PathWork, sizeof(PathWork), "%s", argv[i + 1]);
            if (PathWork[strlen(PathWork) - 1] == '/') PathWork[strlen(PathWork) - 1] = '\0';
        }
        if (cmd == "-T") Tool = argv[i + 1];
        if (cmd == "-R") Reserved = atoi(argv[i + 1]);
        if (cmd == "-q") snprintf(Queue, sizeof(Queue), "%s", argv[i + 1]);
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
    snprintf(ShellCommand, sizeof(ShellCommand), "%s/falist", PathWork);
    ifstream fp_fa;
    fp_fa.open(ShellCommand,ios::in);
    getline(fp_fa, Buffer);
    while (!fp_fa.eof())
    {
        if (Buffer.size() != 0)
        {
            int i = Buffer.rfind('.');
            strbuff = Buffer.substr(0,i);
            ChrName.push_back(strbuff.c_str());
        }
        getline(fp_fa, Buffer);
    }
    fp_fa.close();

    int FileNumber[ChrName.size()];

    //Settings allow parallel nesting.
//    omp_set_nested(1);

    //Split BAM according to each chromosome.
//#pragma omp parallel for
    for (int i = 0; i < (int)ChrName.size(); ++i)
    {
        char TransCommand[CMD_NUM];
        string TransBuffer;
        ifstream fp_sp;
        //Count the current number of files
        snprintf(TransCommand, sizeof(TransCommand), "ls -l %s/fa/%s | grep \"^-\"| grep '.fa$' |wc -l > %s/%s_tmp ", PathWork, ChrName[i].c_str(), PathWork, ChrName[i].c_str());
        system(TransCommand);
        snprintf(TransCommand, sizeof(TransCommand), "%s/%s_tmp", PathWork, ChrName[i].c_str());
        fp_sp.open(TransCommand,ios::in);
        getline(fp_sp, TransBuffer);
        //The number of files after bisection.
        FileNumber[i] = atoi(TransBuffer.c_str());
        fp_sp.close();
        snprintf(TransCommand, sizeof(TransCommand), "%s/%s_tmp", PathWork, ChrName[i].c_str());
        remove(TransCommand);
        for(int n = 0; (int)SampleName.size(); ++n)
        {
            snprintf(TransCommand, sizeof(TransCommand), "%s/sample/%s/%s_%s.bam", PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str());
            if (access(TransCommand,0) == 0) TotalNumber += FileNumber[i];
        }
    }

    //Parallel Computing
#pragma omp parallel for
    for (int i = 0; i < (int)ChrName.size(); ++i)
    {
        for (int n = 0; n < (int)SampleName.size(); ++n)
        {
            char ModCommand[CMD_NUM];
            FILE *fp_trans;
            //If the chromosome is not present in the sample, it is skipped.
            snprintf(ModCommand, sizeof(ModCommand), "%s/sample/%s/%s_%s.bam", PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str());
            if ((fp_trans = fopen(ModCommand, "r")) == NULL) continue;
            fclose(fp_trans);
            //Already exists before the chromosome processing data is deleted from the original data to prepare to write new data.
            snprintf(ModCommand, sizeof(ModCommand), "%s/sample/%s/%s_%s", PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str());
            DIR * bam_dir;
            bam_dir = opendir(ModCommand);
            if (bam_dir != NULL)
            {
                closedir(bam_dir);
                snprintf(ModCommand, sizeof(ModCommand), "rm -rf %s/sample/%s/%s_%s", PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str());
                system(ModCommand);
            }
            snprintf(ModCommand, sizeof(ModCommand), "mkdir -p %s/sample/%s/%s_%s", PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str());
            system(ModCommand);
            //printf("BAM file to extract...\n");
            snprintf(ModCommand, sizeof(ModCommand), "%s/sample/%s/%s_%s/%s_%s.sh", PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str(), SampleName[n].c_str(), ChrName[i].c_str());
            if ((fp_trans = fopen(ModCommand, "w")) == NULL)
                exit(-1);
            snprintf(ModCommand, sizeof(ModCommand), "#BSUB -q %s\n", Queue);
            fputs(ModCommand, fp_trans);
            snprintf(ModCommand, sizeof(ModCommand), "#BSUB -J %s_%s_sort\n", SampleName[n].c_str(), ChrName[i].c_str());
            fputs(ModCommand, fp_trans);
            snprintf(ModCommand, sizeof(ModCommand), "#BSUB -n 1\n\n");
            fputs(ModCommand, fp_trans);
            snprintf(ModCommand, sizeof(ModCommand), "\n%s ParallelBAM -w %s -c %s -b %s -f %d -T %s -r %d\n", argv[0], PathWork, ChrName[i].c_str(), SampleName[n].c_str(), FileNumber[i], Tool.c_str(), Reserved);
            fputs(ModCommand, fp_trans);
            fclose(fp_trans);
            snprintf(ModCommand, sizeof(ModCommand), "bsub < %s/sample/%s/%s_%s/%s_%s.sh", PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str(), SampleName[n].c_str(), ChrName[i].c_str());
            system(ModCommand);
            snprintf(ModCommand, sizeof(ModCommand), "%s/sample/%s/%s_%s/%s_%s.sh", PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str(), SampleName[n].c_str(), ChrName[i].c_str());
            remove(ModCommand);
        }
    }

    ifstream fp_tmp;
    string CountBuffer = "0";
    while (atol(CountBuffer.c_str()) != TotalNumber)
    {
        sleep(1000);
        snprintf(ShellCommand, sizeof(ShellCommand), "wc -l %s/SortCount > %s/SortCount_tmp", PathWork, PathWork);
        system(ShellCommand);
        snprintf(ShellCommand, sizeof(ShellCommand), "%s/SortCount_tmp", PathWork);
        fp_tmp.open(ShellCommand,ios::in);
        getline(fp_tmp, CountBuffer);
        fp_tmp.close();
    }
    snprintf(ShellCommand, sizeof(ShellCommand), "%s/SortCount_tmp", PathWork);
    remove(ShellCommand);
    snprintf(ShellCommand, sizeof(ShellCommand), "%s/SortCount", PathWork);
    remove(ShellCommand);

    long FinishTime = time((time_t*)NULL);
    printf("finish time = %ld\n", FinishTime);
    long RunningTime = FinishTime - StartTime;
    printf("running time = %ld\n", RunningTime);
    return 0;
}
