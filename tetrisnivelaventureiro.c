#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ================================
// ESTRUTURA DAS PEÇAS
// ================================

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome;  // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;     // Identificador único
} Peca;

// ================================
// ESTRUTURA DA FILA (peças futuras)
// ================================

typedef struct {
    Peca pecas[5]; // Capacidade fixa de 5
    int frente;
    int tras;
    int tamanho;
} Fila;

// ================================
// ESTRUTURA DA PILHA (peças reservadas)
// ================================

typedef struct {
    Peca pecas[3]; // Capacidade fixa de 3
    int topo;      // Índice do topo da pilha
} Pilha;


// ================================
// FUNÇÕES DE CONTROLE DA FILA
// ================================

void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
}

int filaCheia(Fila *fila) {
    return fila->tamanho == 5;
}

int filaVazia(Fila *fila) {
    return fila->tamanho == 0;
}

// Adiciona uma nova peça ao final da fila (enqueue)
void inserirPecaFila(Fila *fila, Peca nova) {
    if (filaCheia(fila)) {
        // Nesse jogo, a fila sempre mantém 5 peças — nunca deixamos passar disso.
        return;
    }

    fila->tras = (fila->tras + 1) % 5;
    fila->pecas[fila->tras] = nova;
    fila->tamanho++;
}

// Remove uma peça da frente da fila (dequeue)
Peca removerPecaFila(Fila *fila) {
    Peca removida = {'-', -1}; // Caso padrão (inválido)

    if (!filaVazia(fila)) {
        removida = fila->pecas[fila->frente];
        fila->frente = (fila->frente + 1) % 5;
        fila->tamanho--;
    }
    return removida;
}

// ================================
// FUNÇÕES DE CONTROLE DA PILHA
// ================================

void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

int pilhaCheia(Pilha *pilha) {
    return pilha->topo == 2; // Máximo de 3 peças
}

int pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

// Adiciona uma peça ao topo da pilha (push)
void empilhar(Pilha *pilha, Peca nova) {
    if (pilhaCheia(pilha)) {
        printf("\n⚠️ A pilha de reserva está cheia! Não é possível reservar mais peças.\n");
        return;
    }
    pilha->pecas[++pilha->topo] = nova;
    printf("\n📦 Peça [%c %d] reservada!\n", nova.nome, nova.id);
}

// Remove a peça do topo da pilha (pop)
Peca desempilhar(Pilha *pilha) {
    Peca removida = {'-', -1};

    if (pilhaVazia(pilha)) {
        printf("\n⚠️ A pilha de reserva está vazia! Nenhuma peça para usar.\n");
        return removida;
    }

    removida = pilha->pecas[pilha->topo--];
    printf("\n🎮 Peça [%c %d] usada da reserva!\n", removida.nome, removida.id);
    return removida;
}

// ================================
// GERAÇÃO AUTOMÁTICA DE PEÇAS
// ================================

Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// ================================
// EXIBIÇÃO DE ESTADOS
// ================================

void exibirFila(Fila *fila) {
    printf("\nFila de peças futuras:\n");
    if (filaVazia(fila)) {
        printf("(vazia)\n");
        return;
    }
    for (int i = 0; i < fila->tamanho; i++) {
        int indice = (fila->frente + i) % 5;
        printf("[%c %d] ", fila->pecas[indice].nome, fila->pecas[indice].id);
    }
    printf("\n");
}

void exibirPilha(Pilha *pilha) {
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(pilha)) {
        printf("(vazia)\n");
        return;
    }
    for (int i = pilha->topo; i >= 0; i--) {
        printf("[%c %d] ", pilha->pecas[i].nome, pilha->pecas[i].id);
    }
    printf("\n");
}

// ================================
// FUNÇÃO PRINCIPAL
// ================================

int main() {
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    srand(time(NULL)); // Semente para gerar aleatoriedade
    int idGlobal = 0;  // ID global para diferenciar cada peça

    // Inicializa a fila com 5 peças iniciais
    for (int i = 0; i < 5; i++) {
        inserirPecaFila(&fila, gerarPeca(idGlobal++));
    }

    int opcao;
    do {
        printf("\n==============================\n");
        printf("   ESTADO ATUAL DO JOGO\n");
        printf("==============================\n");
        exibirFila(&fila);
        exibirPilha(&pilha);

        printf("\nOpções de ação:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Reservar peça (mover para pilha)\n");
        printf("3 - Usar peça reservada (retirar da pilha)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                if (!filaVazia(&fila)) {
                    Peca jogada = removerPecaFila(&fila);
                    printf("\n🎯 Peça [%c %d] jogada!\n", jogada.nome, jogada.id);
                    // Após jogar, uma nova peça é gerada e entra na fila
                    inserirPecaFila(&fila, gerarPeca(idGlobal++));
                } else {
                    printf("\n⚠️ A fila está vazia!\n");
                }
                break;
            }

            case 2: {
                if (!filaVazia(&fila)) {
                    // Move a peça da frente da fila para o topo da pilha
                    Peca reservada = removerPecaFila(&fila);
                    empilhar(&pilha, reservada);
                    // Gera nova peça para manter fila cheia
                    inserirPecaFila(&fila, gerarPeca(idGlobal++));
                } else {
                    printf("\n⚠️ A fila está vazia!\n");
                }
                break;
            }

            case 3: {
                // Remove a peça do topo da pilha (usando-a)
                desempilhar(&pilha);
                break;
            }

            case 0:
                printf("\n👋 Encerrando o jogo. Até a próxima rodada!\n");
                break;

            default:
                printf("\n❌ Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
