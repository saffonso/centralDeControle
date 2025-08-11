#include "central.h"
#include <stdio.h>
#include <stdlib.h>

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

