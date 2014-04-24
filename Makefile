SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .cpp .o

BUILDDIR=build

all: programm

programm: $(BUILDDIR)
	g++ obsi.cpp -o $(BUILDDIR)/obsi -ggdb -g3

$(BUILDDIR):
	mkdir $@

clean:
	rm -rf $(BUILDDIR)/*

