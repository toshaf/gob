BINARIES=\
    gob \
    gobc \
    tester

all: $(BINARIES)

GOBFILES=\
    gob.cc \
    gob_load_symbols.cc \
    gob_trace.cc \
    main.cc

GOBHEADERS=\
    gob.hh \
    gob_internal.hh 

gob: $(GOBFILES) $(GOBHEADERS)
	g++ $(GOBFILES) -std=c++11 -o gob -lbfd

GOBCFILES=\
    gob.c \
    gob_symbols.c \
    gob_trace.c

GOBCHEADERS=\
    gob.h

gobc: $(GOBCFILES) $(GOBCHEADERS)
	gcc $(GOBCFILES) -std=c99 -o gobc -lbfd

tester: tester.c
	gcc tester.c -o tester

clean:
	rm $(BINARIES)
