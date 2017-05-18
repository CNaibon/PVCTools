//
// Created by liujiajun on 2017/4/21.
//

//////////////////////////////////////////////////////
//Stitching the calculated VCF file.
//Command:	./StitchVCF	<-w WorkPath>	<-n SplitNumber>
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <dirent.h>
#include <omp.h>
#include <time.h>
#include <limits.h>
#include <algorithm>
#include <unistd.h>

using namespace std;
#define FILE_LINE 16383
#define CMD_NUM 2048

void VCF_Modify(char *buffer, long addresses_number)
{
    int count = 0;
    for (int i = 0; i < (int)strlen(buffer); i++)
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
            snprintf(Front, (size_t)(i + 2), "%s", buffer);
            snprintf(Rear, sizeof(Rear), "%s", &buffer[i + 1 + Length_Old]);
            strncat(Front, Number_New, sizeof(Front) - strlen(Front));
            strncat(Front, Rear, sizeof(Front) - strlen(Front));
            //memset(buffer, 0, FILE_LINE);
            snprintf(buffer, FILE_LINE, "%s", Front);
            //memset(Front, 0, FILE_LINE);
            //memset(Rear, 0, FILE_LINE);
            return;
        }
    }
}

int VCF_Link(char *tarfile, char *formfile)
{
    FILE *fp_tag, *fp_frm;
    if((fp_tag=fopen(tarfile,"a+"))==NULL)
        exit(-1);
    if((fp_frm=fopen(formfile,"r"))==NULL)
        exit(-1);

    fseek(fp_tag,0,SEEK_END);

    //Skip the source file to the beginning of the "##" section.
    char *Buffer = NULL;
    size_t Len = 0;

    while (1)
    {
        getline(&Buffer, &Len,fp_frm);
        if(feof(fp_frm))
        {
            printf("There is no need to stitch the content!");
            fclose(fp_frm);
            fclose(fp_tag);
            return 0;
        }
        if(Buffer[0] != '#') break;
    }

    VCF_Modify(Buffer,0);
    fputs(Buffer,fp_tag);
//    printf("buffer = %s\n",Buffer);
    //Write the cache, modify the address, write the stitching file.
    while (getline(&Buffer, &Len, fp_frm) != -1)
    {
        //Restore the address in VCF.
        VCF_Modify(Buffer,0);
        fputs(Buffer,fp_tag);
//        printf("buffer = %s\n",Buffer);
//        free(Buffer);
//        Buffer = NULL;
    }
    if (Buffer) free(Buffer);
    fclose(fp_frm);
    fclose(fp_tag);
    return 0;

}

int main(int argc,char *argv[])
{
    long StartTime = time((time_t*)NULL);
    printf("start time = %ld\n", StartTime);



    printf("VCF results stitching ...\n");
    char Command[CMD_NUM];
    char VCF_File[CMD_NUM];

    snprintf(VCF_File, sizeof(VCF_File), "Chr18.var.flt.vcf");
    snprintf(Command, sizeof(Command), "Chr18_35.var.flt.vcf");

    printf("%s start!\n", Command);

    VCF_Link(VCF_File, Command);

    printf("%s finished!\n", Command);

    printf("%s finished!\n", Command);
    printf("VCF results stitching done.\n");

    long FinishTime = time((time_t*)NULL);
    printf("finish time = %ld\n", FinishTime);
    long RunningTime = FinishTime - StartTime;
    printf("running time = %ld\n", RunningTime);

    return 0;
}