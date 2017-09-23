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

void Modify(char *buffer, long addresses_number)
{
    if (buffer[0] == '@') return;
    int count = 0;
    int flag = 0;
    for (int i = 0; i < (int)strlen(buffer); i++)
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
            snprintf(Front, (size_t)(i + 2), "%s", buffer);
            snprintf(Rear, sizeof(Rear), "%s", &buffer[i + 1 + Length_Old]);
            strncat(Front, Number_New, sizeof(Front) - strlen(Front));
            strncat(Front, Rear, sizeof(Front) - strlen(Front));
            snprintf(buffer, FILE_LINE, "%s", Front);
            flag = 0;
        }
        if (count == 7) return;
    }
}

int Sam_Address_Modify(char *file_name, long address_count)
{
//    string Buffer;
//    ifstream fp_old;
//    ofstream fp_new;
//    char TmpName[CMD_NUM];
//
//    snprintf(TmpName, sizeof(TmpName), "%s-%d", file_name, (int)getpid());
//    fp_old.open(file_name,ios::in);
//    fp_new.open(TmpName, ios::out);
//
//    getline(fp_old, Buffer);
//    while (!fp_old.eof())
//    {
//        Modify(Buffer, address_count);
//        fp_new<<Buffer<<endl;
//        getline(fp_old,Buffer);
//    }
//    fp_old.close();
//    fp_new.close();

    FILE *fp_old, *fp_new;
    if ((fp_old = fopen(file_name, "r")) == NULL)
        exit(-1);
    char TmpName[CMD_NUM];
    snprintf(TmpName, sizeof(TmpName), "%s-%d", file_name, (int)getpid());
    if ((fp_new = fopen(TmpName, "w")) == NULL)
        exit(-1);
    char *Buffer = NULL;
    size_t Len = FILE_LINE;
    getline(&Buffer, &Len, fp_old);
    while (!feof(fp_old))
    {
        Modify(Buffer, address_count);
        fputs(Buffer, fp_new);
        getline(&Buffer, &Len, fp_old);
    }
    fclose(fp_old);
    fclose(fp_new);

    remove(file_name);
    rename(TmpName, file_name);
    return 0;
}

int SegmentBAM(int argc, char *argv[])
{
    long StartTime = time((time_t*)NULL);
    printf("start time = %ld\n", StartTime);

    string PATH_SAMTOOLS;
    GetToolsPath(argv[0], PATH_SAMTOOLS, "-samtools");
    vector<string> ChrName;
    vector<string> SampleName;
    //Command string.
    char ShellCommand[CMD_NUM];
    string Buffer;
    char PathWork[CMD_NUM];
    int SplitNumber;
    
    for (int i = 0; i < argc; i++)
    {
        string cmd = argv[i];
        if (cmd == "-w")
        {
            snprintf(PathWork, sizeof(PathWork), "%s", argv[i + 1]);
            if (PathWork[strlen(PathWork) - 1] == '/') PathWork[strlen(PathWork) - 1] = '\0';
        }
        if (cmd == "-n") SplitNumber = atoi(argv[i + 1]);
    }

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
    omp_set_nested(1);

    //Parallel Computing
#pragma omp parallel for
    for (int i = 0; i < (int)ChrName.size(); ++i)
    {
#pragma omp parallel for
        for (int n = 0; n < (int)SampleName.size(); ++n)
        {
            char SortCommand[CMD_NUM];
            FILE *fp_sort;
            snprintf(SortCommand, sizeof(SortCommand), "%s/sample/%s/%s_%s.bam", PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str());
            if ((fp_sort = fopen(SortCommand, "r")) == NULL) continue;
            fclose(fp_sort);

            //Sort and index.
            snprintf(SortCommand, sizeof(SortCommand), "mkdir -p %s/sample/%s/%s_%s_TemporarySort", PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str());
            system(SortCommand);

            snprintf(SortCommand, sizeof(SortCommand), "%s sort -T %s/sample/%s/%s_%s_TemporarySort %s/sample/%s/%s_%s.bam > %s/sample/%s/%s_%s_sorted.bam ", PATH_SAMTOOLS.c_str(), PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str(), PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str(), PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str());
            system(SortCommand);

            snprintf(SortCommand, sizeof(SortCommand), "%s index %s/sample/%s/%s_%s_sorted.bam ", PATH_SAMTOOLS.c_str(), PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str());
            system(SortCommand);

            snprintf(SortCommand, sizeof(SortCommand), "rm -rf %s/sample/%s/%s_%s_TemporarySort", PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str());
            system(SortCommand);
        }
    }

    printf("Sort is end.\n");

    long ReadCount[ChrName.size()][2 * SplitNumber];

    //Split BAM according to each chromosome.
#pragma omp parallel for
    for (int i = 0; i < (int)ChrName.size(); ++i)
    {
        char TransCommand[CMD_NUM];
        string TransBuffer;
        ReadCount[i][0] = 0;

        snprintf(TransCommand, sizeof(TransCommand), "touch %s_tmp", ChrName[i].c_str());
        system(TransCommand);

        //Gets the length of each line in the fa file.
        snprintf(TransCommand, sizeof(TransCommand), "wc -L %s/fa/%s.fa > %s_tmp", PathWork, ChrName[i].c_str(), ChrName[i].c_str());
        system(TransCommand);
        snprintf(TransCommand, sizeof(TransCommand), "%s_tmp", ChrName[i].c_str());
        ifstream fp_sp;
        fp_sp.open(TransCommand,ios::in);
        getline(fp_sp, TransBuffer);
        //sscanf(TransBuffer, "%[^ ]", TransCommand);
        int Maxlen_PreLine = atoi(TransBuffer.c_str());
        fp_sp.close();

        //Count the current number of files
        snprintf(TransCommand, sizeof(TransCommand), "ls -l %s/fa/%s |grep \"^-\"|wc -l > %s_tmp ", PathWork, ChrName[i].c_str(), ChrName[i].c_str());
        system(TransCommand);
        snprintf(TransCommand, sizeof(TransCommand), "%s_tmp", ChrName[i].c_str());
        fp_sp.open(TransCommand,ios::in);
        getline(fp_sp, TransBuffer);
        //The number of files after bisection.
        FileNumber[i] = atoi(TransBuffer.c_str());
        fp_sp.close();

        //Statistics read number.
        for (int j = 0; j < FileNumber[i]; j++)
        {
            //Read the number of rows of the current FA file.
            snprintf(TransCommand, sizeof(TransCommand), "wc -l %s/fa/%s/%s_%d.fa > %s_tmp", PathWork, ChrName[i].c_str(), ChrName[i].c_str(), j, ChrName[i].c_str());
            system(TransCommand);
            snprintf(TransCommand, sizeof(TransCommand), "%s_tmp", ChrName[i].c_str());
            fp_sp.open(TransCommand,ios::in);
            getline(fp_sp, TransBuffer);
            fp_sp.close();
            //Record the number of all reads so far.
            ReadCount[i][j + 1] = ReadCount[i][j] + (atol(TransBuffer.c_str()) - 1)*Maxlen_PreLine;
        }
        snprintf(TransCommand, sizeof(TransCommand), "%s_tmp", ChrName[i].c_str());
        remove(TransCommand);
        //Extract and modify BAM
#pragma omp parallel for
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

            //Extract and modify BAM.
            for (int k = 0; k < FileNumber[i]; k++)
            {
                // Extract read in the corresponding interval.
                snprintf(ModCommand, sizeof(ModCommand),
                         "%s view -bh %s/sample/%s/%s_%s_sorted.bam %s:%ld-%ld > %s/sample/%s/%s_%s/%s_%s_%d.bam", PATH_SAMTOOLS.c_str(),
                        PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str(), ChrName[i].c_str(),
                        ReadCount[i][k] + 1, ReadCount[i][k + 1], PathWork, SampleName[n].c_str(),
                        SampleName[n].c_str(), ChrName[i].c_str(),
                        SampleName[n].c_str(), ChrName[i].c_str(), k);
                system(ModCommand);

                //The split BAM file into SAM format.
                snprintf(ModCommand, sizeof(ModCommand), "%s view -h %s/sample/%s/%s_%s/%s_%s_%d.bam > %s/sample/%s/%s_%s/%s_%s_%d.sam", PATH_SAMTOOLS.c_str(), PathWork, SampleName[n].c_str(),
                        SampleName[n].c_str(), ChrName[i].c_str(),
                        SampleName[n].c_str(), ChrName[i].c_str(), k, PathWork, SampleName[n].c_str(),
                        SampleName[n].c_str(), ChrName[i].c_str(),
                        SampleName[n].c_str(), ChrName[i].c_str(), k);
                system(ModCommand);
                snprintf(ModCommand, sizeof(ModCommand), "%s/sample/%s/%s_%s/%s_%s_%d.bam", PathWork, SampleName[n].c_str(),
                        SampleName[n].c_str(), ChrName[i].c_str(),
                        SampleName[n].c_str(), ChrName[i].c_str(), k);
                remove(ModCommand);

                //Sam address modified.
                if (k > 0)
                {
                    snprintf(ModCommand, sizeof(ModCommand), "%s/sample/%s/%s_%s/%s_%s_%d.sam", PathWork, SampleName[n].c_str(),
                            SampleName[n].c_str(), ChrName[i].c_str(),
                            SampleName[n].c_str(), ChrName[i].c_str(), k);
                    Sam_Address_Modify(ModCommand, ReadCount[i][k]);
                }

                //Change the modified sam file back to bam format.
                snprintf(ModCommand, sizeof(ModCommand), "%s view -b %s/sample/%s/%s_%s/%s_%s_%d.sam > %s/sample/%s/%s_%s/%s_%s_%d.bam", PATH_SAMTOOLS.c_str(), PathWork, SampleName[n].c_str(),
                        SampleName[n].c_str(), ChrName[i].c_str(),
                        SampleName[n].c_str(), ChrName[i].c_str(), k, PathWork, SampleName[n].c_str(),
                        SampleName[n].c_str(), ChrName[i].c_str(),
                        SampleName[n].c_str(), ChrName[i].c_str(), k);
                system(ModCommand);
                snprintf(ModCommand, sizeof(ModCommand), "%s/sample//%s/%s_%s/%s_%s_%d.sam", PathWork, SampleName[n].c_str(),
                        SampleName[n].c_str(), ChrName[i].c_str(),
                        SampleName[n].c_str(), ChrName[i].c_str(), k);
                remove(ModCommand);
            }
            snprintf(ModCommand, sizeof(ModCommand), "%s/sample/%s/%s_%s_sorted.bam", PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str());
            remove(ModCommand);
            snprintf(ModCommand, sizeof(ModCommand), "%s/sample/%s/%s_%s_sorted.bam.bai", PathWork, SampleName[n].c_str(), SampleName[n].c_str(), ChrName[i].c_str());
            remove(ModCommand);
        }
    }

    long FinishTime = time((time_t*)NULL);
    printf("finish time = %ld\n", FinishTime);
    long RunningTime = FinishTime - StartTime;
    printf("running time = %ld\n", RunningTime);
    return 0;
}
