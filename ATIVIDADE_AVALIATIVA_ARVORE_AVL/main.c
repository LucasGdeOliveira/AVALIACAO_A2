#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura de usuario
typedef struct {
    int id;
    char nome[100];
    char email[100];
} Usuario;

// Estrutura do no da arvore AVL
typedef struct NoAVL {
    Usuario user;
    int altura;
    struct NoAVL *esq;
    struct NoAVL *dir;
} NoAVL;

// Funcao para retornar a altura de um no
int altura(NoAVL *no) {
    if (no == NULL) return 0;
    return no->altura;
}

// Funcao para obter o maior entre dois inteiros
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Funcao para criar um novo no
NoAVL* criarNo(Usuario u) {
    NoAVL *novo = (NoAVL*)malloc(sizeof(NoAVL));
    novo->user = u;
    novo->altura = 1;
    novo->esq = novo->dir = NULL;
    return novo;
}

// Rotacao simples a direita
NoAVL* rotacaoDireita(NoAVL *y) {
    NoAVL *x = y->esq;
    NoAVL *T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;

    return x;
}

// Rotacao simples a esquerda
NoAVL* rotacaoEsquerda(NoAVL *x) {
    NoAVL *y = x->dir;
    NoAVL *T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;

    return y;
}

// Fator de balanceamento de um no
int fatorBalanceamento(NoAVL *no) {
    if (no == NULL) return 0;
    return altura(no->esq) - altura(no->dir);
}

// Insercao na arvore AVL
NoAVL* inserir(NoAVL *no, Usuario u) {
    if (no == NULL) return criarNo(u);

    if (strcmp(u.nome, no->user.nome) < 0)
        no->esq = inserir(no->esq, u);
    else if (strcmp(u.nome, no->user.nome) > 0)
        no->dir = inserir(no->dir, u);
    else
        return no;

    no->altura = 1 + max(altura(no->esq), altura(no->dir));
    int fb = fatorBalanceamento(no);

    if (fb > 1 && strcmp(u.nome, no->esq->user.nome) < 0)
        return rotacaoDireita(no);
    if (fb < -1 && strcmp(u.nome, no->dir->user.nome) > 0)
        return rotacaoEsquerda(no);
    if (fb > 1 && strcmp(u.nome, no->esq->user.nome) > 0) {
        no->esq = rotacaoEsquerda(no->esq);
        return rotacaoDireita(no);
    }
    if (fb < -1 && strcmp(u.nome, no->dir->user.nome) < 0) {
        no->dir = rotacaoDireita(no->dir);
        return rotacaoEsquerda(no);
    }

    return no;
}

// Funcao para encontrar o menor valor na arvore
NoAVL* minimoValor(NoAVL *no) {
    NoAVL *atual = no;
    while (atual->esq != NULL)
        atual = atual->esq;
    return atual;
}

// Funcao para remover um no da arvore AVL
NoAVL* remover(NoAVL *raiz, char nome[]) {
    if (raiz == NULL) return raiz;

    if (strcmp(nome, raiz->user.nome) < 0)
        raiz->esq = remover(raiz->esq, nome);
    else if (strcmp(nome, raiz->user.nome) > 0)
        raiz->dir = remover(raiz->dir, nome);
    else {
        if (raiz->esq == NULL || raiz->dir == NULL) {
            NoAVL *temp = raiz->esq ? raiz->esq : raiz->dir;
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else {
                *raiz = *temp;
            }
            free(temp);
        } else {
            NoAVL *temp = minimoValor(raiz->dir);
            raiz->user = temp->user;
            raiz->dir = remover(raiz->dir, temp->user.nome);
        }
    }

    if (raiz == NULL) return raiz;

    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));
    int fb = fatorBalanceamento(raiz);

    if (fb > 1 && fatorBalanceamento(raiz->esq) >= 0)
        return rotacaoDireita(raiz);
    if (fb > 1 && fatorBalanceamento(raiz->esq) < 0) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }
    if (fb < -1 && fatorBalanceamento(raiz->dir) <= 0)
        return rotacaoEsquerda(raiz);
    if (fb < -1 && fatorBalanceamento(raiz->dir) > 0) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

// Busca por nome
NoAVL* buscar(NoAVL *raiz, char nome[]) {
    if (raiz == NULL || strcmp(raiz->user.nome, nome) == 0)
        return raiz;

    if (strcmp(nome, raiz->user.nome) < 0)
        return buscar(raiz->esq, nome);
    else
        return buscar(raiz->dir, nome);
}

// Listagem in-order (alfabetica)
void listarEmOrdem(NoAVL *raiz) {
    if (raiz != NULL) {
        listarEmOrdem(raiz->esq);
        printf("ID: %d | Nome: %s | Email: %s\n", raiz->user.id, raiz->user.nome, raiz->user.email);
        listarEmOrdem(raiz->dir);
    }
}

// Menu principal
int main() {
    NoAVL *raiz = NULL;
    int opcao, id = 1;
    char nome[100], email[100];
    Usuario u;
    NoAVL *encontrado = NULL;

    do {
        printf("\n====== MENU ======\n");
        printf("1. Cadastrar usuario\n");
        printf("2. Remover usuario\n");
        printf("3. Buscar usuario\n");
        printf("4. Listar usuarios em ordem alfabetica\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpar buffer

        switch (opcao) {
            case 1:
                printf("Nome: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0;

                printf("Email: ");
                fgets(email, sizeof(email), stdin);
                email[strcspn(email, "\n")] = 0;

                u.id = id++;
                strcpy(u.nome, nome);
                strcpy(u.email, email);

                raiz = inserir(raiz, u);
                printf("Usuario cadastrado!\n");
                break;

            case 2:
                printf("Nome do usuario a remover: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0;

                raiz = remover(raiz, nome);
                printf("Usuario removido, se existia.\n");
                break;

            case 3:
                printf("Nome do usuario a buscar: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0;

                encontrado = buscar(raiz, nome);
                if (encontrado)
                    printf("Encontrado - ID: %d | Nome: %s | Email: %s\n", encontrado->user.id, encontrado->user.nome, encontrado->user.email);
                else
                    printf("Usuario nao encontrado.\n");
                break;

            case 4:
                printf("Lista de usuarios em ordem alfabetica:\n");
                listarEmOrdem(raiz);
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}
