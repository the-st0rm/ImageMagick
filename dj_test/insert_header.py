#!/usr/bin/env python

import sys
import os 

def montage():
	options = [
		"-adjoin",
		"-affine matrix",
		"-alpha option",
		"-alpha-color color",
		"-authenticate password",
		"-blue-primary point",
		"-bordercolor color",
		"-caption string",
		"-colors value",
		"-colorspace type",
		"-comment string",
		"-compose operator",
		"-compress type",
		"-define format:option",
		"-density geometry",
		"-depth value",
		"-display server",
		"-dispose method",
		"-dither method",
		"-draw string",
		"-encoding type",
		"-endian type",
		"-extract geometry",
		"-fill color",
		"-filter type",
		"-font name",
		"-format string",
		"-gamma value",
		"-geometry geometry",
		"-gravity direction",
		"-green-primary point",
		"-identify",
		"-interlace type",
		"-interpolate method",
		"-kerning value",
		"-label string",
		"-limit type",
		"-matte",
		"-mode type",
		"-monitor",
		"-page geometry",
		"-pointsize value",
		"-quality value",
		"-quantize colorspace",
		"-quiet",
		"-red-primary point",
		"-regard-warnings",
		"-respect-parentheses settings",
		"-sampling-factor geometry",
		"-scenes range",
		"-seed value",
		"-set attribute",
		"-shadow",
		"-size geometry",
		"-stroke color",
		"-support factor",
		"-synchronize",
		"-taint",
		"-texture filename",
		"-thumbnail geometry",
		"-tile geometry",
		"-title string",
		"-transparent-color color",
		"-treedepth value",
		"-trim",
		"-units type",
		"-verbose",
		"-virtual-pixel method",
		"-white-point point",
		"-adaptive-sharpen geometry",
		"-annotate geometry",
		"-auto-orient",
		"-blur geometry",
		"-border geometry",
		"-channel mask",
		"-crop geometry",
		"-extent geometry",
		"-flatten",
		"-flip",
		"-flop",
		"-frame geometry",
		#"-monochrome", #takes too much time in processing and slows down fuzzing like fuck
		"-polaroid angle",
		"-repage geometry",
		"-resize geometry",
		"-rotate degrees",
		"-scale geometry",
		"-strip",
		"-transform",
		"-transpose",
		"-transparent color",
		"-type type",
		"-unsharp geometry",
		"-coalesce",
		"-composite",
		"-clone indexes",
		"-delete indexes",
		"-duplicate count,indexes",
		"-insert index",
		"-reverse",
		"-swap indexes"
		]

	output = [] 
	#max_len = -1
	for opt in options:
		if " " not in opt: # <<< just covering options that doesn't not take arguments
			output.append("montage %s" %(opt))
		#	max_len = max(len(opt), max_len)
	#print max_len
	return output

def convert():
	""" I am only covering options that doesn't take arguments for now """
	options = [
		"-alpha",
		"-antialias",
		"-auto-gamma",
		"-auto-level",
		"-auto-orient",
		"-clamp",
		"-clip",
		"-clut",
		"-contrast",
		"-despeckle",
		"-enhance",
		"-equalize",
		"-fft",
		"-flip",
		"-flop",
		"-identify",
		"-ift",
		"-moments",
		"-monitor",
		"-monochrome",
		"-negate",
		"-normalize",
		"-perceptible",
		"-ping",
		"-render",
		"-separate",
		"-strip",
		"-taint",
		"-transform",
		"-transpose",
		"-transverse",
		"-trim",
		"-unique-colors",
		"-verbose",
		"-view"
		]

	#max_len = -1
	output = []
	for opt in options:
		#print "convert %s" %(opt)
		output.append("convert %s" %(opt))
		#max_len = max(len(opt), max_len)
	#print max_len
	return output

PREDATA_SIZE = 40
MARKER = "-_-"

def main():
	if len(sys.argv) < 3:
		print "Usage %s: <input_folder> <output_folder>" %(sys.argv[0])
		return


	commands = convert()# + montage()
	input_folder = sys.argv[1]
	output_folder = sys.argv[2]

	files = os.listdir(sys.argv[1])

	#bug: an entry might be a directory .. I am assming here I am giving the correct folder which contain only files 
	#don't want to do an extra check .. the testcase is huge already
	
	total = 0
	print len(files)
	print len(commands)
	#return
	
	for entry in files:
		i = 0
		fname = os.path.join(input_folder,entry)
		fd = open(fname,"rb")
		fdata = fd.read()
		fd.close()	
		for command in commands:
			target_args = MARKER + command #"convert -negate"

			padding_size = PREDATA_SIZE-len(target_args)

			predata = target_args.ljust(PREDATA_SIZE, "\x00")

			output_file_name = os.path.join(output_folder,entry+".%d.dj"%(i)+entry[entry.rfind('.'):])
			fd2 = open(output_file_name, 'wb')
			fd2.write(fdata+predata)
			fd2.close()
			i+=1
			total+=1
			if total%50000==0:
				print "Total: ", total
	return 

if __name__ == '__main__':
	main()
