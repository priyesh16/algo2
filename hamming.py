#! /usr/bin/env python 
import sys
import itertools

def uniqify(a):
	return list(set(a))

def printbinary(obj):
	if type(obj) is int:
		print bin(obj)

	if type(obj) is list:
		for line in obj:
			print bin(line),
		print "\n" 

	if type(obj) is dict:
		for key, value in obj.iteritems() :
		   for item in value:
				print bin(key), bin(item)
		print "\n" 

	
def readfile(filename):
	""" read file into a list """
	with open(filename) as f:
		everyline = f.readlines()

	""" get the first line and compute no of lines and bits """
	firstline = everyline[0]
	words = firstline.split()
	linecount = words[0]
	bits = int(words[1])
	print linecount, bits

	""" get the rest of the lines and remove "\n"'s and duplicates """
	rawlines  = everyline[1:]
	alllines = map(lambda each:each.replace(" ", ""), rawlines)
	alllines = map(lambda each:each.strip("\n"), alllines)
	print alllines 
	lines = uniqify(alllines)
	lines = [int(member, 2) for member in lines]
	linecount = len(lines)
	print linecount, bits
	printbinary(lines)

	return lines, bits


def hammingpattern(bits):
	""" Create all possible hamming patterns for 1 and 2 bits"""
	max = int(2) ** bits; 
	pattern1 = [];
	pattern2 = [];
	
	for i in range(0, max):
	   if bin(i).count("1") == 1: 
			pattern1.append(i); 
	
	for i in range(0, max):
		if bin(i).count("1") == 2:
			pattern2.append(i); 

	print "--> pattern1"
	printbinary(pattern1)
	print "--> pattern2"
	printbinary(pattern2)
	return pattern1, pattern2

def getpossible(lines, pattern1, pattern2):
	dict1 = {}
	dict2 = {}

	print "--> lines"
	printbinary(lines)
	
	for a,b in list(itertools.product(lines, pattern1)):
	   xor = a ^ b
	   dict1.setdefault(a, []).append(xor)
	
	print "--> xor1"
	printbinary(dict1)
	
	for a,b in list(itertools.product(lines, pattern2)):
	   xor = a ^ b
	   dict2.setdefault(a, []).append(xor)
	
	print "--> xor2"
	printbinary(dict2)
	
	return dict1, dict2;

def weedout(lines, dict1, dict2):
	weeddict1 = {}
	newdict1 = {}
	weeddict2 = {}
	newdict2 = {}

	for key in dict1:
		for item in dict1[key]:
			if item in lines:
				weeddict1.setdefault(key, []).append(item)
	
	""" remove duplicates """
	for key in weeddict1:
		for item in weeddict1[key]:
			for reverseitem in weeddict1[item]:
				if reverseitem == key:
					weeddict1.setdefault(key, []).remove(item)

	for key in dict2:
		for item in dict2[key]:
			if item in lines:
				weeddict2.setdefault(key, []).append(item)
	
	""" remove duplicates """
	for key in weeddict2:
		for item in weeddict2[key]:
			for reverseitem in weeddict2[item]:
				if reverseitem == key:
					weeddict2.setdefault(key, []).remove(item)

	return weeddict1, weeddict2;

	
def nonparticipants(lines, dict1, dict2):

	count = 0

	print "--> xor1"
	printbinary(dict1)
	print "--> xor2"
	printbinary(dict2)

	for edge in lines:
		flag = 0;
		for key in dict1:
			if edge == key:
				flag = 1;
				break;
			else:
				for item in dict1[key]:
					if edge == item:
						flag = 1;
						break;
		if flag == 0:
			count = count + 1;	
	print "--> count"
	print count
	return count

def printtofile(dict1, dict2, lines, count):
	maxvertex = 0;
	i = 0;
	fw = open("pythondata.txt", "w+");
	fw.write("                                        ")

	for key in dict1:
		if key > maxvertex:
			maxvertex = key
		for item in dict1[key]:
			if item > maxvertex:
				maxvertex = item
			tmpstr = "\n" +  str(key) + " " + str(item) + " 1"
			fw.write(tmpstr)
			i = i + 1;

	for key in dict2:
		if key > maxvertex:
			maxvertex = key
		for item in dict2[key]:
			if item > maxvertex:
				maxvertex = item
			tmpstr = "\n" + str(key) + " " + str(item) + " 2"
			fw.write(tmpstr)
			i = i + 1;
	
	print i
	fw.seek(0);
	tmpstr = str(maxvertex) + " " + str(i) + " " +  str(count); 
	fw.write(tmpstr)
	

if __name__ == "__main__":
	argv = sys.argv

	if len(sys.argv) != 2: 
		print "./hamming <filename>" 
		sys.exit(1) 

	filename = sys.argv[1];
	lines, bits = readfile(filename)

	pattern1, pattern2 = hammingpattern(bits)
	dict1, dict2 = getpossible(lines, pattern1, pattern2)
	dict1, dict2 = weedout(lines, dict1, dict2)
	count = nonparticipants(lines, dict1, dict2)
	printtofile(dict1, dict2, lines, count)
