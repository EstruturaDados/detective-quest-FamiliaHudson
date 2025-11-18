#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ============================================================================
// ESTRUTURAS
// ============================================================================

// Nível Novato: Árvore Binária - Mapa da Mansão
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Nível Aventureiro: Árvore Binária de Busca - Pistas
typedef struct Pista {
    char texto[100];
    struct Pista *esquerda;
    struct Pista *direita;
} Pista;

// Nível Mestre: Tabela Hash com Lista Encadeada
#define TAM_HASH 10

typedef struct Associacao {
    char suspeito[50];
    char pista[100];
    struct Associacao *prox;
} Associacao;

typedef struct {
    Associacao *tabela[TAM_HASH];
    int contagem[TAM_HASH];  // quantas pistas cada suspeito tem
} TabelaHash;

// ============================================================================
// PROTÓTIPOS
// ============================================================================
Sala* criarSala(const char* nome);
void conectarSalas(Sala* pai, Sala* esquerda, Sala* direita);
void explorarMansao(Sala* atual);

Pista* inserirPista(Pista* raiz, const char* texto);
void emOrdem(Pista* raiz);
void coletarPistaNaSala(const char* nomeSala);

void inicializarHash(TabelaHash* hash);
int funcaoHash(const char* str);
void inserirAssociacao(TabelaHash* hash, const char* suspeito, const char* pista);
void exibirAssociacoes(TabelaHash* hash);
void exibirSuspeitoMaisProvavel(TabelaHash* hash);
void liberarHash(TabelaHash* hash);

// ============================================================================
// VARIÁVEIS GLOBAIS
// ============================================================================
Pista* arvorePistas = NULL;
TabelaHash tabelaSuspeitos;

// ============================================================================
// MAIN
// ============================================================================
int main() {
    inicializarHash(&tabelaSuspeitos);

    // === MONTAGEM DO MAPA DA MANSÃO (Árvore fixa) ===
    Sala* hall        = criarSala("Hall de Entrada");
    Sala* biblioteca  = criarSala("Biblioteca");
    Sala* cozinha     = criarSala("Cozinha");
    Sala* salaJantar  = criarSala("Sala de Jantar");
    Sala* sotao       = criarSala("Sótão");
    Sala* porao       = criarSala("Porão");
    Sala* quarto      = criarSala("Quarto Principal");
    Sala* jardim      = criarSala("Jardim Secreto");

    conectarSalas(hall, biblioteca, cozinha);
    conectarSalas(biblioteca, salaJantar, sotao);
    conectarSalas(cozinha, porao, NULL);
    conectarSalas(salaJantar, quarto, jardim);

    printf("=== DETETIVE QUEST - MANSÃO MAL-ASSOMBRADA ===\n");
    printf("Explore a mansão, colete pistas e descubra o culpado!\n\n");

    explorarMansao(hall);

    printf("\n=== FIM DA INVESTIGAÇÃO ===\n");
    printf("Pistas coletadas (em ordem alfabética):\n");
    emOrdem(arvorePistas);

    printf("\n=== RELAÇÕES ENTRE SUSPEITOS E PISTAS ===\n");
    exibirAssociacoes(&tabelaSuspeitos);

    printf("\n=== CONCLUSÃO DO CASO ===\n");
    exibirSuspeitoMaisProvavel(&tabelaSuspeitos);

    // Liberação de memória
    liberarHash(&tabelaSuspeitos);
    // (árvore de salas é fixa, não precisa liberar)

    return 0;
}

// ============================================================================
// NÍVEL NOVATO: ÁRVORE BINÁRIA - MAPA DA MANSÃO
// ============================================================================
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esquerda = nova->direita = NULL;
    return nova;
}

void conectarSalas(Sala* pai, Sala* esquerda, Sala* direita) {
    pai->esquerda = esquerda;
    pai->direita = direita;
}

void explorarMansao(Sala* atual) {
    char opcao;
    do {
        printf("\nVocê está em: %s\n", atual->nome);
        coletarPistaNaSala(atual->nome);  // coleta automática

        printf("Para onde deseja ir?\n");
        if (atual->esquerda)  printf("  [e] %s\n", atual->esquerda->nome);
        if (atual->direita)   printf("  [d] %s\n", atual->direita->nome);
        printf("  [m] Mostrar pistas coletadas\n");
        printf("  [s] Sair da mansão\n");
        printf("Opção: ");
        scanf(" %c", &opcao);
        opcao = tolower(opcao);

        if (opcao == 'e' && atual->esquerda) {
            explorarMansao(atual->esquerda);
        } else if (opcao == 'd' && atual->direita) {
            explorarMansao(atual->direita);
        } else if (opcao == 'm') {
            printf("\n--- PISTAS COLETADAS ---\n");
            emOrdem(arvorePistas);
        } else if (opcao == 's') {
            printf("\nSaindo da mansão...\n");
            return;
        } else {
            printf("Caminho inválido ou inexistente!\n");
        }
    } while (1);
}

// ============================================================================
// NÍVEL AVENTUREIRO: ÁRVORE BINÁRIA DE BUSCA - PISTAS
// ============================================================================
void coletarPistaNaSala(const char* nomeSala) {
    if (strcmp(nomeSala, "Biblioteca") == 0) {
        arvorePistas = inserirPista(arvorePistas, "Livro rasgado com assinatura do Coronel Mostarda");
        inserirAssociacao(&tabelaSuspeitos, "Coronel Mostarda", "Livro rasgado");
    }
    if (strcmp(nomeSala, "Cozinha") == 0) {
        arvorePistas = inserirPista(arvorePistas, "Faca com mancha vermelha");
        inserirAssociacao(&tabelaSuspeitos, "Sra. Branco", "Faca manchada");
    }
    if (strcmp(nomeSala, "Sótão") == 0) {
        arvorePistas = inserirPista(arvorePistas, "Carta de amor endereçada à Srta. Escarlate");
        inserirAssociacao(&tabelaSuspeitos, "Srta. Escarlate", "Carta de amor");
    }
    if (strcmp(nomeSala, "Porão") == 0) {
        arvorePistas = inserirPista(arvorePistas, "Pegadas de sapato masculino tamanho 43");
        inserirAssociacao(&tabelaSuspeitos, "Professor Black", "Pegadas grandes");
    }
    if (strcmp(nomeSala, "Quarto Principal") == 0) {
        arvorePistas = inserirPista(arvorePistas, "Veneno no criado-mudo");
        inserirAssociacao(&tabelaSuspeitos, "Sra. Pavão", "Frasco de veneno");
    }
    if (strcmp(nomeSala, "Jardim Secreto") == 0) {
        arvorePistas = inserirPista(arvorePistas, "Revólver enterrado com 5 balas disparadas");
        inserirAssociacao(&tabelaSuspeitos, "Coronel Mostarda", "Revólver");
        inserirAssociacao(&tabelaSuspeitos, "Srta. Escarlate", "Revólver");
    }
}

Pista* inserirPista(Pista* raiz, const char* texto) {
    if (raiz == NULL) {
        Pista* nova = (Pista*)malloc(sizeof(Pista));
        strcpy(nova->texto, texto);
        nova->esquerda = nova->direita = NULL;
        return nova;
    }
    if (strcmp(texto, raiz->texto) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    else
        raiz->direita = inserirPista(raiz->direita, texto);
    return raiz;
}

void emOrdem(Pista* raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda);
        printf("• %s\n", raiz->texto);
        emOrdem(raiz->direita);
    }
}

// ============================================================================
// NÍVEL MESTRE: TABELA HASH COM LISTA ENCADEADA
// ============================================================================
void inicializarHash(TabelaHash* hash) {
    for (int i = 0; i < TAM_HASH; i++) {
        hash->tabela[i] = NULL;
        hash->contagem[i] = 0;
    }
}

int funcaoHash(const char* str) {
    return toupper(str[0]) % TAM_HASH;  // simples: primeira letra maiúscula
}

void inserirAssociacao(TabelaHash* hash, const char* suspeito, const char* pista) {
    int indice = funcaoHash(suspeito);
    Associacao* nova = (Associacao*)malloc(sizeof(Associacao));
    strcpy(nova->suspeito, suspeito);
    strcpy(nova->pista, pista);
    nova->prox = hash->tabela[indice];
    hash->tabela[indice] = nova;
    hash->contagem[indice]++;
}

void exibirAssociacoes(TabelaHash* hash) {
    for (int i = 0; i < TAM_HASH; i++) {
        if (hash->tabela[i] != NULL) {
            printf("\nSuspeito: %s\n", hash->tabela[i]->suspeito);
            Associacao* atual = hash->tabela[i];
            while (atual != NULL) {
                printf("  → %s\n", atual->pista);
                atual = atual->prox;
            }
        }
    }
}

void exibirSuspeitoMaisProvavel(TabelaHash* hash) {
    int max = 0;
    char culpado[50] = "Ninguém";

    for (int i = 0; i < TAM_HASH; i++) {
        if (hash->contagem[i] > max) {
            max = hash->contagem[i];
            if (hash->tabela[i] != NULL)
                strcpy(culpado, hash->tabela[i]->suspeito);
        }
    }

    if (max >= 2) {
        printf("CULPADO ENCONTRADO: %s (citado em %d pistas!)\n", culpado, max);
    } else {
        printf("Caso não resolvido. Mais pistas necessárias.\n");
    }
}

void liberarHash(TabelaHash* hash) {
    for (int i = 0; i < TAM_HASH; i++) {
        Associacao* atual = hash->tabela[i];
        while (atual != NULL) {
            Associacao* temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
}
