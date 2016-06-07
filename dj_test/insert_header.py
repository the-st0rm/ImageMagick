#!/usr/bin/env python

import sys




PREDATA_SIZE = 40
MARKER = "-_-"

def main():
	print "Main"

	fname = sys.argv[1]
	fd = open(fname,"rb")
	fdata = fd.read()
	fd.close()

	target_args = MARKER + "convert -trim"

	padding_size = PREDATA_SIZE-len(target_args)

	predata = target_args.ljust(PREDATA_SIZE, "\x00")

	fd2 = open(fname+".dj"+fname[fname.rfind('.'):],'wb')
	fd2.write(fdata+predata)
	fd2.close()
	return 

if __name__ == '__main__':
	main()
