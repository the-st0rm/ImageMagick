#!/usr/bin/env python

import sys

PREDATA_SIZE = 0x40

fname = sys.argv[1]
fd = open(fname,"rb")
fdata = fd.read()
fd.close()

target_args = " convert"

padding_size = PREDATA_SIZE-len(target_args)

predata = target_args.ljust(PREDATA_SIZE-len(target_args), "A")

fd2 = open(fname+".dj"+fname[fname.rfind('.'):],'wb')
fd2.write(predata + fdata)
fd2.close()

exit()
