#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "central.h" 

void exibir_menu() { 
    mostrar_menu_principal(); 
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
    nova_missao.resultado_batalha = RESULTADO_NAO_DEFINIDO;
    
    inserir_missao_heap(heap, nova_missao);
    printf("Alerta recebido! Missão adicionada ao painel.\n");
}


int main() {

    srand(time(NULL)); // Gerador de números aleatórios
    unsigned int id_missao_counter = 0;

    HeapMissoes* painel_de_alertas = criar_heap(20);
    PilhaDiario* diario_bordo = criar_pilha_diario();
    NoVilao* arquivos_viloes = carregar_viloes_do_arquivo("viloes.txt");
    
    Tartaruga equipe[4];
    Equipamento arsenal[8];
    inicializar_sistema(equipe, arsenal, 8);
    
    Missao m1 = {"Assalto ao Banco Central", "Centro de Pelotas", 8, id_missao_counter++, RESULTADO_NAO_DEFINIDO};
    Missao m2 = {"Roubo de Mutagênico no Anglo", "Anglo", 10, id_missao_counter++, RESULTADO_NAO_DEFINIDO};
    Missao m3 = {"Ataque do Clã do Pé no Capao", "Campus Capao do leao", 7, id_missao_counter++, RESULTADO_NAO_DEFINIDO};
    
    inserir_missao_heap(painel_de_alertas, m1);
    inserir_missao_heap(painel_de_alertas, m2);
    inserir_missao_heap(painel_de_alertas, m3);
    
    printf("\n BEM-VINDO À CENTRAL DE COMANDO, DONATELLO! \n");
    
    int escolha = -1;
    while (escolha != 0) {
        exibir_menu();
        scanf("%d", &escolha);

        switch (escolha) {
            case 1: // Ver Próxima Missão Prioritária
                mostrar_proxima_missao(painel_de_alertas);
                break;
            
            case 2: // Iniciar Missão
                if (painel_de_alertas->tamanho_atual > 0) {
                    iniciar_nova_missao(painel_de_alertas, arquivos_viloes, diario_bordo, equipe, arsenal, 8);
                } else {
                    printf("\nNenhuma missão disponível para iniciar.\n");
                }
                break;

            case 3: // Ver Última Vitória (Diário de Bordo)
                ver_ultima_vitoria(diario_bordo);
                break;

            case 4: // Ver Histórico Completo de Batalhas
                listar_historico_completo(diario_bordo);
                break;

            case 5: // Listar Arquivos de Vilões
                printf("\n--- ARQUIVOS DE VILÕES DO DONATELLO ---\n");
                if (arquivos_viloes == NULL) {
                    printf("Nenhum vilão cadastrado.\n");
                } else {
                    listar_viloes_em_ordem(arquivos_viloes);
                }
                printf("---------------------------------------\n");
                break;
            
            case 6: // Adicionar Nova Missão (Simulação)
                simular_nova_missao(painel_de_alertas, &id_missao_counter);
                break;

            case 0: // Sair
                printf("\n Desligando a Central de Comando. Booyakasha! \n");
                break;

            default:
                printf("\n Opção inválida! Por favor, escolha uma opção do menu.\n");
                break;
        }
        
        // Pausa para o usuário ler a saída
        printf("\nPressione Enter para continuar...");
        while(getchar() != '\n'); // Limpa o buffer
        getchar(); // Espera por Enter
    }

    printf("\n Iniciando procedimento de limpeza de memória...\n");
    liberar_heap(painel_de_alertas);
    liberar_pilha(diario_bordo);
    liberar_bst(arquivos_viloes);
    printf(" Memória liberada com sucesso. Até a próxima, Mestre!\n");
    
    return 0;
}