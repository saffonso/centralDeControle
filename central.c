#include "central.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// cria e copia os dados para os nos
static NoVilao* criar_no_vilao(const Vilao* vilao) {
    NoVilao* novo = (NoVilao*)malloc(sizeof(NoVilao));
    if (!novo) {
        fprintf(stderr, "Erro: falha ao alocar NoVilao.\n");
        exit(EXIT_FAILURE);
    }
    // copia para os buffers
    strncpy(novo->dados.nome, vilao->nome, sizeof(novo->dados.nome) - 1);
    novo->dados.nome[sizeof(novo->dados.nome) - 1] = '\0';
    novo->dados.nivel_habilidade = vilao->nivel_habilidade;
    strncpy(novo->dados.ponto_fraco, vilao->ponto_fraco, sizeof(novo->dados.ponto_fraco) - 1);
    novo->dados.ponto_fraco[sizeof(novo->dados.ponto_fraco) - 1] = '\0';
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

static int comparar_nomes(const char* a, const char* b) {
    return strcmp(a, b);
}

// funcao de insercao, se ja existir nome igual, so atualiza os dados
NoVilao* inserir_vilao_bst(NoVilao* raiz, Vilao novo_vilao) {
    if (raiz == NULL) {
        return criar_no_vilao(&novo_vilao);
    }
    int cmp = comparar_nomes(novo_vilao.nome, raiz->dados.nome);
    if (cmp < 0) {
        raiz->esquerda = inserir_vilao_bst(raiz->esquerda, novo_vilao);
    } else if (cmp > 0) {
        raiz->direita = inserir_vilao_bst(raiz->direita, novo_vilao);
    } else {
        // atualiza os dados
        raiz->dados.nivel_habilidade = novo_vilao.nivel_habilidade;
        strncpy(raiz->dados.ponto_fraco, novo_vilao.ponto_fraco, sizeof(raiz->dados.ponto_fraco) - 1);
        raiz->dados.ponto_fraco[sizeof(raiz->dados.ponto_fraco) - 1] = '\0';
    }
    return raiz;
}

// busca pelo nome, retorna o ponteiro para o NO
NoVilao* buscar_vilao_bst(NoVilao* raiz, const char* nome_vilao) {
    if (raiz == NULL) return NULL;
    int cmp = comparar_nomes(nome_vilao, raiz->dados.nome);
    if (cmp == 0) return raiz;
    if (cmp < 0) return buscar_vilao_bst(raiz->esquerda, nome_vilao);
    return buscar_vilao_bst(raiz->direita, nome_vilao);
}

// encontra o no minimo a partir da raiz 
static NoVilao* encontrar_minimo(NoVilao* raiz) {
    NoVilao* atual = raiz;
    while (atual && atual->esquerda != NULL) {
        atual = atual->esquerda;
    }
    return atual;
}

// remove pelo nome e retorna raiz nova 
NoVilao* remover_vilao_bst(NoVilao* raiz, const char* nome_vilao) {
    if (raiz == NULL) return NULL;
    int cmp = comparar_nomes(nome_vilao, raiz->dados.nome);
    if (cmp < 0) {
        raiz->esquerda = remover_vilao_bst(raiz->esquerda, nome_vilao);
    } else if (cmp > 0) {
        raiz->direita = remover_vilao_bst(raiz->direita, nome_vilao);
    } else {
        if (raiz->esquerda == NULL) {
            NoVilao* direita = raiz->direita;
            free(raiz);
            return direita;
        } else if (raiz->direita == NULL) {
            NoVilao* esquerda = raiz->esquerda;
            free(raiz);
            return esquerda;
        } else {
            NoVilao* sucessor = encontrar_minimo(raiz->direita);
            raiz->dados = sucessor->dados; 
            raiz->direita = remover_vilao_bst(raiz->direita, sucessor->dados.nome);
        }
    }
    return raiz;
}

void listar_viloes_em_ordem(NoVilao* raiz) {
    if (raiz == NULL) return;
    listar_viloes_em_ordem(raiz->esquerda);
    printf("Nome: %s | Nível: %d | Ponto fraco: %s\n",
           raiz->dados.nome,
           raiz->dados.nivel_habilidade,
           raiz->dados.ponto_fraco);
    listar_viloes_em_ordem(raiz->direita);
}

// formatacao da string
static void trim(char* s) {
    if (!s) return;
    size_t len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) {
        s[len - 1] = '\0';
        len--;
    }
    size_t start = 0;
    while (s[start] && isspace((unsigned char)s[start])) {
        start++;
    }
    if (start > 0) {
        memmove(s, s + start, strlen(s + start) + 1);
    }
}

NoVilao* carregar_viloes_do_arquivo(const char* nome_arquivo) {
    FILE* f = fopen(nome_arquivo, "r");
    if (!f) {
        fprintf(stderr, "Aviso: não foi possível abrir '%s'.\n", nome_arquivo);
        return NULL;
    }

    NoVilao* raiz = NULL;
    char linha[512];
    while (fgets(linha, sizeof(linha), f)) {

        linha[strcspn(linha, "\r\n")] = '\0';
        if (linha[0] == '\0') continue;

        char* p1 = strchr(linha, ',');
        if (!p1) continue;
        char* p2 = strchr(p1 + 1, ',');
        if (!p2) continue;

        *p1 = '\0';
        *p2 = '\0';
        char* campo_nome = linha;
        char* campo_nivel = p1 + 1;
        char* campo_ponto_fraco = p2 + 1;

        trim(campo_nome);
        trim(campo_nivel);
        trim(campo_ponto_fraco);

        Vilao v;
        strncpy(v.nome, campo_nome, sizeof(v.nome) - 1);
        v.nome[sizeof(v.nome) - 1] = '\0';
        v.nivel_habilidade = atoi(campo_nivel);
        strncpy(v.ponto_fraco, campo_ponto_fraco, sizeof(v.ponto_fraco) - 1);
        v.ponto_fraco[sizeof(v.ponto_fraco) - 1] = '\0';

        raiz = inserir_vilao_bst(raiz, v);
    }

    fclose(f);
    return raiz;
}

// libera os nos
void liberar_bst(NoVilao* raiz) {
    if (raiz == NULL) return;
    liberar_bst(raiz->esquerda);
    liberar_bst(raiz->direita);
    free(raiz);
}

