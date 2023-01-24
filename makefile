MAKEFLAGS += --silent

test_file = $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))

all_tests = $(wildcard test/*)

sources = $(wildcard c/*.c)

all: make_dir ./target/kim

chad: make_dir ./target/kim

make_dir:
	mkdir -p target
	
install:
	make all
	cp ./target/kim /usr/local/bin/kim

help:
	make all
	./target/kim -h

clean: make_dir
	rm target/*

./target/kim: make_dir ./c/include/*.h $(sources)
	gcc -g -o $@ $(sources) -lncurses
 
run: make_dir ./target/kim 
	make all
	./target/kim $(test_file)

tail:
	clear
	tail -F test.txt