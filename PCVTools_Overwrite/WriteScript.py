import os


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
            \t\t                  Ideal value: The number of samples.
            \t\t                   BUT considering the CPU resources,this value should be less then the queue CPU maximum.
            \t\t-span            : The maximum number of the CPU used on each node (Default value: 20).
            """)

        elif argv[1] == "SegmentFA":
            print("""
            \t/PVCTools SegmentFA <-w WorkPath> <-n SplitNumber> [-lm Size] [-q Queue] [-cpu CPU] [-span Span]
            \tRequired Parameters:
            \t\t-w               : Working directory path for using to store the generated files.
            \tOptional Parameters:
            \t\t-lm              : Minimize the size(MB) of files to be cut(default: Cut all the FA files imported from [falist]).
            \t\t-q               : The queue you want to run the task (Default queue: normal).
            \t\t-cpu             : The number of CPUs you want to allocate for running the task (Default value: 1).
            \t\t                  Ideal value: The number of samples.
            \t\t                   BUT considering the CPU resources,this value should be less then the queue CPU maximum.
            \t\t-span            : The maximum number of the CPU used on each node (Default value: 20).
            \tTips:
            \tYou may need to customize the FA file list that you want to import in [falist].
            \tRunning again will delete the previous data.
            """)
