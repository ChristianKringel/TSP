#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define MAX_CITIES 20

void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

int calculate_path_cost(int *path, int n, int graph[MAX_CITIES][MAX_CITIES])
{
    int cost = 0;
    for (int i = 0; i < n - 1; i++)
    {
        cost += graph[path[i]][path[i + 1]];
    }
    cost += graph[path[n - 1]][path[0]]; // retorna a cidade inicial
    return cost;
}

void permute(int *arr, int start, int end, int *min_cost, int *best_path,
             int n, int graph[MAX_CITIES][MAX_CITIES])
{
    if (start == end)
    {
        if (arr[0] == 0)
        {
            int current_cost = calculate_path_cost(arr, n, graph);
            if (current_cost < *min_cost)
            {
                *min_cost = current_cost;
                memcpy(best_path, arr, n * sizeof(int));
            }
        }
        return;
    }

    for (int i = start; i <= end; i++)
    {
        swap(&arr[start], &arr[i]);
        permute(arr, start + 1, end, min_cost, best_path, n, graph);
        swap(&arr[start], &arr[i]); // backtrack
    }
}

int read_graph_from_file(const char *filename, int graph[MAX_CITIES][MAX_CITIES])
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Erro ao abrir o arquivo %s\n", filename);
        return -1;
    }

    int n = 0;
    char line[1024];
    int row = 0;

    while (fgets(line, sizeof(line), file) && row < MAX_CITIES)
    {
        char *token = strtok(line, " \t\n");
        int col = 0;

        while (token && col < MAX_CITIES)
        {
            graph[row][col] = atoi(token);
            token = strtok(NULL, " \t\n");
            col++;
        }

        if (n == 0)
        {
            n = col;
        }
        row++;
    }

    fclose(file);
    return row;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso: %s <arquivo_entrada>\n", argv[0]);
        return 1;
    }

    int graph[MAX_CITIES][MAX_CITIES];
    int n = read_graph_from_file(argv[1], graph);

    if (n <= 0 || n > MAX_CITIES)
    {
        printf("Erro na leitura do arquivo ou número de cidades inválido\n");
        return 1;
    }

    int *path = (int *)malloc(n * sizeof(int));
    int *best_path = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        path[i] = i;
    }

    int min_cost = INT_MAX;

    clock_t start = clock();

    permute(path, 0, n - 1, &min_cost, best_path, n, graph);

    clock_t end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Menor custo encontrado: %d\n", min_cost);
    printf("Melhor caminho: ");
    for (int i = 0; i < n - 1; i++)
    {
        printf("%d ", best_path[i]);
    }
    printf("0\n");

    printf("Tempo de execucao: %.6f segundos\n", cpu_time_used);

    free(path);
    free(best_path);
    return 0;
}