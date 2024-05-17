#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void two_dimensionalArray(int n) {
    int a[100][100];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = (int) abs(i + j);
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }


}

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


struct Pair {
    char *domain;
    int count;
};

struct Pair *subdomainVisits(char **cpdomains, int cpdomainsSize, int *returnSize) {
    struct Pair *ans = (struct Pair *) malloc(sizeof(struct Pair) * cpdomainsSize);
    int ansSize = 0;

    for (int i = 0; i < cpdomainsSize; i++) {
        char *token = strtok(cpdomains[i], " ");
        int count = atoi(token);
        char *domain = strtok(NULL, " ");

        char *frag = strtok(domain, ".");
        while (frag != NULL) {
            for (int j = 0; j < strlen(frag); j++) {
                if (frag[j] == '.') {
                    frag[j] = '\0';
                    break;
                }
            }
            for (int j = i; j < cpdomainsSize; j++) {
                if (strcmp(ans[j].domain, frag) == 0) {
                    ans[j].count += count;
                    break;
                }
            }
            ans[ansSize].domain = frag;
            ans[ansSize].count = count;
            ansSize++;

            frag = strtok(NULL, ".");
        }
    }

    *returnSize = ansSize;
    return ans;
}


#define n 3

void findPrefixCount(int p_arr[][n], bool arr[][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = n - 1; j >= 0; j--) {
            if (!arr[i][j])
                continue;
            if (j != n - 1)
                p_arr[i][j] += p_arr[i][j + 1];
            p_arr[i][j] += (int)arr[i][j];
        }
    }
}

int matrixAllOne(bool arr[][n]) {
    int p_arr[n][n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            p_arr[i][j] = 0;
        }
    }

    findPrefixCount(p_arr, arr);
    int ans = 0;

    for (int j = 0; j < n; j++) {
        int i = n - 1;
        struct Pair {
            int first;
            int second;
        } *q = malloc(n * sizeof(struct Pair));
        int q_size = 0;
        int to_sum = 0;

        while (i >= 0) {
            int c = 0;
            while (q_size != 0 && q[q_size - 1].first > p_arr[i][j]) {
                to_sum -= (q[q_size - 1].second + 1) * (q[q_size - 1].first - p_arr[i][j]);
                c += q[q_size - 1].second + 1;
                q_size--;
            }
            to_sum += p_arr[i][j];
            ans += to_sum;
            q[q_size].first = p_arr[i][j];
            q[q_size].second = c;
            q_size++;
            i--;
        }
        free(q);
    }

    return ans;
}

void test_matrixAllOne(){
    bool arr[][n] = { { 1, 0, 1 },
                      { 1, 1, 0 },
                      { 1, 1, 0 } };
    matrixAllOne(arr);
}
void test_subdomainVisits() {
    char *cpdomains[] = {"9001 discuss.leetcode.com"};
    int cpdomainsSize = 1;
    int returnSize;
    struct Pair *result = subdomainVisits(cpdomains, cpdomainsSize, &returnSize);

    for (int i = 0; i < returnSize; i++) {
        printf("%d %s\n", result[i].count, result[i].domain);
    }

    free(result);
}

void test_medianFilter() {
    FILE *f1 = fopen("цифровое изображение.txt", "r");
    medianFilter(f1);
}

void test_two_dimensionalArray() {
    int n = 3;

    two_dimensionalArray(n);
}


void test() {
    test_two_dimensionalArray();
    test_medianFilter();
    test_subdomainVisits();
    test_matrixAllOne();
}

void main(int argc, char **argv) {
    test();
}
