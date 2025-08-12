#include "central.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// MODULO 1 PAINEL DE ALERTA (HEAP DE PRIORIDADE)

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
        Missao missao_vazia = { "Nenhuma", "Nenhum", 0, 0, RESULTADO_NAO_DEFINIDO };
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

//MODULO 2: ARQ DE VILOES (ARV DE BUSCA)
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


//MODULO 3: DIARIO DE BORDO (PILHA)

PilhaDiario* criar_pilha_diario() {
    PilhaDiario* pilha = (PilhaDiario*) malloc(sizeof(PilhaDiario));
    if (!pilha) {
        perror("Falha ao alocar memória para a Pilha do Diário");
        return NULL;
    }
    pilha->topo = NULL;
    pilha->n = 0;
    printf("Diário de Bordo inicializado e pronto para registrar vitórias.\n");
    return pilha;
}

void push_missao_diario(PilhaDiario* pilha, Missao missao_concluida) {
    NoMissaoConcluida* novo_no = (NoMissaoConcluida*) malloc(sizeof(NoMissaoConcluida));
    if (!novo_no) {
        perror("Falha crítica: Não foi possível alocar memória para registrar vitória no Diário");
        return;
    }

    novo_no->dados = missao_concluida;
    novo_no->proximo = pilha->topo;

    pilha->topo = novo_no;
    pilha->n++;
}


int pilha_esta_vazia(const PilhaDiario* pilha) {
    return pilha->n == 0;
}

void ver_ultima_vitoria(const PilhaDiario* pilha) {
    printf("\n--- DIÁRIO DE BORDO DO MESTRE SPLINTER ---\n");
    
    if (pilha_esta_vazia(pilha)) {
        printf("  O Diário está vazio. Nenhuma missão foi concluída ainda.\n");
    } else {
        // Busca a última vitória na pilha
        NoMissaoConcluida* atual = pilha->topo;
        const Missao* ultima_vitoria = NULL;
        int total_vitorias = 0;
        
        while (atual != NULL) {
            if (atual->dados.resultado_batalha == VITORIA_PERFEITA || 
                atual->dados.resultado_batalha == VITORIA_COM_CUSTO) {
                if (ultima_vitoria == NULL) {
                    ultima_vitoria = &atual->dados; // Primeira vitória encontrada (mais recente)
                }
                total_vitorias++;
            }
            atual = atual->proximo;
        }
        
        if (ultima_vitoria == NULL) {
            printf("  Nenhuma vitória registrada ainda. As tartarugas precisam treinar mais!\n");
        } else {
            printf("  Última vitória registrada (%d vitórias no total):\n", total_vitorias);
            printf("    Descrição: %s\n", ultima_vitoria->descricao);
            printf("    Local: %s\n", ultima_vitoria->local);
            printf("    Nível de Ameaça enfrentado: %d\n", ultima_vitoria->nivel_ameaca);
            if (ultima_vitoria->resultado_batalha == VITORIA_PERFEITA) {
                printf("    Resultado:  VITÓRIA PERFEITA!\n");
            } else {
                printf("    Resultado:  VITÓRIA COM CUSTO\n");
            }
        }
    }
    printf("-------------------------------------------\n");
}

void listar_historico_completo(const PilhaDiario* pilha) {
    printf("\n--- HISTÓRICO COMPLETO DE BATALHAS ---\n");
    
    if (pilha_esta_vazia(pilha)) {
        printf("  O Diário está vazio. Nenhuma batalha foi registrada ainda.\n");
    } else {
        printf("  Total de batalhas registradas: %d\n\n", pilha->n);
        
        NoMissaoConcluida* atual = pilha->topo;
        int contador = 1;
        
        while (atual != NULL) {
            printf("  %d) %s\n", contador, atual->dados.descricao);
            printf("     Local: %s\n", atual->dados.local);
            printf("     Nível de Ameaça: %d\n", atual->dados.nivel_ameaca);
            
            // Mostra o resultado da batalha
            switch (atual->dados.resultado_batalha) {
                case VITORIA_PERFEITA:
                    printf("     Resultado:  VITÓRIA PERFEITA!\n");
                    break;
                case VITORIA_COM_CUSTO:
                    printf("     Resultado:  VITÓRIA COM CUSTO\n");
                    break;
                case DERROTA:
                    printf("     Resultado:  DERROTA\n");
                    break;
                default:
                    printf("     Resultado:  NÃO DEFINIDO\n");
                    break;
            }
            printf("\n");
            atual = atual->proximo;
            contador++;
        }
    }
    printf("------------------------------------------\n");
}

void liberar_pilha(PilhaDiario* pilha) {
    if (!pilha) return;

    NoMissaoConcluida* atual = pilha->topo;
    NoMissaoConcluida* proximo_no;

    while (atual != NULL) {
        proximo_no = atual->proximo; 
        free(atual);            
        atual = proximo_no;         
    }

    free(pilha);
    printf("Diário de Bordo arquivado e memória liberada.\n");
}

// MODULO 4: Arsenal E PLANO DE ACAO


//FUNCOES AUXILIARES

static const char* arma_favorita_da_tartaruga(const char* nome_tartaruga) {
    if (strcmp(nome_tartaruga, "Leonardo") == 0) return "Katanas";
    if (strcmp(nome_tartaruga, "Donatello") == 0) return "Bo";
    if (strcmp(nome_tartaruga, "Michelangelo") == 0) return "Nunchakus";
    if (strcmp(nome_tartaruga, "Raphael") == 0) return "Sai";
    return NULL;
}


static void limitar_nivel(Tartaruga* t) {
    if (t->nivel < 1.0f) t->nivel = 1.0f;
    if (t->nivel > 10.0f) t->nivel = 10.0f;
}

static void decrementar_ferimentos_pos_missao(Tartaruga equipe[4]) {
    for (int i = 0; i < 4; i++) {
        if (equipe[i].status == FERIDO && equipe[i].missoes_ferido_restantes > 0) {
            equipe[i].missoes_ferido_restantes--;
            if (equipe[i].missoes_ferido_restantes == 0) {
                equipe[i].status = DISPONIVEL;
            }
        }
    }
}

//INICIALIZAR O SISTEMA

void inicializar_sistema(Tartaruga equipe[4], Equipamento arsenal[], int tamanho_arsenal) {
    
    const char* nomes[4] = {"Leonardo", "Donatello", "Raphael", "Michelangelo"};
    for (int i = 0; i < 4; i++) {
        strncpy(equipe[i].nome, nomes[i], sizeof(equipe[i].nome) - 1);
        equipe[i].nome[sizeof(equipe[i].nome) - 1] = '\0';
        equipe[i].status = DISPONIVEL;
        equipe[i].nivel = 5.0f; //nível inicial 
        equipe[i].missoes_ferido_restantes = 0;
        strcpy(equipe[i].equipamento, "");
    }

    
    const char* itens_padrao[] = {
        "Katanas", "Bo", "Nunchakus", "Sai", "Bumerangue-Pizza", "Comunicador-Casco"
    };
    int qtd_padrao = (int)(sizeof(itens_padrao) / sizeof(itens_padrao[0]));
    int limite = tamanho_arsenal < qtd_padrao ? tamanho_arsenal : qtd_padrao;
    for (int i = 0; i < limite; i++) {
        strncpy(arsenal[i].nome, itens_padrao[i], sizeof(arsenal[i].nome) - 1);
        arsenal[i].nome[sizeof(arsenal[i].nome) - 1] = '\0';
    }
}

//INTERFACE

void mostrar_menu_principal() {
    printf("\n========= CENTRAL DE COMANDO DAS TARTARUGAS NINJA =========\n");
    printf("1. Ver Próxima Missão Prioritária\n");
    printf("2. Iniciar Missão\n");
    printf("3. Ver Última Vitória (Diário de Bordo)\n");
    printf("4. Ver Histórico Completo de Batalhas\n");
    printf("5. Listar Arquivos de Vilões\n");
    printf("6. Adicionar Nova Missão (Simulação de Alerta)\n");
    printf("0. Sair da Central de Comando\n");
    printf("===========================================================\n");
    printf("Escolha uma opção: ");
}

//LOGICA PRINCIPAL

void resolver_batalha(Missao missao_atual,
                      Tartaruga* tartarugas_na_missao[],
                      int num_tartarugas,
                      PilhaDiario* diario_bordo) {
    // Calcula força média
    float soma = 0.0f;
    for (int i = 0; i < num_tartarugas; i++) {
        soma += tartarugas_na_missao[i]->nivel;
    }
    float forca_media = (num_tartarugas > 0) ? (soma / (float)num_tartarugas) : 0.0f;

    // Bônus por arma favorita (+0.5 para cada tartaruga com sua arma favorita)
    float bonus = 0.0f;
    for (int i = 0; i < num_tartarugas; i++) {
        const char* favorita = arma_favorita_da_tartaruga(tartarugas_na_missao[i]->nome);
        if (favorita && strcmp(tartarugas_na_missao[i]->equipamento, favorita) == 0) {
            bonus += 0.5f; // +0.5 por cada tartaruga com arma favorita
        }
    }

    // Sorte e Caos
    int sorte = (rand() % 5) - 2;        // -2..+2
    float caos = (float)rand() / (float)RAND_MAX; // 0..1

    float resultado_final = (forca_media + bonus) - (float)missao_atual.nivel_ameaca + (float)sorte - caos;

    printf("\n=== RESOLUÇÃO DA BATALHA ===\n");
    printf("Missão: %s (%s) — Nível de Ameaça: %d\n", missao_atual.descricao, missao_atual.local, missao_atual.nivel_ameaca);
    printf("Equipe e Equipamentos:\n");
    for (int i = 0; i < num_tartarugas; i++) {
        printf("  %s", tartarugas_na_missao[i]->nome);
        if (strlen(tartarugas_na_missao[i]->equipamento) > 0) {
            printf(" - %s", tartarugas_na_missao[i]->equipamento);
        }
        printf("\n");
    }
    printf("Força média: %.2f | Bônus: %.2f | Sorte: %d | Caos: %.2f\n", forca_media, bonus, sorte, caos);
    printf("Resultado Final: %.2f\n", resultado_final);

    if (resultado_final > 3.0f) {
        printf("\nVitória Perfeita! Booyakasha!\n");
        missao_atual.resultado_batalha = VITORIA_PERFEITA;
        // Recupera e sobe nível
        for (int i = 0; i < num_tartarugas; i++) {
            tartarugas_na_missao[i]->status = DISPONIVEL;
            tartarugas_na_missao[i]->nivel += 0.3f;
            limitar_nivel(tartarugas_na_missao[i]);
        }
    } else if (resultado_final >= 0.0f) {
        printf("\nVitória com Custo!\n");
        missao_atual.resultado_batalha = VITORIA_COM_CUSTO;
        // Um ferido aleatório
        if (num_tartarugas > 0) {
            int idx_ferido = rand() % num_tartarugas;
            for (int i = 0; i < num_tartarugas; i++) {
                if (i == idx_ferido) {
                    tartarugas_na_missao[i]->status = FERIDO;
                    tartarugas_na_missao[i]->missoes_ferido_restantes = 2;
                } else {
                    tartarugas_na_missao[i]->status = DISPONIVEL;
                    tartarugas_na_missao[i]->nivel += 0.2f;
                    limitar_nivel(tartarugas_na_missao[i]);
                }
            }
        }
    } else {
        printf("\nDerrota... Recuar e reavaliar.\n");
        missao_atual.resultado_batalha = DERROTA;
        // Todos feridos
        for (int i = 0; i < num_tartarugas; i++) {
            tartarugas_na_missao[i]->status = FERIDO;
            tartarugas_na_missao[i]->missoes_ferido_restantes = 2; // tempo definido pelo programador
        }
    }
    
    // Registra todas as batalhas no diário do Mestre Splinter
    if (diario_bordo) push_missao_diario(diario_bordo, missao_atual);
}

// MISSOES

void iniciar_nova_missao(HeapMissoes* painel_alertas,
                         NoVilao* arvore_viloes,
                         PilhaDiario* diario_bordo,
                         Tartaruga equipe[4],
                         Equipamento arsenal[],
                         int tamanho_arsenal) {
    (void)arvore_viloes; // por ora, consulta opcional
    if (painel_alertas == NULL || painel_alertas->tamanho_atual == 0) {
        printf("\nNenhuma missão disponível para iniciar.\n");
        return;
    }

    // Seleciona missão
    Missao missao = remover_proxima_missao(painel_alertas);
    printf("\nIniciando missão: %s (%s) — Ameaça %d\n", missao.descricao, missao.local, missao.nivel_ameaca);

    // Verifica se há tartarugas disponíveis
    int tartarugas_disponiveis = 0;
    for (int i = 0; i < 4; i++) {
        if (equipe[i].status == DISPONIVEL) {
            tartarugas_disponiveis++;
        }
    }
    
    if (tartarugas_disponiveis == 0) {
        printf("\n NENHUMA TARTARUGA DISPONÍVEL!\n");
        printf("Todas as tartarugas estão feridas ou em missão.\n");
        printf("Aguardando recuperação... (tempo passa)\n");
        
        // O tempo passa, ferimentos cicatrizam naturalmente
        decrementar_ferimentos_pos_missao(equipe);
        
        // Recoloca a missão no heap (não foi executada)
        inserir_missao_heap(painel_alertas, missao);
        printf("Missão reagendada para quando houver equipe disponível.\n");
        return;
    }

    // Seleciona equipe (1 a 4, somente disponíveis)
    Tartaruga* selecionadas[4];
    int num_sel = 0;
    int continuar = 1;
    while (continuar) {
        printf("\nEquipe atual (%d/4): ", num_sel);
        for (int i = 0; i < num_sel; i++) printf("%s%s", selecionadas[i]->nome, (i < num_sel-1)?", ":"\n");
        printf("Tartarugas disponíveis:\n");
        for (int i = 0; i < 4; i++) {
            if (equipe[i].status == DISPONIVEL) {
                printf("  %d) %s (nível %.1f)\n", i+1, equipe[i].nome, equipe[i].nivel);
            } else if (equipe[i].status == FERIDO) {
                printf("  %d) %s (FERIDO, faltam %d missões)\n", i+1, equipe[i].nome, equipe[i].missoes_ferido_restantes);
            } else {
                printf("  %d) %s (EM MISSÃO)\n", i+1, equipe[i].nome);
            }
        }
        printf("Escolha o número da tartaruga para adicionar (0 para finalizar, mínimo 1): ");
        int opc;
        if (scanf("%d", &opc) != 1) {
            while (getchar() != '\n');
            continue;
        }
        if (opc == 0) {
            if (num_sel >= 1) continuar = 0; else printf("Selecione pelo menos 1 tartaruga.\n");
        } else if (opc >= 1 && opc <= 4) {
            Tartaruga* t = &equipe[opc-1];
            if (t->status != DISPONIVEL) {
                printf("%s não está disponível.\n", t->nome);
            } else if (num_sel >= 4) {
                printf("Equipe cheia.\n");
            } else {
                // evita duplicar
                int ja = 0; for (int i = 0; i < num_sel; i++) if (selecionadas[i] == t) ja = 1;
                if (!ja) {
                    selecionadas[num_sel++] = t;
                    t->status = EM_MISSAO;
                }
            }
        }
    }

    // Seleciona equipamentos para cada tartaruga
    printf("\n--- SELEÇÃO DE EQUIPAMENTOS ---\n");
    printf("Arsenal disponível:\n");
    for (int i = 0; i < tamanho_arsenal; i++) {
        printf("  %d) %s\n", i+1, arsenal[i].nome);
    }
    
    for (int i = 0; i < num_sel; i++) {
        printf("\nEquipamento para %s (0 para nenhum): ", selecionadas[i]->nome);
        int escolha;
        if (scanf("%d", &escolha) == 1) {
            if (escolha == 0) {
                strcpy(selecionadas[i]->equipamento, "");
            } else if (escolha >= 1 && escolha <= tamanho_arsenal) {
                strcpy(selecionadas[i]->equipamento, arsenal[escolha-1].nome);
                printf(" %s equipado com %s!\n", selecionadas[i]->nome, arsenal[escolha-1].nome);
            }
        }
    }

    // Resolve a batalha
    resolver_batalha(missao, selecionadas, num_sel, diario_bordo);

    // Decrementa ferimentos globais
    decrementar_ferimentos_pos_missao(equipe);
}
