.PHONY: all clean main run test debug standard
all: run
main.tab.cc: main.y
	bison -o main.tab.cc -v main.y
lex.yy.cc: main.l
	flex -o lex.yy.cc main.l
main: lex.yy.cc main.tab.cc
	g++ $(shell ls *.cpp *.cc) -o ./bin/main.out
run: main
	./bin/main.out
test:main
	for file in $(basename $(shell find test/*.c)); \
	do \
		./bin/main.out <$$file.c >$$file.res; \
	done
clean:
	rm -f *.output *.yy.* *.tab.* *.out test/*.res
standard:
	gcc ./test/test.c -E -m32 -o ./test/main.i
	gcc -O0 -o ./test/main.S -S -m32 -masm=att ./test/main.i
out:
	gcc -o ./test/out ./test/out.s
