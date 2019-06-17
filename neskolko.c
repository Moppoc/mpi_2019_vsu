#include "mpi.h"
#include <stdio.h>
#include <math.h>

double f(double);

double f(int n, double a)
{
	return (pow(a, (2 * n + 1)) / (2 * n + 1));
}


int main(int argc, char *argv[])
{
	int done = 0, n, myid, numprocs, i;
	double desire_res = 0.5*(exp(1) + 1 / exp(1));
	//double desire_res = 2;
	double h, sum, x = 0.7, myres, last = 0.0, val, last2;
	double startwtime = 0.0, endwtime;
	int  namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc, &argv); // вызывается перед всеми MPI функциями
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs); // кол-во процессов или номер
	MPI_Comm_rank(MPI_COMM_WORLD, &myid); // возвращает ранг процесса
	MPI_Get_processor_name(processor_name, &namelen);
	MPI_Status status;

	while (!done) {
		if (myid == 0) {
			fprintf(stdout, "Enter the number of intervals: (0 quits) ");
			fflush(stdout);
			if (scanf("%d", &n) != 1) {
				fprintf(stdout, "No number entered; quitting\n");
				//n = 0;
				//if (n < 1)
				//break;
			}
			startwtime = MPI_Wtime();
		}

		MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD); // рассылка всем остальным процессам от 0 (данного) результата
		if (n == 0)
			done = 1;
		else {
			int prev_i = (myid + numprocs - 2) % numprocs;
			int next_i = (myid + 2) % (numprocs);
			sum = 0.0;
			last = 0.0;
			x = 0.7;
			for (i = myid; i < n; i += numprocs)
			{
				if (i == 0) {
					sum = last = f(0, x);

				}
				else if (i == 1) {
					sum += f(1, x);
					last += f(1, x);

				}
				else {
					MPI_Recv(&last, 1, MPI_DOUBLE, prev_i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					last = last*x*x*x*x;
					sum += (last / (2 * i + 1));
				}
				if (i < n - 2) {
					MPI_Send(&last, 1, MPI_DOUBLE, next_i, 0, MPI_COMM_WORLD);
				}
			}
		}


		val = sum;
		MPI_Reduce(&val, &myres, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

		if (myid == 0) {
			printf("sum is approximately %.16f, Error is %.16f\n",
				myres, fabs(myres - 0.8673005276940531));
			endwtime = MPI_Wtime();
			printf("wall clock time = %f\n", endwtime - startwtime);
			fflush(stdout);
		}
	}

	MPI_Finalize();
	return 0;
}
