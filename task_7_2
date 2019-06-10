#include "mpi.h"

#include <stdio.h>

#include <stdlib.h>

#include <iostream>

using namespace std;

//поиск строки-образца в матрице

void main(int argc, char *argv[])

{

	const int m=5;

    int  n, myid, numprocs, i,j,jj;

    int *buf_matr;//матрица

    int *buf_str; //одна строка матрицы, с которой будет работать процесс

    int *temp_str; //строка-образец

    int *res;//результаты поиска для всей матрицы

    int *res_i;//результаты поиска для одной строки

    int ok;

    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);

	n=numprocs;

    MPI_Comm_rank(MPI_COMM_WORLD,&myid);

    buf_str=(int*)malloc(m*sizeof(int));

	int k=3;

	temp_str=(int*)malloc(k*sizeof(int));

       res_i==(int*)malloc(m*sizeof(int));

    if (myid == 0)

        {

		// ввод матрицы

			printf("Enter matr\n");

			fflush(stdout);

			buf_matr=(int*)malloc(n*m*sizeof(int));

		    res=(int*)malloc(n*m*sizeof(int));

			for(i=0;i<n*m;i++)

					cin>>buf_matr[i];

			// вывод матрицы 

             	for(i=0;i<n;i++)

				{

					for(j=0;j<m;j++)

						cout<<*(buf_matr+i*m+j)<<"   ";

					cout<<endl;

				}

			// ввод строки-образца	

			printf("Enter temp_str\n");

			fflush(stdout);

			for(i=0;i<k;i++)

					cin>>temp_str[i];

		}

	// рассылка всем процессам по одной строке матрицы

            MPI_Scatter(buf_matr,m,MPI_INT,buf_str,m,MPI_INT,0,MPI_COMM_WORLD);

			// рассылка всем процессам строки-образца

			MPI_Bcast(temp_str,k,MPI_INT,0,MPI_COMM_WORLD);	

           // поиск в строке подстроки ok==1, если нашли  

             for(i=0;i<m;i++)

                  res_i[i]=0;

            i = 0;

            ok = 0;

            while (i <= n - k)

            {

                if (buf_str[i] == temp_str[0])

                {

                    j = i+1;

                    jj = 1;

                    ok=1;

                    while ( jj < k && ok==1)

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

                    res_i[i]=1;

                    i += k;

                    ok = 0;

                }

                else

                i++;

            } 

			// отправка результата

            MPI_Gather(res_i, m,  MPI_INT, res, m, MPI_INT,0, MPI_COMM_WORLD);

			// вывод результата

            if (myid == 0)

			{

				ok=0;

				cout<<"result"<<endl;

				for(i=0;i<n;i++)

                             for(j=0;j<m;j++)



					if(res[m*i+j]==1)

					{

						ok=1;

                                          cout<<i<<'  '<<j<<endl;

					}

					if(ok==0)

						cout<<"not found";

					cout<<endl;		

			}           

    MPI_Finalize();

}
