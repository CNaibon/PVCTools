import os


def printevmt():
    file_cfg = open('config', 'r')
    file_line = file_cfg.readline()
    while file_line != '':
        print(file_line)
        file_line = file_cfg.readline()
    file_cfg.close()


def settoolspath(order, path):
    file_cfg = open('config', 'r')
    file_tmp = open('config_tmp', 'w')
    cmd = '<PATH_%s>\t=\t' % order[1:].upper()
    file_line = file_cfg.readline()
    while file_line != '':
        if file_line.find(cmd) != -1:
            file_line = cmd + path
        file_tmp.write(file_line)
        file_line = file_cfg.readline()
    file_cfg.close()
    file_tmp.close()
    os.remove('config')
    os.rename('config_tmp', 'config')


def gettoolspath(order):
    file_cig = open('config', 'r')
    file_tools = "<PATH_%s\t=\t" % order.upper()
    file_line = file_cig.readline()
    path = ''
    while file_line != '':
        if file_line.find(file_tools) != -1:
            count = 0
            for i in range(len(file_line)):
                if file_line[i] == '\t':
                    count = count + 1
                if count == 2:
                    path = file_line[i + 1:]
                    if path[-1] == '\n' or path[-1] == '\r':
                        path = path[:-1]
                    file_cig.close()
                    return path
    file_cig.close()
    return path


def setevmt(*argv):
    for i in range(len(argv)):
        if argv[i][0] == '-':
            settoolspath(argv[i], argv[i + 1])
    print("Now, the enviornment variable is :\n")
    printevmt()
    return


