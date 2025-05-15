# CS32: Huffman Coding

## Overview

This repository contains a C implementation of the Huffman Coding algorithm, specifically for the Machine Problem 2 (MP2) as a course requirement for CS 32 (24.2 WFX_MLNO) at the University of the Philippines Diliman, Department of Computer Science. The project focuses on constructing a Huffman encoding tree using a priority queue (min-heap) as part of a data compression process.

## Problem Statement

Given a text file (terminated by a `;`), the program must:

1. **Count the frequency** of each character in the input.
2. **Build a Huffman encoding tree** from these frequencies using a priority queue (min-heap).
3. **Generate a character encoding map** from the tree.
4. **Output** the Huffman code for each character, sorted by increasing code value.

**Note:** You are required to implement only step (2) — the construction of the Huffman tree using a priority queue.

## Technical Details

### Huffman Coding Algorithm
Huffman coding is a greedy algorithm used for lossless data compression. It assigns variable-length codes to input characters, with shorter codes assigned to more frequent characters. The process involves:
- Counting character frequencies.
- Building a binary tree (Huffman tree) where each leaf node represents a character and its frequency.
- Using a priority queue (min-heap) to repeatedly combine the two nodes with the lowest frequencies until a single tree remains.
- Traversing the tree to assign binary codes to each character.

### Data Structures Used
- **Priority Queue (Min-Heap):** Used to efficiently extract the two nodes with the lowest frequencies during tree construction.
- **Binary Tree Nodes:** Each node contains a character, its frequency, and pointers to left and right children.

### Implementation Approach
- **PQ_INSERT:** Inserts a new node into the min-heap, maintaining the heap property.
- **PQ_EXTRACT:** Removes and returns the node with the minimum frequency from the heap.
- **HEAPIFY:** Restores the min-heap property from a given index.
- **BUILD_HUFFMAN_TREE:** Repeatedly extracts the two lowest-frequency nodes, combines them into a new node, and inserts the new node back into the heap until only one node remains (the root of the Huffman tree).
- **Encoding Map Generation:** After building the tree, a traversal assigns binary codes to each character based on their path from the root.

## Input Format

- Any text file, terminated by a `;`.
- All characters (including spaces, newlines, tabs, and special characters) are significant and case-sensitive.

**Example Input:**
```
I
see
trees!!!!!;
```

## Output Format

- For each character (before the `;`), print its Huffman code in the format:
  ```
  %c :: [code]
  ```
- Output is sorted by increasing Huffman code.

**Example Output:**
```
r :: 000
I :: 0010
t :: 0011
<newline> :: 010
s :: 011
e :: 10
! :: 11
```
*(Note: `<newline>` represents the actual newline character.)*

## Implementation Details

You are provided with a C template that implements steps (1) and (3). Your task is to implement the following functions for step (2):

- `void PQ_INSERT (PriorityQueue *P, BinaryTreeNode *x)`  
  *Insert a node into the priority queue (min-heap).*

- `BinaryTreeNode *PQ_EXTRACT (PriorityQueue *P)`  
  *Extract the node with the minimum frequency from the priority queue.*

- `void HEAPIFY (PriorityQueue *P, int idx)`  
  *Restore the min-heap property starting from index `idx`.*

- `BinaryTreeNode *BUILD_HUFFMAN_TREE (PriorityQueue *P)`  
  *Construct the Huffman encoding tree from the priority queue.*

**Restrictions:**  
- You may add helper functions, but **do not remove or change the parameters/return values of the provided functions**.

## How to Build and Run (C)

1. **Clone the repository:**
   ```sh
   git clone https://github.com/yourusername/cs32-huffman-coding.git
   cd cs32-huffman-coding
   ```

2. **Compile the code using GCC:**
   ```sh
   gcc -o huffman main.c
   ```
   *(Replace `main.c` with the actual source file names if different.)*

3. **Run the program:**
   ```sh
   ./huffman < input.txt
   ```
   Where `input.txt` contains your input data.

## References

- [Huffman Coding (Wikipedia)](https://en.wikipedia.org/wiki/Huffman_coding)

## Authors

- Leonard Ang (2022-01328)
- Justin Chuah (2023-00514)

## License

This project is for educational purposes as part of the CS32 course at the University of the Philippines Diliman, Department of Computer Science.
