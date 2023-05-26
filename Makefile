all: 
	gcc -o prog dataManipulator.c dataFile.c primaryIndex.c secondaryIndex.c
	
run:
	./prog < input.txt
