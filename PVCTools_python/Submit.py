import os


def submit(*argv):

    workpath = ''
    queuw = ''
    span = ''
    samtools_path = ''
    bcftools_path = ''
    gatk_path = ''
    freebayes_path = ''
    tools = 'samtools'

    chrname = []
    samplename = []

    for i in argv:
        if argv[i] == '-w':
            workpath = argv[i + 1]
        if argv[i] == '-q':
            queuw = argv[i + 1]
        if argv[i] == '-span':
            span = argv[i + 1]
        if argv[i] == '-T':
            tools = argv[i + 1]

    file_list = open(workpath + '/bamlist', 'r')
    file_line = file_list.readline()
    while file_line != '':
        if len(file_line) != 0:
            (shotname, extension) = os.path.splitext(file_line)
            samplename.append(shotname)
        file_line = file_list.readline()
    file_list.close()

    file_list = open(workpath + '/falist', 'r')
    file_line = file_list.readline()
    while file_line != '':
        if len(file_line) != 0:
            (shotname, extension) = os.path.splitext(file_line)
            chrname.append(shotname)
        file_line = file_list.readline()
    file_list.close()

    file_number = []

    for i in range(len(chrname)):
        file_number[i] = int(os.popen("ls -l %s/fa/%s |grep \"^-\"|wc -l" % (workpath, chrname[i])).read())

    os.mkdir("%s/out" % workpath)
    os.mkdir("%s/err" % workpath)
    os.mkdir("%s/sub_script" % workpath)
    os.mkdir("%s/vcf" % workpath)

    for i in range(len(chrname)):
        os.mkdir("%s/vcf/%s" % (workpath, chrname[i]))
        for j in range(file_number[i]):
            file_scirpt = open("%s/sub_script/%s_%d_%s.sh" % (workpath, chrname[i], j, tools), 'w')
            file_scirpt.write("#BSUB -q %s\n" % Queue)
            file_scirpt.write("#BSUB -J %s_%d_%s" % (chrname[i], j, tools))
            file_scirpt.write("#BSUB -o %s/out/%s_%d_%s.out" % (workpath, chrname[i], j, tools))
            file_scirpt.write("#BSUB -o %s/err/%s_%d_%s.out" % (workpath, chrname[i], j, tools))
            file_scirpt.write("#BSUB -n 1")
            file_scirpt.write("#BSUB -R \"span[ptile=%s]\"" % span)

            if tools == 'samtools':
                file_scirpt.write("%s mpileup -u -t DP,AD,ADF -f " % samtools_path)
                file_scirpt.write("%s/fa/%s/%s_%d.fa " % (workpath, chrname[i], chrname[i], j))
                for n in range(len(samplename)):
                    os.mkdir("%s/sample/%s/%s_%s" % (workpath, samplename[n], samplename[n], chrname[i]))
                    if not os.path.exists("%s/sample/%s/%s_%s" % (workpath, samplename[n], samplename[n], chrname[i])):
                        os.mknod("%s/sample/%s/%s_%s" % (workpath, samplename[n], samplename[n], chrname[i]))
                    file_scirpt.write("%s/sample/%s/%s_%s/%s_%s_%d.bam " % (workpath, samplename[n], samplename[n],
                                                                            chrname[i], samplename[n], chrname[i], j))
                file_scirpt.write("| %s call -vmO v -o %s/vcf/%s/%s_%d.var.flt.vcf" % (bcftools_path, workpath,
                                                                                       chrname[i], chrname[i], j))
            elif tools == 'gatk':
                file_scirpt.write("java -jar %s -T HaplotypeCaller " % gatk_path)
                file_scirpt.write("-R %s/fa/%s/%s_%d.fa -nct 1 " % (workpath, chrname[i], chrname[i], j))
                for n in range(len(samplename)):
                    os.mkdir("%s/sample/%s/%s_%s" % (workpath, samplename[n], samplename[n], chrname[i]))
                    if not os.path.exists("%s/sample/%s/%s_%s" % (workpath, samplename[n], samplename[n], chrname[i])):
                        os.mknod("%s/sample/%s/%s_%s" % (workpath, samplename[n], samplename[n], chrname[i]))
                    file_scirpt.write("-I %s/sample/%s/%s_%s/%s_%s_%d.bam " % (workpath, samplename[n], samplename[n],
                                                                               chrname[i], samplename[n], chrname[i], j)
                                      )
                file_scirpt.write("-o %s/vcf/%s/%s_%d.vcf " % (workpath, chrname[i], chrname[i], j))

            elif tools == 'freebayes':
                file_scirpt.write("/usr/bin/time -f \"%E\" %s --strict-vcf " % freebayes_path)
                file_scirpt.write("-f %s/fa/%s/%s_%d.fa " % (workpath, chrname[i], chrname[i], j))
                for n in range(len(samplename)):
                    os.mkdir("%s/sample/%s/%s_%s" % (workpath, samplename[n], samplename[n], chrname[i]))
                    if not os.path.exists("%s/sample/%s/%s_%s" % (workpath, samplename[n], samplename[n], chrname[i])):
                        os.mknod("%s/sample/%s/%s_%s" % (workpath, samplename[n], samplename[n], chrname[i]))
                    file_scirpt.write("-b %s/sample/%s/%s_%s/%s_%s_%d.bam " % (workpath, samplename[n], samplename[n],
                                                                               chrname[i], samplename[n], chrname[i], j)
                                      )
                file_scirpt.write("-v %s/vcf/%s/%s_%d.vcf " % (workpath, chrname[i], chrname[i], j))

            os.system("bsub < %s/sub_script/%s_%d_%s.sh" % (workpath, chrname[i], j, tools))
