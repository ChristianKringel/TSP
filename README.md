# Resolução do Problema do Caixeiro Viajante (TSP)

Este repositório apresenta a implementação de algoritmos para resolver o **Problema do Caixeiro Viajante (TSP - Travelling Salesperson Problem)**. Aqui são demonstradas tanto uma abordagem exata (força bruta) quanto uma aproximativa (Algoritmo de Christofides), com comparações entre tempos de execução, caminhos obtidos, e qualidade das soluções encontradas.

---
### Colaboradores

- [@ChristianKringel](https://github.com/ChristianKringel)  
- [@Ravazolo88](https://github.com/Ravazolo88)

## O Problema do Caixeiro Viajante

O TSP consiste em encontrar o menor percurso que visita cada cidade exatamente uma vez e retorna à cidade inicial. É um problema clássico da computação combinatória, famoso por ser **NP-difícil**, o que significa que o tempo necessário para resolvê-lo cresce exponencialmente com o número de cidades.

### Exemplo:
- Dado um conjunto de cidades e as distâncias entre elas, o objetivo é determinar a ordem de visita que minimize a distância total percorrida.

---
## Como Executar

### Execução em Python

Para executar a versão Python do algoritmo, siga os passos abaixo:

1. Certifique-se de ter o Python 3 instalado.
2. Execute o script passando o arquivo de entrada desejado. Exemplo:

```bash
python bruteForce.py tsp1_253
```

3. O resultado será exibido no terminal, incluindo o custo do caminho e o melhor caminho encontrado.

### Execução em C

Para executar a versão em C do algoritmo:
- Existem dois códigos exatos e um aproximativo. O exato utilizado como métrica é o bruteForce.c e o algoritmo aproximativo é o arquivo christofides.c
- Para compilar ambos arquivos basta seguir os passos abaixo:

1. Compile o código usando um compilador como `gcc`:

```bash
gcc bruteForce.c -o bruteForce
```

2. Execute o programa passando o arquivo de entrada desejado. Exemplo:

```bash
./bruteForce tsp1_253
```

3. O resultado será exibido no terminal, incluindo o custo do caminho e o melhor caminho encontrado.

---

## Algoritmo Exato: Força Bruta

O método de força bruta avalia todas as possíveis permutações das cidades para encontrar o caminho ótimo. Embora seja garantido encontrar a melhor solução, o custo computacional é muito alto devido à complexidade de **O(n!)**.

### Características:
- **Vantagem:** Garante a solução ótima.
- **Desvantagem:** Ineficiente para instâncias com muitas cidades.

### Resultados Obtidos:
| Arquivo     | Tamanho da Matriz | Tempo de Execução | Custo | Melhor Caminho             |
|-------------|--------------------|-------------------|-------|---------------------------|
| tsp1_253    | 11 x 11           | 0.446s           | 253   | 0 -> 7 -> 4 -> 3 -> 9 -> 5 -> 2 -> 6 -> 1 -> 10 -> 0 |
| tsp2_1248   | 6 x 6             | 0.0s (instantâneo)| 1248  | 0 -> 1 -> 2 -> 3 -> 4 -> 0 |
| tsp3_1194   | 15 x 15           | 61136.982s (~17h) | 1194  | 0 -> 1 -> 14 -> 13 -> 12 -> 11 -> 10 -> 9 -> 8 -> 7 -> 6 -> 5 -> 4 -> 3 -> 0 |

---

## Algoritmo Aproximativo: Christofides

A abordagem aproximativa implementada utiliza o **Algoritmo de Christofides**. Esse método constrói uma solução próxima do ótimo usando técnicas como emparelhamento perfeito mínimo e árvores geradoras mínimas.

### Características:
- **Vantagem:** Executa em tempo polinomial e fornece uma solução que é no máximo 1,5 vezes o custo do ótimo.
- **Desvantagem:** Pode ser computacionalmente mais caro que outros heurísticos.

### Resultados Obtidos:
| Arquivo     | Tamanho da Matriz | Tempo de Execução | Custo | Melhor Caminho             |
|-------------|--------------------|-------------------|-------|---------------------------|
| tsp1_253    | 11 x 11           | 0.0040s           | 256   | 0 -> 7 -> 2 -> 4 -> 3 -> 9 -> 5 -> 6 -> 1 -> 10 -> 8 -> 0 |
| tsp2_1248   | 6 x 6             | 0.0030s           | 1248  | 0 -> 5 -> 4 -> 3 -> 2 -> 1 -> 0 |
| tsp3_1194   | 15 x 15           | 0.0060s           | 1194  | 0 -> 1 -> 14 -> 13 -> 12 -> 11 -> 10 -> 9 -> 8 -> 7 -> 6 -> 5 -> 4 -> 3 -> 0 |
| tsp4_7013   | 44 x 44           | 0.0500s           | 7367  | 7 -> 15 -> 22 -> 6 -> 23 -> 3 -> 5 -> 11 -> 12 -> 8 -> 29 -> 28 -> 27 -> 24 -> 25 -> 9 -> 14 -> 17 -> 16 -> 10 -> 4 -> 19 -> 13 -> 26 -> 7 |
| tsp5_27603  | 29 x 29           | 0.0080s           | 30401 | 20 -> 22 -> 21 -> 18 -> 14 -> 10 -> 7 -> 15 -> 5 -> 0 -> 13 -> 2 -> 3 -> 6 -> 8 -> 11 -> 12 -> 16 -> 17 -> 19 -> 23 -> 27 -> 28 -> 25 -> 24 -> 9 -> 4 -> 1 -> 20 |

---


## Referências

1. **CLRS** - Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). *Introduction to Algorithms* (3rd ed.). MIT Press.
2. **Wikipedia** - Travelling Salesperson Problem: [https://en.wikipedia.org/wiki/Travelling_salesman_problem](https://en.wikipedia.org/wiki/Travelling_salesman_problem)
3. **Alon Krymgand** - Approximating the Metric TSP Problem: An Introduction to Combinatorial Optimization: [https://alon.kr/posts/christofides](https://alon.kr/posts/christofides)

---

