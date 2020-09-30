#include <stdio.h>
#include <mpi.h>
#include <time.h>

// Function to print timestamps
void printTimeStamp(int *timeStmpArr) {
    for (int i=0; i<3; i++) 
    	printf("%d ", timeStmpArr[i]);
    	
    printf("\n");
}

// Function to perform Rule RA4
void rule4 (int *localArr, int *otherArr, int rank) {
    if (localArr[rank] <= otherArr[rank])
        localArr[rank] = 1 + otherArr[rank];
    for(int i=0; i<3; i++)
	    if (otherArr[i] > localArr[i]) 
	        localArr[i] = otherArr[i];
}

int main ( argc, argv )
int argc;
char **argv;
{
    int rank, size;
    int timeStampArr[3]={0};    // Rule RA1
    
    MPI_Request rq[20];         // Status variables
    MPI_Init( &argc, &argv );   // Initialize
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    
    int recvArr[3];
    
    int nextP1 = (rank + 1) % size; // one process next to current process
    int nextP2 = (rank + 2) % size; // two processes next to current process
    
    // if not process 0, do local computation
    if (rank != 0) {
    	timeStampArr[rank] += 1;    // Rule RA2
    	printf("Process: %d doing local computation, Timestamp: ", rank);
    	printTimeStamp(timeStampArr);
    }
    
    if (rank == 0) {
        
        // Sends something to process 1
        timeStampArr[rank] += 1;    // Rule RA2
        printf("Process: %d, Sends to: %d, Timestamp: ", rank, nextP1);
        printTimeStamp(timeStampArr);
        MPI_Isend(timeStampArr, 3, MPI_INT, nextP1, 0, MPI_COMM_WORLD, &rq[rank]);  // Rule RA3
        
        // Do a local computation
        timeStampArr[rank] += 1;    // Rule RA2
    	printf("Process: %d doing local computation, Timestamp: ", rank);
    	printTimeStamp(timeStampArr);
    	
    	// Sends something to process 1
    	timeStampArr[rank] += 1;    // Rule RA2
        printf("Process: %d, Sends to: %d, Timestamp: ", rank, nextP1);
        printTimeStamp(timeStampArr);
        MPI_Isend(timeStampArr, 3, MPI_INT, nextP1, 0, MPI_COMM_WORLD, &rq[rank+10]);   // Rule RA3
        
        // Receives something from process 2
        MPI_Irecv(&recvArr, 3, MPI_INT, nextP2, 0, MPI_COMM_WORLD, &rq[rank]);
        MPI_Wait(&rq[rank], NULL);
        timeStampArr[rank] += 1;    // Rule RA2
        // Rule RA4
        rule4(timeStampArr, recvArr, nextP2);
        	    
        printf("Process: %d, receives from: %d, aaaTimestamp: ", rank, nextP2);
        printTimeStamp(timeStampArr);
        
    } else if (rank == 1) {
        
        // Receives something from process 0
        MPI_Irecv(&recvArr, 3, MPI_INT, nextP2, 0, MPI_COMM_WORLD, &rq[rank]);
        MPI_Wait(&rq[rank], NULL);
        timeStampArr[rank] += 1;    // Rule RA2
        // Rule RA4
        rule4(timeStampArr, recvArr, nextP2);
        	    
        printf("Process: %d, receives from: %d, aaaTimestamp: ", rank, nextP2);
        printTimeStamp(timeStampArr);
        
        // Receives something from process 3
        MPI_Irecv(&recvArr, 3, MPI_INT, nextP1, 1, MPI_COMM_WORLD, &rq[rank+10]);
        MPI_Wait(&rq[rank+10], NULL);
        timeStampArr[rank] += 1;    // Rule RA2
        // Rule RA4
        rule4(timeStampArr, recvArr, nextP1);

        printf("Process: %d, receives from: %d, Timestamp: ", rank, nextP1);
        printTimeStamp(timeStampArr);
        
        // Sends something to process 2
        timeStampArr[rank] += 1;    // Rule RA2
        printf("Process: %d, Sends to: %d, Timestamp: ", rank, nextP1);
        printTimeStamp(timeStampArr);
        MPI_Isend(timeStampArr, 3, MPI_INT, nextP1, 0, MPI_COMM_WORLD, &rq[rank]);  // Rule RA3
        
        // Receives something from process 0
        MPI_Irecv(&recvArr, 3, MPI_INT, nextP2, 0, MPI_COMM_WORLD, &rq[rank+10]);
        MPI_Wait(&rq[rank+10], NULL);
        timeStampArr[rank] += 1;    // Rule RA2
        // Rule RA4
        rule4(timeStampArr, recvArr, nextP2);        
        	    
        printf("Process: %d, receives from: %d, aaaTimestamp: ", rank, nextP2);
        printTimeStamp(timeStampArr);
        
        // Do local computation
        timeStampArr[rank] += 1;    // Rule RA2
    	printf("Process: %d doing local computation, Timestamp: ", rank);
    	printTimeStamp(timeStampArr);
    
    } else if (rank == 2) {
        
        // Sends something to process 1
        timeStampArr[rank] += 1;    // Rule RA2
        printf("Process: %d, Sends to: %d, Timestamp: ", rank, nextP2);
        printTimeStamp(timeStampArr);
        MPI_Isend(timeStampArr, 3, MPI_INT, nextP2, 1, MPI_COMM_WORLD, &rq[rank+10]);   // Rule RA3

        // Sends something to process 0
        timeStampArr[rank] += 1;    // Rule RA2
        printf("Process: %d, Sends to: %d, Timestamp: ", rank, nextP1);
        printTimeStamp(timeStampArr);
        MPI_Isend(timeStampArr, 3, MPI_INT, nextP1, 0, MPI_COMM_WORLD, &rq[rank]);  // Rule RA3
        
        // Do local computation
        timeStampArr[rank] += 1;    // Rule RA2
    	printf("Process: %d doing local computation, Timestamp: ", rank);
    	printTimeStamp(timeStampArr);
    	
    	// Receives something from process 1
        MPI_Irecv(&recvArr, 3, MPI_INT, nextP2, 0, MPI_COMM_WORLD, &rq[rank]);
        MPI_Wait(&rq[rank], NULL);
        timeStampArr[rank] += 1;    // Rule RA2
        // Rule RA4
        rule4(timeStampArr, recvArr, nextP2);
        	    
        printf("Process: %d, receives from: %d, Timestamp: ", rank, 0);
        printTimeStamp(timeStampArr);
    }
    
    MPI_Finalize();
    
    return 0;
}
