#include <stdlib.h>

int main(int argc, char *argv[]) {

    int A = malloc(5000 * sizeof(u8*));
    for (int i = 0; i < 5000; i++) {
        A[i] = malloc(5000 * sizeof(int));
    }
    sobel_filter();
    display();
}

void sobel_filter() {

    for (int i = 1; i < 4999; i++) {

        for (int j = 1; j < 4999; j++) {

            int diag =inp_mat[i - 1][j - 1] * -1 + inp_mat[i + 1][j + 1] * 1;
            int Gx =  diag + inp_mat[i - 1][j + 1] + inp_mat[i][j - 1] * -2 + inp_mat[i][j + 1] * 2 + inp_mat[i + 1][j - 1] * -1; 
            int Gy =  diag + inp_mat[i - 1][j] * -2 + inp_mat[i - 1][j + 1] * -1  + inp_mat[i + 1][j - 1] * 1 + inp_mat[i + 1][j] * 2; 

            Gx = (Gx >= 0) ? Gx : -Gx;
            Gy = (Gy >= 0) ? Gy : -Gy;

            A[i][j] = Gx + Gy;
        }
    }
}

void display() {

    for (int i = 0; i < 5000; i++) {

        printf("%d, ", inp_mat[0][i];

    }
    printf("\n");

    for (int i = 1; i < 4999; i++) {

        for (int j = 0; j < 5000; j++) {

            if (j == 0 || j == 4999) {
            
                printf("%d ", inp_mat[i][j]);

            } else {

                printf("%d ", A[i][j]);

            }
        }

        printf("\n");
    }
    for (int i = 0; i < 5000; i++) {

        printf("%d, ", inp_mat[4999][i];

    }
    printf("\n");
}
                
