#!/bin/bash

NCL_ARGS="test.ncl"
WORK_PATH=/home/rschuste/Programm/NetBeansProjects/mpi4ncl

export NCARG_ROOT=/home/rschuste/local/ncarg
export NCL_DEF_LIB_DIR=$NCARG_ROOT/lib/mpi
export PATH=$NCARG_ROOT/bin:$PATH

cd $WORK_PATH
ncl $NCL_ARGS args="\"$0 $*\""
