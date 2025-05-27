#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define newline printf("\n")

typedef struct binarytreenode BinaryTreeNode;
struct binarytreenode {
    BinaryTreeNode *LSON;
    char DATA;
    int FREQ;
    BinaryTreeNode *RSON;
};

typedef struct priorityqueue {
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

int main() {
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

BinaryTreeNode *create_node(char data) {
    BinaryTreeNode *node = NULL;
    node = (BinaryTreeNode *)malloc(sizeof(BinaryTreeNode));
    node->DATA = data;
    node->FREQ = 1;
    node->LSON = NULL;
    node->RSON = NULL;
    return node;
}

void free_node(BinaryTreeNode *node) {
    free(node);
    return;
}

void free_node_array(BinaryTreeNode **array, int size) {
    int i;
    for (i = 0; i < size; i++)
        free_node(array[i]);
    return;
}

BinaryTreeNode *search_node_array(BinaryTreeNode **array, int size, char data) {
    int i;
    for (i = 0; i < size; i++)
        if (array[i]->DATA == data)
            return array[i];
    return NULL;
}

BinaryTreeNode **read_input(int *size) {
    int n = 0, i;
    char symbol;
    BinaryTreeNode **array = NULL, **temp;
    BinaryTreeNode *node = NULL;

    while (scanf("%c", &symbol) != EOF) {
        if (n == 0) {
            array = (BinaryTreeNode **)malloc(sizeof(BinaryTreeNode *));
            array[0] = create_node(symbol);
            n++;
        } else {
            node = search_node_array(array, n, symbol);
            if (node == NULL) {
                temp = array;
                array = (BinaryTreeNode **)malloc((n + 1) * sizeof(BinaryTreeNode *));
                for (i = 0; i < n; i++)
                    array[i] = temp[i];
                array[n] = create_node(symbol);
                free(temp);
                n++;
            } else {
                node->FREQ++;
            }
        }
    }

    *size = n;
    return array;
}

void CREATE_PQ(PriorityQueue **P) {
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

void FREE_PQ(PriorityQueue *P) {
    free(P->array);
    free(P);
    return;
}

void FREE_TREE(BinaryTreeNode *alpha) {
    if (alpha != NULL) {
        FREE_TREE(alpha->LSON);
        FREE_TREE(alpha->RSON);
        free(alpha);
    }
}

void PQ_INSERT(PriorityQueue *P, BinaryTreeNode *x) {
    if (P->size == P->capacity)
        PQ_OVERFLOW();

    int i = P->size;
    P->array[i] = x;
    P->size++;

    while (i > 0 && P->array[(i - 1) / 2]->FREQ > P->array[i]->FREQ) {
        BinaryTreeNode *temp = P->array[i];
        P->array[i] = P->array[(i - 1) / 2];
        P->array[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
    return;
}

BinaryTreeNode *PQ_EXTRACT(PriorityQueue *P) {
    if (P->size == 0)
        PQ_UNDERFLOW();

    BinaryTreeNode *x = P->array[0];
    P->array[0] = P->array[P->size - 1];
    P->size--;
    HEAPIFY(P, 0);
    return x;
}

void HEAPIFY(PriorityQueue *P, int r) {
    int smallest = r;
    int left = 2 * r + 1;
    int right = 2 * r + 2;

    if (left < P->size && P->array[left]->FREQ < P->array[smallest]->FREQ)
        smallest = left;

    if (right < P->size && P->array[right]->FREQ < P->array[smallest]->FREQ)
        smallest = right;

    if (smallest != r) {
        BinaryTreeNode *temp = P->array[r];
        P->array[r] = P->array[smallest];
        P->array[smallest] = temp;
        HEAPIFY(P, smallest);
    }
}

BinaryTreeNode *BUILD_HUFFMAN_TREE(PriorityQueue *P) {
    while (P->size > 1) {
        BinaryTreeNode *x = PQ_EXTRACT(P);
        BinaryTreeNode *y = PQ_EXTRACT(P);

        BinaryTreeNode *z = (BinaryTreeNode *)malloc(sizeof(BinaryTreeNode));
        z->LSON = x;
        z->RSON = y;
        z->DATA = '*';
        z->FREQ = x->FREQ + y->FREQ;

        PQ_INSERT(P, z);
    }
    return PQ_EXTRACT(P);
}

void PRINT_HUFFMAN_CODE(BinaryTreeNode *root, int *code, int top) {
    int i;
    if (root->LSON != NULL) {
        code[top] = 0;
        PRINT_HUFFMAN_CODE(root->LSON, code, top + 1);
    }

    if (root->RSON != NULL) {
        code[top] = 1;
        PRINT_HUFFMAN_CODE(root->RSON, code, top + 1);
    }

    if (root->LSON == NULL && root->RSON == NULL) {
        printf("%c :: ", root->DATA);
        for (i = 0; i < top; i++)
            printf("%d", code[i]);
        newline;
    }
}

void PQ_OVERFLOW(void) {
    printf("Priority Queue overflow detected.\n");
    exit(1);
}

void PQ_UNDERFLOW(void) {
    printf("Priority Queue underflow detected.\n");
    exit(1);
}
