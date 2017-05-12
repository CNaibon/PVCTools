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

    for i in range(len(chrname)):
        file_small = open("%s/sub_script/smallFA/%s_%s.sh" % (workpath, chrname[i], tools), 'w')
        file_small.write("#BSUB -q %s\n" % queue)
        file_small.write("#BSUB -J %s_%s\n" % (chrname[i], tools))
        file_small.write("#BSUB -o %s/out/smallFA/%s_%s.out\n" % (workpath, chrname[i], tools))
        file_small.write("#BSUB -e %s/err/smallFA/%s_%s.out\n" % (workpath, chrname[i], tools))
        file_small.write("#BSUB -n 1\n")
        file_small.write("#BSUB -R \"span[ptile=%s]\"\n" % span)
        if tools == 'smatools':
            file_small.write("%s mpileup -u -t DP,AD,ADF -f " % samtools_path)
            file_small.write("%s/fa/%s.fa " % (workpath, chrname[i]))
            for j in range(len(samplename)):
                os.system("touch %s/sample/%s/%s_%s.bam" % (workpath, samplename[j], samplename[j], chrname[i]))
                file_small.write("%s/sample/%s/%s_%s.bam " % (workpath, samplename[j], samplename[j], chrname[i]))
            file_small.write("| %s call -vmO v -o %s/vcf/Final_Result/%s.var.flt.vcf " % (bcftools_path, workpath,
                                                                                          chrname[i]))
        elif tools == 'gatk':
            file_small.write("java -jar %s -T HaplotypeCaller " % gatk_path)
            file_small.write("-R %s/fa/%s.fa -nct 1 " % (workpath, chrname[i]))
            for j in range(len(samplename)):
                os.system("touch %s/sample/%s/%s_%s.bam" % (workpath, samplename[j], samplename[j], chrname[i]))
                file_small.write("-I %s/sample/%s/%s_%s.bam " % (workpath, samplename[j], samplename[j], chrname[i]))
            file_small.write("-o %s/vcf/Final_Result/%s.vcf" % (workpath, chrname[i]))
        elif tools == 'freebayes':
            file_small.write("/usr/bin/time -f \"%%E\" %s --strict-vcf " % freebayes_path)
            file_small.write("-f %s/fa/%s.fa " % (workpath, chrname[i]))
            for j in range(len(samplename)):
                os.system("touch %s/sample/%s/%s_%s.bam" % (workpath, samplename[j], samplename[j], chrname[i]))
                file_small.write("-b %s/sample/%s/%s_%s.bam " % (workpath, samplename[j], samplename[j], chrname[i]))
            file_small.write("-v %s/vcf/Final_Result/%s.vcf" % ( workpath, chrname[i]))
        file_small.close()
        os.system("bsub < %s/sub_script/smallFA/%s_%s.sh" % (workpath, chrname[i], tools))

