#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

uint8_t **matrix;
uint8_t **A;

void read_img();
void sobel_filter();
void display();

int main(int argc, char *argv[]) {

    read_img();
    sobel_filter();
    display();
}

void sobel_filter() {

    for (int i = 1; i < 4999; i++) {

        for (int j = 1; j < 4999; j++) {

            int diag = matrix[i - 1][j - 1] * -1 + matrix[i + 1][j + 1] * 1;
            int Gx =  diag + matrix[i - 1][j + 1] + matrix[i][j - 1] * -2 + matrix[i][j + 1] * 2 + matrix[i + 1][j - 1] * -1; 
            int Gy =  diag + matrix[i - 1][j] * -2 + matrix[i - 1][j + 1] * -1  + matrix[i + 1][j - 1] * 1 + matrix[i + 1][j] * 2; 

            Gx = (Gx >= 0) ? Gx : -Gx;
            Gy = (Gy >= 0) ? Gy : -Gy;

            A[i][j] = Gx + Gy;
        }
    }
}

void display() {

    for (int i = 0; i < 5000; i++) {

        printf("%d, ", matrix[0][i]);

    }
    printf("\n");

    for (int i = 1; i < 4999; i++) {

        for (int j = 0; j < 5000; j++) {

            if (j == 0 || j == 4999) {
            
                printf("%d ", matrix[i][j]);

            } else {

                printf("%d ", A[i][j]);

            }
        }

        printf("\n");
    }

    for (int i = 0; i < 5000; i++) {

        printf("%d ", matrix[4999][i]);

    }
    printf("\n");
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

                printf("%d", val);
                matrix[i][j] = val;
            }
        }
    }

    A = malloc(5000 * sizeof(uint8_t*));
    A[0] = malloc(25000000); 
    ptr = A[0];

    for (int i = 1; i < 5000; i++) {
	
        ptr += 5000;
        A[i] = ptr;
    }

}
