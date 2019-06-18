#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

void main(int argc, char* argv[])
{
	const int m = 5;
	const int n = 4;
	const int k = 3;
	int  myid, numprocs, i, j, jj;
	int buf_matr[n][m];
	int buf_str[m];
	int temp_str[k];
	int res[n];
	int res_i[m];
	int max_all, min_all;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	if (myid == 0)
	{
		printf("Enter matr\n");
		fflush(stdout);
		buf_matr[0][0] = 3;
		buf_matr[0][1] = 1;
		buf_matr[0][2] = 1;
		buf_matr[0][3] = 1;
		buf_matr[0][4] = 1;

		buf_matr[1][0] = 1;
		buf_matr[1][1] = 3;
		buf_matr[1][2] = 1;
		buf_matr[1][3] = 1;
		buf_matr[1][4] = 6;

		buf_matr[2][0] = 3;
		buf_matr[2][1] = 1;
		buf_matr[2][2] = 1;
		buf_matr[2][3] = 3;
		buf_matr[2][4] = 2;

		buf_matr[3][0] = 3;
		buf_matr[3][1] = 3;
		buf_matr[3][2] = 1;
		buf_matr[3][3] = 1;
		buf_matr[3][4] = 6;

		for (i = 0; i < n; i++) {
			cout << endl;
			for (j = 0; j < m; j++)
				cout << buf_matr[i][j] << " ";
		}

		cout << endl;

		printf("Enter temp_str\n");

		fflush(stdout);

		for (i = 0; i < k; i++)
			cin >> temp_str[i];
	}

	MPI_Scatter(buf_matr, m, MPI_INT, buf_str, m, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(temp_str, k, MPI_INT, 0, MPI_COMM_WORLD);
	for (i = 0; i < m; i++)
		res_i[i] = 0;
	int max = buf_str[0];
	int min = buf_str[0];
	for (i=0; i<n; i++)
	if (max<buf_str[i])max=buf_str[i]
else 
	if (min > buf_str[i])
		min=buf_str[i];
	MPI_Reduce(&max,&max_all,1, MPI_INT, MPI_MAX,0,MPI_COMM_WORLD)
	MPI_Reduce(&min,&min_all,1,MPI_INT, MPI_MIN,0,MPI_COMM_WORLD)
	
	i = 0;
	ok = 0;
	while (i <= n - k)
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
		if (ok == 1)
		{
			res_i[i] = 1;
			i += k;
			ok = 0;
		}
		else
			i++;
	}
	MPI_Gather(res_i, m, MPI_INT, res, m, MPI_INT, 0, MPI_COMM_WORLD);
	
	if (myid == 0)
		cout << "min_max" << min_all << "-" << max_all << endl;
	{
		ok = 0;
		cout << "result" << endl;
		for (i = 0; i < n; i++)
			for (j = 0; j < m; j++)
				if (res[m * i + j] == 1)
				{
					ok = 1;
					cout << i << '  ' << j << endl;
				}
		if (ok == 0)

			cout << "not found";

		cout << endl;
	}
	MPI_Finalize();
}
