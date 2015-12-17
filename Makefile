BINARIES=\
    gob \
    bin/tester \
    bin/hi

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

bin/tester: tester.c
	gcc tester.c -g -o bin/tester

bin/hi: src/hi/main.go
	./build-go.sh

clean:
	rm $(BINARIES)
