#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

#define MAX 20

// Estrutura para representar uma aresta
typedef struct {
    int src, dest, weight;
} Edge;

Edge edges[MAX * MAX];
int graph[MAX][MAX];
int edgeCount = 0;
int tamanho = 0;

// Função para ler o grafo a partir de um arquivo
void readGraphFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Contar número de linhas para determinar tamanho
    tamanho = 0;
    char line[1024];
    while (fgets(line, sizeof(line), file) && tamanho < MAX) {
        tamanho++;
    }

    // Voltar ao início do arquivo
    rewind(file);

    // Ler a matriz de adjacência
    edgeCount = 0;
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            if (fscanf(file, "%d", &graph[i][j]) != 1) {
                fprintf(stderr, "Erro ao ler valor na posição [%d][%d]\n", i, j);
                exit(EXIT_FAILURE);
            }
            // Adicionar aresta se for acima da diagonal principal e peso > 0
            if (i < j && graph[i][j] > 0) {
                if (edgeCount >= MAX * MAX) {
                    fprintf(stderr, "Número máximo de arestas excedido\n");
                    exit(EXIT_FAILURE);
                }
                edges[edgeCount++] = (Edge){i, j, graph[i][j]};
            }
        }
    }

    fclose(file);
    
}
// Funções auxiliares para o algoritmo de Kruskal
int find(int parent[], int i) {
    if (parent[i] == i) {
        return i;
    }
    return find(parent, parent[i]);
}

void unionSets(int parent[], int rank[], int x, int y) {
    int rootX = find(parent, x);
    int rootY = find(parent, y);

    if (rank[rootX] < rank[rootY]) {
        parent[rootX] = rootY;
    } else if (rank[rootX] > rank[rootY]) {
        parent[rootY] = rootX;
    } else {
        parent[rootY] = rootX;
        rank[rootX]++;
    }
}

int compareEdges(const void *a, const void *b) {
    Edge *edgeA = (Edge *)a;
    Edge *edgeB = (Edge *)b;
    return edgeA->weight - edgeB->weight;
}

// Algoritmo de Kruskal para encontrar a MST
void kruskal(Edge result[], int *mstEdgeCount) {
    int parent[MAX], rank[MAX];
    int e = 0; // Contador para arestas na MST
    int i = 0; // Contador para arestas ordenadas

    // Inicializar os conjuntos
    for (int v = 0; v < tamanho; v++) {
        parent[v] = v;
        rank[v] = 0;
    }

    // Ordenar as arestas por peso
    qsort(edges, edgeCount, sizeof(Edge), compareEdges);

    // Construir a MST
    while (e < tamanho - 1 && i < edgeCount) {
        Edge nextEdge = edges[i++];

        int x = find(parent, nextEdge.src);
        int y = find(parent, nextEdge.dest);

        if (x != y) {
            result[e++] = nextEdge;
            unionSets(parent, rank, x, y);
        }
    }

    *mstEdgeCount = e;
}

// Identificar vértices de grau ímpar
void findOddDegreeVertices(int mst[MAX][MAX], int oddVertices[], int *oddCount) {
    int degree[MAX] = {0};

    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            if (mst[i][j] != 0) {
                degree[i]++;
            }
        }
    }

    *oddCount = 0;
    for (int i = 0; i < tamanho; i++) {
        if (degree[i] % 2 != 0) {
            oddVertices[(*oddCount)++] = i;
        }
    }
}

// Encontrar emparelhamento mínimo de vértices de grau ímpar
void findMinimumMatching(int oddVertices[], int oddCount, int mst[MAX][MAX]) {
    bool matched[MAX] = {false};

    for (int i = 0; i < oddCount; i++) {
        if (!matched[oddVertices[i]]) {
            int minWeight = INT_MAX, minIndex = -1;

            for (int j = i + 1; j < oddCount; j++) {
                if (!matched[oddVertices[j]] && graph[oddVertices[i]][oddVertices[j]] < minWeight) {
                    minWeight = graph[oddVertices[i]][oddVertices[j]];
                    minIndex = j;
                }
            }

            if (minIndex != -1) {
                mst[oddVertices[i]][oddVertices[minIndex]] = graph[oddVertices[i]][oddVertices[minIndex]];
                mst[oddVertices[minIndex]][oddVertices[i]] = graph[oddVertices[minIndex]][oddVertices[i]];
                matched[oddVertices[i]] = true;
                matched[oddVertices[minIndex]] = true;
            }
        }
    }
}

// Encontrar um circuito Euleriano
void findEulerianCircuit(int mst[MAX][MAX], int start, int circuit[], int *circuitSize) {
    int stack[MAX], top = -1;
    int current = start;
    *circuitSize = 0;

    // Criar uma cópia do grafo
    int tempGraph[MAX][MAX];
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            tempGraph[i][j] = mst[i][j];
        }
    }

    // Empilhar o vértice inicial
    stack[++top] = current;

    while (top >= 0) {
        current = stack[top];
        
        // Procurar próxima aresta não utilizada
        int next = -1;
        for (int i = 0; i < tamanho; i++) {
            if (tempGraph[current][i] > 0) {
                next = i;
                break;
            }
        }

        if (next != -1) {
            // Se encontrou uma aresta, empilha o próximo vértice
            stack[++top] = next;
            // Remove a aresta do grafo temporário
            tempGraph[current][next] = 0;
            tempGraph[next][current] = 0;
        } else {
            // Se não há mais arestas, adiciona o vértice ao circuito
            circuit[(*circuitSize)++] = stack[top--];
        }
    }
}

// Converter circuito Euleriano em circuito Hamiltoniano
void eulerianToHamiltonian(int circuit[], int circuitSize) {
    bool visited[MAX] = {false};
    int hamiltonianPath[MAX];
    int pathSize = 0;
    int totalCost = 0;

    // Construir caminho hamiltoniano
    for (int i = 0; i < circuitSize; i++) {
        if (!visited[circuit[i]]) {
            hamiltonianPath[pathSize++] = circuit[i];
            visited[circuit[i]] = true;
        }
    }

    // Adicionar o vértice inicial para fechar o ciclo
    hamiltonianPath[pathSize++] = hamiltonianPath[0];

    // Calcular custo total
    for (int i = 0; i < pathSize - 1; i++) {
        totalCost += graph[hamiltonianPath[i]][hamiltonianPath[i + 1]];
    }

    printf("\nCaminho aproximado do Caixeiro Viajante:\n");
    for (int i = 0; i < pathSize; i++) {
        printf("%d ", hamiltonianPath[i]);
    }
    printf("\nCusto total: %d\n", totalCost);
}

// Algoritmo de Christofides
void christofides() {
    Edge mstEdges[MAX];
    int mstEdgeCount;
    int mst[MAX][MAX] = {0};

    // Etapa 1: Encontrar a MST
    kruskal(mstEdges, &mstEdgeCount);
    for (int i = 0; i < mstEdgeCount; i++) {
        mst[mstEdges[i].src][mstEdges[i].dest] = mstEdges[i].weight;
        mst[mstEdges[i].dest][mstEdges[i].src] = mstEdges[i].weight;
    }

    // Etapa 2: Identificar vértices de grau ímpar
    int oddVertices[MAX], oddCount;
    findOddDegreeVertices(mst, oddVertices, &oddCount);

    // Etapa 3: Encontrar emparelhamento mínimo
    findMinimumMatching(oddVertices, oddCount, mst);

    // Etapa 4: Encontrar um circuito Euleriano
    int circuit[MAX * MAX], circuitSize;
    findEulerianCircuit(mst, 0, circuit, &circuitSize);

    // Etapa 5: Converter para circuito Hamiltoniano
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

    // Início da medição
    inicio = clock();

    // Ler o grafo a partir do arquivo
    readGraphFromFile(argv[1]);

    // Executar o algoritmo de Christofides
    christofides();

    // Fim da medição
    fim = clock();
    tempo_execucao = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
    printf("\nTempo de execucao: %f segundos\n", tempo_execucao);

    return 0;
}