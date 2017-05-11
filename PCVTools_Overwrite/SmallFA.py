import os


def smallfa(*argv):
    samtools_path = ''
    bcftools_path = ''
    gatk_path = ''
    freebayes_path = ''

    workpath = ''
    queue = ''
    span = ''
    chrname = []
    samplename = []
    tools = 'samtools'

    for i in argv:
        if argv[i] == '-w':
            workpath = argv[i + 1]
        if argv[i] == '-q':
            queue = argv[i + 1]
        if argv[i] == '-span':
            span = argv[i + 1]
        if argv[i] == '-I':
            tools = argv[i + 1]

    os.system("ls -al %s/fa | grep '^-' | grep '.fa$' | awk '{print $9}' > %s/smalllist_tmp" % (workpath, workpath))
    os.system("sort %s/smalllist_tmp %s/falist %s/falist | uniq -u > %s/smalllist" % (workpath, workpath, workpath,
                                                                                      workpath))
    os.system("rm %s/smalllist_tmp" % workpath)

    file_list = open(workpath + '/bamlist', 'r')
    file_line = file_list.readline()
    while file_line != '':
        if len(file_line) != 0:
            (shotname, extension) = os.path.splitext(file_line)
            samplename.append(shotname)
        file_line = file_list.readline()
    file_list.close()

    file_list = open(workpath + '/smalllist', 'r')
    file_line = file_list.readline()
    while file_line != '':
        if len(file_line) != 0:
            (shotname, extension) = os.path.splitext(file_line)
            chrname.append(shotname)
        file_line = file_list.readline()
    file_list.close()

    os.mkdir("%s/out" % workpath)
    os.mkdir("%s/out/smallFA" % workpath)
    os.mkdir("%s/err" % workpath)
    os.mkdir("%s/err/smallFA" % workpath)
    os.mkdir("%s/sub_script" % workpath)
    os.mkdir("%s/sub_script/smallFA" % workpath)
    os.mkdir("%s/vcf" % workpath)
    os.mkdir("%s/vcf/Final_Result" % workpath)

    
