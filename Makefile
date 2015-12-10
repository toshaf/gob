BINARIES=\
    gob \
    gobp \
    tester

all: $(BINARIES)

GOBFILES=\
    gob.c \
    gob_symbols.c \
    gob_trace.c

gob: $(GOBFILES)
	gcc $(GOBFILES) -std=c99 -o gob -lbfd

tester: tester.c
	gcc tester.c -o tester

GOBPFILES=\
    gob.cc \
    gob_load_symbols.cc \
    gob_trace.cc \
    main.cc

gobp: $(GOBPFILES)
	g++ $(GOBPFILES) -std=c++11 -o gobp -lbfd

clean:
	rm $(BINARIES)
