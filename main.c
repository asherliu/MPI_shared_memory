//Graph format: Json based format
//Storage format: 
//struct{
//		int: src_ver
//		Arr: [ver_0|ver_1|ver_2|...]
//		Int: num_conn_ver
//	}
/* main.cu */
#include "graph.h"
#include "wtime.h"
#include "scan.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

#include <stdint.h>
#include <sys/stat.h>

#define N 256*256
using namespace std;



int main(int argc, char **argv) {

	int myid, numprocs;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	cout<<"total machine # is "<<numprocs<<endl;
cout<<"rank "<<myid<<endl;
	
	

//	pthread_t thd1;
	std::cout<<"Input format: ./exe \n";

    int sm_rank, sm_size;
    MPI_Comm shmcomm;
    MPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0, MPI_INFO_NULL, &shmcomm);
    MPI_Comm_rank(shmcomm, &sm_rank);
    MPI_Comm_size(shmcomm, &sm_size);
if(sm_rank ==0)
    cout<<"sm size = "<<sm_size<<endl;
cout<<"rank = "<<myid<<" sm rank = "<<sm_rank<<endl;     
//sm_size = 8, means 8 procs in this node share the same memory

    int *   sm_ptr = NULL;
    MPI_Win sm_win;
    MPI_Info win_info;
    MPI_Info_create(&win_info);
    MPI_Info_set(win_info, "alloc_shared_noncontig", "true");


MPI_Aint k = 0;
if(sm_rank==0) k = sm_size;
    MPI_Win_allocate_shared(k*sizeof(int), sizeof(int), win_info, shmcomm, &sm_ptr, &sm_win);
//cout<<"sm_win "<<sm_win<<endl;
//    MPI_Win_allocate_shared (sizeof(int), sizeof(int), win_info, shmcomm, sm_ptr, &sm_win);
    MPI_Info_free(&win_info);

    MPI_Win_lock_all(0 , sm_win);
    MPI_Win_sync(sm_win);
    MPI_Barrier(shmcomm);

    MPI_Aint b_size[sm_size];
    int b_disp[sm_size];
    int *buf[sm_size];
    int i;
        
    int *buffer;


//MPI_Win_shared_query(sm_win,i , &(b_size[i]), &(b_disp[i]), &buffer);
//MPI_Win_shared_query(sm_win, myid , &(b_size[myid]), &(b_disp[myid]), &buffer);
//cout<<"rank "<<myid<<" read value "<<*buffer<<endl;
//*buffer = myid;
//cout<<"rank "<<myid<<" modified value "<<*buffer<<endl;
    
//MPI_Win_unlock_all(sm_win);

///*
//    if(sm_rank!=0){

        MPI_Win_shared_query(sm_win, 0, &(b_size[0]), &(b_disp[0]), &(buf[0]));
        buf[0][sm_rank] = myid;
//    for (i=0; i<sm_size; i++) {
//        MPI_Win_shared_query(sm_win, i, &(b_size[i]), &(b_disp[i]), &(buf[i]));
//printf("rank=%d target=%d bufadress=%d \n", sm_rank, i, buf[i]);
//        if(i==sm_rank) {
//            *buf[i]=sm_rank+100;
//            printf("rank=%d modified *buf[%d]=%d\n",sm_rank, i, *buf[i]);
//        }
//     }
//    } 
//    else
//    printf("rank=%d Master just watches it:)\n",sm_rank);

    MPI_Win_unlock_all(sm_win);

//    if(sm_rank!=0)
    for (i=0; i<sm_size; i++) {
//        if(buf[i]!=NULL)
//            printf("rank=%d target=%d buf=%d b_size=%zu b_disp=%d \n", myid, i, buf[0][i], (size_t)b_size[0], b_disp[0]);
    }
//*/
    MPI_Win_free(&sm_win);
    MPI_Comm_free(&shmcomm);
/*
*/
//if(myid=0){
//    *sm_ptr = 999;
//}

//cout<<"rank "<<myid<<" "<<*sm_ptr<<endl;


	MPI_Finalize();
	return 0;
}
