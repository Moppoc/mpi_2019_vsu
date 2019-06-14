#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
//поиск строки-образца в матрице (7_1)
void main(int argc, char *argv[])
{
	const int m = 5;
	const int n = 4;
	const int k = 3;
	int  myid, numprocs, i, j, jj;
	int buf_matr[n][m];
        int buf_str[m];
	int temp_str[k];
        int res[n];
	int ok;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	if (myid == 0)
	{
		printf("Enter matr\n");
		fflush(stdout);
                for (i = 0; i<n; i++)

			for (j = 0; j<m; j++)

				cin >> buf_matr[i][j];
		for (i = 0; i<n; i++)

		{
			for (j = 0; j<m; j++)
			cout << buf_matr[i, j] << "   ";
			cout << endl;
		}
		printf("Enter temp_str\n");

		fflush(stdout);

		for (i = 0; i<k; i++)
		cin >> temp_str[i];
	}

	MPI_Scatter(buf_matr, m, MPI_INT, buf_str, m, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(temp_str, k, MPI_INT, 0, MPI_COMM_WORLD);
	i = 0;
	ok = 0;
	while (i <= n - k && ok == 0)
	{
		if (buf_str[i] == temp_str[0])
		{

			j = i + 1;

			jj = 1;

			ok = 1;

			while (jj < k && ok == 1)

			{

				if (buf_str[j] == temp_str[jj])

				{
					j++;
                                        jj++;
				}
				else ok = 0;
			}
		}
		i++;
	}
MPI_Gather(&ok, 1, MPI_INT, res, 1, MPI_INT, 0, MPI_COMM_WORLD);


	if (myid == 0)

	{

		ok = 0;

		cout << "result" << endl;

		for (i = 0; i<n; i++)

			if (res[i] == 1)

			{
                                ok = 1;
				cout << i << '  ';
			}
		if (ok == 0)
		cout << "not found";
		cout << endl;
	}

	MPI_Finalize();
}
