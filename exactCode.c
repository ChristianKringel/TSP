#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

void generatePermutations(int *numbers, int start, int n, int **matrix, int *path_size, int *path_temp, int *path_minim);
int calculatePath(int *path, int **matrix, int n);

int main(int argc, char *argv[])
{
    char *row = NULL;
    size_t len = 0;
    int rows = 0, column = 0;

    if (argc < 2)
    {
        fprintf(stderr, "Eh necessario um arquivo de entrada!\n");
        return EXIT_FAILURE;
    }

    const char *filepath = argv[1];
    printf("Arquivo: %s\n", filepath);

    FILE *file = fopen(filepath, "r");
    if (!file)
    {
        fprintf(stderr, "Arquivo nao encontrado!\n");
        return EXIT_FAILURE;
    }

    int **matrix_values = (int **)malloc(sizeof(int *));
    if (!matrix_values)
    {
        fprintf(stderr, "Erro ao alocar memoria para a matriz!\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    while (getline(&row, &len, file) != -1)
    {
        matrix_values = (int **)realloc(matrix_values, (rows + 1) * sizeof(int *));
        matrix_values[rows] = (int *)malloc(sizeof(int) * 100);
        if (!matrix_values[rows])
        {
            fprintf(stderr, "Erro ao alocar memoria!\n");
            fclose(file);
            return EXIT_FAILURE;
        }

        column = 0;
        char *token = strtok(row, " ");
        while (token != NULL)
        {
            matrix_values[rows][column++] = atoi(token);
            token = strtok(NULL, " ");
        }
        rows++;
    }

    free(row);
    fclose(file);

    int matrix_size = rows;
    printf("Tam matriz: %d\n", matrix_size);

    int *permutation_numbers = (int *)malloc(matrix_size * sizeof(int));
    for (int i = 0; i < matrix_size; ++i)
    {
        permutation_numbers[i] = i;
    }

    printf("Matriz:\n");
    for (int i = 0; i < matrix_size; ++i)
    {
        for (int j = 0; j < column; ++j)
        {
            printf("%d ", matrix_values[i][j]);
        }
        printf("\n");
    }

    int best_size = INT_MAX;
    int *path_min = (int *)malloc(matrix_size * sizeof(int));
    int *path_temp = (int *)malloc(matrix_size * sizeof(int));

    clock_t start = clock();
    generatePermutations(permutation_numbers, 1, matrix_size, matrix_values, &best_size, path_temp, path_min);
    clock_t end = clock();

    // Corrected path print
    printf("Melhor caminho: ");
    for (int i = 0; i < matrix_size - 1; ++i)
        printf("%d ", path_min[i]);
    printf("%d %d\n", path_min[matrix_size - 1], path_min[0]);

    printf("Tamanho do melhor caminho: %d\n", best_size);
    printf("Tempo de execucao: %f\n", (double)(end - start) / CLOCKS_PER_SEC);

    for (int i = 0; i < matrix_size; ++i)
        free(matrix_values[i]);
    free(path_min);
    free(path_temp);
    free(permutation_numbers);

    return 0;
}

int calculatePath(int *path, int **matrix, int n)
{
    int path_size = 0;
    for (int i = 0; i < n - 1; ++i)
        path_size += matrix[path[i]][path[i + 1]];
    path_size += matrix[path[n - 1]][path[0]]; // Fecha o ciclo
    return path_size;
}

void generatePermutations(int *numbers, int start, int n, int **matrix, int *path_size, int *path_temp, int *path_minim)
{
    if (start == n)
    {
        // Adicione esta verificação
        if (numbers[0] == 0)
        {
            int temp_size_path = calculatePath(numbers, matrix, n);
            if (*path_size > temp_size_path)
            {
                *path_size = temp_size_path;
                for (int i = 0; i < n; ++i)
                    path_minim[i] = numbers[i];
            }
        }
    }
    else
    {
        for (int i = start; i < n; ++i)
        {
            int temp = numbers[start];
            numbers[start] = numbers[i];
            numbers[i] = temp;

            generatePermutations(numbers, start + 1, n, matrix, path_size, path_temp, path_minim);

            temp = numbers[start];
            numbers[start] = numbers[i];
            numbers[i] = temp;
        }
    }
}