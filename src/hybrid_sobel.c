#include <mpi.h>
#include <stdio.h>

int taskid;
int numtasks;

int main(int argc, char *argv[]) {

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);

    init_matrices();
    send_recv();

}

void send_recv() {
    
    int *imm_buf;
    if (taskid != 0) {
        imm_buf = malloc(send_per_worker * send_per_worker * sizeof(int));
    }

    MPI_Scatterv(inp_matrix, sendcounts, disp_arr, MPI_INT, imm_buf, send_per_worker,,MPI_INT, 0, MPI_COMM_WORLD);    

    for (int i = 0; i < sendcounts[taskid]; i++) {


   MPI_Gatherv(calc_reorder_buf, send_per_worker, MPI_INT, matrix, recvcount_arr, disp_arr, 0, MPI_COMM_WORLD);

void init_marices() {
    send_per_worker = 5000/numtasks;
    extra_send = 5000 % numtasks;
    
    for (int i = 0; i < numtasks - 1; i++) {

        displ[i] = i * send_per_worker;
        sendcounts[i] = send_per_worker; 

    }

    displ[numtasks - 1] = (numtasks - 1) * send_per_worker;
    sendcounts[numtasks - 1] = (numtasks - 1) * send_per_worker + extra_send;

}

