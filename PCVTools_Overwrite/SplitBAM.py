import os

def splitbam(*argv):
    samplename = []
    workpath = ''
    bampath = []
    importbam = 0

    for i in argv:
        if argv[i] == '-w':
            workpath = argv[i + 1]
        if argv[i] == '-bam':
            bampath = argv[i + 1]
        if argv[i] == '-I':
            importbam = 1

    os.mkdir(workpath + '/sample')
    if importbam == 0:
        bamfile = []
        files_bam = open(workpath + '/bamlist', w)
        