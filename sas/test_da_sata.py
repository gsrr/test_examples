import os
import sys
import traceback


def test_temp_17(path, off):
    data = ""
    with open(path, "r") as fr:
        data = fr.read()

    if ord(data[off + 2]) != 0x02:
        return False
    return True

def test_temp_18(path, off):
    data = ""
    with open(path, "r") as fr:
        data = fr.read()

    if ord(data[off + 2]) != 0x03:
        return False
    
    return True

def test_temp_19(path, off):
    data = ""
    with open(path, "r") as fr:
        data = fr.read()

    if ord(data[off + 2]) != 0x04:
        return False
    
    return True

def read_data(path, offset, length, tnum):
    try:
        func = getattr(sys.modules[__name__], "test_temp_%d"%tnum)
        if func(path, offset) == False:
            print "Fail", offset, length, tnum
        else:
            print "Successful", "offset=%d, length=%d, template=%d"%(offset, length, tnum)
    except:
        #print traceback.format_exc()
        pass
    '''
    data = ""
    with open(path, "r") as fr:
        data = fr.read()

    if tnum <= 315:
        print length == 4 + (ord(data[offset + 2]) << 8) + ord(data[offset + 3])
        return

    if tnum <= 328:
        print length == 4 + ord(data[offset + 3])
        return
        
    if tnum == 330:
        print length == 8 

    if tnum == 331:
        print length == 32

    if tnum == 332 or tnum == 333:
        print length,  8 + (ord(data[offset + 4]) << 24) + (ord(data[offset + 5]) << 16) + (ord(data[offset + 6])
        << 8 ) + ord(data[offset + 7])

    if tnum == 334:
        print length,  8 + ord(data[offset + 4])
    '''

def read_sas_header(path):
    recs = []
    with open(path, "r") as fr:
        lines = fr.readlines()
        for line in lines:
            if "Record" in line and "Records" not in line:
                recs.append(line.strip().split()[-1])
    
    for i in xrange(0, len(recs), 3):
        read_data(path, int(recs[i]), int(recs[i + 1]), int(recs[i + 2]))
        

def main():
    read_sas_header(sys.argv[1])


if __name__ == "__main__":
    main()
