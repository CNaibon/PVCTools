import os

def vcf_modify(buffer, addresses_number):
    for i in range(len(buffer)):
        if buffer[i] == '\t':
            front = buffer[:i + 1]
            number_str = buffer[i + 1:]
            for k in range(len(number_str)):
                if number_str[k] == '\t':
                    buffer = number_str[k:]
                    number_str = str(int(number_str[:k]) + addresses_number)
                    break
            buffer = front + number_str + buffer
            return
    return

def vcf_link(tarfile, formfile, ChrName, add_count):
    file_tag = open(tarfile, 'a+')
    file_frm = open(formfile, 'r')

    file_line = file_frm.readline()
    while file_line != '':
        if file_line[0] != '#':
            break
        file_line = file_frm.readline()

    while file_line != '':
        vcf_modify(file_line, add_count)
        file_tag.write(file_line)
        file_line = file_frm.readline()

    file_tag.close()
    file_frm.close()
    return

def stichvcf(*argv):
    chrname = []
    workpath = ''
    splitnumber = 0

    for i in argv:
        if argv[i] == '-w':
            workpath = argv[i + 1]
        if argv[i] == '-n':
            splitnumber = argv[i + 1]

    file_list = open(workpath + '/falist', 'w')
    file_line = file_list.readline()
    while file_line != '':
        if len(file_line) != 0:
            (shotname, extension) = os.path.splitext(file_line)
            chrname.append(shotname)
        file_line = file_list.readline()
    file_list.close()

    filenumber = []

    for i in range(len(chrname)):
        fa_read = [0, ]
        filenumber.append(int(int(os.popen("ls -l %s/fa/%s |grep \"^-\"|wc -l" % (workpath, chrname[i])).read())/2))
        max_perline = int(os.popen("wc -L %s/fa/%s.fa" % (workpath, chrname[i])).read())

        for j in range(filenumber[i]):
            fa_read.append((int(os.popen("wc -l %s/fa/%s/%s_%d.fa" % (workpath, chrname[i], chrname[i], j)).read()) - 1)
                           * max_perline)

        os.system("cp -f %s/vcf/%s/%s_0.var.flt.vcf %s/vcf/Final_Result/%s.var.flt.vcf" % (workpath, chrname[i],
                                                                                           chrname[i], workpath,
                                                                                           chrname[i]))

        vcf_file = "%s/vcf/Final_Result/%s.var.flt.vcf" % (workpath, chrname[i])

        for j in range(filenumber[i] - 1):
            command = "%s/vcf/%s/%s_%d.var.flt.vcf" % (workpath, chrname[i], chrname[i], j)
            vcf_link(vcf_file, command, fa_read[j + 1])
