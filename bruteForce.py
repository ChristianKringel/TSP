import sys
import time
from itertools import permutations

def read_graph_from_file(filename):
    try:
        with open(filename, 'r') as file:
            # Lê todas as linhas e divide em números
            graph = []
            for line in file:
                # Remove espaços extras e quebras de linha
                row = [int(x) for x in line.strip().split()]
                graph.append(row)
            return graph
    except FileNotFoundError:
        print(f"Erro ao abrir o arquivo {filename}")
        sys.exit(1)
    except Exception as e:
        print(f"Erro ao ler o arquivo: {e}")
        sys.exit(1)

def calculate_path_cost(path, graph):
    cost = 0
    n = len(path)
    # Calcula o custo do caminho
    for i in range(n-1):
        cost += graph[path[i]][path[i+1]]
    # Adiciona o custo do retorno à cidade inicial
    cost += graph[path[-1]][path[0]]
    return cost

def solve_tsp(graph):
    n = len(graph)
    cities = list(range(n))
    min_cost = float('inf')
    best_path = None
    
    # Gera todas as permutações começando com a cidade 0
    for perm in permutations(cities[1:]):
        path = (0,) + perm  # Adiciona a cidade 0 no início
        cost = calculate_path_cost(path, graph)
        
        if cost < min_cost:
            min_cost = cost
            best_path = path
            
    return min_cost, best_path

def main():
    if len(sys.argv) != 2:
        print(f"Uso: python {sys.argv[0]} <arquivo_entrada>")
        sys.exit(1)

    filename = sys.argv[1]
    graph = read_graph_from_file(filename)
    
    # Inicia a contagem do tempo
    start_time = time.time()
    
    # Resolve o TSP
    min_cost, best_path = solve_tsp(graph)
    
    # Finaliza a contagem do tempo
    end_time = time.time()
    cpu_time = end_time - start_time
    
    # Imprime os resultados
    print(f"Menor custo encontrado: {min_cost}")
    print("Melhor caminho: ", end="")
    
    # Imprime o caminho (exceto o último vértice) e adiciona o retorno à origem
    for i in range(len(best_path) - 1):
        print(f"{best_path[i]} ", end="")
    print("0")  # Imprime o retorno à cidade inicial
    
    print(f"Tempo de execução: {cpu_time:.6f} segundos")

if __name__ == "__main__":
    main()