#! /usr/bin/env python 
import sys
import itertools

def readfile(filename):
	str = [" ", "\n"]
	replacestr = [""]
	kslist = [] 
	""" read file into a list """
	with open(filename) as f:
		everyline = f.readlines()

	""" get the first line and compute no of lines and bits """
	firstline = everyline[0]
	words = firstline.split()
	size = int(words[0])
	items = int(words[1])

	"""
	print size, items 
	"""

	""" get the rest of the lines and remove "\n"'s and duplicates """
	tmplist = [0, 0]
	kslist.append(tmplist);

	for line in everyline[1:]:
		words = line.split()
		tmplist = []
		tmplist.append(int(words[0]));
		tmplist.append(int(words[1]));
		kslist.append(tmplist);


	"""
	print kslist
	"""

	return size, items, kslist 


def knapsack(kslist, size, items):
	dynarr = []
	curlist = []

	for i in range(0, size + 1):
		curlist.append(0);

	dynarr.append(curlist);
	
	for i in range (1, items+1):
		"""
		print i
		"""
		curlist = []
		for x in range(0, size+1):
			prev = dynarr[i - 1][x];
			value  = kslist[i][0];
			weight = kslist[i][1];
			"""
			print prev, value, weight
			"""
			if (x - weight) < 0:
				curlist.append(prev)
			else:
				poss = dynarr[i - 1][x - weight] + value ;
				if (poss > prev):
					curlist.append(poss)
				else:
					curlist.append(prev)
		dynarr.append(curlist)

	"""
	print dynarr
	"""
	print dynarr[items][size]


if __name__ == "__main__":
	argv = sys.argv

	if len(sys.argv) != 2: 
		print "./knapsack.py <filename>" 
		sys.exit(1) 

	filename = sys.argv[1];
	size, items, kslist = readfile(filename)
	knapsack(kslist, size, items)

