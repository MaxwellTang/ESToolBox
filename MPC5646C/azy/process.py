#coding=utf8
import re
import sys

def getpid(data,sid):
    pid = []
    n = 0

    while n < len(data):
        try:
            if data[n] == 'Frame' and data[n-2] == sid:
                if len(pid) == 0:
                    pid.append(data[n+4])
                elif pid.index(data[n+4]) == 0:
                    break
            n = n + 1
        except IndexError:
            break
        except ValueError:
            pid.append(data[n+4])

    return pid


def getsid(data):
    sid = []
    n = 0

    while n < len(data):
        try:
            if data[n] == 'Frame':
                if len(sid) == 0:
                    sid.append(data[n-2])
                elif sid.index(data[n-2]) == 0:
                    break
            n = n + 1
        except IndexError:
            break
        except ValueError:
            sid.append(data[n-2])

    return sid

def getdata(data,SID,PID):
    loop_max = 6553500
    n = 0
    timeTx=0.0
    timeRx=0.0
    while n < loop_max:
        n = n + 1
        try:
            if data[n] == PID:
                if data[n-6] == SID:
                    if data[n-3] == 'Rx':
                        timeRx =float(data[n-9])
                        break
                    elif data[n-3] == 'Tx':
                        timeTx = float(data[n-9])
        except IndexError:
            break
    if timeTx == 0:
        return 0
    elif timeRx == 0:
        return -1
    return timeRx - timeTx

def getallpid(data):
    sid = getsid(data)
    pid = [[] for i in range(len(sid))]
    i = 0
    while i < len(sid):
        pid[i].append(getpid(data,sid[i]))
        i = i + 1
    return pid

def getalldata(data):
    sid = getsid(data)
    pid = getallpid(data)
    i = 0
    j = 0
    while i < len(sid):
        print "----------------------"+"CAN ID:"+sid[i] + "---------------------------"
        while j < len(pid[i][0]):
           # print str(pid[i][j]) + '\t' + str(getdata(data,str(sid[i]),str(pid[i][j]))) + '\n'
            print pid[i][0][j] + '\t' + str(getdata(data,sid[i],pid[i][0][j]))
            j = j + 1
        j = 0
        i = i + 1
#初始化分隔符，打开数据文件，依据分隔符对文件中的数据进行分割，读取和存储

p = re.compile('\s')
print "初始化分隔符成功"
files = open('/home/jason/ESToolBox/MPC5646C/azy/data')
print "打开文件成功"
data = p.split(files.read())
print "读取文件成功"


#声明变量
time=[]
pid=[]
sid=[]
loop_max = 6553500


#去除列表中的空元素
n = 0
count = len(data)*0.01*(36.0/52.0)
pcent = 0
print "去除空元素中"
while '' in data:
    data.remove('')
    pcent = n/count;
    sys.stdout.write(str(int(pcent)) + '%' + '\b\b\b\b')
    sys.stdout.flush()
    n = n + 1
print "去除空元素成功"


"""
数据格式:
    |time  |CAN|No.|SID|CAN|FRAME|RorT|DLC|DLC|PID|
    |27.908|CAN|1  |8A |CAN|FRAME|Rx  |8  |8  |16 |
    |28.908|CAN|2  |8A |CAN|FRAME|Tx  |8  |8  |1D |

"""

#处理数据 逻辑主体
getalldata(data)
