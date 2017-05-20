//
// Created by liujiajun on 2017/4/21.
//

//////////////////////////////////////////////////////
//Stitching the calculated VCF file.
//Command:	./StitchVCF	<-w WorkPath>	<-n SplitNumber>
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StitchVCF.h"
#include "Environment.h"

using namespace std;

void VCF_Modify(string &buffer, long addresses_number)
{
    int count = 0;
    for (int i = 0; i < (int)buffer.size(); i++)
    {
        if (buffer[i] == '\t')
        {
            count++;
        }
        if (count == 1)
        {
            char *p = &buffer[i + 1];
            char Front[FILE_LINE], Rear[FILE_LINE];
            char Number_Old[CMD_NUM], Number_New[CMD_NUM];
            long Num_Old = atol(p);
            snprintf(Number_Old, sizeof(Number_Old), "%ld", Num_Old);
            int Length_Old = (int)strlen(Number_Old);
            //Modify the starting address.
            long Num_New = Num_Old + addresses_number;
            snprintf(Number_New, sizeof(Number_New), "%ld", Num_New);
            snprintf(Front, (size_t)(i + 2), "%s", buffer.c_str());
            snprintf(Rear, sizeof(Rear), "%s", &buffer[i + 1 + Length_Old]);
            strncat(Front, Number_New, sizeof(Front) - strlen(Front));
            strncat(Front, Rear, sizeof(Front) - strlen(Front));
            buffer = Front;
            return ;
        }
    }
}

int VCF_Link(char *tarfile, char *formfile, long add_count)
{
    string buff;
    string cmd = formfile;
    ifstream infile;
    ofstream outfile;
    infile.open(formfile,ios::in);
    outfile.open(tarfile, ios::out|ios::app);
    while (1)
    {
        getline(infile,buff);
        if(infile.eof())
        {
            printf("There is no need to stitch the content!");
            infile.close();
            outfile.close();
            return 0;
        }
        if(buff[0] != '#') break;
    }
    VCF_Modify(buff,add_count);
    outfile<<buff;
    //Write the cache, modify the address, write the stitching file.
    while (1)
    {
        getline(infile,buff);
        if(infile.eof()) break;
        //Restore the address in VCF.
        VCF_Modify(buff,add_count);
        outfile<<buff;
    }


    infile.close();
    outfile.close();
    return 0;

}

int StitchVCF(int argc,char *argv[])
{
    long StartTime = time((time_t*)NULL);
    printf("start time = %ld\n", StartTime);

    vector<string> ChrName;
    //Command string.
    char ShellCommand[CMD_NUM];
    char *Buffer = NULL;
    size_t Len = 0;

    char PathWork[CMD_NUM];
    int SplitNumber;

    for (int i = 0; i < argc; i++)
    {
        string cmd = argv[i];
        if (cmd == "-w")
        {
            snprintf(PathWork, sizeof(PathWork), "%s", argv[i + 1]);
            if (PathWork[strlen(PathWork) - 1] == '/')
                PathWork[strlen(PathWork) - 1] = '\0';
        }
        if (cmd == "-n")
        {
            SplitNumber = atoi(argv[i + 1]);
        }
    }

    //Import FA list, if you need to customize the list, you should modify the [falist], fill in the need to split the FA file.
    snprintf(ShellCommand, sizeof(ShellCommand), "%s/falist", PathWork);
    FILE *fp_fa;
    if ((fp_fa = fopen(ShellCommand, "r")) == NULL)
        exit(-1);
    getline(&Buffer, &Len,fp_fa);
    while (!feof(fp_fa))
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
            ChrName.push_back(Buffer);
        }
        getline(&Buffer, &Len,fp_fa);
    }
    fclose(fp_fa);
    if (Buffer)
    {
        free(Buffer);
        Buffer = NULL;
    }

//    omp_set_nested(1);

    int FileNumber[ChrName.size()];
    long ReadCount[ChrName.size()][2 * SplitNumber];

    printf("The VCF results are in the statistics...\n");
#pragma omp parallel for
    for (int i = 0; i < (int)ChrName.size(); ++i)
    {
        char Command[CMD_NUM];
        char *VCFBuffer = NULL;
        size_t VCFLen = 0;

        ReadCount[i][0] = 0;

        snprintf(Command, sizeof(Command), "touch %s_tmp", ChrName[i].c_str());
        system(Command);
        //Count the current number of FA files.
        snprintf(Command, sizeof(Command), "ls -l %s/fa/%s |grep \"^-\"|wc -l > %s_tmp", PathWork, ChrName[i].c_str(), ChrName[i].c_str());
        system(Command);
        snprintf(Command, sizeof(Command), "%s_tmp", ChrName[i].c_str());
        FILE *fp_chr;
        if ((fp_chr = fopen(Command, "r")) == NULL)
            exit(-1);
        getline(&VCFBuffer, &VCFLen, fp_chr);
        FileNumber[i] = atoi(VCFBuffer) / 2;
        fclose(fp_chr);

        //Get the length of each line in the FA files.
        snprintf(Command, sizeof(Command), "wc -L %s/fa/%s.fa > %s_tmp", PathWork, ChrName[i].c_str(),
                 ChrName[i].c_str());
        system(Command);
        snprintf(Command, sizeof(Command), "%s_tmp", ChrName[i].c_str());
        if ((fp_chr = fopen(Command, "r")) == NULL)
            exit(-1);
        getline(&VCFBuffer, &VCFLen, fp_chr);
        int Maxlen_PreLine = atoi(VCFBuffer);
        fclose(fp_chr);

        for (int j = 0; j < FileNumber[i]; j++)
        {
            //Read the current FA file number of rows.
            snprintf(Command, sizeof(Command), "wc -l %s/fa/%s/%s_%d.fa > %s_tmp", PathWork, ChrName[i].c_str(),
                     ChrName[i].c_str(), j, ChrName[i].c_str());
            system(Command);
            snprintf(Command, sizeof(Command), "%s_tmp", ChrName[i].c_str());
            if ((fp_chr = fopen(Command, "r")) == NULL)
                exit(-1);
            getline(&VCFBuffer, &VCFLen, fp_chr);
            fclose(fp_chr);

            //Record the number of all reads so far.
            ReadCount[i][j + 1] = ReadCount[i][j] + (atol(VCFBuffer) - 1) * Maxlen_PreLine;

        }
        snprintf(Command, sizeof(Command), "%s_tmp", ChrName[i].c_str());
        remove(Command);
        if (VCFBuffer)
        {
            free(VCFBuffer);
            VCFBuffer = NULL;
        }
    }
    printf("VCF results are completed.\n");

    snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/vcf/Final_Result", PathWork);
    system(ShellCommand);

    printf("VCF results stitching ...\n");
    for (int i = 0; i < (int)ChrName.size(); i++)
    {
        char Command[CMD_NUM];

        //Copy a copy of the first VCF file.
        snprintf(Command, sizeof(Command), "cp -f %s/vcf/%s/%s_0.var.flt.vcf %s/vcf/Final_Result/%s.var.flt.vcf", PathWork,
                 ChrName[i].c_str(), ChrName[i].c_str(), PathWork,
                 ChrName[i].c_str());
        system(Command);

        char VCF_File[CMD_NUM];
        snprintf(VCF_File, sizeof(VCF_File), "%s/vcf/Final_Result/%s.var.flt.vcf", PathWork, ChrName[i].c_str());

        for (int n = 1; n < FileNumber[i]; n++)
        {
            snprintf(Command, sizeof(Command), "%s/vcf/%s/%s_%d.var.flt.vcf", PathWork, ChrName[i].c_str(),ChrName[i].c_str(),n);
            VCF_Link(VCF_File, Command, ReadCount[i][n]);
        }
    }
    printf("VCF results stitching done.\n");


    long FinishTime = time((time_t*)NULL);
    printf("finish time = %ld\n", FinishTime);
    long RunningTime = FinishTime - StartTime;
    printf("running time = %ld\n", RunningTime);

    return 0;
}