# Environment 
MKDIR=mkdir
CP=cp
SYSTEM=$(shell uname)

ifeq "$(SYSTEM)" "Darwin"
    CC=openmpicc  
    LDFLAGS=-dynamiclib -flat_namespace -undefined suppress
else
    CC=mpicc 
    LDFLAGS=-shared 
endif

# build
all: build dist dist/makeHeader.exe dist/mpi4ncl.ncl dist/libmpi4ncl.so

build:
	${MKDIR} -p build
	
dist:
	${MKDIR} -p dist
	
dist/makeHeader.exe: build/makeHeader.o
	${CC} build/makeHeader.o -o dist/makeHeader.exe

dist/mpi4ncl.ncl: dist/makeHeader.exe
	dist/makeHeader.exe

dist/libmpi4ncl.so: build/mpi4ncl.o
	${CC} ${LDFLAGS} build/mpi4ncl.o -o dist/libmpi4ncl.so

build/makeHeader.o: makeHeader.c
	${CC} -c makeHeader.c -o build/makeHeader.o

build/mpi4ncl.o: mpi4ncl.c
	${CC} -c -I${NCARG_ROOT}/include -fPIC -std=c99 mpi4ncl.c -o build/mpi4ncl.o

# clean
clean: 
	${RM} -rf ${NCARG_ROOT}/lib/mpi
	${RM} ${NCARG_ROOT}/bin/ncl_Make_MPI_script
	${RM} dist/makeHeader.exe
	${RM} build/makeHeader.o
	${RM} build/mpi4ncl.o
	${RM} -rf ${NCARG_ROOT}/lib/ncarg/nclscripts/mpi

install: all
	${MKDIR} -p ${NCARG_ROOT}/lib/mpi
	${CP} dist/libmpi4ncl.so ${NCARG_ROOT}/lib/mpi/libmpi4ncl.so
	${CP} ncl_Make_MPI_script ${NCARG_ROOT}/bin/ncl_Make_MPI_script
	${MKDIR} -p ${NCARG_ROOT}/lib/ncarg/nclscripts/mpi
	${CP} dist/mpi4ncl.ncl ${NCARG_ROOT}/lib/ncarg/nclscripts/mpi/mpi4ncl.ncl

