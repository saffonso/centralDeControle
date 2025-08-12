#ifndef CENTRAL_H
#define CENTRAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum {
    DISPONIVEL,
    EM_MISSAO,
    FERIDO
} StatusTartaruga;

typedef enum {
    RESULTADO_NAO_DEFINIDO,
    VITORIA_PERFEITA,
    VITORIA_COM_CUSTO,
    DERROTA
} ResultadoBatalha;

typedef struct {
    char nome[50];
    StatusTartaruga status;
    float nivel;
    int missoes_ferido_restantes;
    char equipamento[50]; // Equipamento atribuído à tartaruga
} Tartaruga;

typedef struct {
    char nome[50];
    int nivel_habilidade;
    char ponto_fraco[100];
} Vilao;

typedef struct {
    char descricao[100];
    char local[100];
    int nivel_ameaca;
    unsigned int id_chegada;
    ResultadoBatalha resultado_batalha;
} Missao;

typedef struct {
    char nome[50];
} Equipamento;


//MODULO 1: PAINEL DE ALERTA(HEAP DE PRIORIDADE)

typedef struct {
    Missao* missoes;
    int tamanho_atual;
    int capacidade;
} HeapMissoes;

// Funções do Módulo 1
HeapMissoes* criar_heap(int capacidade);
void inserir_missao_heap(HeapMissoes* heap, Missao nova_missao);
Missao remover_proxima_missao(HeapMissoes* heap);
void mostrar_proxima_missao(const HeapMissoes* heap);
void liberar_heap(HeapMissoes* heap);

//MODULO2: ARQ VILOES ARV BIN BUSCA

typedef struct NoVilao {
    Vilao dados;
    struct NoVilao* esquerda;
    struct NoVilao* direita;
} NoVilao;

// Funções do Módulo 2
NoVilao* inserir_vilao_bst(NoVilao* raiz, Vilao novo_vilao);
NoVilao* buscar_vilao_bst(NoVilao* raiz, const char* nome_vilao);
NoVilao* remover_vilao_bst(NoVilao* raiz, const char* nome_vilao);
void listar_viloes_em_ordem(NoVilao* raiz);
NoVilao* carregar_viloes_do_arquivo(const char* nome_arquivo);
void liberar_bst(NoVilao* raiz);

//MODULO 3: DIARIO  DE BORDO: PILHA

typedef struct NoMissaoConcluida {
    Missao dados;
    struct NoMissaoConcluida* proximo;
} NoMissaoConcluida;

typedef struct {
    NoMissaoConcluida* topo; 
    int n;
} PilhaDiario;

// Funções do Módulo 3
PilhaDiario* criar_pilha_diario();
void push_missao_diario(PilhaDiario* pilha, Missao missao_concluida);
void ver_ultima_vitoria(const PilhaDiario* pilha);
void listar_historico_completo(const PilhaDiario* pilha);
void liberar_pilha(PilhaDiario* pilha);
int pilha_esta_vazia(const PilhaDiario* pilha);

//MODULO 4 E ARSENAL

// Funções de Sistema e Interface
void inicializar_sistema(Tartaruga equipe[4], Equipamento arsenal[], int tamanho_arsenal);
void mostrar_menu_principal();

// Funções de Batalha e Missões
void iniciar_nova_missao(HeapMissoes* painel_alertas, NoVilao* arvore_viloes, PilhaDiario* diario_bordo, Tartaruga equipe[4], Equipamento arsenal[], int tamanho_arsenal);
void resolver_batalha(Missao missao_atual, Tartaruga* tartarugas_na_missao[], int num_tartarugas, PilhaDiario* diario_bordo);

#endif 