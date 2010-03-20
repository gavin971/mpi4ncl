/* 
 * File:   makeHeader.c
 * Author: rschuste
 *
 * Created on 18. März 2010, 18:14
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/*
 * Create a NCL script with constants from the mpi.h file
 */
int main(int argc, char** argv) {

    //create and open the ncl-script
    FILE* script = fopen("dist/mpi4ncl.ncl", "w");

    //write the constants
	#ifdef OMPI_MPI_H
	    fprintf(script, "MPI_COMM_WORLD = %d \n", -10001);
    	fprintf(script, "MPI_COMM_SELF = %d \n", -10002);
	#else
	    fprintf(script, "MPI_COMM_WORLD = %d \n", MPI_COMM_WORLD);
    	fprintf(script, "MPI_COMM_SELF = %d \n", MPI_COMM_SELF);	
	#endif
	
    //close the file
    fclose(script);
    
    return (EXIT_SUCCESS);
}

