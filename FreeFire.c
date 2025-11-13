// Sistema de Inventário – Free Fire (Lista Sequencial + Structs)
// Nível: Inventário básico com busca sequencial
// Funcionalidades: cadastrar, remover, listar e buscar itens em um vetor de structs.

#include <stdio.h>
#include <string.h>
#include <strings.h>
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

// Nó para lista encadeada
typedef struct No {
    Item dados;
    struct No* proximo;
} No;

// Flag para saber se vetor está ordenado por nome
static int g_vetorOrdenado = 0;

// Protótipos
void limparBufferEntrada(void);
void removerQuebraLinha(char* s);
// Vetor (lista sequencial)
void listarItens(const Item* mochila, int numItens);
int  buscarItem(const Item* mochila, int numItens, const char* nome);
int  buscarSequencialVetor(const Item* mochila, int numItens, const char* nome, long* comps);
void inserirItem(Item* mochila, int* numItens);
void removerItem(Item* mochila, int* numItens);
void ordenarVetorPorNome(Item* mochila, int numItens, long* comps);
void ordenarVetorPorNomeBubble(Item* mochila, int numItens, long* comps);
int  buscarBinariaVetor(const Item* mochila, int numItens, const char* nome, long* comps);

// Lista encadeada
void listarItensLista(const No* inicio);
int  buscarSequencialLista(const No* inicio, const char* nome, long* comps);
void inserirItemLista(No** inicio, const Item* novo);
int  removerItemLista(No** inicio, const char* nome);
void liberarLista(No** inicio);

int main(void) {
    // Estrutura com vetor
    Item mochila[MAX_ITENS];
    int numItens = 0;
    g_vetorOrdenado = 0;

    // Estrutura com lista encadeada
    No* lista = NULL;

    int opcao;
    do {
        printf("\n================ INVENTARIO =================\n");
        printf("[Vetor]  1- Inserir  2- Remover  3- Listar  4- Busca Seq.  5- Ordenar (Selection)  6- Busca Binaria  11- Ordenar (Bubble)  12- Comparar Sorts\n");
        printf("[Lista]  7- Inserir  8- Remover  9- Listar 10- Busca Seq.\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) {
            limparBufferEntrada();
            opcao = -1;
        }
        limparBufferEntrada();

        switch (opcao) {
            // Vetor
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
                char nome[TAM_NOME]; long comps = 0;
                printf("Nome do item para busca (vetor): ");
                if (fgets(nome, sizeof(nome), stdin)) {
                    removerQuebraLinha(nome);
                    int idx = buscarSequencialVetor(mochila, numItens, nome, &comps);
                    if (idx >= 0) {
                        printf("\nItem encontrado (seq): %s | %s | qtd: %d\n",
                               mochila[idx].nome, mochila[idx].tipo, mochila[idx].quantidade);
                    } else {
                        printf("Item '%s' nao encontrado (seq).\n", nome);
                    }
                    printf("Comparacoes (seq): %ld\n", comps);
                }
                break;
            }
            case 11: {
                long comps = 0;
                ordenarVetorPorNomeBubble(mochila, numItens, &comps);
                g_vetorOrdenado = 1;
                printf("Vetor ordenado por nome (Bubble). Comparacoes: %ld\n", comps);
                listarItens(mochila, numItens);
                break;
            }
            case 5: {
                long comps = 0;
                ordenarVetorPorNome(mochila, numItens, &comps);
                g_vetorOrdenado = 1;
                printf("Vetor ordenado por nome. Comparacoes: %ld\n", comps);
                listarItens(mochila, numItens);
                break;
            }
            case 12: {
                if (numItens <= 1) {
                    long comps = 0;
                    // Nada ou 1 item: ambos terão 0 comparações
                    printf("Selection: %ld comparacoes | Bubble: %ld comparacoes\n", comps, comps);
                    g_vetorOrdenado = 1;
                    break;
                }
                Item sel[MAX_ITENS];
                Item bub[MAX_ITENS];
                for (int i = 0; i < numItens; i++) { sel[i] = mochila[i]; bub[i] = mochila[i]; }
                long compsSel = 0, compsBub = 0;
                ordenarVetorPorNome(sel, numItens, &compsSel);
                ordenarVetorPorNomeBubble(bub, numItens, &compsBub);
                printf("Selection: %ld comparacoes | Bubble: %ld comparacoes\n", compsSel, compsBub);
                // Adota o resultado (Selection) no vetor principal
                for (int i = 0; i < numItens; i++) mochila[i] = sel[i];
                g_vetorOrdenado = 1;
                listarItens(mochila, numItens);
                break;
            }
            case 6: {
                if (!g_vetorOrdenado) {
                    printf("Ordene o vetor por nome antes de realizar busca binaria.\n");
                    break;
                }
                char nome[TAM_NOME]; long comps = 0;
                printf("Nome do item para busca binaria (vetor): ");
                if (fgets(nome, sizeof(nome), stdin)) {
                    removerQuebraLinha(nome);
                    int idx = buscarBinariaVetor(mochila, numItens, nome, &comps);
                    if (idx >= 0) {
                        printf("\nItem encontrado (bin): %s | %s | qtd: %d\n",
                               mochila[idx].nome, mochila[idx].tipo, mochila[idx].quantidade);
                    } else {
                        printf("Item '%s' nao encontrado (bin).\n", nome);
                    }
                    printf("Comparacoes (bin): %ld\n", comps);
                }
                break;
            }

            // Lista encadeada
            case 7: {
                Item novo;
                printf("Nome do item (max %d): ", TAM_NOME - 1);
                if (!fgets(novo.nome, sizeof(novo.nome), stdin)) break;
                removerQuebraLinha(novo.nome);
                if (!novo.nome[0]) { printf("Nome invalido.\n"); break; }
                printf("Tipo do item (max %d): ", TAM_TIPO - 1);
                if (!fgets(novo.tipo, sizeof(novo.tipo), stdin)) break;
                removerQuebraLinha(novo.tipo);
                if (!novo.tipo[0]) { printf("Tipo invalido.\n"); break; }
                printf("Quantidade: ");
                if (scanf("%d", &novo.quantidade) != 1 || novo.quantidade < 1) { limparBufferEntrada(); printf("Quantidade invalida.\n"); break; }
                limparBufferEntrada();
                inserirItemLista(&lista, &novo);
                listarItensLista(lista);
                break;
            }
            case 8: {
                char nome[TAM_NOME];
                printf("Nome do item para remover (lista): ");
                if (!fgets(nome, sizeof(nome), stdin)) break;
                removerQuebraLinha(nome);
                if (removerItemLista(&lista, nome)) printf("Item removido.\n"); else printf("Item nao encontrado.\n");
                listarItensLista(lista);
                break;
            }
            case 9:
                listarItensLista(lista);
                break;
            case 10: {
                char nome[TAM_NOME]; long comps = 0;
                printf("Nome do item para busca (lista): ");
                if (fgets(nome, sizeof(nome), stdin)) {
                    removerQuebraLinha(nome);
                    int found = buscarSequencialLista(lista, nome, &comps);
                    if (found) {
                        printf("Item encontrado (lista).\n");
                    } else {
                        printf("Item '%s' nao encontrado (lista).\n", nome);
                    }
                    printf("Comparacoes (lista seq): %ld\n", comps);
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

    liberarLista(&lista);
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
        if (strcasecmp(mochila[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

// Versão com contagem de comparações
int buscarSequencialVetor(const Item* mochila, int numItens, const char* nome, long* comps) {
    if (comps) *comps = 0;
    for (int i = 0; i < numItens; i++) {
        if (comps) (*comps)++;
        if (strcasecmp(mochila[i].nome, nome) == 0) return i;
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
    g_vetorOrdenado = 0; // após inserir, vetor deixa de estar garantidamente ordenado
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
    g_vetorOrdenado = 0;
}

// Ordena vetor por nome (Selection Sort) contabilizando comparações
void ordenarVetorPorNome(Item* mochila, int numItens, long* comps) {
    if (comps) *comps = 0;
    for (int i = 0; i < numItens - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < numItens; j++) {
            if (comps) (*comps)++;
            if (strcasecmp(mochila[j].nome, mochila[minIdx].nome) < 0) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            Item tmp = mochila[i];
            mochila[i] = mochila[minIdx];
            mochila[minIdx] = tmp;
        }
    }
}

// Ordena vetor por nome (Bubble Sort) com early-exit e contagem de comparações
void ordenarVetorPorNomeBubble(Item* mochila, int numItens, long* comps) {
    if (comps) *comps = 0;
    if (numItens <= 1) return;
    int n = numItens;
    int trocou;
    do {
        trocou = 0;
        for (int i = 1; i < n; i++) {
            if (comps) (*comps)++;
            if (strcasecmp(mochila[i - 1].nome, mochila[i].nome) > 0) {
                Item tmp = mochila[i - 1];
                mochila[i - 1] = mochila[i];
                mochila[i] = tmp;
                trocou = 1;
            }
        }
        n--; // último elemento já está na posição correta
    } while (trocou);
}

// Busca binária (vetor deve estar ordenado por nome)
int buscarBinariaVetor(const Item* mochila, int numItens, const char* nome, long* comps) {
    int l = 0, r = numItens - 1;
    if (comps) *comps = 0;
    while (l <= r) {
        int m = l + (r - l) / 2;
        int cmp = strcasecmp(mochila[m].nome, nome);
        if (comps) (*comps)++;
        if (cmp == 0) return m;
        if (cmp < 0) l = m + 1; else r = m - 1;
    }
    return -1;
}

// ---------------- Lista encadeada ----------------
void listarItensLista(const No* inicio) {
    printf("\n----------- MOCHILA (lista) -----------\n");
    if (!inicio) { printf("(vazia)\n"); return; }
    int i = 1;
    for (const No* p = inicio; p; p = p->proximo, i++) {
        printf("%2d) %-29s | %-19s | qtd: %d\n", i, p->dados.nome, p->dados.tipo, p->dados.quantidade);
    }
}

int buscarSequencialLista(const No* inicio, const char* nome, long* comps) {
    if (comps) *comps = 0;
    for (const No* p = inicio; p; p = p->proximo) {
        if (comps) (*comps)++;
        if (strcasecmp(p->dados.nome, nome) == 0) return 1;
    }
    return 0;
}

void inserirItemLista(No** inicio, const Item* novo) {
    if (!inicio || !novo) return;
    // Se existir, acumula quantidade
    for (No* p = *inicio; p; p = p->proximo) {
        if (strcasecmp(p->dados.nome, novo->nome) == 0) {
            p->dados.quantidade += novo->quantidade;
            strncpy(p->dados.tipo, novo->tipo, sizeof(p->dados.tipo) - 1);
            p->dados.tipo[sizeof(p->dados.tipo) - 1] = '\0';
            printf("Item existente (lista); quantidade atualizada.\n");
            return;
        }
    }
    // Inserção no início (simples e O(1))
    No* n = (No*)malloc(sizeof(No));
    if (!n) { printf("Falha de memoria.\n"); return; }
    n->dados = *novo;
    n->proximo = *inicio;
    *inicio = n;
    printf("Item cadastrado (lista).\n");
}

int removerItemLista(No** inicio, const char* nome) {
    if (!inicio || !*inicio) return 0;
    No* ant = NULL; No* p = *inicio;
    while (p) {
        if (strcasecmp(p->dados.nome, nome) == 0) {
            if (ant) ant->proximo = p->proximo; else *inicio = p->proximo;
            free(p);
            return 1;
        }
        ant = p; p = p->proximo;
    }
    return 0;
}

void liberarLista(No** inicio) {
    if (!inicio) return;
    No* p = *inicio;
    while (p) {
        No* nxt = p->proximo;
        free(p);
        p = nxt;
    }
    *inicio = NULL;
}
