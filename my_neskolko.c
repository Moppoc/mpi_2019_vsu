#include "mpi.h"
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
	int done = 0, n, myid, numprocs, i;

	double EXP=(exp(1.0)-1/exp(1.0))/2;
	double ryad, sum, promej, prom;
	double startwtime = 0.0, endwtime;
	int namelen;
	int from, to;
	char processor_name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Get_processor_name(processor_name, &namelen);
	MPI_Status status;


	fprintf(stdout, "Process %d of %d is on %s\n",
	myid, numprocs, processor_name);
	fflush(stdout);


	while (!done) {
		if (myid == 0) {
			fprintf(stdout, "Enter the number of intervals: (0 quits) ");
			fflush(stdout);
			if (scanf("%d", &n) != 1) {
				fprintf(stdout, "No number entered; quitting\n"); fflush(stdout);
				n = 0;
			}
			startwtime = MPI_Wtime();
		}
		MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
		if (n == 0)
			done = 1;
		else {
			sum = 0.0;
                        if(myid==0)
                          from= numprocs;
                         else from=myid-1;
                         if(myid== numprocs-1)
                             to=0;
                         else to=myid+1;
			for (i = myid ; i <= n; i += numprocs) {
				printf("%d %d %d\n", myid, n, i); fflush(stdout);
				if (i == 0)
				{
					prom = 1;

					sum = 1;
					MPI_Send(&prom, 1, MPI_DOUBLE, to, 0, MPI_COMM_WORLD);
					printf("send %d <- %d %f\n", 2, myid, prom); fflush(stdout);

				}
				else
				{
                                        MPI_Recv(&prom, 1, MPI_DOUBLE, from, 0, MPI_COMM_WORLD, &status);
					prom *= 2*i*2*(i+1);
					sum +=1/prom;
                                        if(i<n) 
					{
                                          MPI_Send(&prom, 1, MPI_DOUBLE, to, 0, MPI_COMM_WORLD);
					  printf("send %d <- %d %f\n", 3, myid, prom); fflush(stdout);
                                        }
				}
				

					if (i < (n - 1))
					{
						printf("send %d <- %d %f\n", to, myid, prom); fflush(stdout);
						MPI_Send(&prom, 1, MPI_DOUBLE, to, 0, MPI_COMM_WORLD);
					}
				}
			}
			MPI_Reduce(&sum, &ryad, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

			if (myid == 0) {
				printf("ryad is approximately %.16f, Error is %.16f\n",
					ryad, fabs(ryad - EXP));
				endwtime = MPI_Wtime();
				printf("wall clock time = %f\n", endwtime - startwtime);
				fflush(stdout);
			}
		}
	}
	MPI_Finalize();
	return 0;
}
