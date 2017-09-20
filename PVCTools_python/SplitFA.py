#

import os


def fa_chr(workpath, fa_path):
    file_fa = open(fa_path, 'r')
    fa_dir = workpath + '/fa'
    if not os.path.exists(fa_dir):
        os.mkdir(fa_dir)
    fa_line = file_fa.readline()
    while fa_line != '':
        fa_name = fa_line[1:]
        for i in range(len(fa_name)):
            if fa_name[i] == ' ' or fa_name[i] == '\t' or fa_name[i] == '\n':
                fa_name = fa_dir + fa_name[:i] + '.fa'
                break
        file_name = open(fa_name, 'w')
        file_name.write(fa_line + '\n')
        fa_line = file_fa.readline()
        while fa_line[0] != '>' and fa_line != '':
            file_name.write(fa_line + '\n')
            fa_line = file_fa.readline()
        file_name.close()
    file_fa.close()


def splitfa(*argv):
    workpath = ''
    fapath = ''
    for i in argv:
        if argv[i] == '-w':
            workpath = argv[i + 1]
        if argv[i] == '-f':
            fapath = argv[i + 1]
    file_list = open(workpath + '/falist', 'w')
    file_list.close()
    fa_chr(workpath, fapath)

