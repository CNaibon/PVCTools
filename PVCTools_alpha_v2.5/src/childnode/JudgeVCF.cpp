//
// Created by liujiajun on 2017/5/21.
//

int JudgeVCF(int argc, char *argv[])
{
    char PathWork[CMD_NUM];
    string FileName;
    long FileCount;
    long TotalNumber;
    char ShellCommand[CMD_NUM];

    for (int i = 0; i < argc; i++)
    {
        string cmd = argv[i];
        if (cmd == "-w")
        {
            snprintf(PathWork, sizeof(PathWork), "%s", argv[i + 1]);
            if (PathWork[strlen(PathWork) - 1] == '/')
                PathWork[strlen(PathWork) - 1] = '\0';
        }
        if (cmd == "-C")
        {
            FileCount = atol(argv[i + 1]);
        }
        if (cmd == "-N")
        {
            FileName = argv[i + 1];
        }
        if (cmd == "-n")
        {
            TotalNumber = atol(argv[i + 1]);
        }
    }

    ofstream outfile;
    snprintf(ShellCommand, sizeof(ShellCommand), "%s/FA_Count", PathWork);
    outfile.open(ShellCommand, ios::out|ios::app);
    outfile<<FileName<<endl;
    outfile.close();



    return 0;
}