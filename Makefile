all: compiler stdlib

grammar:
	cd ./grammar && make jank 

test:
	cd ./testing && make test 

compiler:
	cd ./compiler && make all 

stdlib:
	cd ./stdlib 
	make all 
	cd ..

.PHONY: grammar test compiler stdlib

