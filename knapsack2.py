#! /usr/bin/env python 
import sys
import itertools

kslist = [] 
dynarr = [] 

def readfile(filename):
	global dynarr 

	str = [" ", "\n"]
	replacestr = [""]
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

	print "file read"

	kslist.sort(key=lambda weight:weight[0])
	print "file sorted"

	"""
	print kslist
	"""
	dynarr = [-1 for x in range(size + 1)] 

	return size, items, kslist 

def knapsack(items, size):
	global dynarr

	"""
	print items, size
	print items, size - weight
	print dynarr
	"""

	"""
	print "values", items, size
	"""

	if size <= 0 or items <= 0:
		dynarr[size] = 0;
		return dynarr[size]; 

	value  = kslist[items][0];
	weight = kslist[items][1];

	prev = dynarr[size];
	if prev == -1:
		prev = knapsack(items - 1, size);

	if (size - weight) < 0:
		dynarr[size] = prev;
		return dynarr[size]; 
	
	poss = dynarr[size - weight];
	if poss == -1:
		poss = knapsack(items - 1, size - weight) +  value ;
	else:
		poss = poss + value;

	if (poss > prev):
		dynarr[size] = poss;
		return dynarr[size]; 
	else:
		dynarr[size] = prev;
		return dynarr[size]; 

if __name__ == "__main__":
	sys.setrecursionlimit(99000); 
	argv = sys.argv

	if len(sys.argv) != 2: 
		print "./knapsack.py <filename>" 
		sys.exit(1) 

	filename = sys.argv[1];
	size, items, kslist = readfile(filename)
	answer = knapsack(items, size)
	print answer

