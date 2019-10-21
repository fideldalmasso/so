GCC=gcc
GPP=g++
CFLAGS=  -fshow-column -Wall -O2   
CXXFLAGS=  -fshow-column -Wall -O2   -std=c++14
LDFLAGS=  -s  
OBJS=Release/buddyFS.o Release/ejercicio1.o Release/ejercicio2.o Release/ejercicio3.o 

all: Release buddyFS
	@echo [100%] Built target buddyFS

clean:
	rm -rf ${OBJS} buddyFS

buddyFS: ${OBJS}
	@echo "[ 80%]" Linking executable buddyFS
	@${GPP} ${OBJS} ${LDFLAGS} -o $@

Release:
	mkdir Release

Release/buddyFS.o: buddyFS.c buddyFS.h
	@echo "[  0%]" Building C object Release/buddyFS.o
	@${GCC} ${CFLAGS} -c buddyFS.c -o $@

Release/ejercicio1.o: ejercicio1.c buddyFS.h
	@echo "[ 20%]" Building C object Release/ejercicio1.o
	@${GCC} ${CFLAGS} -c ejercicio1.c -o $@

Release/ejercicio2.o: ejercicio2.c buddyFS.h
	@echo "[ 40%]" Building C object Release/ejercicio2.o
	@${GCC} ${CFLAGS} -c ejercicio2.c -o $@

Release/ejercicio3.o: ejercicio3.c buddyFS.h
	@echo "[ 60%]" Building C object Release/ejercicio3.o
	@${GCC} ${CFLAGS} -c ejercicio3.c -o $@

