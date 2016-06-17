import sys


def read_injected_data(file_name):
	f = open(file_name, 'r')
	data = f.read()[-40:]
	f.close()
	return data



def main():
	if len(sys.argv) < 2:
		print "Usage: %s file_name [file_name].." %(sys.argv[0])

	for file_name in sys.argv[1:]:
		print "%s: %s" %(file_name, read_injected_data(file_name))

	print "Main"

if __name__ == '__main__':
	main()
