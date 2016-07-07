import re
import sys

class azy:

    def __init__(self):
        pass

    def getdata(self,filepath):
        fileobj = open(filepath)
        filestr = fileobj.read()
        m = re.findall(r"\d+\.\d+\s1\s+\w+\s+\wx\s+d\s8\s\w{2}\s",filestr)
        print m
        return m


if __name__ == '__main__':
    m = azy()
    ren = m.getdata(sys.argv[1])
    print ren
    for i in ren:
        print

