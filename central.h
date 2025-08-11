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

typedef struct {
    char nome[50];
    StatusTartaruga status;
    float nivel;
    int missoes_ferido_restantes; // Contador para saber quando volta a ficar disponível
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
} Missao;

typedef struct {
    char nome[50];
} Equipamento;



// Módulo 1: Estrutura para o Painel de Alertas (Heap de Missões)
typedef struct {
    Missao* missoes; // Ponteiro para um array de missões
    int tamanho_atual;
    int capacidade;
} HeapMissoes;

// Módulo 2: Estrutura para os Nós da Árvore de Vilões
typedef struct NoVilao {
    Vilao dados;
    struct NoVilao* esquerda;
    struct NoVilao* direita;
} NoVilao;

// Módulo 3: Estrutura para os Nós do Diário de Bordo (Pilha de Missões Concluídas)
typedef struct NoMissaoConcluida {
    Missao dados;
    struct NoMissaoConcluida* proximo;
} NoMissaoConcluida;

// Módulo 4: Estrutura para os Nós da Lista de Equipamentos (Lista Encadeada)
typedef struct NoEquipamento {
    Equipamento dados;
    struct NoEquipamento* proximo;
} NoEquipamento;



// Funções do Módulo 1: Painel de Alertas (Heap)
HeapMissoes* criar_heap(int capacidade);
void inserir_missao_heap(HeapMissoes* heap, Missao nova_missao);
Missao remover_proxima_missao(HeapMissoes* heap); // Remove e retorna a de maior prioridade
void mostrar_proxima_missao(const HeapMissoes* heap);
void liberar_heap(HeapMissoes* heap);

// Funções do Módulo 2: Arquivos de Vilões (BST)
NoVilao* inserir_vilao_bst(NoVilao* raiz, Vilao novo_vilao);
NoVilao* buscar_vilao_bst(NoVilao* raiz, const char* nome_vilao);
NoVilao* remover_vilao_bst(NoVilao* raiz, const char* nome_vilao);
void listar_viloes_em_ordem(NoVilao* raiz); // Função extra para visualização
NoVilao* carregar_viloes_do_arquivo(const char* nome_arquivo); // Carrega do viloes.txt
void liberar_bst(NoVilao* raiz);

// Funções do Módulo 3: Diário de Bordo (Pilha)
void push_missao_diario(NoMissaoConcluida** topo_pilha, Missao missao_concluida);
void ver_ultima_vitoria(const NoMissaoConcluida* topo_pilha);
void liberar_pilha(NoMissaoConcluida** topo_pilha);

// Funções do Módulo 4: Arsenal e Plano de Ação (Lista Encadeada)
void adicionar_equipamento_lista(NoEquipamento** cabeca_lista, Equipamento novo_equipamento);
void remover_equipamento_lista(NoEquipamento** cabeca_lista, const char* nome_equipamento);
void mostrar_equipamentos_da_missao(const NoEquipamento* cabeca_lista);
void liberar_lista_equipamentos(NoEquipamento** cabeca_lista);
int verificar_equipamento_na_lista(const NoEquipamento* cabeca_lista, const char* nome_equipamento);


// Funções da Lógica Central e Batalha
void inicializar_sistema(Tartaruga equipe[4], Equipamento arsenal[], int tamanho_arsenal);
void mostrar_menu_principal();
void iniciar_nova_missao(HeapMissoes* painel_alertas, NoVilao* arvore_viloes, NoMissaoConcluida** diario_bordo, Tartaruga equipe[4], Equipamento arsenal[], int tamanho_arsenal);
void resolver_batalha(Missao missao_atual, Tartaruga* tartarugas_na_missao[], int num_tartarugas, NoEquipamento* equipamentos_selecionados, NoMissaoConcluida** diario_bordo);


#endif 