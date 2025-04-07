#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  
#include <limits.h>  

#define MAX_NOME 100
#define MAX_ITENS 1000
#define ARQUIVO_ESTOQUE "estoque.txt"

typedef struct {
    char nome[MAX_NOME];
    int quantidade;
} Item;


void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


int carregarEstoque(Item* estoque) {
    FILE* arquivo = fopen(ARQUIVO_ESTOQUE, "r");
    if (!arquivo) return 0;

    int i = 0;
    while (fgets(estoque[i].nome, MAX_NOME, arquivo)) {
        estoque[i].nome[strcspn(estoque[i].nome, "\n")] = 0; 
        fscanf(arquivo, "%d\n", &estoque[i].quantidade);
        i++;
        if (i >= MAX_ITENS) break;
    }

    fclose(arquivo);
    return i;
}


void salvarEstoque(Item* estoque, int tamanho) {
    FILE* arquivo = fopen(ARQUIVO_ESTOQUE, "w");
    if (!arquivo) return;

    for (int i = 0; i < tamanho; i++) {
        fprintf(arquivo, "%s\n%d\n", estoque[i].nome, estoque[i].quantidade);
    }

    fclose(arquivo);
}


void adicionarItem() {
    char nome[MAX_NOME];
    int quantidade;

    printf("Digite o nome do item a ser adicionado: ");
    limparBuffer();
    fgets(nome, MAX_NOME, stdin);
    nome[strcspn(nome, "\n")] = 0;

    printf("Digite a quantidade do item: ");
    scanf("%d", &quantidade);

    Item estoque[MAX_ITENS];
    int tamanho = carregarEstoque(estoque);

    int encontrado = 0;
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(estoque[i].nome, nome) == 0) {
            estoque[i].quantidade += quantidade;
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        strcpy(estoque[tamanho].nome, nome);
        estoque[tamanho].quantidade = quantidade;
        tamanho++;
    }

    salvarEstoque(estoque, tamanho);
    printf("Item adicionado com sucesso!\n");
}


void removerItem() {
    char nome[MAX_NOME];
    int quantidade;

    printf("Digite o nome do item: ");
    limparBuffer();
    fgets(nome, MAX_NOME, stdin);
    nome[strcspn(nome, "\n")] = 0;

    printf("Digite a quantidade a ser removida: ");
    scanf("%d", &quantidade);

    Item estoque[MAX_ITENS];
    int tamanho = carregarEstoque(estoque);
    int encontrado = 0;

    for (int i = 0; i < tamanho; i++) {
        if (strcmp(estoque[i].nome, nome) == 0) {
            encontrado = 1;
            if (quantidade < estoque[i].quantidade) {
                estoque[i].quantidade -= quantidade;
                printf("Quantidade atualizada com sucesso!\n");
            } else if (quantidade == estoque[i].quantidade) {
                for (int j = i; j < tamanho - 1; j++) {
                    estoque[j] = estoque[j + 1];
                }
                tamanho--;
                printf("Item removido do estoque!\n");
            } else {
                printf("Estoque insuficiente. Quantidade disponível: %d\n", estoque[i].quantidade);
            }
            break;
        }
    }

    if (!encontrado) {
        printf("Item não encontrado.\n");
    } else {
        salvarEstoque(estoque, tamanho);
    }
}


void listarEstoque() {
    Item estoque[MAX_ITENS];
    int tamanho = carregarEstoque(estoque);

    if (tamanho == 0) {
        printf("O estoque está vazio.\n");
        return;
    }

    printf("=============================\n");
    printf("        Estoque Atual        \n");
    printf("=============================\n");

    for (int i = 0; i < tamanho; i++) {
        printf("Nome: %s\n", estoque[i].nome);
        printf("Quantidade: %d\n\n", estoque[i].quantidade);
    }
}


void limparEstoque() {
    FILE* arquivo = fopen(ARQUIVO_ESTOQUE, "w");
    if (arquivo) {
        fclose(arquivo);
        printf("Arquivo de estoque limpo com sucesso!\n");
    }
}


void mostrarDiretorioAtual() {
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        printf("Diretório atual: %s\n", buffer);
    } else {
        perror("getcwd");
    }
}


int main() {


    while (1) {
        printf("================================\n");
        printf("Controle de Estoque\n");
        printf("================================\n");
        printf("Selecione uma opção:\n");
        printf("1. Adicionar Item\n");
        printf("2. Remover Item\n");
        printf("3. Listar Estoque\n");
        printf("4. Sair\n");
        printf("Opção: ");

        int opcao;
        scanf("%d", &opcao);

        if (opcao == 1) {
            adicionarItem();
        } else if (opcao == 2) {
            removerItem();
        } else if (opcao == 3) {
            listarEstoque();
        } else if (opcao == 4) {
            printf("Obrigado por usar o Controle de Estoques! Até a próxima.\n");
            break;
        } else {
            printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf("Opção inválida! Tente novamente.\n");
            printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            continue;
        }

        while (1) {
            char resposta;
            printf("Deseja realizar outra operação? (s/n): ");
            limparBuffer();
            scanf("%c", &resposta);

            if (resposta == 'n' || resposta == 'N') {
                printf("Obrigado por usar o Controle de Estoques! Até a próxima.\n");
                return 0;
            } else if (resposta == 's' || resposta == 'S') {
                break;
            } else {
                printf("Resposta inválida. Por favor, digite 's' ou 'n'.\n");
            }
        }
    }

    return 0;
}
