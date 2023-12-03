# AI Algorithms: Depth, Breath, A-Star, Best

## Overview

Welcome to the "AI Algorithms: Depth, Breadth, A-Star, Best" repository! This project serves as a comprehensive resource for understanding and implementing fundamental artificial intelligence algorithms. As part of your university coursework, this repository explores and provides clear implementations of key algorithms such as Depth-First Search, Breadth-First Search, A-Star, and Best-First Search. These algorithms are not only pivotal in academic settings but also form the bedrock of real-world applications in artificial intelligence.
## Table of Contents

- [Introduction](#introduction)
- [Algorithms Included](#algorithms-included)
- [Usage](#usage)
- [Examples](#examples)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

## Introduction

Artificial Intelligence algorithms play a crucial role in solving complex problems by providing efficient solutions. This repository aims to provide clear and concise implementations of popular AI algorithms, allowing developers to understand and use them in their projects.

## Algorithms Included

1. **Depth-First Search (DFS):** A graph traversal algorithm that explores as far as possible along each branch before backtracking.
2. **Breadth-First Search (BFS):** A graph traversal algorithm that explores all the vertices of a graph in breadth-first order.
3. **A-Star (A***): A widely used pathfinding algorithm that finds the shortest path between two points in a graph.
4. **Best-First Search:** An algorithm that explores a graph by selecting the most promising node based on a heuristic.

## Usage

To use the algorithms in your project, follow these steps:

1. Clone the repository to your local machine:

   ```bash
   git clone https://github.com/cppenloglou/AI-Algorithms-Deapth-Breath-A-Star-Best.git
   ```

2. Navigate to the algorithm of your choice in the repository.

3. Include the algorithm in your project.

4. Use the provided functions to apply the algorithm to your specific problem.

## Compile and Run
```c
//To compile your file in c named "temp.c"
gcc -Wall -g temp.c -o temp.exe

//To run the project with named "temp" file
./temp.exe start_Point target_Point solution.txt (solution output file)
```

## Examples

Here's a quick example of how to use the A-Star algorithm:

```c
#define BREATH 0
#define DEAPTH 1
#define A_STAR 2
#define BEST   3


enum bool{
    FALSE, TRUE
};

#define INCREASE    0
#define DECREASE    1
#define DOUBLE_OP   2
#define HALF        3
#define SQUARE      4
#define ROOT_OP     5

typedef struct tree_node {
    int number;
    int h;
    int f;
    int g;
    struct tree_node *parent;
    int last_operation;
} tree_node;

typedef struct frontier_node{
    tree_node *leaf;
    struct frontier_node *next;
    struct frontier_node *prev;
} frontier_node;
```

## Contributing

If you'd like to contribute to this project, feel free to fork the repository and submit a pull request. Please adhere to the [Contributing Guidelines](CONTRIBUTING.md).

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contact

If you have any questions or suggestions, feel free to reach out to the repository owner:

- Name: cppenloglou
- Email: xrisostomospenloglou13@gmail.com
- GitHub: [cppenloglou](https://github.com/cppenloglou)
