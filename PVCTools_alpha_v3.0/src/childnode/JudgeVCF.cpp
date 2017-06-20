//
// Created by liujiajun on 2017/5/21.
//

#include "JudgeVCF.h"
#include "Environment.h"

using namespace std;

void VCF_Modify(string &buffer, long addresses_number)
{
    int count = 0;
    for (int i = 0; i < (int)buffer.size(); i++)
    {
        if (buffer[i] == '\t') count++;
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
        if(buff.at(0) != '#') break;
    }
    VCF_Modify(buff,add_count);
    outfile<<buff<<endl;
    //Write the cache, modify the address, write the stitching file.
    while (1)
    {
        getline(infile,buff);
        if(infile.eof()) break;
        //Restore the address in VCF.
        VCF_Modify(buff,add_count);
        outfile<<buff<<endl;
    }
    infile.close();
    outfile.close();
    return 0;
}

int TotalVCF(const char *workdir, int judge)
{
    char BigFA[CMD_NUM];
    snprintf(BigFA, sizeof(BigFA), "%s/big_success", workdir);
    char SmallFA[CMD_NUM];
    snprintf(SmallFA, sizeof(SmallFA), "%s/small_success", workdir);

    if(access(BigFA,0) == 0 && judge && access(SmallFA,0) == 0)
    {
        char ShellCommand[CMD_NUM];
        vector<string> ChrName;
        string Buffer;
        string strbuff;
        ifstream fp_list;

        remove(BigFA);
        remove(SmallFA);

        snprintf(ShellCommand, sizeof(ShellCommand), "%s/falist", workdir);
        fp_list.open(ShellCommand,ios::in);
        getline(fp_list, Buffer);
        while (!fp_list.eof())
        {
            if (Buffer.size() != 0)
            {
                int i = Buffer.rfind('.');
                strbuff = Buffer.substr(0,i);
                ChrName.push_back(strbuff.c_str());
            }
            getline(fp_list, Buffer);
        }
        fp_list.close();

        snprintf(ShellCommand, sizeof(ShellCommand), "%s/smalllist", workdir);
        fp_list.open(ShellCommand,ios::in);
        getline(fp_list, Buffer);
        while (!fp_list.eof())
        {
            if (Buffer.size() != 0)
            {
                int i = Buffer.rfind('.');
                strbuff = Buffer.substr(0,i);
                ChrName.push_back(strbuff.c_str());
            }
            getline(fp_list, Buffer);
        }
        fp_list.close();

        string PATH_SAMTOOLS;
        GetToolsPath(workdir, PATH_SAMTOOLS, "-samtools");
        string PATH_BCFTOOLS;
        GetToolsPath(workdir, PATH_BCFTOOLS, "-bcftools");
        snprintf(ShellCommand, sizeof(ShellCommand), "%s mpileup -u -t DP,AD,ADF %s/header.bam | %s call -vmO v -o %s/header.vcf", PATH_SAMTOOLS.c_str(), workdir, PATH_BCFTOOLS.c_str(), workdir);
        system(ShellCommand);
        snprintf(ShellCommand, sizeof(ShellCommand), "%s/header.bam", workdir);
        remove(ShellCommand);

        snprintf(ShellCommand, sizeof(ShellCommand), "cp -f %s/header.vcf %s/vcf/Final_Result.var.flt.vcf", workdir, workdir);
        system(ShellCommand);

        char VCF_File[CMD_NUM];
        snprintf(VCF_File, sizeof(VCF_File), "%s/vcf/Final_Result.var.flt.vcf", workdir);

        ofstream outfile;
        outfile.open(VCF_File, ios::out|ios::app);
        for (long i = 1; i < long(ChrName.size()); i++)
        {
            snprintf(ShellCommand, sizeof(ShellCommand), "%s/vcf/Final_Result/%s.var.flt.vcf", workdir, ChrName[i].c_str());
            ifstream infile;
            infile.open(ShellCommand,ios::in);
            getline(infile,Buffer);
            while (!infile.eof())
            {
                if(Buffer.at(0) != '#') outfile<<Buffer<<endl;
                getline(infile,Buffer);
            }
            infile.close();
        }
        outfile.close();
        snprintf(ShellCommand, sizeof(ShellCommand), "%s/header.vcf", workdir);
        remove(ShellCommand);
    } else if(access(BigFA,0) == 0 && judge == 0)
    {
        char ShellCommand[CMD_NUM];
        vector<string> ChrName;
        string Buffer;
        string strbuff;
        ifstream fp_list;

        remove(BigFA);
        remove(SmallFA);

        snprintf(ShellCommand, sizeof(ShellCommand), "%s/falist", workdir);
        fp_list.open(ShellCommand,ios::in);
        getline(fp_list, Buffer);
        while (!fp_list.eof())
        {
            if (Buffer.size() != 0)
            {
                int i = Buffer.rfind('.');
                strbuff = Buffer.substr(0,i);
                ChrName.push_back(strbuff.c_str());
            }
            getline(fp_list, Buffer);
        }
        fp_list.close();

        string PATH_SAMTOOLS;
        GetToolsPath(workdir, PATH_SAMTOOLS, "-samtools");
        string PATH_BCFTOOLS;
        GetToolsPath(workdir, PATH_BCFTOOLS, "-bcftools");
        snprintf(ShellCommand, sizeof(ShellCommand), "%s mpileup -u -t DP,AD,ADF %s/header.bam | %s call -vmO v -o %s/header.vcf", PATH_SAMTOOLS.c_str(), workdir, PATH_BCFTOOLS.c_str(), workdir);
        system(ShellCommand);
        snprintf(ShellCommand, sizeof(ShellCommand), "%s/header.bam", workdir);
        remove(ShellCommand);
        snprintf(ShellCommand, sizeof(ShellCommand), "cp -f %s/header.vcf %s/vcf/Final_Result.var.flt.vcf", workdir, workdir);
        system(ShellCommand);

        char VCF_File[CMD_NUM];
        snprintf(VCF_File, sizeof(VCF_File), "%s/vcf/Final_Result.var.flt.vcf", workdir);

        ofstream outfile;
        outfile.open(VCF_File, ios::out|ios::app);
        for (long i = 1; i < long(ChrName.size()); i++)
        {
            snprintf(ShellCommand, sizeof(ShellCommand), "%s/vcf/Final_Result/%s.var.flt.vcf", workdir, ChrName[i].c_str());
            ifstream infile;
            infile.open(ShellCommand,ios::in);
            getline(infile,Buffer);
            while (!infile.eof())
            {
                if(Buffer.at(0) != '#') outfile<<Buffer<<endl;
                getline(infile,Buffer);
            }
            infile.close();
        }
        outfile.close();
        snprintf(ShellCommand, sizeof(ShellCommand), "%s/header.vcf", workdir);
        remove(ShellCommand);
    }
    return 0;
}

int StitchVCF(int argc,char *argv[])
{
    vector<string> ChrName;
    //Command string.
    char ShellCommand[CMD_NUM];
    string Buffer;
    char PathWork[CMD_NUM];
    int SplitNumber;
    double Reserved = 0;

    for (int i = 0; i < argc; i++)
    {
        string cmd = argv[i];
        if (cmd == "-w")
        {
            snprintf(PathWork, sizeof(PathWork), "%s", argv[i + 1]);
            if (PathWork[strlen(PathWork) - 1] == '/') PathWork[strlen(PathWork) - 1] = '\0';
        }
        if (cmd == "-n") SplitNumber = atoi(argv[i + 1]);
        if (cmd == "-R") Reserved = atol(argv[i + 1]);
    }

    string strbuff;
    //Import FA list, if you need to customize the list, you should modify the [falist], fill in the need to split the FA file.
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

//    omp_set_nested(1);

    int FileNumber[ChrName.size()];
    long ReadCount[ChrName.size()][2 * SplitNumber];

    printf("The VCF results are in the statistics...\n");
#pragma omp parallel for
    for (int i = 0; i < (int)ChrName.size(); ++i)
    {
        char Command[CMD_NUM];
        string VCFBuffer;
        ifstream fp_chr;

        ReadCount[i][0] = 0;

        snprintf(Command, sizeof(Command), "touch %s_tmp", ChrName[i].c_str());
        system(Command);
        //Count the current number of FA files.
        snprintf(Command, sizeof(Command), "ls -l %s/fa/%s |grep \"^-\"|wc -l > %s_tmp", PathWork, ChrName[i].c_str(), ChrName[i].c_str());
        system(Command);
        snprintf(Command, sizeof(Command), "%s_tmp", ChrName[i].c_str());
        fp_chr.open(Command,ios::in);
        getline(fp_chr, VCFBuffer);
        FileNumber[i] = atoi(VCFBuffer.c_str()) / 2;
        fp_chr.close();

        //Get the length of each line in the FA files.
        snprintf(Command, sizeof(Command), "wc -L %s/fa/%s.fa > %s_tmp", PathWork, ChrName[i].c_str(),
                 ChrName[i].c_str());
        system(Command);
        snprintf(Command, sizeof(Command), "%s_tmp", ChrName[i].c_str());
        fp_chr.open(Command,ios::in);
        getline(fp_chr, VCFBuffer);
        int Maxlen_PreLine = atoi(VCFBuffer.c_str());
        fp_chr.close();

        for (int j = 0; j < FileNumber[i]; j++)
        {
            //Read the current FA file number of rows.
            snprintf(Command, sizeof(Command), "wc -l %s/fa/%s/%s_%d.fa > %s_tmp", PathWork, ChrName[i].c_str(),
                     ChrName[i].c_str(), j, ChrName[i].c_str());
            system(Command);
            snprintf(Command, sizeof(Command), "%s_tmp", ChrName[i].c_str());
            fp_chr.open(Command,ios::in);
            getline(fp_chr, VCFBuffer);
            fp_chr.close();
            //Record the number of all reads so far.
            ReadCount[i][j + 1] = ReadCount[i][j] + (atol(VCFBuffer.c_str()) - 1 - (int)(ceil(Reserved / Maxlen_PreLine))) * Maxlen_PreLine;
        }
        snprintf(Command, sizeof(Command), "%s_tmp", ChrName[i].c_str());
        remove(Command);
    }
    printf("VCF results are completed.\n");

    snprintf(ShellCommand, sizeof(ShellCommand), "mkdir -p %s/vcf/Final_Result", PathWork);
    system(ShellCommand);

    printf("VCF results stitching ...\n");
    for (int i = 0; i < (int)ChrName.size(); i++)
    {
        char Command[CMD_NUM];

        //Copy a copy of the first VCF file.
        snprintf(Command, sizeof(Command), "cp -f %s/vcf/%s/%s_0.var.flt.vcf %s/vcf/Final_Result/%s.var.flt.vcf", PathWork, ChrName[i].c_str(), ChrName[i].c_str(), PathWork, ChrName[i].c_str());
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
    return 0;
}

int JudgeVCF(int argc, char *argv[])
{
    char PathWork[CMD_NUM];
    string FileName;
    string Size;
    long TotalNumber = 0;
    char Command[CMD_NUM];
    char SplitNumber[CMD_NUM];
    int single_judge = 1;
    char Reserved[CMD_NUM] = "0";

    for (int i = 0; i < argc; i++)
    {
        string cmd = argv[i];
        if (cmd == "-w")
        {
            snprintf(PathWork, sizeof(PathWork), "%s", argv[i + 1]);
            if (PathWork[strlen(PathWork) - 1] == '/') PathWork[strlen(PathWork) - 1] = '\0';
        }
        if (cmd == "-C") TotalNumber = atol(argv[i + 1]);
        if (cmd == "-N") FileName = argv[i + 1];
        if (cmd == "-n") snprintf(SplitNumber, sizeof(SplitNumber), "%s", argv[i + 1]);
        if (cmd == "-S")
        {
            snprintf(Command, sizeof(Command), "%s_Count", argv[i + 1]);
            Size = Command;
        }
        if (cmd == "-single") single_judge = 0;
        if (cmd == "-R") snprintf(Reserved, sizeof(SplitNumber), "%s", argv[i + 1]);
    }

    FILE *fp;
    snprintf(Command, sizeof(Command), "%s/%s", PathWork, Size.c_str());
    if ((fp = fopen(Command, "a+")) == NULL)
    {
        cerr<<"Can't open the Count file."<<std::endl;
        exit(-1);
    }
    int fd = fileno(fp);
    flock(fd,LOCK_EX);
    snprintf(Command, sizeof(Command), "%s finished!\n", FileName.c_str());
    fputs(Command,fp);
    fclose(fp);
    ifstream fp_tmp;
    string Buffer;
    snprintf(Command, sizeof(Command), "wc -l %s/%s > %s/%s_tmp", PathWork, Size.c_str(), PathWork, FileName.c_str());
    system(Command);
    snprintf(Command, sizeof(Command), "%s/%s_tmp", PathWork, FileName.c_str());
    fp_tmp.open(Command,ios::in);
    getline(fp_tmp, Buffer);
    fp_tmp.close();
    remove(Command);
    flock(fd,LOCK_UN);
    if (atol(Buffer.c_str()) == TotalNumber && Size == "big_Count")
    {
        snprintf(Command, sizeof(Command), "%s/%s", PathWork, Size.c_str());
        remove(Command);
        char* ary[6];
        char ary1[] = "-w";
        char ary2[] = "-n";
        char ary3[] = "-R";
        ary[0] = ary1;
        ary[1] = PathWork;
        ary[2] = ary2;
        ary[3] = SplitNumber;
        ary[4] = ary3;
        ary[5] = Reserved;
        printf("staring stitch VCF!\n");
        StitchVCF(6, ary);
        snprintf(Command, sizeof(Command), "%s/big_success", PathWork);
        fp = fopen(Command, "w");
        fclose(fp);
        TotalVCF(PathWork, single_judge);
    }
    else if (atol(Buffer.c_str()) == TotalNumber && Size == "small_Count")
    {
        snprintf(Command, sizeof(Command), "%s/%s", PathWork, Size.c_str());
        remove(Command);
        snprintf(Command, sizeof(Command), "%s/small_success", PathWork);
        fp = fopen(Command, "w");
        fclose(fp);
        TotalVCF(PathWork, single_judge);
    }
    return 0;
}