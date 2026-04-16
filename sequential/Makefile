CFLAGS=-O3 -std=c11 -fPIC -g
CXXFLAGS=-O3 -std=c++17 -fPIC -g
LD=g++


all: mergesort_seq_nocopy


mergesort_seq_nocopy: mergesort_seq_nocopy.o
	$(LD) $(LDFLAGS) mergesort_seq_nocopy.o $(ARCHIVES) -o mergesort_seq_nocopy


bench: mergesort_seq_nocopy
	./queue.sh

clean:
	-rm *.o
	-rm mergesort_seq_nocopy

distclean:
	-rm *.sh.*

.PHONY:  mergesort_seq_nocopy
