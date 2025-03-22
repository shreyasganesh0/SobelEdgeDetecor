#include <mpi.h>
#include <stdio.h>

int taskid;
int numtasks;
int *disp_arr;
int *sendcounts;

int main(int argc, char *argv[]) {

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);

    init_matrices();
    send_recv();

}

void send_recv() {
    
    int *inp_mat;
    if (taskid != 0) {
        inp_mat = malloc(sendcounts[taskid] * sendcounts[taskid] * sizeof(int));
    }

    MPI_Scatterv(matrix, sendcounts, disp_arr, MPI_INT, inp_mat, sendcounts[taskid], MPI_INT, 0, MPI_COMM_WORLD);

    int curr_num_rows = sendcounts[taskid] / 5000;
    int **res = malloc(curr_num_rows * sizeof(int*));

    for (int i  = 0; i < curr_num_rows - 2; i++) {

        res[i] = malloc(4998 * sizeof(int));

    }

    for (int i = 1; i < curr_num_rows - 1; i++) {

        for (int j = 0; j < 5000; j++) {

            if (j == 0 || j == 4999) {

                res[i][j] = inp_mat[i][j];
                continue;

            }

            int diag = inp_mat[i - 1][j - 1] * -1 + inp_mat[i + 1][j + 1] * 1;
            int Gx =  diag + inp_mat[i - 1][j + 1] + inp_mat[i][j - 1] * -2 + inp_mat[i][j + 1] * 2 + inp_mat[i + 1][j - 1] * -1; 
            int Gy =  diag + inp_mat[i - 1][j] * -2 + inp_mat[i - 1][j + 1] * -1  + inp_mat[i + 1][j - 1] * 1 + inp_mat[i + 1][j] * 2; 

            Gx = (Gx >= 0) ? Gx : -Gx;
            Gy = (Gy >= 0) ? Gy : -Gy;

            res[i][j] = Gx + Gy;
        }

    }

    MPI_Gatherv(res, recvcounts[taskid], MPI_INT, matrix, recvcounts, recv_disp_arr, MPI_INT, 0, MPI_COMM_WORLD);

void init_marices() {
    int disp_jmp = (5000 * 5000) / numtasks;
    int send_per_worker = disp_jmp + 10000;
    tnt extra_send = (5000 * 5000) % numtasks;
    
    for (int i = 0; i < numtasks - 1; i++) {

        disp_arr[i] = i * disp_jmp; 
        recv_disp_arr[i] = i * disp_jmp + 5000;
        sendcounts[i] = send_per_worker; 
        recvcounts[i] = disp_jmp; 

    }

    disp_arr[numtasks - 1] = (numtasks - 1) * rows_per_worker;
    recv_disp_arr[numtasks - 1] = (numtasks - 1) * disp_jmp + 5000;
    sendcounts[numtasks - 1] = send_per_worker + extra_send;
    recvcounts[numtasks - 1] = disp_jmp + extra_send; 

}

