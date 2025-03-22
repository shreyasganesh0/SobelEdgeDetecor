#include <mpi.h>
#include <stdio.h>
#include <stdint.h>

int taskid;
int numtasks;
int *disp_arr;
int *sendcounts;
uint8_t **matrix;

void read_img();
void init_arrays();
void send_recv();

int main(int argc, char *argv[]) {

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);

    read_img();
    init_arrays();
    send_recv();

}

void send_recv() {
    
    uint8_t *inp_mat;
    if (taskid != 0) {
        inp_mat = malloc(sendcounts[taskid] * sendcounts[taskid] * sizeof(int));
    }

    MPI_Scatterv(matrix, sendcounts, disp_arr, MPI_UINT8_T, inp_mat, sendcounts[taskid], MPI_UINT8_T, 0, MPI_COMM_WORLD);

    int curr_num_rows = sendcounts[taskid] / 5000;
    uint8_t **res = malloc(curr_num_rows * sizeof(uint8_t*));
    res[0] = malloc(curr_num_rows * 5000 * sizeof(uint8_t));
    uint8_t *ptr = res[0];

    for (int i  = 0; i < curr_num_rows - 2; i++) {

	ptr += 5000;
        res[i] = ptr; 

    }

    for (int i = 1; i < curr_num_rows - 1; i++) {

        for (int j = 0; j < 5000; j++) {

            if (j == 0 || j == 4999) {

                res[i][j] = inp_mat[i * 5000 + j];
                continue;

            }

            int diag = inp_mat[(i - 1) * 5000 + (j - 1)] * -1 + inp_mat[(i + 1) * 5000 + (j + 1)] * 1;
            int Gx =  diag + inp_mat[(i - 1) * 5000 + (j + 1)] + inp_mat[(i) * 5000 + (j - 1)] * -2 + inp_mat[(i) * 5000 + (j + 1)] * 2 + inp_mat[(i + 1) * 5000 + (j - 1)] * -1; 
            int Gy =  diag + inp_mat[(i - 1) * 5000 + j] * -2 + inp_mat[(i - 1) * 5000 + (j + 1)] * -1  + inp_mat[(i + 1) * 5000 + (j - 1)] * 1 + inp_mat[(i + 1) * 5000 + j] * 2; 

            Gx = (Gx >= 0) ? Gx : -Gx;
            Gy = (Gy >= 0) ? Gy : -Gy;

            res[i][j] = Gx + Gy;
        }

    }

    MPI_Gatherv(res, recvcounts[taskid], MPI_UINT8_T, matrix, recvcounts, recv_disp_arr, MPI_UINT8_T, 0, MPI_COMM_WORLD);

    for (int i = 0; i < 5000; i++) {
	    for (int j = 0; j < 5000; j++) {
		printf("%d, ", matrix[i][j]);
	}
	printf("\n");
    }
}

void init_arrays() {
    int disp_jmp = (5000 * 5000) / numtasks;
    int send_per_worker = disp_jmp + 10000;
    int extra_send = (5000 * 5000) % numtasks;
    
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

void read_img() {
    
    int fd = open("input.txt", O_RDWR);
    if (fd == -1) {
	
	printf("Failed open\n");
    }
    matrix = malloc(5000 * sizeof(uint8_t*));
    matrix[0] = malloc(25000000); 
    uint8_t *ptr = matrix[0];

    for (int i = 1; i = 5000; i++) {
	
	ptr += 5000;
	matrix[i] = ptr;
    }

    int pos = 0;
    char *tmpptr = matrix[0];
    while ((num_read = read(fd, matrix[pos], 5000)) > 0) {

	tmpptr = matrix[pos];	
	if (num_read != 5000) {
	    tmpptr += num_read;
            read(fd, tmpptr, 5000 - num_read);
	}
	
	pos++;
    }
}
	
	
