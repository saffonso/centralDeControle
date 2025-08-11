#include "central.h"

int main(void) {
    // Carrega vilões do arquivo
    NoVilao* arvore = carregar_viloes_do_arquivo("viloes.txt");

    printf("\n=== Lista de vilões (in-order) ===\n");
    listar_viloes_em_ordem(arvore);

    // Busca alguns vilões
    const char* nomes_para_busca[] = {"Destruidor", "Bebop", "VilaoInexistente"};
    for (int i = 0; i < 3; i++) {
        const char* nome = nomes_para_busca[i];
        NoVilao* encontrado = buscar_vilao_bst(arvore, nome);
        if (encontrado) {
            printf("\n[BUSCA] Encontrado: %s (nivel %d)\n", encontrado->dados.nome, encontrado->dados.nivel_habilidade);
        } else {
            printf("\n[BUSCA] Não encontrado: %s\n", nome);
        }
    }

    // Remove um vilão e lista novamente
    printf("\n[REMOVER] Removendo 'Bebop'...\n");
    arvore = remover_vilao_bst(arvore, "Bebop");

    printf("\n=== Lista após remoção ===\n");
    listar_viloes_em_ordem(arvore);

    // Libera memória
    liberar_bst(arvore);
    return 0;
}
