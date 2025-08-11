#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "central.h" 

void exibir_menu() {
    printf("\n========= CENTRAL DE COMANDO DAS TARTARUGAS NINJA =========\n");
    printf("1. Ver Próxima Missão Prioritária\n");
    printf("2. Iniciar Missão\n");
    printf("3. Ver Última Vitória (Diário de Bordo)\n");
    printf("4. Listar Arquivos de Vilões\n");
    printf("5. Adicionar Nova Missão (Simulação de Alerta)\n");
    printf("0. Sair da Central de Comando\n");
    printf("===========================================================\n");
    printf("Escolha uma opção: ");
}

// Função para simular a chegada de novas missões
void simular_nova_missao(HeapMissoes* heap, unsigned int* id_counter) {
    Missao nova_missao;
    printf("\n--- Registrando Novo Alerta de Missão ---\n");
    printf("Descrição da missão: ");
    scanf(" %[^\n]", nova_missao.descricao);
    printf("Local da missão: ");
    scanf(" %[^\n]", nova_missao.local);
    printf("Nível de Ameaça (1-10): ");
    scanf("%d", &nova_missao.nivel_ameaca);

    nova_missao.id_chegada = (*id_counter)++; 
    
    inserir_missao_heap(heap, nova_missao);
    printf("Alerta recebido! Missão adicionada ao painel.\n");
}


int main() {
    // Gerador de números aleatórios
    srand(time(NULL));
    
    unsigned int id_missao_counter = 0;

    HeapMissoes* painel_de_alertas = criar_heap(20); // Capacidade do heap 20
    PilhaDiario* diario_bordo = criar_pilha_diario();
    
    NoVilao* arquivos_viloes = NULL;

    arquivos_viloes = carregar_viloes_do_arquivo("viloes.txt");
    
    // 3. Prepara dados iniciais (Tartarugas e Missões)
    Missao m1 = {"Assalto ao Banco Central", "Centro da cidade", 8, id_missao_counter++};
    Missao m2 = {"Roubo de Mutagênico na TCRI", "Distrito Industrial", 10, id_missao_counter++};
    Missao m3 = {"Ataque do Clã do Pé no Metrô", "Estação Central", 7, id_missao_counter++};
    
    inserir_missao_heap(painel_de_alertas, m1);
    inserir_missao_heap(painel_de_alertas, m2);
    inserir_missao_heap(painel_de_alertas, m3);

    // TODO: Inicializar as tartarugas e o arsenal conforme a lógica do seu central.c
    // Ex: inicializar_sistema(equipe_tartarugas, arsenal);
    
    printf("\nBEM-VINDO À CENTRAL DE COMANDO, DONATELLO!\n");
    
    int escolha = -1;
    while (escolha != 0) {
        exibir_menu();
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                mostrar_proxima_missao(painel_de_alertas);
                break;
            
            case 2:
                // LÓGICA CENTRAL: Inicia a preparação e resolução da batalha
                // TODO: Esta função deve ser implementada em central.c
                // iniciar_nova_missao(painel_de_alertas, arquivos_viloes, diario_bordo, ...);
                printf("\nFunção 'iniciar_nova_missao' ainda não implementada.\n");
                // Exemplo de como seria:
                // if (painel_de_alertas->tamanho_atual > 0) {
                //    iniciar_nova_missao(...);
                // } else {
                //    printf("\nNenhuma missão disponível para iniciar.\n");
                // }
                break;

            case 3:
                ver_ultima_vitoria(diario_bordo);
                break;

            case 4:
                printf("\n--- ARQUIVOS DE VILÕES DO DONATELLO ---\n");
                if (arquivos_viloes == NULL) {
                    printf("Nenhum vilão cadastrado.\n");
                } else {
                    // TODO: Implementar a função em central.c que percorre a árvore
                    listar_viloes_em_ordem(arquivos_viloes);
                    printf("Função 'listar_viloes_em_ordem' ainda não implementada.\n");
                }
                printf("---------------------------------------\n");
                break;
            
            case 5:
                simular_nova_missao(painel_de_alertas, &id_missao_counter);
                break;

            case 0:
                printf("\nDesligando a Central de Comando. Booyakasha!\n");
                break;

            default:
                printf("\nOpção inválida! Por favor, escolha uma opção do menu.\n");
                break;
        }
        printf("\nPressione Enter para continuar...");
        // Limpa o buffer de entrada para a próxima leitura
        while(getchar() != '\n'); // Limpa o buffer antigo
        getchar(); // Espera por um novo Enter
    }

    printf("\nIniciando procedimento de limpeza de memória...\n");
    liberar_heap(painel_de_alertas);
    liberar_pilha(diario_bordo);
    // TODO: Implementar a função para liberar a árvore
    // liberar_bst(arquivos_viloes);
    printf("Memória liberada com sucesso. Até a próxima, Mestre!\n");
    
    return 0;
}