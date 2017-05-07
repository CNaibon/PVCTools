import os
import shutil


def segmentfa(*argv):
    charname = []
    workpath = ''
    limit = 0
    splitnumber = 0
    for i in argv:
        if argv[i] == '-w':
            workpath = argv[i + 1]
        if argv[i] == '-n':
            splitnumber = int(argv[i + 1])
        if argv[i] == '-lm':
            limit = int(argv[i + 1])

    shellcommand = workpath + '/falist'

    if limit == 0:
        fa_list = open(shellcommand, 'r')
        fa_line = fa_list.readline()
        while fa_line != '':
            if len(fa_line) != 0:
                for i in range(len(fa_line)):
                    if fa_line[-i - 1] == '.':
                        charname.append(fa_line[:i])
                        break
            fa_line = fa_list.readline()
        fa_list.close()
    else:
        fa_list = open(shellcommand, 'w')
        shellcommand = workpath + '/fa'
        for i in os.listdir(shellcommand):
            (shotname, extension) = os.path.splitext(i)
            fa_file = os.path.join(workpath, i)
            if extension == '.fa' and os.path.getsize(fa_file)/1048576 >= limit:
                charname.append(shotname)
        fa_list.close()

    files_number = []

    for i in range(len(charname)):
        fa_file = os.path.join(workpath, charname[i] + '.fa')
        fa_size = int(os.path.getsize(fa_file)/1048576/splitnumber)
        fa_dir = workpath + '/fa/' + charname[i]
        if os.path.exists(fa_dir):
            shutil.rmtree(fa_dir)
        os.mkdir(fa_dir)
        shellcommand = "split -C %dM %s %s/%s_" % (fa_size, fa_file, fa_dir, charname[i])
        os.system(shellcommand)
        files_number[i] = len(os.listdir(fa_dir))
        for n in range(int(files_number[i])):
            oldname = fa_dir + '/' + charname[i] + '_' + chr(ord('a') + int(n / 26)) + chr(ord('a') + n % 26) + '.fa'
            newname = fa_dir + '/' + charname[i] + '_' + str(n) + '.fa'
            os.rename(oldname, newname)
            if n > 0:
                shellcommand = "sed -i \"1i>%s\" %s/%s_%d.fa" % (charname[i], fa_dir, charname[i], n)
                os.system(shellcommand)
