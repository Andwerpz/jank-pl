all: compiler stdlib

grammar:
	cd ./grammar && make jank 

test:
	cd ./testing && make test 

compiler:
	cd ./compiler && make all 

stdlib:
	cd ./compiler && make all

install: compiler stdlib

.PHONY: all grammar test compiler stdlib install

