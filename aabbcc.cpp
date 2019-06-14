#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <iostream>
using namespace std;

void main(int argc, char *argv[])
{
	const int m=20;
    int   myid, numprocs, i;
    char a[m];//строка, которую будем передавать
	MPI_Aint s;
	int TAG=0;
	int count;
	MPI_Datatype sect,dub;
	MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    count=2;//кол-во блоков
	
	MPI_Type_extent(MPI_CHAR,&s);  //узнать размер типа MPI_CHAR
/*!!!!
	MPI_Type_vector(count,1,0,MPI_CHAR,&sect);//конструктор одного блока удваивает один символ
  //count-количество блоков, 1- длина блока, MPI_CHAR-тип элемента, sect- название типа

	MPI_Type_hvector(m/2,1,s,sect,&dub);//тот же строитель типов, но длина в байтах
  
	MPI_Type_commit(&dub);  //зафиксировали тип
	
	
   if (myid == 0)
        {char j='a';// заполнение строки
			for( i=0;i<m;i++)
			  {
				  a[i]=j;
				  j++;
			  }
			  cout<<endl;
			
			 cout<<"Process 0->"<<endl;
             	for(i=0;i<m;i++)
						 cout<<a[i];
					cout<<endl;
			
		    MPI_Send(a,1,dub,1,TAG,MPI_COMM_WORLD);//отправили
			
		}
         else
		 {
		        MPI_Recv(a,m,MPI_CHAR,0,TAG,MPI_COMM_WORLD,&status);//получили
				
                cout<<"Process 1->"<<endl;
             	
             	for(i=0;i<m;i++) 
						 cout<<a[i];
				cout<<endl;
		 }
    MPI_Type_free(&dub);                 
    MPI_Finalize();
}
