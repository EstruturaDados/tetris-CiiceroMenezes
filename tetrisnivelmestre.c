#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ================================
// DEFINIÇÃO DAS ESTRUTURAS
// ================================

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // Identificador único da peça
} Peca;

// Estrutura de fila circular para peças futuras
typedef struct {
    Peca pecas[5];
    int frente;
    int tras;
    int tamanho;
} Fila;

// Estrutura de pilha linear para peças reservadas
typedef struct {
    Peca pecas[3];
    int topo;
} Pilha;


// ================================
// FUNÇÕES AUXILIARES DE CONTROLE
// ================================

// Inicializa a fila circular
void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
}

// Inicializa a pilha linear
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

// Verifica estados
int filaCheia(Fila *fila) { return fila->tamanho == 5; }
int filaVazia(Fila *fila) { return fila->tamanho == 0; }
int pilhaCheia(Pilha *pilha) { return pilha->topo == 2; }
int pilhaVazia(Pilha *pilha) { return pilha->topo == -1; }

// ================================
// GERAÇÃO AUTOMÁTICA DE PEÇAS
// ================================

// Gera uma nova peça com tipo aleatório e ID único
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// ================================
// FUNÇÕES DE FILA
// ================================

// Insere uma peça ao final da fila
void inserirPecaFila(Fila *fila, Peca nova) {
    if (filaCheia(fila)) return;
    fila->tras = (fila->tras + 1) % 5;
    fila->pecas[fila->tras] = nova;
    fila->tamanho++;
}

// Remove e retorna a peça da frente da fila
Peca removerPecaFila(Fila *fila) {
    Peca removida = {'-', -1};
    if (filaVazia(fila)) return removida;

    removida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % 5;
    fila->tamanho--;
    return removida;
}

// Retorna (sem remover) a peça da frente da fila
Peca frenteFila(Fila *fila) {
    if (filaVazia(fila)) return (Peca){'-', -1};
    return fila->pecas[fila->frente];
}

// ================================
// FUNÇÕES DE PILHA
// ================================

// Empilha uma peça
void empilhar(Pilha *pilha, Peca nova) {
    if (pilhaCheia(pilha)) {
        printf("\n⚠️  A pilha está cheia! Não é possível reservar mais peças.\n");
        return;
    }
    pilha->pecas[++pilha->topo] = nova;
    printf("\n📦 Peça [%c %d] reservada!\n", nova.nome, nova.id);
}

// Desempilha e retorna a peça do topo
Peca desempilhar(Pilha *pilha) {
    if (pilhaVazia(pilha)) {
        printf("\n⚠️  A pilha está vazia! Nenhuma peça para usar.\n");
        return (Peca){'-', -1};
    }
    Peca removida = pilha->pecas[pilha->topo--];
    printf("\n🎮 Peça [%c %d] usada da reserva!\n", removida.nome, removida.id);
    return removida;
}

// Retorna (sem remover) a peça do topo
Peca topoPilha(Pilha *pilha) {
    if (pilhaVazia(pilha)) return (Peca){'-', -1};
    return pilha->pecas[pilha->topo];
}

// ================================
// FUNÇÕES DE EXIBIÇÃO
// ================================

void exibirFila(Fila *fila) {
    printf("Fila de peças futuras: ");
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
// FUNÇÕES DE TROCA
// ================================

// Troca a peça da frente da fila com o topo da pilha
void trocarFrenteComTopo(Fila *fila, Pilha *pilha) {
    if (filaVazia(fila) || pilhaVazia(pilha)) {
        printf("\n⚠️  Não é possível realizar a troca: estruturas vazias.\n");
        return;
    }

    int indiceFrente = fila->frente;
    Peca temp = fila->pecas[indiceFrente];
    fila->pecas[indiceFrente] = pilha->pecas[pilha->topo];
    pilha->pecas[pilha->topo] = temp;

    printf("\n🔄 Troca realizada entre frente da fila e topo da pilha!\n");
}

// Troca as 3 primeiras peças da fila com as 3 da pilha
void trocarTresEmTres(Fila *fila, Pilha *pilha) {
    if (fila->tamanho < 3 || pilha->topo < 2) {
        printf("\n⚠️  É necessário pelo menos 3 peças em cada estrutura para a troca múltipla.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int indiceFila = (fila->frente + i) % 5;
        Peca temp = fila->pecas[indiceFila];
        fila->pecas[indiceFila] = pilha->pecas[pilha->topo - i];
        pilha->pecas[pilha->topo - i] = temp;
    }

    printf("\n🔁 Troca múltipla entre as 3 primeiras da fila e as 3 da pilha realizada!\n");
}

// ================================
// FUNÇÃO PRINCIPAL
// ================================

int main() {
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    srand(time(NULL));
    int idGlobal = 0;
    int opcao;

    // Inicializa a fila com 5 peças
    for (int i = 0; i < 5; i++) {
        inserirPecaFila(&fila, gerarPeca(idGlobal++));
    }

    do {
        printf("\n=================================\n");
        printf("       ESTADO ATUAL DO JOGO\n");
        printf("=================================\n");
        exibirFila(&fila);
        exibirPilha(&pilha);

        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar as 3 primeiras da fila com as 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: { // Jogar peça
                if (!filaVazia(&fila)) {
                    Peca jogada = removerPecaFila(&fila);
                    printf("\n🎯 Peça [%c %d] jogada!\n", jogada.nome, jogada.id);
                    inserirPecaFila(&fila, gerarPeca(idGlobal++));
                } else {
                    printf("\n⚠️  A fila está vazia!\n");
                }
                break;
            }

            case 2: { // Reservar peça
                if (!filaVazia(&fila)) {
                    Peca reservada = removerPecaFila(&fila);
                    empilhar(&pilha, reservada);
                    inserirPecaFila(&fila, gerarPeca(idGlobal++));
                } else {
                    printf("\n⚠️  A fila está vazia!\n");
                }
                break;
            }

            case 3: { // Usar peça da reserva
                desempilhar(&pilha);
                break;
            }

            case 4: { // Troca individual
                trocarFrenteComTopo(&fila, &pilha);
                break;
            }

            case 5: { // Troca em bloco (3x3)
                trocarTresEmTres(&fila, &pilha);
                break;
            }

            case 0:
                printf("\n👋 Encerrando o jogo. Obrigado por jogar o Tetris Stack!\n");
                break;

            default:
                printf("\n❌ Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
