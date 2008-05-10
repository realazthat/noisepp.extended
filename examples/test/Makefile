CXX=g++
CXXFLAGS=-Wall -O3 -ffast-math -funroll-loops -I../../noisepp/core -I../../noisepp/threadpp -pthread
LDFLAGS=-lnoise -lrt -pthread

OBJS = main.o

test: ${OBJS}
	${CXX} -o test ${LDFLAGS} ${OBJS}

clean:
	rm -f test main.o

