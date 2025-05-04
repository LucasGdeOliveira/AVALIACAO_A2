#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { RED, BLACK } Color;

typedef struct Product {
    int code;
    char name[100];
    int quantity;
    float price;
} Product;

typedef struct Node {
    Product product;
    Color color;
    struct Node *left, *right, *parent;
} Node;

Node *root = NULL;

// Funcao auxiliar para criar um novo no
Node* createNode(Product product) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->product = product;
    node->color = RED;
    node->left = node->right = node->parent = NULL;
    return node;
}

// Rotacao a esquerda
void rotateLeft(Node **root, Node *x) {
    Node *y = x->right;
    x->right = y->left;
    if (y->left != NULL) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// Rotacao a direita
void rotateRight(Node **root, Node *y) {
    Node *x = y->left;
    y->left = x->right;
    if (x->right != NULL) x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == NULL)
        *root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else y->parent->left = x;
    x->right = y;
    y->parent = x;
}

// Balanceamento apos insercao
void fixInsertion(Node **root, Node *z) {
    while (z->parent && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node *y = z->parent->parent->right;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotateLeft(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotateRight(root, z->parent->parent);
            }
        } else {
            Node *y = z->parent->parent->left;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotateRight(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotateLeft(root, z->parent->parent);
            }
        }
    }
    (*root)->color = BLACK;
}

// Insercao de produto na arvore
void insertProduct(Product product) {
    Node *z = createNode(product);
    Node *y = NULL;
    Node *x = root;
    while (x != NULL) {
        y = x;
        if (z->product.code < x->product.code)
            x = x->left;
        else if (z->product.code > x->product.code)
            x = x->right;
        else {
            printf("Codigo ja cadastrado!\n");
            free(z);
            return;
        }
    }
    z->parent = y;
    if (y == NULL) root = z;
    else if (z->product.code < y->product.code)
        y->left = z;
    else
        y->right = z;
    fixInsertion(&root, z);
    printf("Produto cadastrado com sucesso!\n");
}

// Busca produto por codigo
Node* search(Node *root, int code) {
    if (root == NULL || code == root->product.code)
        return root;
    if (code < root->product.code)
        return search(root->left, code);
    return search(root->right, code);
}

// Minimo da arvore
Node* treeMinimum(Node* node) {
    while (node->left != NULL)
        node = node->left;
    return node;
}

// Transplanta dois nos
void transplant(Node **root, Node *u, Node *v) {
    if (u->parent == NULL)
        *root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v != NULL)
        v->parent = u->parent;
}

// Balanceamento apos remocao
void fixDeletion(Node **root, Node *x) {
    while (x != *root && (x == NULL || x->color == BLACK)) {
        if (x == x->parent->left) {
            Node *w = x->parent->right;
            if (w && w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateLeft(root, x->parent);
                w = x->parent->right;
            }
            if ((!w->left || w->left->color == BLACK) &&
                (!w->right || w->right->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (!w->right || w->right->color == BLACK) {
                    if (w->left) w->left->color = BLACK;
                    w->color = RED;
                    rotateRight(root, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->right) w->right->color = BLACK;
                rotateLeft(root, x->parent);
                x = *root;
            }
        } else {
            Node *w = x->parent->left;
            if (w && w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateRight(root, x->parent);
                w = x->parent->left;
            }
            if ((!w->right || w->right->color == BLACK) &&
                (!w->left || w->left->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (!w->left || w->left->color == BLACK) {
                    if (w->right) w->right->color = BLACK;
                    w->color = RED;
                    rotateLeft(root, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->left) w->left->color = BLACK;
                rotateRight(root, x->parent);
                x = *root;
            }
        }
    }
    if (x) x->color = BLACK;
}

// Remocao de produto
void deleteProduct(int code) {
    Node *z = search(root, code);
    if (z == NULL) {
        printf("Produto nao encontrado.\n");
        return;
    }

    Node *y = z;
    Color y_original_color = y->color;
    Node *x;

    if (z->left == NULL) {
        x = z->right;
        transplant(&root, z, z->right);
    } else if (z->right == NULL) {
        x = z->left;
        transplant(&root, z, z->left);
    } else {
        y = treeMinimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x) x->parent = y;
        } else {
            transplant(&root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(&root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free(z);

    if (y_original_color == BLACK && x != NULL)
        fixDeletion(&root, x);

    printf("Produto removido com sucesso!\n");
}

// Listagem in-order
void listProducts(Node *node) {
    if (node == NULL) return;
    listProducts(node->left);
    printf("Codigo: %d | Nome: %s | Qtd: %d | Preco: R$%.2f\n",
           node->product.code, node->product.name, node->product.quantity, node->product.price);
    listProducts(node->right);
}

// Menu principal
void menu() {
    int op, code;
    Product p;

    do {
        printf("\n==== SISTEMA DE INVENTARIO ====\n");
        printf("1 - Cadastrar Produto\n");
        printf("2 - Remover Produto\n");
        printf("3 - Buscar Produto\n");
        printf("4 - Listar Produtos\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                printf("Codigo: "); scanf("%d", &p.code);
                printf("Nome: "); getchar(); fgets(p.name, 100, stdin); p.name[strcspn(p.name, "\n")] = 0;
                printf("Quantidade: "); scanf("%d", &p.quantity);
                printf("Preco: "); scanf("%f", &p.price);
                insertProduct(p);
                break;
            case 2:
                printf("Codigo a remover: "); scanf("%d", &code);
                deleteProduct(code);
                break;
            case 3: {
                printf("Codigo a buscar: "); scanf("%d", &code);
                Node *found = search(root, code);
                if (found) {
                    printf("Encontrado: %s - Qtd: %d - R$%.2f\n",
                           found->product.name, found->product.quantity, found->product.price);
                } else {
                    printf("Produto nao encontrado.\n");
                }
                break;
            }
            case 4:
                printf("Produtos cadastrados (em ordem):\n");
                listProducts(root);
                break;
            case 0:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (op != 0);
}

int main() {
    menu();
    return 0;
}
