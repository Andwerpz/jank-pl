all: compiler stdlib

grammar:
	cd ./grammar && make jank

test:
	cd ./testing && make test

compiler:
	cd ./compiler && make all

stdlib:
	cd ./stdlib && dylan compile

install:
	bash ./scripts/install_all.sh

.PHONY: all grammar test compiler stdlib install
