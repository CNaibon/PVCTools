import os
import Environment


def splitbam(*argv):
    samplename = []
    workpath = ''
    bampath = []
    bamutilpath = Environment.gettoolspath('bamUtil')
    importbam = 0

    for i in argv:
        if argv[i] == '-w':
            workpath = argv[i + 1]
        if argv[i] == '-bam':
            bampath = argv[i + 1]
        if argv[i] == '-I':
            importbam = 1

    os.mkdir(workpath + '/sample')
    shellcommand = workpath + '/bamlist'
    if importbam == 0:
        bam_list = open(shellcommand, 'w')
        for i in os.listdir(bampath):
            (shotname, extension) = os.path.splitext(i)
            if extension == '.bam':
                bam_list.write(str(i) + '\n')
        bam_list.close()
    bam_list = open(shellcommand, 'r')
    bam_line = bam_list.readline()
    while bam_line != '':
        if len(bam_line) != 0:
            (shotname, extension) = os.path.splitext(bam_line)
            samplename.append(shotname)
            os.mkdir(workpath + '/sample/' + shotname)
        bam_line = bam_list.readline()
    bam_list.close()

# 尝试进行并行计算
    for k in range(len(samplename)):
        shellcommand = "%s splitChromosome --in %s/%s.bam --out %s/sample/%s/%s_ --bamout" % \
                       (bamutilpath, bampath, samplename[k], workpath, samplename[k], samplename[k])
        os.system(shellcommand)
