import os
import shutil
import Environment


def modify(buffer,addresses_number):
    if buffer[0] == '@':
        return
    count = 0
    flag =0
    for i in range(len(buffer)):
        if buffer[i] == '\t':
            count += 1
            if count == 3 or count == 7:
                flag = 1
        if flag == 1:
            front = buffer[:i + 1]
            number_str = buffer[i + 1:]
            for k in range(len(number_str)):
                if number_str[k] == '\t':
                    buffer = number_str[k:]
                    number_str = str(int(number_str[:k]) - addresses_number)
                    break
            buffer = front + number_str + buffer
            flag = 0
        if count == 7:
            break


def sam_address_modify(file_name, address_count):
    file_old = open(file_name, 'r')
    file_new = open(file_name + str(os.getpid()), 'w')
    file_line = file_old.readline()
    while file_line != '':
        modify(file_line, address_count)
        file_new.write(file_line)
        file_line = file_old.readline()
    file_old.close()
    file_new.close()
    os.remove(file_name)
    os.rename(file_name + str(os.getpid()), file_name)


def segmentbam(*argv):

    samtoolspath = Environment.gettoolspath('samtools')
    workpath = ''
    chrname = []
    samplename = []

    for i in argv:
        if argv[i] == '-w':
            workpath = argv[i + 1]

    file_list = open(workpath + '/bamlist', 'w')
    file_line = file_list.readline()
    while file_line != '':
        if len(file_line) != 0:
            (shotname, extension) = os.path.splitext(file_line)
            samplename.append(shotname)
        file_line = file_list.readline()
    file_list.close()

    file_list = open(workpath + '/falist', 'w')
    file_line = file_list.readline()
    while file_line != '':
        if len(file_line) != 0:
            (shotname, extension) = os.path.splitext(file_line)
            chrname.append(shotname)
        file_line = file_list.readline()
    file_list.close()

    file_number = []

    for i in range(len(chrname)):
        for j in range(len(samplename)):
            if os.path.exists("%s/sample/%s/%s_%s.bam" % (workpath, samplename[j], samplename[j], chrname[i])):
                os.remove("%s/sample/%s/%s_%s.bam" % (workpath, samplename[j], samplename[j], chrname[i]))

            os.mkdir("%s/sample/%s/%s_%s_TemporarySort" % (samtoolspath, samplename[j], samplename[j], chrname[i]))
            os.system("%s sort -T %s/sample/%s/%s_%s_TemporarySort %s/sample/%s/%s_%s.bam \
            > %s/sample/%s/%s_%s_sorted.bam " % (samtoolspath, workpath, samplename[j], samplename[j], chrname[i],
                                                 workpath, samplename[j], samplename[j], chrname[i],
                                                 workpath, samplename[j], samplename[j], chrname[i],))
            os.system("%s index %s/sample/%s/%s_%s_sorted.bam" % (samtoolspath, workpath, samplename[j], samplename[j],
                                                                  chrname[i]))
            shutil.rmtree("%s/sample/%s/%s_%s_TemporarySort" % (workpath, samplename[j], samplename[j], chrname[i]))

    for i in range(len(chrname)):

        chrcount = [0, ]
        maxlen_perline = int(os.popen("wc -L %s/fa/%s.fa" % (workpath, chrname[i])).read())

        file_number[i] = int(os.popen("ls -l %s/fa/%s |grep \"^-\"|wc -l" % (workpath, chrname[i])).read())

        for j in range(file_number[i]):
            chrcount.append(maxlen_perline * (int(os.popen("wc -l %s/fa/%s/%s_%d.fa" %
                                                           (workpath, chrname[i], chrname[i], j)).read()) - 1))

        for j in range(len(samplename)):
            if not os.path.exists("%s/sample/%s/%s_%s.bam" % (workpath, samplename[j], samplename[j], chrname[i])):
                continue
            if os.path.exists("%s/sample/%s/%s_%s" % (workpath, samplename[j], samplename[j], chrname[i])):
                shutil.rmtree("%s/sample/%s/%s_%s" % (workpath, samplename[j], samplename[j], chrname[i]))
            os.mkdir("%s/sample/%s/%s_%s" % (workpath, samplename[j], samplename[j], chrname[i]))
            for k in range(file_number[i]):
                os.system("%s view -bh %s/sample/%s/%s_%s_sorted.bam %s:%ld-%ld > %s/sample/%s/%s_%s/%s_%s_%d.bam" % (
                    samtoolspath, workpath, samplename[j], samplename[j], chrname[i], chrname[i], chrcount[k] + 1,
                    chrcount[k + 1], workpath, samplename[j], samplename[j], chrname[i],
                    samplename[j], chrname[i], k))

                os.system("%s view -h %s/sample/%s/%s_%s/%s_%s_%d.bam > %s/sample/%s/%s_%s/%s_%s_%d.sam" % (
                    samtoolspath, workpath, samplename[j], samplename[j], chrname[i], samplename[j], chrname[i], k,
                    workpath, samplename[j], samplename[j], chrname[i],
                    samplename[j], chrname[i], k))
                os.remove("%s/sample/%s/%s_%s/%s_%s_%d.bam" % (workpath, samplename[j], samplename[j],
                                                               chrname[i], samplename[j], chrname[i], k))
                if k > 0 :
                    file_sam = "%s/sample/%s/%s_%s/%s_%s_%d.sam" % (workpath, samplename[j], samplename[j],
                                                                    chrname[i], samplename[j], chrname[i], k)
                    sam_address_modify(file_sam, chrcount[k])

                os.system("%s view -b %s/sample/%s/%s_%s/%s_%s_%d.sam > %s/sample/%s/%s_%s/%s_%s_%d.bam" % (
                    samtoolspath, workpath, samplename[j], samplename[j], chrname[i], samplename[j], chrname[i], k,
                    workpath, samplename[j], samplename[j], chrname[i],
                    samplename[j], chrname[i], k))
                os.remove("%s/sample/%s/%s_%s/%s_%s_%d.sam" % (workpath, samplename[j], samplename[j],
                                                               chrname[i], samplename[j], chrname[i], k))

            os.system("%s/sample/%s/%s_%s_sorted.bam" % (workpath, samplename[j], samplename[j], chrname[i]))
            os.system("%s/sample/%s/%s_%s_sorted.bam.bai" % (workpath, samplename[j], samplename[j], chrname[i]))
