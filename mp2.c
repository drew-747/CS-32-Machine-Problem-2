#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARS 256

typedef struct BinaryTreeNode
{
    char ch;
    int freq;
    struct BinaryTreeNode *left, *right;
    unsigned char min_char_in_subtree;
} BinaryTreeNode;

typedef struct
{
    BinaryTreeNode **data;
    int size;
    int capacity;
} PriorityQueue;

typedef struct
{
    char ch;
    char code[MAX_CHARS];
} HuffmanCode;

void PQ_INSERT(PriorityQueue *P, BinaryTreeNode *x);
BinaryTreeNode *PQ_EXTRACT(PriorityQueue *P);
void HEAPIFY(PriorityQueue *P, int idx);
BinaryTreeNode *BUILD_HUFFMAN_TREE(PriorityQueue *P);
void swap_nodes(BinaryTreeNode **a, BinaryTreeNode **b);
int parent(int i);
int left(int i);
int right(int i);
PriorityQueue *create_priority_queue(int capacity);
BinaryTreeNode *create_node(char ch_val, int freq_val);
void generate_codes_recursive(BinaryTreeNode *root, char *current_code_str, int depth, HuffmanCode *codes_array, int *codes_count);
int compare_huffman_codes(const void *a, const void *b);
void free_huffman_tree(BinaryTreeNode *root);

void swap_nodes(BinaryTreeNode **a, BinaryTreeNode **b)
{
    BinaryTreeNode *tmp = *a;
    *a = *b;
    *b = tmp;
}

int parent(int i) { return (i - 1) / 2; }
int left(int i) { return 2 * i + 1; }
int right(int i) { return 2 * i + 2; }

int is_less_than(BinaryTreeNode *n1, BinaryTreeNode *n2)
{
    if (n1->freq < n2->freq)
        return 1;
    if (n1->freq > n2->freq)
        return 0;

    return n1->min_char_in_subtree < n2->min_char_in_subtree;
}

void HEAPIFY(PriorityQueue *P, int idx)
{
    int l = left(idx);
    int r = right(idx);
    int smallest = idx;

    if (l < P->size && is_less_than(P->data[l], P->data[smallest]))
        smallest = l;
    if (r < P->size && is_less_than(P->data[r], P->data[smallest]))
        smallest = r;

    if (smallest != idx)
    {
        swap_nodes(&P->data[idx], &P->data[smallest]);
        HEAPIFY(P, smallest);
    }
}

void PQ_INSERT(PriorityQueue *P, BinaryTreeNode *x)
{
    if (P->size == P->capacity)
    {
        P->capacity *= 2;
        P->data = realloc(P->data, P->capacity * sizeof(BinaryTreeNode *));
        if (!P->data)
        {
            perror("Failed to reallocate memory for priority queue");
            exit(EXIT_FAILURE);
        }
    }
    int i = P->size++;
    P->data[i] = x;

    while (i != 0 && is_less_than(P->data[i], P->data[parent(i)]))
    {
        swap_nodes(&P->data[i], &P->data[parent(i)]);
        i = parent(i);
    }
}

BinaryTreeNode *PQ_EXTRACT(PriorityQueue *P)
{
    if (P->size == 0)
        return NULL;
    BinaryTreeNode *min_node = P->data[0];
    P->data[0] = P->data[--P->size];
    if (P->size > 0)
    {
        HEAPIFY(P, 0);
    }
    return min_node;
}

PriorityQueue *create_priority_queue(int capacity)
{
    PriorityQueue *P = malloc(sizeof(PriorityQueue));
    if (!P)
    {
        perror("Failed to allocate memory for priority queue");
        exit(EXIT_FAILURE);
    }
    P->data = malloc(capacity * sizeof(BinaryTreeNode *));
    if (!P->data)
    {
        perror("Failed to allocate memory for priority queue data");
        free(P);
        exit(EXIT_FAILURE);
    }
    P->size = 0;
    P->capacity = capacity;
    return P;
}

BinaryTreeNode *create_node(char ch_val, int freq_val)
{
    BinaryTreeNode *node = malloc(sizeof(BinaryTreeNode));
    if (!node)
    {
        perror("Failed to allocate memory for node");
        exit(EXIT_FAILURE);
    }
    node->ch = ch_val;
    node->freq = freq_val;
    node->min_char_in_subtree = (unsigned char)ch_val;
    node->left = node->right = NULL;
    return node;
}

BinaryTreeNode *BUILD_HUFFMAN_TREE(PriorityQueue *P)
{
    if (P == NULL || P->size == 0)
        return NULL;

    while (P->size > 1)
    {
        BinaryTreeNode *left_c = PQ_EXTRACT(P);
        BinaryTreeNode *right_c = PQ_EXTRACT(P);

        BinaryTreeNode *parent_node = create_node('\0', left_c->freq + right_c->freq);

        parent_node->min_char_in_subtree = (left_c->min_char_in_subtree < right_c->min_char_in_subtree) ? left_c->min_char_in_subtree : right_c->min_char_in_subtree;

        parent_node->left = left_c;
        parent_node->right = right_c;

        PQ_INSERT(P, parent_node);
    }
    return PQ_EXTRACT(P);
}

void generate_codes_recursive(BinaryTreeNode *root, char *current_code_str, int depth, HuffmanCode *codes_array, int *codes_count)
{
    if (!root)
        return;

    if (!root->left && !root->right)
    {
        current_code_str[depth] = '\0';
        codes_array[*codes_count].ch = root->ch;
        strcpy(codes_array[*codes_count].code, current_code_str);
        (*codes_count)++;
        return;
    }

    current_code_str[depth] = '0';
    generate_codes_recursive(root->left, current_code_str, depth + 1, codes_array, codes_count);

    current_code_str[depth] = '1';
    generate_codes_recursive(root->right, current_code_str, depth + 1, codes_array, codes_count);
}

int compare_huffman_codes(const void *a, const void *b)
{
    HuffmanCode *hc1 = (HuffmanCode *)a;
    HuffmanCode *hc2 = (HuffmanCode *)b;
    return strcmp(hc1->code, hc2->code);
}

void free_huffman_tree(BinaryTreeNode *root)
{
    if (root == NULL)
    {
        return;
    }
    free_huffman_tree(root->left);
    free_huffman_tree(root->right);
    free(root);
}

int main()
{
    int freq[MAX_CHARS] = {0};
    int c;
    int unique_char_count = 0;
    while ((c = getchar()) != EOF && c != ';')
    {
        if (freq[(unsigned char)c] == 0)
        {
            unique_char_count++;
        }
        freq[(unsigned char)c]++;
    }

    if (unique_char_count == 0)
    {
        return 0;
    }

    PriorityQueue *P = create_priority_queue(unique_char_count > 0 ? unique_char_count : 16);
    for (int i = 0; i < MAX_CHARS; ++i)
    {
        if (freq[i] > 0)
        {
            PQ_INSERT(P, create_node((char)i, freq[i]));
        }
    }

    BinaryTreeNode *root = BUILD_HUFFMAN_TREE(P);

    HuffmanCode codes_array[MAX_CHARS];
    int codes_count = 0;
    char current_code_str[MAX_CHARS];

    if (root)
    {
        if (unique_char_count == 1)
        {
            codes_array[0].ch = root->ch;
            strcpy(codes_array[0].code, "0");
            codes_count = 1;
        }
        else
        {
            generate_codes_recursive(root, current_code_str, 0, codes_array, &codes_count);
        }
    }

    qsort(codes_array, codes_count, sizeof(HuffmanCode), compare_huffman_codes);

    for (int i = 0; i < codes_count; ++i)
    {
        printf("%c :: %s\n", codes_array[i].ch, codes_array[i].code);
    }

    free_huffman_tree(root);
    if (P)
    {
        free(P->data);
        free(P);
    }

    return 0;
}
