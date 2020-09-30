#include <stdio.h>
#include <mpi.h>
#include <time.h>

// Function to print timestamps
void printTimeStamp(int *timeStmpArr) {
    for (int i=0; i<3; i++) 
    	printf("%d ", timeStmpArr[i]);
    	
    printf("\n");
}

// Function to perform Rule RS3
void rule3(int *localArr, int *otherArr) {
    for(int i=0; i<3; i++)
    	if (otherArr[i] > localArr[i]) 
    	    localArr[i] = otherArr[i];
}

int main ( argc, argv )
int argc;
char **argv;
{
    int rank, size;
    int timeStampArr[3]={0};    // Rule RS1
    
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    
    int recvArr[3];
    
    // Do 2 local computation events without sending or receiving anything from other processes
    if (rank == 2) {
    	timeStampArr[rank] += 1;    // Rule RS2
    	printf("Process: %d doing local computation, Timestamp: ", rank);
    	printTimeStamp(timeStampArr);
    	
    	timeStampArr[rank] += 1;    // Rule RS2
    	printf("Process: %d doing local computation, Timestamp: ", rank);
    	printTimeStamp(timeStampArr);
    }
    
    if (rank == 0){
        // Do local computation
        timeStampArr[rank] += 1;    // Rule RS2
    	printf("Process: %d doing local computation, Timestamp: ", rank);
    	printTimeStamp(timeStampArr);
    	
    	// Send something to process 1
        timeStampArr[rank] += 1;    // Rule RS2
        printf("Process: %d, Sends to: %d, Timestamp: ", rank, 1);
        printTimeStamp(timeStampArr);
        MPI_Send(timeStampArr, 3, MPI_INT, 1, 0, MPI_COMM_WORLD);
        // Rule RS3
        MPI_Recv(recvArr, 3, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        rule3(timeStampArr, recvArr);
        printf("Process: %d, Returned from: %d, Returned timestamp: ", rank, 1);
        printTimeStamp(timeStampArr);
        
        // Do a local computation events
        timeStampArr[rank] += 1;    // Rule RS2
    	printf("Process: %d doing local computation, Timestamp: ", rank);
    	printTimeStamp(timeStampArr);
    	
    } else if (rank == 1) {
        MPI_Recv(recvArr, 3, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        timeStampArr[rank] += 1;    // Rule RS2
        // Rule RS3
        rule3(timeStampArr, recvArr);
        printf("Process: %d, receives from: %d, Returns timestamp: ", rank, 0);
        printTimeStamp(timeStampArr);
        // Handshake and sends back timestamp to it's source for tag 1
        MPI_Send(timeStampArr, 3, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }
    
    // Do a local computation events
    timeStampArr[rank] += 1;    // Rule RS2
	printf("Process: %d doing local computation, Timestamp: ", rank);
	printTimeStamp(timeStampArr);
    
    if (rank == 1) {
        // Send something to process 2, tag 0 and 1
        timeStampArr[rank] += 1;    // Rule RS2
        printf("Process: %d, Sends to: %d, Timestamp: ", rank, 2);
        printTimeStamp(timeStampArr);
        MPI_Send(timeStampArr, 3, MPI_INT, 2, 0, MPI_COMM_WORLD);
        // Rule RS3
        MPI_Recv(recvArr, 3, MPI_INT, 2, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        rule3(timeStampArr, recvArr);
        printf("Process: %d, Returned from: %d, Returned timestamp: ", rank, 2);
        printTimeStamp(timeStampArr);
        
        
        // Send something to process 2, tag 2 and 3
        timeStampArr[rank] += 1;    // Rule RS2
        printf("Process: %d, Sends to: %d, Timestamp: ", rank, 2);
        printTimeStamp(timeStampArr);
        MPI_Send(timeStampArr, 3, MPI_INT, 2, 2, MPI_COMM_WORLD);
        // Rule RS3
        MPI_Recv(recvArr, 3, MPI_INT, 2, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        rule3(timeStampArr, recvArr);;
        printf("Process: %d, Returned from: %d, Returned timestamp: ", rank, 2);
        printTimeStamp(timeStampArr);
        
        
        // Send something to process 0, tag 4 and 5
        timeStampArr[rank] += 1;    // Rule RS2
        printf("Process: %d, Sends to: %d, Timestamp: ", rank, 0);
        printTimeStamp(timeStampArr);
        MPI_Send(timeStampArr, 3, MPI_INT, 0, 4, MPI_COMM_WORLD);
        // Rule RS3
        MPI_Recv(recvArr, 3, MPI_INT, 0, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        rule3(timeStampArr, recvArr);;
        printf("Process: %d, Returned from: %d, Returned timestamp: ", rank, 0);
        printTimeStamp(timeStampArr);
        
        
    } else {
        if (rank == 2) {   
            // Receives from process 1, tag 0
            MPI_Recv(recvArr, 3, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            timeStampArr[rank] += 1;    // Rule RS2
            // Rule RS3
            rule3(timeStampArr, recvArr);
            printf("Process: %d, receives from: %d, Returns timestamp: ", rank, 1);
            printTimeStamp(timeStampArr);
            // Handshake and sends back timestamp to it's source for tag 1
            MPI_Send(timeStampArr, 3, MPI_INT, 1, 1, MPI_COMM_WORLD);
            
            // Receives from process 1, tag 2
            MPI_Recv(recvArr, 3, MPI_INT, 1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            timeStampArr[rank] += 1;    // Rule RS2
            // Rule RS3
            rule3(timeStampArr, recvArr);
            printf("Process: %d, receives from: %d, Returns timestamp: ", rank, 1);
            printTimeStamp(timeStampArr);
            // Handshake and sends back timestamp to it's source for tag 3
            MPI_Send(timeStampArr, 3, MPI_INT, 1, 3, MPI_COMM_WORLD);
      
        } else {
            // Receives from process 1, tag 4
            MPI_Recv(recvArr, 3, MPI_INT, 1, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            timeStampArr[rank] += 1;    // Rule RS2
            // Rule RS3
            rule3(timeStampArr, recvArr);
            printf("Process: %d, receives from: %d, Returns timestamp: ", rank, 1);
            printTimeStamp(timeStampArr);
            // Handshake and sends back timestamp to it's source for tag 5
            MPI_Send(timeStampArr, 3, MPI_INT, 1, 5, MPI_COMM_WORLD);
        
        }
    }
    
    if (rank != 1) {
        // Do a local computation events
        timeStampArr[rank] += 1;    // Rule RS2
	    printf("Process: %d doing local computation, Timestamp: ", rank);
	    printTimeStamp(timeStampArr);
	    
	    if (rank == 2) {
	        // Send something to process 0
            timeStampArr[rank] += 1;    // Rule RS2
            printf("Process: %d, Sends to: %d, Timestamp: ", rank, 0);
            printTimeStamp(timeStampArr);
            MPI_Send(timeStampArr, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
            // Rule RS3
            MPI_Recv(recvArr, 3, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            rule3(timeStampArr, recvArr);
            printf("Process: %d, Returned from: %d, Returned timestamp: ", rank, 0);
            printTimeStamp(timeStampArr);
            
	    } else {
	        // Receives from process 1, tag 0
	        MPI_Recv(recvArr, 3, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            timeStampArr[rank] += 1;    // Rule RS2
            // Rule RS3
            rule3(timeStampArr, recvArr);
            printf("Process: %d, receives from: %d, Returns timestamp: ", rank, 1);
            printTimeStamp(timeStampArr);
            // Handshake and sends back timestamp to it's source fpr tag 2 and 3
            MPI_Send(timeStampArr, 3, MPI_INT, 2, 1, MPI_COMM_WORLD);
	    }
    }
    
    MPI_Finalize();
    return 0;
}
