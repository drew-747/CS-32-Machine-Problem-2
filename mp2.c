#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define newline printf("\n")

typedef struct binarytreenode BinaryTreeNode;
struct binarytreenode
{
    BinaryTreeNode *LSON;
    char DATA;
    int FREQ;
    BinaryTreeNode *RSON;
    unsigned char min_char_in_subtree;
};

typedef struct priorityqueue
{
    int size;
    int capacity;
    BinaryTreeNode **array;
} PriorityQueue;

void PQ_INSERT(PriorityQueue *, BinaryTreeNode *);
BinaryTreeNode *PQ_EXTRACT(PriorityQueue *);
void HEAPIFY(PriorityQueue *, int);
void PQ_OVERFLOW(void);
void PQ_UNDERFLOW(void);

void CREATE_PQ(PriorityQueue **);
BinaryTreeNode *BUILD_HUFFMAN_TREE(PriorityQueue *);
void PRINT_HUFFMAN_CODE(BinaryTreeNode *, int *, int);
BinaryTreeNode **read_input(int *);
void FREE_PQ(PriorityQueue *);
void FREE_TREE(BinaryTreeNode *);

int is_smaller(BinaryTreeNode *n1, BinaryTreeNode *n2)
{
    if (n1->FREQ < n2->FREQ)
        return 1;
    if (n1->FREQ > n2->FREQ)
        return 0;

    int n1_is_input_leaf = (n1->DATA != '\0');
    int n2_is_input_leaf = (n2->DATA != '\0');

    if (n1_is_input_leaf && !n2_is_input_leaf)
        return 1;
    if (!n1_is_input_leaf && n2_is_input_leaf)
        return 0;

    return n1->min_char_in_subtree < n2->min_char_in_subtree;
}

int main()
{
    PriorityQueue *HuffmanCode;
    BinaryTreeNode *HuffmanRoot;

    CREATE_PQ(&HuffmanCode);
    HuffmanRoot = BUILD_HUFFMAN_TREE(HuffmanCode);
    int *code = (int *)malloc(HuffmanCode->capacity * sizeof(int));
    PRINT_HUFFMAN_CODE(HuffmanRoot, code, 0);
    FREE_TREE(HuffmanRoot);
    FREE_PQ(HuffmanCode);
    free(code);
    return 0;
}

BinaryTreeNode *create_node(char data)
{
    BinaryTreeNode *node = NULL;

    node = (BinaryTreeNode *)malloc(sizeof(BinaryTreeNode));
    if (node == NULL)
    {
        perror("Failed to allocate node");
        exit(EXIT_FAILURE);
    }
    node->DATA = data;
    node->FREQ = 1;
    node->LSON = NULL;
    node->RSON = NULL;
    node->min_char_in_subtree = (unsigned char)data;

    return node;
}

void free_node(BinaryTreeNode *node)
{
    free(node);
    return;
}

void free_node_array(BinaryTreeNode **array, int size)
{
    int i;

    for (i = 0; i < size; i++)
        free_node(array[i]);

    return;
}

BinaryTreeNode *search_node_array(BinaryTreeNode **array, int size, char data)
{
    int i;

    for (i = 0; i < size; i++)
        if (array[i]->DATA == data)
            return array[i];

    return NULL;
}

BinaryTreeNode **read_input(int *size)
{
    int n = 0, i;
    char symbol;
    BinaryTreeNode **array = NULL, **temp;
    BinaryTreeNode *node = NULL;

    while (scanf("%c", &symbol) != EOF)
    {
        if (n == 0)
        {
            array = (BinaryTreeNode **)malloc(sizeof(BinaryTreeNode *));
            array[0] = create_node(symbol);
            n++;
        }
        else
        {
            node = search_node_array(array, n, symbol);

            if (node == NULL)
            {
                temp = array;
                array = (BinaryTreeNode **)malloc((n + 1) * sizeof(BinaryTreeNode *));
                for (i = 0; i < n; i++)
                    array[i] = temp[i];
                array[n] = create_node(symbol);
                free(temp);
                n++;
            }
            else
            {
                node->FREQ++;
            }
        }
    }

    *size = n;

    return array;
}

void CREATE_PQ(PriorityQueue **P)
{
    BinaryTreeNode **array = NULL;
    BinaryTreeNode *x;
    int i, n;

    *P = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    array = read_input(&(*P)->capacity);

    (*P)->array = array;
    (*P)->size = (*P)->capacity;

    n = (*P)->size - 1;
    for (i = (n - 1) / 2; i >= 0; i--)
        HEAPIFY(*P, i);

    return;
}

void FREE_PQ(PriorityQueue *P)
{

    free(P->array);
    free(P);
    return;
}

void FREE_TREE(BinaryTreeNode *alpha)
{
    if (alpha != NULL)
    {
        FREE_TREE(alpha->LSON);
        FREE_TREE(alpha->RSON);
        free(alpha);
    }
}

void PQ_INSERT(PriorityQueue *P, BinaryTreeNode *x)
{
    if (P->size == P->capacity)
    {
        PQ_OVERFLOW();
        return;
    }
    int i = P->size;
    P->array[i] = x;
    P->size++;
    while (i > 0)
    {
        int parent = (i - 1) / 2;
        if (is_smaller(P->array[i], P->array[parent]))
        {
            BinaryTreeNode *tmp = P->array[i];
            P->array[i] = P->array[parent];
            P->array[parent] = tmp;
            i = parent;
        }
        else
            break;
    }
}

BinaryTreeNode *PQ_EXTRACT(PriorityQueue *P)
{
    if (P->size == 0)
    {
        PQ_UNDERFLOW();
        return NULL;
    }
    BinaryTreeNode *min_node = P->array[0];
    P->array[0] = P->array[P->size - 1];
    P->size--;
    HEAPIFY(P, 0);
    return min_node;
}

void HEAPIFY(PriorityQueue *P, int idx)
{
    int smallest = idx;
    int l = 2 * idx + 1;
    int r = 2 * idx + 2;
    if (l < P->size && is_smaller(P->array[l], P->array[smallest]))
        smallest = l;
    if (r < P->size && is_smaller(P->array[r], P->array[smallest]))
        smallest = r;
    if (smallest != idx)
    {
        BinaryTreeNode *tmp = P->array[idx];
        P->array[idx] = P->array[smallest];
        P->array[smallest] = tmp;
        HEAPIFY(P, smallest);
    }
}

BinaryTreeNode *BUILD_HUFFMAN_TREE(PriorityQueue *P)
{
    if (P == NULL || P->size == 0)
        return NULL;
    while (P->size > 1)
    {
        BinaryTreeNode *left = PQ_EXTRACT(P);
        BinaryTreeNode *right = PQ_EXTRACT(P);
        BinaryTreeNode *internal = (BinaryTreeNode *)malloc(sizeof(BinaryTreeNode));
        if (!internal)
        {
            perror("Failed to allocate internal node");
            exit(EXIT_FAILURE);
        }
        internal->DATA = '\0';
        internal->FREQ = left->FREQ + right->FREQ;
        internal->LSON = left;
        internal->RSON = right;
        internal->min_char_in_subtree = (left->min_char_in_subtree < right->min_char_in_subtree) ? left->min_char_in_subtree : right->min_char_in_subtree;
        PQ_INSERT(P, internal);
    }
    return PQ_EXTRACT(P);
}

void PRINT_HUFFMAN_CODE(BinaryTreeNode *root, int *code, int top)
{
    int i;

    if (root->LSON != NULL)
    {
        code[top] = 0;
        PRINT_HUFFMAN_CODE(root->LSON, code, top + 1);
    }

    if (root->RSON != NULL)
    {
        code[top] = 1;
        PRINT_HUFFMAN_CODE(root->RSON, code, top + 1);
    }

    if (root->LSON == NULL && root->RSON == NULL)
    {
        printf("%c :: ", root->DATA);
        for (i = 0; i < top; i++)
            printf("%d", code[i]);
        newline;
    }
}

void PQ_OVERFLOW(void)
{
    printf("Priority Queue overflow detected.\n");
    exit(1);
}

void PQ_UNDERFLOW(void)
{
    printf("Priority Queue underflow detected.\n");
    exit(1);
}
