#!/usr/bin/python

import sys, getopt, os

def main(argv):

	inputfile = ''
	try:
		opts, args = getopt.getopt(argv,"hi:",["ifile="])
	except getopt.GetoptError:
		print 'parsefile.py -i <inputfile>'
		sys.exit(2)
	for opt, arg in opts:
		if opt == '-h':
			print 'parsefile.py -i <inputfile>'
			sys.exit()
		elif opt in ("-i", "--ifile"):
			inputfile = arg
	
	if inputfile is '':
		print "No input file defined"
		sys.exit()
	else:
		print 'Parsing file: "', inputfile


	if not os.path.exists('./parsed'):
		os.makedirs('./parsed')

	outputfile = open('./parsed/%s-parsed.log' % (os.path.splitext(os.path.basename(inputfile)))[0], 'w+')
	f = open(inputfile)
	lines = f.readlines()
	outputfile.write("measurement_1;measurement_2;measurement_3;measurement_4;timeslice\n")
	for i,line in enumerate(lines):
		index = line.split(';')[0]
		
		if index is "4":
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

	f.close()
	outputfile.close()


if __name__ == '__main__':
	main(sys.argv[1:])



