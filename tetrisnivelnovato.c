#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ================================
// ESTRUTURA DE DADOS DO JOGO
// ================================

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome;  // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;     // Identificador único da peça
} Peca;

// Estrutura que representa a Fila de peças
typedef struct {
    Peca pecas[5]; // Capacidade fixa da fila (5 posições)
    int frente;    // Índice da frente (de onde a peça é jogada)
    int tras;      // Índice do final (onde novas peças entram)
    int tamanho;   // Quantidade atual de peças na fila
} Fila;


// ================================
// FUNÇÕES DE CONTROLE DA FILA
// ================================

// Inicializa a fila
void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *fila) {
    return fila->tamanho == 5;
}

// Verifica se a fila está vazia
int filaVazia(Fila *fila) {
    return fila->tamanho == 0;
}

// ================================
// FUNÇÃO DE GERAÇÃO DE PEÇA
// ================================

// Gera automaticamente uma nova peça com tipo aleatório
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'}; // Tipos possíveis
    nova.nome = tipos[rand() % 4];       // Sorteia uma das 4 letras
    nova.id = id;                        // Define ID único
    return nova;
}

// ================================
// OPERAÇÕES DA FILA
// ================================

// Insere nova peça no final da fila (enqueue)
void inserirPeca(Fila *fila, Peca nova) {
    if (filaCheia(fila)) {
        printf("\n⚠️ A fila está cheia! Não é possível inserir nova peça.\n");
        return;
    }

    // Cálculo circular do índice do final
    fila->tras = (fila->tras + 1) % 5;
    fila->pecas[fila->tras] = nova;
    fila->tamanho++;
    printf("\n✅ Peça [%c %d] adicionada à fila!\n", nova.nome, nova.id);
}

// Remove a peça da frente da fila (dequeue)
void jogarPeca(Fila *fila) {
    if (filaVazia(fila)) {
        printf("\n⚠️ A fila está vazia! Nenhuma peça para jogar.\n");
        return;
    }

    Peca removida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % 5;
    fila->tamanho--;

    printf("\n🎮 Peça [%c %d] foi jogada!\n", removida.nome, removida.id);
}

// Exibe o estado atual da fila
void exibirFila(Fila *fila) {
    printf("\n==============================\n");
    printf("  Fila de peças atuais:\n");
    printf("==============================\n");

    if (filaVazia(fila)) {
        printf("(vazia)\n");
        return;
    }

    // Percorre a fila de forma circular
    for (int i = 0; i < fila->tamanho; i++) {
        int indice = (fila->frente + i) % 5;
        printf("[%c %d] ", fila->pecas[indice].nome, fila->pecas[indice].id);
    }
    printf("\n");
}

// ================================
// FUNÇÃO PRINCIPAL
// ================================
int main() {
    Fila fila;
    inicializarFila(&fila);

    srand(time(NULL)); // Semente para gerar peças aleatórias

    int idGlobal = 0;  // Contador global de IDs únicos

    // Inicializa a fila com 5 peças
    for (int i = 0; i < 5; i++) {
        inserirPeca(&fila, gerarPeca(idGlobal++));
    }

    int opcao;
    do {
        exibirFila(&fila);

        printf("\nOpções de ação:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogarPeca(&fila);
                break;
            case 2:
                inserirPeca(&fila, gerarPeca(idGlobal++));
                break;
            case 0:
                printf("\n👋 Encerrando o jogo. Até a próxima!\n");
                break;
            default:
                printf("\n❌ Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
