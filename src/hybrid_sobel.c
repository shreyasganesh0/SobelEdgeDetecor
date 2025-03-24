#include <mpi.h>
#include <omp.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int taskid;
int numtasks;
int *disp_arr;
int *recv_disp_arr;
int *sendcounts;
int *recvcounts;
uint8_t **matrix;

void read_img();
void init_arrays();
void send_recv();

/*   ttype: type to use for representing time */
typedef double ttype;
ttype tdiff(struct timespec a, struct timespec b)
/* Find the time difference. */
{
  ttype dt = (( b.tv_sec - a.tv_sec ) + ( b.tv_nsec - a.tv_nsec ) / 1E9);
  return dt;
}

struct timespec now()
/* Return the current time. */
{
  struct timespec t;
  clock_gettime(CLOCK_REALTIME, &t);
  return t;
}

int main(int argc, char *argv[]) {

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);

    read_img();
    init_arrays();
    send_recv();

}

void send_recv() {
    
    struct timespec begin, end;
    double time_spent;

    uint8_t *inp_mat;
    inp_mat = malloc(sendcounts[taskid] * sizeof(uint8_t));

    int curr_num_rows = sendcounts[taskid] / 5000;
    uint8_t **res = malloc(curr_num_rows * sizeof(uint8_t*));
    res[0] = malloc(sendcounts[taskid] * sizeof(uint8_t));
    uint8_t *ptr = res[0];

    for (int i = 1; i < curr_num_rows - 2; i++) {

        ptr += 5000;
        res[i] = ptr; 

    }
    begin = now();

    MPI_Scatterv(matrix[0], sendcounts, disp_arr, MPI_UINT8_T, inp_mat, sendcounts[taskid], MPI_UINT8_T, 0, MPI_COMM_WORLD);


    #pragma omp parallel for schedule(dynamic, 100)
    for (int i = 1; i < curr_num_rows - 1; i++) {

        for (int j = 0; j < 5000; j++) {

            if (j == 0 || j == 4999) {

                res[i - 1][j] = inp_mat[i * 5000 + j];
                continue;

            }

            int diag = inp_mat[(i - 1) * 5000 + (j - 1)] * -1 + inp_mat[(i + 1) * 5000 + (j + 1)] * 1;
            int Gx =  diag + inp_mat[(i - 1) * 5000 + (j + 1)] + inp_mat[(i) * 5000 + (j - 1)] * -2 + inp_mat[(i) * 5000 + (j + 1)] * 2 + inp_mat[(i + 1) * 5000 + (j - 1)] * -1; 
            int Gy =  diag + inp_mat[(i - 1) * 5000 + j] * -2 + inp_mat[(i - 1) * 5000 + (j + 1)] * -1  + inp_mat[(i + 1) * 5000 + (j - 1)] * 1 + inp_mat[(i + 1) * 5000 + j] * 2; 

            Gx = (Gx >= 0) ? Gx : -Gx;
            Gy = (Gy >= 0) ? Gy : -Gy;

            res[i - 1][j] = Gx + Gy;
        }

    }

    MPI_Gatherv(res[0], recvcounts[taskid], MPI_UINT8_T, matrix[0], recvcounts, recv_disp_arr, MPI_UINT8_T, 0, MPI_COMM_WORLD);

    end = now();
    time_spent = tdiff(begin, end);
    printf("Time spent %f\n", time_spent);

    //for (int i = 0; i < 5000; i++) {

	   // for (int j = 0; j < 5000; j++) {

            //printf("%d ", matrix[i][j]);

	    //}
        //printf("\n");
   // }
}

void init_arrays() {
    
    int N = 5000;
    int rows_per_worker = (N - 2)/(numtasks);
    int extra_rows_count = (N - 2) - rows_per_worker * (numtasks);
    int recv_per_worker = rows_per_worker * N;
    int send_per_worker = (rows_per_worker + 2) * N;
    int last_worker_send = send_per_worker + extra_rows_count * N;
    int last_worker_recv = recv_per_worker + extra_rows_count * N;

    recv_disp_arr = malloc(numtasks * sizeof(int));
    disp_arr = malloc(numtasks * sizeof(int));
    sendcounts = malloc(numtasks * sizeof(int));
    recvcounts = malloc(numtasks * sizeof(int));

    int offset = 0;
    int offset_recv = N;

    for (int i = 0; i < numtasks - 1; i++) {

        disp_arr[i] = offset;	
        recv_disp_arr[i] = offset_recv;
        sendcounts[i] = send_per_worker;
        recvcounts[i] = recv_per_worker;
        offset += recv_per_worker;
        offset_recv += recv_per_worker;
    }

    disp_arr[numtasks - 1] = offset; 
    sendcounts[numtasks - 1] = last_worker_send;

    recv_disp_arr[numtasks - 1] = offset_recv; 
    recvcounts[numtasks - 1] = last_worker_recv;

}

void read_img() {
    
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
	
	printf("Failed open\n");
    }
    matrix = malloc(5000 * sizeof(uint8_t*));
    matrix[0] = malloc(25000000); 
    uint8_t *ptr = matrix[0];

    for (int i = 1; i < 5000; i++) {
	
        ptr += 5000;
        matrix[i] = ptr;
    }

    uint8_t val;
    for (int i = 0; i < 5000; i++) {

        for (int j = 0; j < 5000; j++) {
            
            if (fscanf(file, "%d", &val) == 1) {        

                matrix[i][j] = val;
            }
        }
    }
}
