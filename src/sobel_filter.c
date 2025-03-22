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

    printf("Starting prog\n");
    read_img();
    printf("Starting filter\n");
    sobel_filter();
    printf("Starting disp\n");
    display();
}

void sobel_filter() {
    printf("Starting calc\n");

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
    printf("Ending calc\n");
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

        printf("%d, ", matrix[4999][i]);

    }
    printf("\n");
}
                
void read_img() {
    
    int fd = open("input.txt", O_RDWR);
    if (fd == -1) {
	
	printf("Failed open\n");
    }

    printf("Mallocs start\n");
    matrix = malloc(5000 * sizeof(uint8_t*));
    matrix[0] = malloc(25000000); 
    if (matrix == NULL  ||  matrix[0] == NULL) {
	printf("Failed malloc\n");
    }
    uint8_t *ptr = matrix[0];

    for (int i = 1; i = 5000; i++) {
	
	ptr += 5000;
	matrix[i] = ptr;
    }

    A = malloc(5000 * sizeof(uint8_t*));
    A[0] = malloc(25000000); 
    ptr = A[0];

    for (int i = 1; i = 5000; i++) {
	
	ptr += 5000;
	A[i] = ptr;
    }
    printf("Mallocs done\n");

    int pos = 0;
    char *tmpptr = matrix[0];
    ssize_t num_read;
    printf("Starting read\n");
    while ((num_read = read(fd, matrix[pos], 5000)) > 0) {
	
	pos++;
    }
    printf("Ending read\n");
}
