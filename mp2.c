#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARS 256
#define MAX_CODE_LEN 256

typedef struct BinaryTreeNode
{
    char data;
    int freq;
    struct BinaryTreeNode *left;
    struct BinaryTreeNode *right;
} BinaryTreeNode;

typedef struct
{
    BinaryTreeNode *A[MAX_CHARS];
    int heap_size;
} PriorityQueue;

#define PARENT(i) (((i) - 1) / 2)
#define LEFT(i) (2 * (i) + 1)
#define RIGHT(i) (2 * (i) + 2)

void count_frequencies(FILE *inputFile, int freq_map[MAX_CHARS]);
void PQ_INSERT(PriorityQueue *P, BinaryTreeNode *x);
BinaryTreeNode *PQ_EXTRACT(PriorityQueue *P);
void HEAPIFY(PriorityQueue *P, int idx);
BinaryTreeNode *BUILD_HUFFMAN_TREE(PriorityQueue *P);
void build_encoding_map(BinaryTreeNode *huffmanTree, char encoding_map[MAX_CHARS][MAX_CODE_LEN]);
void traverse_huffman_tree(BinaryTreeNode *root, char *current_code, int depth, char encoding_map[MAX_CHARS][MAX_CODE_LEN]);

typedef struct
{
    char c;
    char code[MAX_CODE_LEN];
} HuffmanCodeEntry;

int compare_huffman_codes(const void *a, const void *b);
void print_huffman_codes(char encoding_map[MAX_CHARS][MAX_CODE_LEN], int freq_map[MAX_CHARS]);

int main()
{
    FILE *inputFile = stdin;

    if (inputFile == NULL)
    {
        perror("Error opening input");
        return 1;
    }

    int freq_map[MAX_CHARS] = {0};
        count_frequencies(inputFile, freq_map);

    PriorityQueue P;
    P.heap_size = 0;

    for (int i = 0; i < MAX_CHARS; i++)
    {
        if (freq_map[i] > 0)
        {
            BinaryTreeNode *newNode = (BinaryTreeNode *)malloc(sizeof(BinaryTreeNode));
            if (!newNode)
            {
                perror("Failed to allocate memory for node");
                exit(EXIT_FAILURE);
            }
            newNode->data = (char)i;
            newNode->freq = freq_map[i];
            newNode->left = NULL;
            newNode->right = NULL;
            PQ_INSERT(&P, newNode);
        }
    }

    BinaryTreeNode *huffmanTree = BUILD_HUFFMAN_TREE(&P);

    char encoding_map[MAX_CHARS][MAX_CODE_LEN];
    for (int i = 0; i < MAX_CHARS; i++)
    {
        encoding_map[i][0] = '\0';
    }
    if (huffmanTree)
    {
        build_encoding_map(huffmanTree, encoding_map);
    }

    print_huffman_codes(encoding_map, freq_map);

    return 0;
}

void count_frequencies(FILE *inputFile, int freq_map[MAX_CHARS])
{
    int c;
    while ((c = fgetc(inputFile)) != EOF && c != ';')
    {
        freq_map[(unsigned char)c]++;
    }
}

void HEAPIFY(PriorityQueue *P, int idx)
{
    int l = LEFT(idx);
    int r = RIGHT(idx);
    int smallest = idx;

    if (l < P->heap_size && P->A[l]->freq < P->A[smallest]->freq)
    {
        smallest = l;
    }
    if (r < P->heap_size && P->A[r]->freq < P->A[smallest]->freq)
    {
        smallest = r;
    }

    if (smallest != idx)
    {
        BinaryTreeNode *temp = P->A[idx];
        P->A[idx] = P->A[smallest];
        P->A[smallest] = temp;
        HEAPIFY(P, smallest);
    }
}

void PQ_INSERT(PriorityQueue *P, BinaryTreeNode *x)
{
    if (P->heap_size == MAX_CHARS)
    {
        return;
    }

    P->heap_size++;
    int i = P->heap_size - 1;
    P->A[i] = x;

    while (i > 0 && P->A[PARENT(i)]->freq > P->A[i]->freq)
    {
        BinaryTreeNode *temp = P->A[i];
        P->A[i] = P->A[PARENT(i)];
        P->A[PARENT(i)] = temp;
        i = PARENT(i);
    }
}

BinaryTreeNode *PQ_EXTRACT(PriorityQueue *P)
{
    if (P->heap_size < 1)
    {
        return NULL;
    }

    BinaryTreeNode *min_node = P->A[0];
    P->A[0] = P->A[P->heap_size - 1];
    P->heap_size--;

    if (P->heap_size > 0)
    {
        HEAPIFY(P, 0);
    }
    return min_node;
}

BinaryTreeNode *BUILD_HUFFMAN_TREE(PriorityQueue *P)
{
    if (P->heap_size == 0)
    {
        return NULL;
    }

    if (P->heap_size == 1)
    {
        BinaryTreeNode *single_char_node = PQ_EXTRACT(P);

        BinaryTreeNode *dummy_parent = (BinaryTreeNode *)malloc(sizeof(BinaryTreeNode));
        if (!dummy_parent)
        {
            perror("Failed to allocate memory for dummy parent");
            exit(EXIT_FAILURE);
        }
        dummy_parent->data = 0;
        dummy_parent->freq = single_char_node->freq;
        dummy_parent->left = single_char_node;
        dummy_parent->right = NULL;
        return dummy_parent;
    }

    while (P->heap_size > 1)
    {
        BinaryTreeNode *left_child = PQ_EXTRACT(P);
        BinaryTreeNode *right_child = PQ_EXTRACT(P);

        BinaryTreeNode *internal_node = (BinaryTreeNode *)malloc(sizeof(BinaryTreeNode));
        if (!internal_node)
        {
            perror("Failed to allocate memory for internal node");
            exit(EXIT_FAILURE);
        }
        internal_node->data = 0;
        internal_node->freq = left_child->freq + right_child->freq;
        internal_node->left = left_child;
        internal_node->right = right_child;

        PQ_INSERT(P, internal_node);
    }
    return PQ_EXTRACT(P);
}

void build_encoding_map(BinaryTreeNode *huffmanTree, char encoding_map[MAX_CHARS][MAX_CODE_LEN])
{
    char current_code[MAX_CODE_LEN];
    traverse_huffman_tree(huffmanTree, current_code, 0, encoding_map);
}

void traverse_huffman_tree(BinaryTreeNode *root, char *current_code, int depth, char encoding_map[MAX_CHARS][MAX_CODE_LEN])
{
    if (root == NULL)
    {
        return;
    }

    if (root->left == NULL && root->right == NULL)
    {
        current_code[depth] = '\0';
        strcpy(encoding_map[(unsigned char)root->data], current_code);
        return;
    }

    if (root->left)
    {
        current_code[depth] = '0';
        traverse_huffman_tree(root->left, current_code, depth + 1, encoding_map);
    }

    if (root->right)
    {
        current_code[depth] = '1';
        traverse_huffman_tree(root->right, current_code, depth + 1, encoding_map);
    }
}

int compare_huffman_codes(const void *a, const void *b)
{
    HuffmanCodeEntry *entryA = (HuffmanCodeEntry *)a;
    HuffmanCodeEntry *entryB = (HuffmanCodeEntry *)b;
    int cmp = strcmp(entryA->code, entryB->code);
    if (cmp == 0)
    {
        return entryA->c - entryB->c;
    }
    return cmp;
}

void print_huffman_codes(char encoding_map[MAX_CHARS][MAX_CODE_LEN], int freq_map[MAX_CHARS])
{
    HuffmanCodeEntry sorted_codes[MAX_CHARS];
    int num_entries = 0;

    for (int i = 0; i < MAX_CHARS; i++)
    {
        if (freq_map[i] > 0)
        {
            sorted_codes[num_entries].c = (char)i;
            if (encoding_map[i][0] != '\0')
            {
                strcpy(sorted_codes[num_entries].code, encoding_map[i]);
            }
            else if (freq_map[i] > 0 && encoding_map[i][0] == '\0')
            {
                strcpy(sorted_codes[num_entries].code, "0");
            }
            else
            {
                strcpy(sorted_codes[num_entries].code, "");
            }
            num_entries++;
        }
    }

    if (num_entries == 0)
        return;

    qsort(sorted_codes, num_entries, sizeof(HuffmanCodeEntry), compare_huffman_codes);

    for (int i = 0; i < num_entries; i++)
    {
        if (sorted_codes[i].c == '\n')
        {
            printf(" :: %s\n", sorted_codes[i].code);
        }
        else if (sorted_codes[i].c == '\t')
        {
            printf("\\t :: %s\n", sorted_codes[i].code);
        }
        else
        {
            printf("%c :: %s\n", sorted_codes[i].c, sorted_codes[i].code);
        }
    }
}