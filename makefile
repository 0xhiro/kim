MAKEFLAGS += --silent

test_file = $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))

all_tests = $(wildcard test/*)

sources = $(wildcard c/*.c c/process/*.c c/view/*.c c/buffer/*.c)

all: make_dir ./target/kim

chad: make_dir ./target/kim

debug: make_dir ./target/kim-debug

make_dir:
	mkdir -p target

install:
	make all
	sudo cp ./target/kim /usr/local/bin

uninstall:
	sudo rm /usr/local/bin/kim

install-debug:
	make all
	sudo cp ./target/kim-debug /usr/local/bin

uninstall-debug:
	sudo rm /usr/local/bin/kim-debug
	
help:
	make all
	./target/kim -h

clean: make_dir
	rm target/*

./target/kim: make_dir ./c/include/*.h $(sources)
	# cd rust/view && cargo build --quiet --release
	gcc -w -Os -o $@ $(sources) -L./rust/view/target/release -lkimview
	strip $@


./target/kim-debug: make_dir ./c/include/*.h $(sources)
	cd rust/view && cargo build --quiet
	gcc -w -g -o $@ $(sources) -L./rust/view/target/debug -lkimview

 
run: make_dir ./target/kim 
	make all
	./target/kim $(test_file)

logs:
	cat ./target/kim-logs.txt

tail:
	clear
	tail -F ./target/kim-logs.txt