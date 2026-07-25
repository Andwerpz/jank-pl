all: compiler stdlib

grammar:
	cd ./grammar && make jank

test:
	cd ./testing && make test

compiler:
	cd ./compiler && make all

stdlib:
	cd ./stdlib && make all

install: compiler stdlib
	cd ./compiler && make install

.PHONY: all grammar test compiler stdlib install
