#include <iostream>
#include <time.h>
#include "mpi.h"
//Damascus university Parallel Programming assignment 2018

using namespace std;


void main(int argc, char* argv[]){

	clock_t tStart = clock(); // time counter 
	int p1, //number of distrubitors 
		p2,	//number of pharmacies 
		p3,	//number of clinics 
		p4; // mean time
	int rank, // current task rank in the MPI_COMM_WORLD
		tasks,// number of tasks in MPI_COMM_WORLD
		rc;   // variable used for confirming send & recive operation  
	int mod, source;
	int tag = 9;
	char in, done = 't';
	MPI_Status stat;

	if (argc == 5){

		p1 = atoi(argv[1]);
		p2 = atoi(argv[2]);
		p3 = atoi(argv[3]);

		MPI_Init(&argc, &argv);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		MPI_Comm_size(MPI_COMM_WORLD, &tasks);
		//provider task
		if (rank <p1){

			for (int i = p1; i<tasks; i++){
				mod = i%p1;
				if (rank == mod){
					rc = MPI_Send(&done, 1, MPI_CHAR, i, tag, MPI_COMM_WORLD);

					if (rc == MPI_SUCCESS)
						cout << "Provider id :" << rank << " Sent a Delivery to location :" << i - p1 << endl;
				}
			}
		}
		else{//delivery
			source = rank %p1;
			rc = MPI_Recv(&in, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &stat);
			if (rc == MPI_SUCCESS){
				if (rank <p1 + p2)
					cout << "pharmacy :" << rank - p1 << " Recived a delivery from provider :" << source << endl;
				else{
					cout << "clinic :" << rank - p1 - p2 << " Recived a delivery from provider :" << source << endl;

				}
			}
		}

		MPI_Finalize();

		cout <<"task # "<<rank <<" time of execution is : " << (double)(clock() - tStart) / CLOCKS_PER_SEC;

	}
	else{

		

		cout << "Erorr! bad input ";

	}
}
