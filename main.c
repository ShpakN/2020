#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <io.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_LENGTH 1000
#define INVALID "INVALID"

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

#define MESSAGE "\n--- no more data ---\n"
#define NLINES 20

#ifndef NOUSE_HANDLER
volatile
#endif
int fd, N = NLINES, stop = 0;

void
pri_signal(int sig) {
#ifndef NOUSE_HANDLER
    char buf[4096];
    int i, l, o = 0;
    off_t pos = lseek(fd, 0, SEEK_CUR);

    do {
        if ((l = read(fd, buf, 4096)) > 0) {
            for (i = 0; i < l && o < N; i++, pos++)
                if (buf[i] == '\n')
                    o++;
            write(STDOUT_FILENO, buf, i);
        } else {
            write(STDOUT_FILENO, MESSAGE, sizeof(MESSAGE) - 1);
            stop = 1;
            break;
        }
    } while (o < N);

    if (!stop)
        lseek(fd, pos, SEEK_SET);
#endif
}


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

            if (!arr[i][j]) {
                continue;
            }
            if (j != n - 1) {
                p_arr[i][j] += p_arr[i][j + 1];
            }

            p_arr[i][j] += (int) arr[i][j];
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
        }
                *q = malloc(n * sizeof(struct Pair));
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


void PrintMinNumberForPattern(char arr[]) {
    int curr_max = 0;
    int last_entry = 0;
    int j;
    int length = strlen(arr);

    for (int i = 0; i < length; i++) {
        int noOfNextD = 0;

        switch (arr[i]) {
            case 'I':
                j = i + 1;
                while (arr[j] == 'D' && j < length) {
                    noOfNextD++;
                    j++;
                }

                if (i == 0) {
                    curr_max = noOfNextD + 2;
                    printf(" %d", ++last_entry);
                    printf(" %d", curr_max);
                    last_entry = curr_max;
                } else {
                    curr_max = curr_max + noOfNextD + 1;
                    last_entry = curr_max;
                    printf(" %d", last_entry);
                }

                for (int k = 0; k < noOfNextD; k++) {
                    printf(" %d", --last_entry);
                    i++;
                }
                break;

            case 'D':
                if (i == 0) {
                    j = i + 1;
                    while (arr[j] == 'D' && j < length) {
                        noOfNextD++;
                        j++;
                    }

                    curr_max = noOfNextD + 2;
                    printf(" %d %d", curr_max, curr_max - 1);
                    last_entry = curr_max - 1;
                } else {
                    printf(" %d", last_entry - 1);
                    last_entry--;
                }
                break;
        }
    }

    printf("\n");
}


struct TreeNode *constructMaximumBinaryTree(int *nums, int numsSize) {
    struct TreeNode **v = (struct TreeNode **) malloc(numsSize * sizeof(struct TreeNode *));
    int vSize = 0;

    for (int i = 0; i < numsSize; i++) {
        struct TreeNode *cur = (struct TreeNode *) malloc(sizeof(struct TreeNode));
        cur->val = nums[i];
        cur->left = NULL;
        cur->right = NULL;

        while (vSize > 0 && v[vSize - 1]->val < nums[i]) {
            cur->left = v[vSize - 1];
            vSize--;
        }

        if (vSize > 0) {
            v[vSize - 1]->right = cur;
        }

        v[vSize] = cur;
        vSize++;
    }

    return v[0];
}


char *GetShuffledString(char *s, int *index, int len) {
    char *res = (char *) malloc(len * sizeof(char));

    for (int i = 0; i < len; i++) {
        int ind = index[i];
        res[ind] = s[i];
    }

    return res;
}


void sequenceIntegersLessN(int argc, char **argv) {
    {
        int fd, z;
        char buf[1];

        fd = open(argv[1], O_RDONLY, 0666);
        int fn = open(argv[2], O_RDWR | O_CREAT, 0666);
        z = read(fd, buf, sizeof(buf)) > 0;

        while (z) {
            if (atoi(buf) < z) {
                write(fn, buf, sizeof(int));
            }
            z = read(fd, buf, sizeof(buf)) > 0;
        }
    }
}


int contentsOfFilePortionsNLines(int argc, char **argv) {
    if (argv[1]) {

        if ((N = atoi(argv[1])) < 1)
            N = NLINES;
        argv++;
    }

    char *name = argv[1] ? argv[1] : (char *) __FILE__;
    fd = open(name, O_RDONLY);

    signal(SIGINT, pri_signal);

#ifdef NOUSE_HANDLER
    FILE *f = fdopen(fd, "r");
#endif

    do {
#ifdef NOUSE_HANDLER
        int n = 0, c;
    while ((c = fgetc(f)) != EOF && n < N) {
      putchar(c);
      if (c == '\n')
        n++;
    }
    if (c == EOF) {
      fputs(MESSAGE, stdout);
      stop = 1;
    }
#endif

    } while (!stop);

    return puts("End") == EOF;
}


typedef struct {
    char mainset[MAX_LENGTH][MAX_LENGTH];
    char array[MAX_LENGTH][MAX_LENGTH];
    char main_ary[MAX_LENGTH][MAX_LENGTH];
    int length;
} find_str;

void process(find_str *fin, char *input) {
    int len = strlen(input);

    for (int i = 0; i < len; i++) {
        for (int j = i; j < len; j++) {
            strcpy(fin->mainset[fin->length], &input[i]);
            fin->length++;
        }
    }
}

void initialize(find_str *fin) {
    fin->length = 0;

    for (int i = 0; i < MAX_LENGTH; i++) {
        strcpy(fin->main_ary[i], fin->mainset[i]);
    }
}

char *query(find_str *fin, int index) {
    if (index < fin->length) {
        return fin->main_ary[index];
    } else {
        return INVALID;
    }
}

void Auto_completionLowercaseEnglishLetters() {
    find_str fin;
    int num_of_strings;
    int num_of_queries;
    char input[MAX_LENGTH];

    scanf("%d", &num_of_strings);

    for (int i = num_of_strings; --i >= 0;) {
        scanf("%s", input);
        process(&fin, input);
    }
    initialize(&fin);

    scanf("%d", &num_of_queries);

    for (int i = num_of_queries; --i >= 0;) {
        int query_index;
        scanf("%d", &query_index);
        printf("%s\n", query(&fin, query_index - 1));
    }

}

void test_matrixAllOne() {
    bool arr[][n] = {{1, 0, 1},
                     {1, 1, 0},
                     {1, 1, 0}};
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
    int q = 3;

    two_dimensionalArray(q);
}

void test_PrintMinNumberForPattern() {
    PrintMinNumberForPattern("IIIDIDDD");
}


void test_constructMaximumBinaryTree() {
    int *a = NULL;
    int nums = a[3, 2, 1];
    int num_size = 3;

    constructMaximumBinaryTree(&nums, num_size);
}


void test_GetShuffledString() {
    char *result1 = GetShuffledString("abc", (int[]) {0, 1, 2}, 3);

    printf("%s\n", result1);

    free(result1);
}


void test_sequenceIntegersLessN() {
    sequenceIntegersLessN(4, (char **) 8);
}


void test_contentsOfFilePortionsNLines() {
    contentsOfFilePortionsNLines(6, (char **) 9);
}


void test_Auto_completionLowercaseEnglishLetters() {
    Auto_completionLowercaseEnglishLetters();
}

void test() {
    test_two_dimensionalArray();
    test_medianFilter();
    test_subdomainVisits();
    test_matrixAllOne();
    test_PrintMinNumberForPattern();
    test_constructMaximumBinaryTree();
    test_GetShuffledString();
    test_sequenceIntegersLessN();
    test_contentsOfFilePortionsNLines();
    test_Auto_completionLowercaseEnglishLetters();

}

int main(int argc, char **argv) {
    test();
}
