#include <stdio.h>

void insertionSort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

char medianFilter(FILE *f) {
    int array[2000][2000];
    int arr[2000][2000];
    int window[9], row = 0, col = 0, numrows = 0, numcols = 0, MAX = 0;
    fscanf(f, "P2\n");
    printf("Version : P2\n");

    fscanf(f, "%d %d %d", &numcols, &numrows, &MAX);
    printf("%d columns and %d rows\nMaximum Intensity %d\n", numcols, numrows, MAX);

    for (row = 0; row <= numrows; ++row)
        array[row][0] = 0;
    for (col = 0; col <= numcols; ++col)
        array[0][col] = 0;

    for (row = 1; row <= numrows; ++row) {
        for (col = 1; col <= numcols; ++col) {
            fscanf(f, "%d", &array[row][col]);
        }
    }

    for (row = 1; row <= numrows; ++row) {
        for (col = 1; col <= numcols; ++col) {
            window[0] = array[row - 1][col - 1];
            window[1] = array[row - 1][col];
            window[2] = array[row - 1][col + 1];
            window[3] = array[row][col - 1];
            window[4] = array[row][col];
            window[5] = array[row][col + 1];
            window[6] = array[row + 1][col - 1];
            window[7] = array[row + 1][col];
            window[8] = array[row + 1][col + 1];

            insertionSort(window, 9);

            arr[row][col] = window[4];
        }
    }

    FILE *outfile = fopen("Medianfilter.pnm", "w");
    fprintf(outfile, "P2\n");
    fprintf(outfile, "%d %d\n", numcols, numrows);
    fprintf(outfile, "255\n");

    for (row = 1; row <= numrows; ++row) {
        for (col = 1; col <= numcols; ++col) {
            fprintf(outfile, "%d ", arr[row][col]);
        }
    }

    fclose(outfile);
    fclose(f);
    return 0;
}

void main(int argc, char **argv) {
    FILE *f1 = fopen("цифровое изображение.txt", "r");
    medianFilter(f1);
}
