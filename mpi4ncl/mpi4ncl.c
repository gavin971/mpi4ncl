#include <stdio.h>
/*
 * The following are the required NCAR Graphics include files.
 * They should be located in ${NCARG_ROOT}/include
 */
#include <ncarg/hlu/hlu.h>
#include <ncarg/hlu/NresDB.h>
#include <ncarg/ncl/defs.h>
#include <ncarg/ncl/NclDataDefs.h>
#include <ncarg/ncl/NclBuiltInSupport.h>
#include <ncarg/ncl/NclBuiltIns.h>

#include <mpi.h>

/*
 * The most MPI-Implementation use integers for the MPI_Comm type, but not 
 * all. One example is OpenMPI with a special struct
 **/
MPI_Comm getMPI_Comm(int* commid) {
	#ifdef OMPI_MPI_H
		if (*commid == -10001) return(MPI_COMM_WORLD);
		if (*commid == -10002) return(MPI_COMM_SELF);		
	#else
		return(*commid);
	#endif
}

/*
 * This function converts the ncl-datatyp to the mpi-datatyp and returns the size of
 * each element.
 **/
void NCL_type_to_MPI_type(NclBasicDataTypes* ncltype, MPI_Datatype* mpitype, int* size) {
    if (*ncltype == NCL_short) {
        *mpitype = MPI_SHORT;
        *size = sizeof(short);
    } else if (*ncltype == NCL_int) {
        *mpitype = MPI_INT;
        *size = sizeof(int);
    } else if (*ncltype == NCL_long) {
        *mpitype = MPI_LONG;
        *size = sizeof(long);
    } else if (*ncltype == NCL_float) {
        *mpitype = MPI_FLOAT;
        *size = sizeof(float);
    } else if (*ncltype == NCL_double) {
        *mpitype = MPI_DOUBLE;
        *size = sizeof(double);
    } else if (*ncltype == NCL_char) {
        *mpitype = MPI_CHAR;
        *size = sizeof(char);
    } else if (*ncltype == NCL_byte) {
        *mpitype = MPI_BYTE;
        *size = sizeof(byte);
    } else {
        *size = 0;
        *mpitype = MPI_DATATYPE_NULL;
    }  
}

/*
 * Call the C-Function MPI_Init.
 * @args    the complete command line
 **/
NhlErrorTypes MPI_Init_W(void) {
    int result;
    int result_dim[1];
    int argc = 0;
    char* argv[100];
    string* nclargs;

    //get the commandline
    nclargs = (string*) NclGetArgValue(0, 1, NULL, NULL, NULL, NULL, NULL, 2);
    char* args = NrmQuarkToString(*nclargs);

    //split the command line
    char* token = strtok(args," ");
    while (token != NULL) {
        argv[argc] = token;
        argc++;
        token = strtok(NULL, " ");
    }
    char** argumente = argv;

    result_dim[0] = 1;
    result = MPI_Init(&argc, &argumente);
    return(NclReturnValue(&result,1,result_dim,NULL,NCL_int,1));
}

/*
 * Call MPI_Finalize without any arguments
 **/
NhlErrorTypes MPI_Finalize_W(void) {
    int result;
    int result_dim[1];
    result_dim[0] = 1;
    result = MPI_Finalize();
    return(NclReturnValue(&result,1,result_dim,NULL,NCL_int,1));
}

/*
 * Call MPI_Comm_rank
 * @MPI_comm    the communicator
 * @rank        the rank of this process
 **/
NhlErrorTypes MPI_Comm_rank_W(void) {
    int result;
    int result_dim[1];

    //get the MPI_Comm
    int* comm = (int*) NclGetArgValue(0, 2, NULL, NULL, NULL, NULL, NULL, 2);

    //get the rank
    int* rank = (int*) NclGetArgValue(1, 2, NULL, NULL, NULL, NULL, NULL, 1);

    result_dim[0] = 1;
    result = MPI_Comm_rank(getMPI_Comm(comm), rank);
    return(NclReturnValue(&result,1,result_dim,NULL,NCL_int,1));
}

/*
 * Call MPI_Comm_size
 * @MPI_comm    the communicator
 * @size        the size of this communicator
 **/
NhlErrorTypes MPI_Comm_size_W(void) {
    int result;
    int result_dim[1];

    //get the MPI_Comm
    int* comm = (int*) NclGetArgValue(0, 2, NULL, NULL, NULL, NULL, NULL, 2);

    //get the rank
    int* size = (int*) NclGetArgValue(1, 2, NULL, NULL, NULL, NULL, NULL, 1);

    result_dim[0] = 1;
    result = MPI_Comm_size(getMPI_Comm(comm), size);
    return(NclReturnValue(&result,1,result_dim,NULL,NCL_int,1));
}

/*
 * Call MPI_Send
 * @MPI_comm    the communicator
 * @size        the size of this communicator
 **/
NhlErrorTypes MPI_Send_W(void) {
    int result;
    int result_dim[1];

    //get the Message
    int message_dimssizes[NCL_MAX_DIMENSIONS];
    int message_ndims;
    NclBasicDataTypes message_type;
    void* message = NclGetArgValue(0, 4, &message_ndims, message_dimssizes, NULL, NULL, &message_type, 2);

    //get the type
    MPI_Datatype type;
    int size;
    NCL_type_to_MPI_type(&message_type, &type, &size);
    if (size == 0) {
        NhlPError(NhlFATAL,NhlEUNKNOWN,"MPI_Send: please use data type: int, long, float, double, char or byte");
        return(NhlFATAL);        
    }

    //calculate the size
    int count = 1;
    for (int i=0;i<message_ndims;i++) {
        count *= message_dimssizes[i];
    }

    //get the destination
    int* dest = (int*) NclGetArgValue(1, 4, NULL, NULL, NULL, NULL, NULL, 1);

    //get the tag
    int* tag = (int*) NclGetArgValue(2, 4, NULL, NULL, NULL, NULL, NULL, 1);

    //get the MPI_Comm
    int* comm = (int*) NclGetArgValue(3, 4, NULL, NULL, NULL, NULL, NULL, 1);

    result_dim[0] = 1;
    result = MPI_Send(message, count, type, *dest, *tag, getMPI_Comm(comm));
    return(NclReturnValue(&result,1,result_dim,NULL,NCL_int,1));
}

/*
 * Call MPI_Recv
 * @MPI_comm    the communicator
 * @size        the size of this communicator
 **/
NhlErrorTypes MPI_Recv_W(void) {
    int result;
    int result_dim[1];
    result_dim[0] = 1;
    MPI_Status status;

    //get the Message
    int message_dimssizes[NCL_MAX_DIMENSIONS];
    int message_ndims;
    NclBasicDataTypes message_type;
    void* message = NclGetArgValue(0, 4, &message_ndims, message_dimssizes, NULL, NULL, &message_type, 1);

    //calculate the item count
    int count = 1;
    for (int i=0;i<message_ndims;i++) {
        count *= message_dimssizes[i];
    }

    //get the destination
    int* dest = (int*) NclGetArgValue(1, 4, NULL, NULL, NULL, NULL, NULL, 1);

    //get the tag
    int* tag = (int*) NclGetArgValue(2, 4, NULL, NULL, NULL, NULL, NULL, 1);

    //get the MPI_Comm
    int* comm = (int*) NclGetArgValue(3, 4, NULL, NULL, NULL, NULL, NULL, 1);

    //get the type
    MPI_Datatype type;
    int size;
    NCL_type_to_MPI_type(&message_type, &type, &size);
    if (size == 0) {
        NhlPError(NhlFATAL,NhlEUNKNOWN,"MPI_Recv: please use data type: int, long, float, double, char or byte");
        return(NhlFATAL);
    }

    //receive the data
    byte daten[size*count];
    result = MPI_Recv(&daten, count, type, *dest, *tag, getMPI_Comm(comm), &status);
    int received;
    MPI_Get_count(&status, type, &received);
    memcpy(message, daten, received*size);

    return(NclReturnValue(&result,1,result_dim,NULL,NCL_int,1));
}

/*
 * Call MPI_Recv
 * @MPI_comm    the communicator
 * @size        the size of this communicator
 **/
NhlErrorTypes MPI_Barrier_W(void) {
    int result;
    int result_dim[1];
    result_dim[0] = 1;

    //get the MPI_Comm
    int* comm = (int*) NclGetArgValue(0, 1, NULL, NULL, NULL, NULL, NULL, 0);

    //call the MPI_Barrier function
    result = MPI_Barrier(getMPI_Comm(comm));

    return(NclReturnValue(&result,1,result_dim,NULL,NCL_int,1));
}


void Init(void){
    void *args;
    int dimsizes[NCL_MAX_DIMENSIONS];

    //Register MPI_Init function.-----------------------------------
    args = NewArgs(1);
    dimsizes[0] = 1;
    SetArgTemplate(args,0,"string",1,dimsizes);
    NclRegisterFunc(MPI_Init_W,args,"MPI_Init",1);

    //Register MPI_Finalize function.-----------------------------------
    args = NewArgs(0);
    NclRegisterFunc(MPI_Finalize_W,args,"MPI_Finalize",0);

    //Register MPI_Comm_rank function.-----------------------------------
    args = NewArgs(2);
    SetArgTemplate(args,0,"integer",1,dimsizes);
    SetArgTemplate(args,1,"integer",1,dimsizes);
    NclRegisterFunc(MPI_Comm_rank_W,args,"MPI_Comm_rank",2);

    //Register MPI_Comm_size function.-----------------------------------
    args = NewArgs(2);
    SetArgTemplate(args,0,"integer",1,dimsizes);
    SetArgTemplate(args,1,"integer",1,dimsizes);
    NclRegisterFunc(MPI_Comm_size_W,args,"MPI_Comm_size",2);

    //Register MPI_Send function-----------------------------------------
    args = NewArgs(4);
    SetArgTemplate(args,0,NclANY, 0,NclANY);
    SetArgTemplate(args,1,"integer",1,dimsizes);
    SetArgTemplate(args,2,"integer",1,dimsizes);
    SetArgTemplate(args,3,"integer",1,dimsizes);
    NclRegisterFunc(MPI_Send_W,args,"MPI_Send",4);

    //Register MPI_Recv function-----------------------------------------
    args = NewArgs(4);
    SetArgTemplate(args,0,NclANY, 0,NclANY);
    SetArgTemplate(args,1,"integer",1,dimsizes);
    SetArgTemplate(args,2,"integer",1,dimsizes);
    SetArgTemplate(args,3,"integer",1,dimsizes);
    NclRegisterFunc(MPI_Recv_W,args,"MPI_Recv",4);

    //Register MPI_Barrier function--------------------------------------
    args = NewArgs(1);
    SetArgTemplate(args,0,"integer",1,dimsizes);
    NclRegisterFunc(MPI_Barrier_W,args,"MPI_Barrier",1);

}

