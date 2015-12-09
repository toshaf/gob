all: gob tester

gob: gob.c gob_symbols.c gob_trace.c
	gcc -std=c99 gob.c gob_symbols.c gob_trace.c -o gob -lbfd

tester: tester.c
	gcc tester.c -o tester

clean:
	rm gob tester
