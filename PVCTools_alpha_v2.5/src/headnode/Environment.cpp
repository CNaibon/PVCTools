//
// Created by liujiajun on 2017/4/23.
//

#include "Environment.h"

int PrintEvmt(const char *dir)
{
    char CurrentPath[CMD_NUM];
    snprintf(CurrentPath, sizeof(CurrentPath), "%s", dir);
    int Count = 0;
    for (int j = int(strlen(CurrentPath)) - 1; j > 0; j--)
    {
        if (CurrentPath[j] == '/')
        {
            Count ++;
            if (Count == 2)
            {
                CurrentPath[j] = '\0';
                break;
            }
        }
    }
    if (Count < 2) snprintf(CurrentPath, sizeof(CurrentPath), ".");
    char Config[CMD_NUM];
    snprintf(Config, sizeof(Config), "%s/config", CurrentPath);
    FILE *fp;
    if ((fp = fopen(Config, "r")) == NULL)
        exit(-1);
    char *Buffer = NULL;
    size_t Len = FILE_LINE;
    getline(&Buffer,&Len,fp);
    while (!feof(fp))
    {
        std::cout << Buffer << std::endl;
        getline(&Buffer,&Len,fp);
    }
    fclose(fp);
    printf("\n");
    return 0;
}

int SetToolsPath(const char *dir, const char *order, const char *path)
{
    char CurrentPath[CMD_NUM];
    snprintf(CurrentPath, sizeof(CurrentPath), "%s", dir);
    int Count = 0;
    for (int j = int(strlen(CurrentPath)) - 1; j > 0; j--)
    {
        if (CurrentPath[j] == '/')
        {
            Count ++;
            if (Count == 2)
            {
                CurrentPath[j] = '\0';
                break;
            }
        }
    }
    if (Count < 2) snprintf(CurrentPath, sizeof(CurrentPath), ".");
    char Config[CMD_NUM];
    char Config_tmp[CMD_NUM];
    snprintf(Config, sizeof(Config), "%s/config", CurrentPath);
    snprintf(Config_tmp, sizeof(Config_tmp), "%s/config_tmp", CurrentPath);

    FILE *fp_f, *fp_t;
    if ((fp_f = fopen(Config, "r")) == NULL)
        exit(-1);
    if ((fp_t = fopen(Config_tmp, "w")) == NULL)
        exit(-1);
    std::string cmd = order;
    std::transform(cmd.begin() + 1, cmd.end(), cmd.begin(), toupper);
    cmd.at(cmd.size() - 1) = '\0';
    char Command[CMD_NUM];
    snprintf(Command, sizeof(Command), "<PATH_%s>\t=\t", cmd.c_str());
    char *Buffer = NULL;
    size_t Len = FILE_LINE;
    getline(&Buffer,&Len,fp_f);
    while (!feof(fp_f))
    {
        if(strstr(Buffer, Command) != NULL)
        {
            int count = 0;
            for (int i = 0; i < (int)strlen(Buffer); i++)
            {
                if (Buffer[i] == '\t') count++;
                if (count == 2)
                {
                    strncat(Command, path, strlen(path));
                    snprintf(Buffer, FILE_LINE, "%s\n", Command);
                    break;
                }
            }
        }
        fputs(Buffer, fp_t);
        getline(&Buffer,&Len,fp_f);
    }
    fclose(fp_f);
    fclose(fp_t);
    remove(Config);
    rename(Config_tmp, Config);
    return 0;
}

int GetToolsPath(const char *dir, string &path, const char *order)
{
    char CurrentPath[CMD_NUM];
    snprintf(CurrentPath, sizeof(CurrentPath), "%s", dir);
    int Count = 0;
    for (int j = int(strlen(CurrentPath)) - 1; j > 0; j--)
    {
        if (CurrentPath[j] == '/')
        {
            Count ++;
            if (Count == 3)
            {
                CurrentPath[j] = '\0';
                break;
            }
        }
    }
    if (Count < 2) snprintf(CurrentPath, sizeof(CurrentPath), "..");
    char Config[CMD_NUM];
    snprintf(Config, sizeof(Config), "%s/config", CurrentPath);

    FILE *fp;
    if ((fp = fopen(Config, "r")) == NULL)
    {
        printf("config打开失败\n");
        exit(-1);
    }
    std::string cmd = order;
    std::transform(cmd.begin() + 1, cmd.end(), cmd.begin(), toupper);
    cmd.at(cmd.size() - 1) = '\0';
    char Command[CMD_NUM];
    snprintf(Command, sizeof(Command), "<PATH_%s>\t=\t", cmd.c_str());
    char *Buffer = NULL;
    size_t Len = FILE_LINE;
    getline(&Buffer,&Len,fp);
    while (!feof(fp))
    {
        if(strstr(Buffer, Command) != NULL)
        {
            int count = 0;
            for (int i = 0; i < (int)strlen(Buffer); i++)
            {
                if (Buffer[i] == '\t') count++;
                if (count == 2)
                {
                    if (Buffer[strlen(Buffer)-1] == '\n' || Buffer[strlen(Buffer)-1] == '\r') Buffer[strlen(Buffer)-1] = '\0';
                    path = &Buffer[i+1];
                    fclose(fp);
                    return 0;
                }
            }
        }
        getline(&Buffer,&Len,fp);
    }
    fclose(fp);
    return 0;
}

int SetEvmt(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        std::string cmd = argv[i];
        if (cmd[0] == '-') SetToolsPath(argv[0], argv[i] ,argv[i+1]);
    }
    printf("Now, the environment variable is :\n");
    PrintEvmt(argv[0]);
    return 0;
}