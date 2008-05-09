CXX=g++
CXXFLAGS=-Wall -O3 -ffast-math -funroll-loops -I../../noisepp/core
LDFLAGS=-lnoise -lrt -lboost_thread

OBJS = main.o

test: ${OBJS}
	${CXX} -o test ${LDFLAGS} ${OBJS}

clean:
	rm -f test main.o

