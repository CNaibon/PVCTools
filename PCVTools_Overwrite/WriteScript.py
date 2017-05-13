import os,sys


def writescript(*argv):
    if len(argv) == 1:
        print("""
        SplitFA              - The source gene sequence FA file is split into small portions according to the included chromosomes.
        SegmentFA            - Each FA file is segmented into small portions according to the split number.
        SplitBAM             - The source ratio of the sample BAM file is split into small portions according to the included chromosome.
        SegmentBAM           - Each BAM file is segmented into small portions according to the split number.
        Submit               - Submit and calculate VCF results.
        StitchVCF            - Stitching the calculated VCF file.
        SmallFA              - Calculate the small chromosome FA files.
        GetVCF               - 整合运行步骤.
        Environment          - Setting environment variable.
        """)
    elif len(argv) == 2:
        if argv[1] == "SplitFA":
            print("""
            \t./PVCTools SplitFA <-w WorkPath> <-fa FAPath> [-q Queue] [-cpu CPU] [-span Span]
            \tRequired Parameters:
            \t\t-w               : Working directory path for using to store the generated files.
            \t\t-fa              : The path of the original FA file.
            \tOptional Parameters:
            \t\t-q               : The queue you want to run the task (Default queue: normal).
            \t\t-cpu             : The number of CPUs you want to allocate for running the task (Default value: 1).
            \t\t                   Ideal value: The number of samples.
            \t\t                   BUT considering the CPU resources,this value should be less then the queue CPU maximum.
            \t\t-span            : The maximum number of the CPU used on each node (Default value: 20).
            """)
        elif argv[1] == "SegmentFA":
            print("""
            \t./PVCTools SegmentFA <-w WorkPath> <-n SplitNumber> [-lm Size] [-q Queue] [-cpu CPU] [-span Span]
            \tRequired Parameters:
            \t\t-w               : Working directory path for using to store the generated files.
            \t\t-n               : The number of divisions.
            \tOptional Parameters:
            \t\t-lm              : Minimize the size(MB) of files to be cut(default: Cut all the FA files imported from [falist]).
            \t\t-q               : The queue you want to run the task (Default queue: normal).
            \t\t-cpu             : The number of CPUs you want to allocate for running the task (Default value: 1).
            \t\t                   Ideal value: The number of samples.
            \t\t                   BUT considering the CPU resources,this value should be less then the queue CPU maximum.
            \t\t-span            : The maximum number of the CPU used on each node (Default value: 20).
            \tTips:
            \tYou may need to customize the FA file list that you want to import in [falist].
            \tRunning again will delete the previous data.
            """)
        elif argv[1] == "SplitBAM":
            print("""
            \t./PVCTools SplitBAM <-w WorkPath> <-bam BAMPath> [-I] [-q Queue] [-cpu CPU] [-span Span]
            \tRequired Parameters:
            \t\t-w               : Working directory path for using to store the generated files.
            \t\t-bam             : The path of the original comparison sample BAM files group.
            \tOptional Parameters:
            \t\t-I               : Will import [bamlist](By default, do not import, all of the BAM files in the target path will be calculated).
            \t\t-q               : The queue you want to run the task (Default queue: normal).
            \t\t-cpu             : The number of CPUs you want to allocate for running the task (Default value: 1).
            \t\t                   Ideal value: The number of samples.
            \t\t                   BUT considering the CPU resources,this value should be less then the queue CPU maximum.
            \t\t-span            : The maximum number of the CPU used on each node (Default value: 20).
            """)
        elif argv[1] == "SegmentBAM":
            print("""
            \t./PVCTools SegmentBAM <-w WorkPath> <-n SplitNumber> [-q Queue] [-cpu CPU] [-span Span]
            \tRequired Parameters:
            \t\t-w               : Working directory path for using to store the generated files.
            \t\t-n               : The number of divisions.
            \tOptional Parameters:
            \t\t-q               : The queue you want to run the task (Default queue: normal).
            \t\t-cpu             : The number of CPUs you want to allocate for running the task (Default value: 1).
            \t\t                   Ideal value: The number of samples.
            \t\t                   BUT considering the CPU resources,this value should be less then the queue CPU maximum.
            \t\t-span            : The maximum number of the CPU used on each node (Default value: 20).
            \tTips:
            \tYou may need to customize the FA file list that you want to import in [falist].
            \tRunning again will delete the previous data.
            """)
        elif argv[1] == "Submit":
            print("""
            \t./PVCTools Submit <-w WorkPath> [-T Tools] [-q Queue] [-cpu CPU] [-span Span]
            \tRequired Parameters:
            \t\t-w               : Working directory path for using to store the generated files.
            \tOptional Parameters:
            \t\t-T               : The tool you want to use to run the task (Default tool: samtools).
            \t\t                   Optional tools : [samtools] [gatk] [freebayes].
            \t\t-q               : The queue you want to run the task (Default queue: normal).
            \t\t-cpu             : The number of CPUs you want to allocate for running the task (Default value: 1).
            \t\t                   Ideal value: The number of samples.
            \t\t                   BUT considering the CPU resources,this value should be less then the queue CPU maximum.
            \t\t-span            : The maximum number of the CPU used on each node (Default value: 20).
            \tTips:
            \tYou may need to customize the FA file list that you want to import in [falist].
            """)
        elif argv[1] == "StitchVCF":
            print("""
            \t./PVCTools StitchVCF <-w WorkPath> <-n SplitNumber> [-q Queue] [-cpu CPU] [-span Span]
            \tRequired Parameters:
            \t\t-w               : Working directory path for using to store the generated files.
            \t\t-n               : The number of divisions.
            \tOptional Parameters:
            \t\t-q               : The queue you want to run the task (Default queue: normal).
            \t\t-cpu             : The number of CPUs you want to allocate for running the task (Default value: 1).
            \t\t                   Ideal value: The number of samples.
            \t\t                   BUT considering the CPU resources,this value should be less then the queue CPU maximum.
            \t\t-span            : The maximum number of the CPU used on each node (Default value: 20).
            \tTips:
            \tYou may need to customize the FA file list that you want to import in [falist].
            """)
        elif argv[1] == "GetVCF":
            print("""
            \t./PVCTools GetVCF <-w WorkPath> <-fa FAPath> <-bam BAMPath> <-n SplitNumber> [-lm Size] [-I] [-T Tools] [-q Queue] [-cpu CPU] [-span Span]
            \tRequired Parameters:
            \t\t-w               : Working directory path for using to store the generated files.
            \t\t-fa              : The path of the original FA file.
            \t\t-n               : The number of divisions.
            \t\t-bam             : The path of the original comparison sample BAM files group.
            \tOptional Parameters:
            \t\t-lm              : Minimize the size(MB) of files to be cut(default: Cut all the FA files imported from [falist]).
            \t\t-I               : Will import [bamlist](By default, do not import, all of the BAM files in the target path will be calculated).
            \t\t-T               : The tool you want to use to run the task (Default tool: samtools).
            \t\t                   Optional tools : [samtools] [gatk] [freebayes].
            \t\t-q               : The queue you want to run the task (Default queue: normal).
            \t\t-cpu             : The number of CPUs you want to allocate for running the task (Default value: 1).
            \t\t                   Ideal value: The number of samples.
            \t\t                   BUT considering the CPU resources,this value should be less then the queue CPU maximum.
            \t\t-span            : The maximum number of the CPU used on each node (Default value: 20).
            """)
        elif argv[1] == "Environment":
            print("""
            \t./PVCTools Environment [-samtools PATH] [-bcftools PATH] [-bamUtil PATH]
            \tOptional Parameters:
            \t\t-samtools        : Setting samtools path.
            \t\t-bcftools        : Setting bcftools path.
            \t\t-bamUtil         : Setting bamUtil path.
            """)
        else:
            if argv[1] == "SplitFA" or argv[1] == "SegmentFA" or argv[1] == "SplitBAM" or argv[1] == "SegmentBAM" or\
                 argv[1] == "Submit" or argv[1] == "StitchVCF" or argv[1] == "SmallFA" or argv[1] == "GetVCF":
                queue = 'normal'
                cpu = '1'
                span = '20'
                workpath = ''
                for j in range(len(argv)):
                    if argv[j] == '-w':
                        workpath = argv[j + 1]
                    if argv[j] == 'q':
                        queue = argv[j + 1]
                    if argv[j] == '-cpu':
                        cpu = argv[j + 1]
                    if argv[j] == '-span':
                        span = argv[j + 1]
                file_sh = open("%s/run.sh" % str(sys.path[0]))
                file_sh.write("#BSUB -q %s\n" % queue)
                file_sh.write("#BSUB -J %s\n" % argv[i])
                file_sh.write("#BSUB -o %s/%s.out" % (workpath, argv[1]))
                file_sh.write("#BSUB -e %s/%s.err" % (workpath, argv[1]))
                file_sh.write("#BSUB -n %s\n" % cpu)
                file_sh.write("#BSUB -R \"span[ptile=%s]\"" % span)
                file_sh.write("#BSUB -a openmpi\n\n\n")

                file_sh.write("%s/bin/main.py" % str(sys.path[0]))
                for k in range(len(argv)):
                    file_sh.write("%s" % argv[k + 1])
                file_sh.close()
                os.system("bsub < %s/run.sh" % str(sys.path[0]))
            elif argv[1] == "Environment":
                setevmt(argv + 2)
            else:
                print("""
                Error: Error Parameters.
                Please check again.
                """)
