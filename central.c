#include "central.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Módulo 1
// Funções Privadas para auxiliar no heap

// Função para trocar duas missões de lugar no array do heap
static void trocar_missoes(Missao* a, Missao* b) {
    Missao temp = *a;
    *a = *b;
    *b = temp;
}

// Função para restaurar a propriedade do Max-Heap "subindo" a partir de um índice
static void heapify_up(HeapMissoes* heap, int index) {
    if (index == 0) return; 
    int parent_index = (index-1)/2;

    if (heap->missoes[index].nivel_ameaca > heap->missoes[parent_index].nivel_ameaca || 
        (heap->missoes[index].nivel_ameaca == heap->missoes[parent_index].nivel_ameaca && 
        heap->missoes[index].id_chegada < heap->missoes[parent_index].id_chegada)) {

        trocar_missoes(&heap->missoes[index], &heap->missoes[parent_index]);
        heapify_up(heap, parent_index); 
    }
}

// Função para restaurar a propriedade do Max-Heap "descendo" a partir de um índice
static void heapify_down(HeapMissoes* heap, int index) {
    int maior = index;
    int esquerda = 2*index + 1;
    int direita = 2*index + 2;

    // Verifica se o filho da esquerda existe e tem maior prioridade que o nó atual
    if (esquerda < heap->tamanho_atual) {
        if (heap->missoes[esquerda].nivel_ameaca > heap->missoes[maior].nivel_ameaca ||
            (heap->missoes[esquerda].nivel_ameaca == heap->missoes[maior].nivel_ameaca &&
             heap->missoes[esquerda].id_chegada < heap->missoes[maior].id_chegada)) {
            maior = esquerda;
        }
    }

    // Verifica se o filho da direita existe e tem maior prioridade que o 'maior' encontrado até agora
    if (direita < heap->tamanho_atual) {
        if (heap->missoes[direita].nivel_ameaca > heap->missoes[maior].nivel_ameaca ||
            (heap->missoes[direita].nivel_ameaca == heap->missoes[maior].nivel_ameaca &&
             heap->missoes[direita].id_chegada < heap->missoes[maior].id_chegada)) {
            maior = direita;
        }
    }

    // Se o maior não for o próprio nó, troca e continua a descida recursivamente
    if (maior != index) {
        trocar_missoes(&heap->missoes[index], &heap->missoes[maior]);
        heapify_down(heap, maior);
    }
}

// Funções Principais do Heap

HeapMissoes* criar_heap(int capacidade) {
    HeapMissoes* heap = (HeapMissoes*) malloc(sizeof(HeapMissoes));
    if (!heap) {
        perror("Falha ao alocar memória para o Heap");
        return NULL;
    }

    heap->missoes = (Missao*) malloc(capacidade * sizeof(Missao));
    if (!heap->missoes) {
        perror("Falha ao alocar memória para o array de missões");
        free(heap);
        return NULL;
    }

    heap->capacidade = capacidade;
    heap->tamanho_atual = 0;
    printf("Painel de Alertas de Missões inicializado.\n");
    return heap;
}


void inserir_missao_heap(HeapMissoes* heap, Missao nova_missao) {
    if (heap->tamanho_atual == heap->capacidade) {
        fprintf(stderr, "Erro: Painel de Alertas está cheio! Não é possível adicionar nova missão.\n");
        return;
    }

    // Adiciona a nova missão no final do array
    heap->missoes[heap->tamanho_atual] = nova_missao;
    heap->tamanho_atual++;

    // Sobe com o elemento para restaurar a ordem do heap
    heapify_up(heap, heap->tamanho_atual - 1);
}


Missao remover_proxima_missao(HeapMissoes* heap) {
    if (heap->tamanho_atual == 0) {
        fprintf(stderr, "Erro: Não há missões pendentes no painel de alertas.\n");
        // Retorna uma missão "vazia" em caso de erro. O ideal é que o chamador verifique o tamanho antes.
        Missao missao_vazia = { "Nenhuma", "Nenhum", 0, 0 };
        return missao_vazia;
    }

    // A missão de maior prioridade está sempre na raiz
    Missao proxima_missao = heap->missoes[0];

    // Move a última missão para a raiz
    heap->missoes[0] = heap->missoes[heap->tamanho_atual - 1];
    heap->tamanho_atual--;

    // Desce com a nova raiz para restaurar a ordem do heap
    heapify_down(heap, 0);

    return proxima_missao;
}

// Mostra a próxima missão prioritária
void mostrar_proxima_missao(const HeapMissoes* heap) {
    if (heap->tamanho_atual == 0) {
        printf("\nNenhuma missão pendente no painel de alertas. A cidade está segura... por enquanto.\n");
        return;
    }

    const Missao* proxima = &heap->missoes[0];
    printf("\n--- ALERTA DE MISSÃO PRIORITÁRIA ---\n");
    printf("  Descrição: %s\n", proxima->descricao);
    printf("  Local: %s\n", proxima->local);
    printf("  Nível de Ameaça: %d\n", proxima->nivel_ameaca);
    printf("-------------------------------------\n");
}

void liberar_heap(HeapMissoes* heap) {
    if (heap) {
        if (heap->missoes) {
            free(heap->missoes);
        }
        free(heap);
    }
    printf("Painel de Alertas de Missões desligado.\n");
}


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

