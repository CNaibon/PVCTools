//
// Created by liujiajun on 2017/4/23.
//

#include "Environment.h"

int PrintEvmt()
{
    char CurrentPath[CMD_NUM];
    getcwd(CurrentPath, sizeof(CurrentPath));
    char Config[CMD_NUM];
    snprintf(Config, sizeof(Config), "%s/config", CurrentPath);

    FILE *fp;
    if ((fp = fopen(Config, "r")) == NULL)
        exit(-1);
    char *Buffer = NULL;
    size_t Len = FILE_LINE;
    while (!feof(fp))
    {
        getline(&Buffer,&Len,fp);
        std::cout << Buffer << std::endl;
    }
    fclose(fp);
    printf("\n");
    return 0;
}

int SetToolsPath(const char *order, const char *path)
{
    char CurrentPath[CMD_NUM];
    getcwd(CurrentPath, sizeof(CurrentPath));
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
    while (!feof(fp_f))
    {
        getline(&Buffer,&Len,fp_f);
        if(strstr(Buffer, Command) != NULL)
        {
            int count = 0;
            for (int i = 0; i < (int)strlen(Buffer); i++)
            {
                if (Buffer[i] == '\t')
                {
                    count++;
                }
                if (count == 2)
                {
                    strncat(Command, path, strlen(path));
                    snprintf(Buffer, FILE_LINE, "%s\n", Command);
                    break;
                }
            }
        }
        fputs(Buffer, fp_t);
    }
    fclose(fp_f);
    fclose(fp_t);
    remove(Config);
    rename(Config_tmp, Config);
    return 0;
}

int GetToolsPath(char *path, const char *order)
{
    char CurrentPath[CMD_NUM];
    getcwd(CurrentPath, sizeof(CurrentPath));
    for(int i = strlen(CurrentPath) - 1; i >= 0; i--)
    {
        if(CurrentPath[i] == '/')
        {
            CurrentPath[i] = '\0';
            break;
        }
    }
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
    while (!feof(fp))
    {
        getline(&Buffer,&Len,fp);
        if(strstr(Buffer, Command) != NULL)
        {
            int count = 0;
            for (int i = 0; i < (int)strlen(Buffer); i++)
            {
                if (Buffer[i] == '\t')
                {
                    count++;
                }
                if (count == 2)
                {
                    snprintf(path, CMD_NUM, "%s", &Buffer[i+1]);
                    if (path[strlen(path)-1] == '\n' || path[strlen(path)-1] == '\r')
                    {
                        path[strlen(path)-1] = '\0';
                    }
                    fclose(fp);
                    return 0;
                }
            }
        }
    }
    fclose(fp);
    return 0;
}

int SetEvmt(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        std::string cmd = argv[i];
        if (cmd[0] == '-')
        {
            SetToolsPath(argv[i] ,argv[i+1]);
        }

    }
    printf("Now, the environment variable is :\n");
    PrintEvmt();
    return 0;
}