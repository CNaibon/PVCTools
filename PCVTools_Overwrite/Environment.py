import os,string


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
