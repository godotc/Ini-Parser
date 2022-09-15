cc = g++

asm = bin/inter/*.S

$(shell mkdir -p bin/inter)

run : bin/test
	$(shell bin/test)

test: $(asm)
	$(cc) $(asm) -g -o bin/test

$(asm) : example parser

example : test/main.cc
	$(cc) test/main.cc -g -S -o bin/inter/main.S

parser : src/*.cc src/*.h
	${cc} src/*.cc  -I src -S  -o bin/inter/parser.S

clean:
	rm bin/* -rf