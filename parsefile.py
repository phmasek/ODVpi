#!/usr/bin/python
#
# Author: Philip Masek
#
# Usage: python parsefile.py -i [input file]
# Description: Parses data derived from the project ODVpi
# Url: https://www.github.com/Pletron/ODVpi
#

import sys, getopt, os


def parse_data_file(inputfile, outputfile):
	lines = inputfile.readlines()

	for i,line in enumerate(lines):
		index = line.split(';')[0]
		
		if index is "4" and i > 4:
			try:
				first = int(lines[i-3].split(';')[1][6:])-int(lines[i-4].split(';')[1][6:])
				second = int(lines[i-2].split(';')[1][6:])-int(lines[i-3].split(';')[1][6:])
				third = int(lines[i-1].split(';')[1][6:])-int(lines[i-2].split(';')[1][6:])
				fourth = int(lines[i].split(';')[1][6:])-int(lines[i-1].split(';')[1][6:])

				sliceTime = first+second+third+fourth

				outputfile.write("%d;" % (first))
				outputfile.write("%d;" % (second))
				outputfile.write("%d;" % (third))
				outputfile.write("%d;" % (fourth))
				outputfile.write("%d\n" % (sliceTime))

			except ValueError:
				print "Failed to subtract"

	print "Parsed and saved data from %s" % os.path.basename(inputfile.name)


def main(argv):

	inputfile = ''
	try:
		opts, args = getopt.getopt(argv,"hi:",["inputfile="])
	except getopt.GetoptError:
		print 'parsefile.py -i <input (file or folder)>'
		sys.exit(2)
	for opt, arg in opts:
		if opt == '-h':
			print 'parsefile.py -i <input>'
			sys.exit()
		elif opt in ("-i", "--input"):
			inputfile = arg
	


	if inputfile is '':
		print "No input file or folder defined"
		sys.exit()
	else:
		print 'Parsing %s: "' % 'file' if os.path.isfile(inputfile) else 'folder', inputfile


	files = []
	if os.path.isdir(inputfile):
		for file in os.listdir(inputfile):
			files.append(os.path.join(inputfile, file))
	elif os.path.isfile(inputfile):
		files.append(inputfile)
	else:
		print "File does not exist"
		sys.exit()


	if not os.path.exists('./parsed'):
		os.makedirs('./parsed')


	if os.path.exists('./parsed/%s-parsed.log' % (os.path.splitext(os.path.basename(files[0])))[0][2:]):
		outputfile = open('./parsed/%s-parsed.log' % (os.path.splitext(os.path.basename(files[0])))[0][2:], 'a')
	else:
		outputfile = open('./parsed/%s-parsed.log' % (os.path.splitext(os.path.basename(files[0])))[0][2:], 'w+')
		outputfile.write("measurement_1;measurement_2;measurement_3;measurement_4;timeslice\n")
	

	for filepath in files:
		f = open(filepath)
		parse_data_file(f,outputfile)
		f.close()
	
	outputfile.close()


if __name__ == '__main__':
	main(sys.argv[1:])



