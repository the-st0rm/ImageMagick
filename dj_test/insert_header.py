#!/usr/bin/env python

import sys

PREDATA_SIZE = 0x20

fname = sys.argv[1]
fd = open(fname,"rb")
fdata = fd.read()
fd.close()

target_args = " convert infile.jpg outfile.png"

padding_size = PREDATA_SIZE-len(target_args)

predata = target_args + ('\x00'*padding_size)

fd2 = open(fname+".dj"+fname[fname.rfind('.'):],'wb')
fd2.write(predata + fdata)
fd2.close()

exit()
