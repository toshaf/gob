all: gob gobp tester

gob: gob.c gob_symbols.c gob_trace.c
	gcc -std=c99 gob.c gob_symbols.c gob_trace.c -o gob -lbfd

tester: tester.c
	gcc tester.c -o tester

gobp: gob.cc gob_symbols.cc
	g++ -std=c++11 gob.cc gob_symbols.cc -o gobp -lbfd

clean:
	rm gob gobp tester
