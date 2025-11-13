// Sistema de Inventário – Free Fire (Lista Sequencial + Structs)
// Nível: Inventário básico com busca sequencial
// Funcionalidades: cadastrar, remover, listar e buscar itens em um vetor de structs.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ITENS 10
#define TAM_NOME 30
#define TAM_TIPO 20

// Representa um item de loot na mochila
typedef struct {
    char nome[TAM_NOME];   // ex.: "AK-47", "KitMedico"
    char tipo[TAM_TIPO];   // ex.: "arma", "municao", "cura"
    int quantidade;        // ex.: 1, 30, 3
} Item;

// Protótipos
void limparBufferEntrada(void);
void removerQuebraLinha(char* s);
void listarItens(const Item* mochila, int numItens);
int  buscarItem(const Item* mochila, int numItens, const char* nome);
void inserirItem(Item* mochila, int* numItens);
void removerItem(Item* mochila, int* numItens);

int main(void) {
    Item mochila[MAX_ITENS];
    int numItens = 0;

    int opcao;
    do {
        printf("\n================ INVENTARIO =================\n");
        printf("1 - Cadastrar item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar item por nome\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) {
            limparBufferEntrada();
            opcao = -1;
        }
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                inserirItem(mochila, &numItens);
                listarItens(mochila, numItens);
                break;
            case 2:
                removerItem(mochila, &numItens);
                listarItens(mochila, numItens);
                break;
            case 3:
                listarItens(mochila, numItens);
                break;
            case 4: {
                char nome[TAM_NOME];
                printf("Nome do item para busca: ");
                if (fgets(nome, sizeof(nome), stdin)) {
                    removerQuebraLinha(nome);
                    int idx = buscarItem(mochila, numItens, nome);
                    if (idx >= 0) {
                        printf("\nItem encontrado:\n");
                        printf("- Nome: %s\n- Tipo: %s\n- Quantidade: %d\n",
                               mochila[idx].nome, mochila[idx].tipo, mochila[idx].quantidade);
                    } else {
                        printf("Item '%s' nao encontrado.\n", nome);
                    }
                }
                break;
            }
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}

// Lê e descarta caracteres pendentes até '\n' ou EOF
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Remove a quebra de linha final (se existir)
void removerQuebraLinha(char* s) {
    if (!s) return;
    s[strcspn(s, "\n")] = '\0';
}

// Imprime todos os itens da mochila
void listarItens(const Item* mochila, int numItens) {
    printf("\n----------- MOCHILA (itens: %d/%d) -----------\n", numItens, MAX_ITENS);
    if (numItens == 0) {
        printf("(vazia)\n");
        return;
    }
    for (int i = 0; i < numItens; i++) {
        printf("%2d) %-29s | %-19s | qtd: %d\n",
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

// Busca sequencial por nome; retorna índice ou -1 se não encontrado
int buscarItem(const Item* mochila, int numItens, const char* nome) {
    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

// Cadastra um novo item (ou aumenta quantidade se nome já existir)
void inserirItem(Item* mochila, int* numItens) {
    if (*numItens >= MAX_ITENS) {
        printf("Mochila cheia. Remova algo antes de adicionar.\n");
        return;
    }

    Item novo;
    printf("Nome do item (max %d): ", TAM_NOME - 1);
    if (!fgets(novo.nome, sizeof(novo.nome), stdin)) return;
    removerQuebraLinha(novo.nome);
    if (novo.nome[0] == '\0') {
        printf("Nome invalido.\n");
        return;
    }

    printf("Tipo do item (ex: arma, municao, cura) (max %d): ", TAM_TIPO - 1);
    if (!fgets(novo.tipo, sizeof(novo.tipo), stdin)) return;
    removerQuebraLinha(novo.tipo);
    if (novo.tipo[0] == '\0') {
        printf("Tipo invalido.\n");
        return;
    }

    printf("Quantidade: ");
    if (scanf("%d", &novo.quantidade) != 1 || novo.quantidade < 1) {
        limparBufferEntrada();
        printf("Quantidade invalida.\n");
        return;
    }
    limparBufferEntrada();

    int idx = buscarItem(mochila, *numItens, novo.nome);
    if (idx >= 0) {
        // Se já existe, acumula quantidade
        mochila[idx].quantidade += novo.quantidade;
        // Opcional: atualizar tipo para o último informado
        strncpy(mochila[idx].tipo, novo.tipo, sizeof(mochila[idx].tipo) - 1);
        mochila[idx].tipo[sizeof(mochila[idx].tipo) - 1] = '\0';
        printf("Item existente; quantidade atualizada.\n");
    } else {
        mochila[*numItens] = novo;
        (*numItens)++;
        printf("Item cadastrado com sucesso.\n");
    }
}

// Remove um item pelo nome e compacta o vetor
void removerItem(Item* mochila, int* numItens) {
    if (*numItens == 0) {
        printf("Mochila vazia.\n");
        return;
    }
    char nome[TAM_NOME];
    printf("Nome do item para remover: ");
    if (!fgets(nome, sizeof(nome), stdin)) return;
    removerQuebraLinha(nome);
    if (nome[0] == '\0') {
        printf("Nome invalido.\n");
        return;
    }

    int idx = buscarItem(mochila, *numItens, nome);
    if (idx < 0) {
        printf("Item '%s' nao encontrado.\n", nome);
        return;
    }

    for (int i = idx; i < *numItens - 1; i++) {
        mochila[i] = mochila[i + 1];
    }
    (*numItens)--;
    printf("Item removido.\n");
}
