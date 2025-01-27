#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

#define MAX 30

typedef struct {
    int src, dest, weight;
} Edge;

Edge edges[MAX * MAX];
int graph[MAX][MAX];
int edgeCount = 0;
int tamanho = 0;

void readGraphFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    tamanho = 0;
    char line[1024];
    while (fgets(line, sizeof(line), file) && tamanho < MAX) {
        tamanho++;
    }
    
    rewind(file);
    
    edgeCount = 0;
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            if (fscanf(file, "%d", &graph[i][j]) != 1) {
                fprintf(stderr, "Erro ao ler valor na posição [%d][%d]\n", i, j);
                exit(EXIT_FAILURE);
            }
            
            if (i < j && graph[i][j] > 0) {
                edges[edgeCount++] = (Edge){i, j, graph[i][j]};
            }
        }
    }
    fclose(file);
}

int find(int parent[], int i) {
    if (parent[i] == i)
        return i;
    return parent[i] = find(parent, parent[i]);
}

void unionSet(int parent[], int rank[], int x, int y) {
    int rootX = find(parent, x);
    int rootY = find(parent, y);
    
    if (rank[rootX] < rank[rootY])
        parent[rootX] = rootY;
    else if (rank[rootX] > rank[rootY])
        parent[rootY] = rootX;
    else {
        parent[rootY] = rootX;
        rank[rootX]++;
    }
}

int compareEdges(const void *a, const void *b) {
    return ((Edge*)a)->weight - ((Edge*)b)->weight;
}

void kruskal(Edge result[], int *mstEdgeCount) {
    int parent[MAX], rank[MAX];
    
    for (int i = 0; i < tamanho; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
    
    qsort(edges, edgeCount, sizeof(Edge), compareEdges);
    
    *mstEdgeCount = 0;
    int e = 0;
    
    while (*mstEdgeCount < tamanho - 1 && e < edgeCount) {
        Edge nextEdge = edges[e++];
        int x = find(parent, nextEdge.src);
        int y = find(parent, nextEdge.dest);
        
        if (x != y) {
            result[(*mstEdgeCount)++] = nextEdge;
            unionSet(parent, rank, x, y);
        }
    }
}

void findOddDegreeVertices(int mst[MAX][MAX], int oddVertices[], int *oddCount) {
    int degree[MAX] = {0};
    *oddCount = 0;
    
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            if (mst[i][j] > 0)
                degree[i]++;
        }
        if (degree[i] % 2 != 0)
            oddVertices[(*oddCount)++] = i;
    }
}

typedef struct {
    int v1, v2;
    int weight;
} MatchPair;

int compareMatchPairs(const void *a, const void *b) {
    return ((MatchPair*)a)->weight - ((MatchPair*)b)->weight;
}

void findMinimumWeightMatching(int oddVertices[], int oddCount, int mst[MAX][MAX]) {
    if (oddCount == 0) return;
    
    MatchPair *pairs = malloc(oddCount * oddCount * sizeof(MatchPair));
    int pairCount = 0;
    bool *matched = calloc(oddCount, sizeof(bool));
    
    // Criar todos os possíveis pares
    for (int i = 0; i < oddCount; i++) {
        for (int j = i + 1; j < oddCount; j++) {
            pairs[pairCount].v1 = oddVertices[i];
            pairs[pairCount].v2 = oddVertices[j];
            pairs[pairCount].weight = graph[oddVertices[i]][oddVertices[j]];
            pairCount++;
        }
    }
    
    // Ordenar pares por peso
    qsort(pairs, pairCount, sizeof(MatchPair), compareMatchPairs);
    
    // Selecionar o melhor matching
    int matchCount = 0;
    for (int i = 0; i < pairCount && matchCount < oddCount/2; i++) {
        int idx1 = -1, idx2 = -1;
        
        // Encontrar índices dos vértices no array oddVertices
        for (int j = 0; j < oddCount; j++) {
            if (oddVertices[j] == pairs[i].v1) idx1 = j;
            if (oddVertices[j] == pairs[i].v2) idx2 = j;
        }
        
        if (!matched[idx1] && !matched[idx2]) {
            mst[pairs[i].v1][pairs[i].v2] = pairs[i].weight;
            mst[pairs[i].v2][pairs[i].v1] = pairs[i].weight;
            matched[idx1] = matched[idx2] = true;
            matchCount++;
        }
    }
    
    free(pairs);
    free(matched);
}

void findEulerianCircuit(int mst[MAX][MAX], int start, int circuit[], int *circuitSize) {
    int *stack = malloc(MAX * MAX * sizeof(int));
    int top = -1;
    *circuitSize = 0;
    
    int **tempGraph = malloc(tamanho * sizeof(int*));
    for (int i = 0; i < tamanho; i++) {
        tempGraph[i] = malloc(tamanho * sizeof(int));
        for (int j = 0; j < tamanho; j++) {
            tempGraph[i][j] = mst[i][j];
        }
    }
    
    stack[++top] = start;
    
    while (top >= 0) {
        int current = stack[top];
        int next = -1;
        int minWeight = INT_MAX;
        
        // Encontrar próxima aresta não visitada de menor peso
        for (int i = 0; i < tamanho; i++) {
            if (tempGraph[current][i] > 0 && tempGraph[current][i] < minWeight) {
                minWeight = tempGraph[current][i];
                next = i;
            }
        }
        
        if (next != -1) {
            stack[++top] = next;
            tempGraph[current][next] = tempGraph[next][current] = 0;
        } else {
            circuit[(*circuitSize)++] = stack[top--];
        }
    }
    
    for (int i = 0; i < tamanho; i++)
        free(tempGraph[i]);
    free(tempGraph);
    free(stack);
}

void optimizeHamiltonianPath(int path[], int pathSize) {
    bool improved;
    do {
        improved = false;
        for (int i = 0; i < pathSize - 2; i++) {
            for (int j = i + 2; j < pathSize - 1; j++) {
                int currentCost = graph[path[i]][path[i+1]] + graph[path[j]][path[j+1]];
                int newCost = graph[path[i]][path[j]] + graph[path[i+1]][path[j+1]];
                
                if (newCost < currentCost) {
                    // Reverter o segmento do caminho
                    for (int k = 0; k < (j - i) / 2; k++) {
                        int temp = path[i + 1 + k];
                        path[i + 1 + k] = path[j - k];
                        path[j - k] = temp;
                    }
                    improved = true;
                }
            }
        }
    } while (improved);
}

void eulerianToHamiltonian(int circuit[], int circuitSize) {
    bool *visited = calloc(tamanho, sizeof(bool));
    int *hamiltonianPath = malloc((tamanho + 1) * sizeof(int));
    int pathSize = 0;
    
    // Construir caminho hamiltoniano
    for (int i = 0; i < circuitSize; i++) {
        if (!visited[circuit[i]]) {
            hamiltonianPath[pathSize++] = circuit[i];
            visited[circuit[i]] = true;
        }
    }
    hamiltonianPath[pathSize++] = hamiltonianPath[0];  // Fechar o ciclo
    
    // Otimizar o caminho
    optimizeHamiltonianPath(hamiltonianPath, pathSize);
    
    // Calcular e imprimir o custo total
    int totalCost = 0;
    for (int i = 0; i < pathSize - 1; i++) {
        totalCost += graph[hamiltonianPath[i]][hamiltonianPath[i + 1]];
    }
    
    printf("\nCaminho aproximado do Caixeiro Viajante:\n");
    for (int i = 0; i < pathSize; i++) {
        printf("%d ", hamiltonianPath[i]);
    }
    printf("\nCusto total: %d\n", totalCost);
    
    free(visited);
    free(hamiltonianPath);
}

void christofides() {
    Edge mstEdges[MAX * MAX];
    int mstEdgeCount;
    int mst[MAX][MAX] = {0};
    
    // Encontrar MST
    kruskal(mstEdges, &mstEdgeCount);
    for (int i = 0; i < mstEdgeCount; i++) {
        mst[mstEdges[i].src][mstEdges[i].dest] = mstEdges[i].weight;
        mst[mstEdges[i].dest][mstEdges[i].src] = mstEdges[i].weight;
    }
    
    // Encontrar vértices de grau ímpar
    int oddVertices[MAX], oddCount;
    findOddDegreeVertices(mst, oddVertices, &oddCount);
    
    // Encontrar matching mínimo para vértices de grau ímpar
    findMinimumWeightMatching(oddVertices, oddCount, mst);
    
    // Encontrar circuito euleriano
    int circuit[MAX * MAX], circuitSize;
    findEulerianCircuit(mst, 0, circuit, &circuitSize);
    
    // Converter para circuito hamiltoniano
    eulerianToHamiltonian(circuit, circuitSize);
}

int main(int argc, char *argv[]) {
    clock_t inicio, fim;
    double tempo_execucao;
    
    if (argc != 2) {
        printf("Uso: %s <arquivo_entrada>\n", argv[0]);
        return 1;
    }
    
    printf("Arquivo de entrada: %s\n", argv[1]);
    
    inicio = clock();
    readGraphFromFile(argv[1]);
    christofides();
    fim = clock();
    
    tempo_execucao = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
    printf("\nTempo de execucao: %f segundos\n", tempo_execucao);
    
    return 0;
}