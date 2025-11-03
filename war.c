// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h> // Para setlocale

// --- Constantes Globais ---
// *** CORREÇÃO APLICADA AQUI ***
// Trocamos 'const int' por '#define' para criar constantes
// que o compilador C aceita para definir o tamanho de vetores.
#define NUM_TERRITORIOS 5
#define NUM_MISSOES 2
#define MAX_NOME 50
#define MAX_COR 20

// --- Estrutura de Dados ---
// Define a estrutura para um território.
// Agora o C vai ler char nome[50] e char cor[20], o que é válido.
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa.

// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa(int numTerritorios);
void inicializarTerritorios(Territorio* mapa);
void liberarMemoria(Territorio* mapa); // <-- Verifique se o seu bate com este

// Funções de interface com o usuário:
void exibirMenuPrincipal();
void exibirMapa(const Territorio* mapa); 
void exibirMissao(int idMissao, const char* missoes[]);

// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio* mapa, const char* corJogador);
void simularAtaque(Territorio* atacante, Territorio* defensor, const char* corJogador);
int sortearMissao(int totalMissoes);
int verificarVitoria(const Territorio* mapa, int idMissao, const char* corJogador); 

// Função utilitária:
void limparBufferEntrada();

// --- Função Principal (main) ---
int main() {
    // 1. Configuração Inicial (Setup):
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    // Aloca o mapa
    // O C agora entende NUM_TERRITORIOS como '5'
    Territorio* mapa = alocarMapa(NUM_TERRITORIOS);
    if (mapa == NULL) {
        printf("Erro fatal: Falha ao alocar memória para o mapa!\n");
        return 1;
    }

    inicializarTerritorios(mapa);

    const char* corJogador = "Azul";
    
    // Define as missões
    // O C agora entende NUM_MISSOES como '2'
    const char* missoes[NUM_MISSOES] = {
        "DESTRUIR O EXERCITO VERDE",
        "CONQUISTAR 3 TERRITORIOS"
    };
    // Sorteia a missão
    int idMissaoJogador = sortearMissao(NUM_MISSOES);
    
    printf("--- BEM-VINDO AO WAR ESTRUTURADO ---\n");
    printf("Você joga com a cor: %s\n", corJogador);

    // 2. Laço Principal do Jogo (Game Loop):
    int escolha = -1;
    int venceu = 0;
    
    do {
        exibirMapa(mapa);
        exibirMissao(idMissaoJogador, missoes);
        exibirMenuPrincipal();

        scanf("%d", &escolha);
        limparBufferEntrada();

        switch (escolha) {
            case 1:
                faseDeAtaque(mapa, corJogador);
                break;
            case 2:
                if (verificarVitoria(mapa, idMissaoJogador, corJogador)) {
                    printf("\n************************************************\n");
                    printf("*** VITORIA! Voce cumpriu sua missao! ***\n");
                    printf("************************************************\n");
                    venceu = 1; 
                } else {
                    printf("\n-> Missao ainda nao cumprida. Continue lutando!\n");
                }
                break;
            case 0:
                printf("Encerrando o jogo... Ate a proxima!\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }

        // Pausa
        if (escolha != 0 && !venceu) {
             printf("\n(Pressione ENTER para continuar...)\n");
             getchar();
        }

    } while (escolha != 0 && !venceu);

    // 3. Limpeza:
    liberarMemoria(mapa);

    return 0;
}

// --- Implementação das Funções ---

/**
 * @brief Aloca dinamicamente a memória para o vetor de territórios.
 */
Territorio* alocarMapa(int numTerritorios) {
    Territorio* mapa = (Territorio*) calloc(numTerritorios, sizeof(Territorio));
    return mapa;
}

/**
 * @brief Preenche os dados iniciais de cada território no mapa.
 */
void inicializarTerritorios(Territorio* mapa) {
    // Usamos 'strcpy' para copiar strings para dentro da struct
    strcpy(mapa[0].nome, "Brasil");
    strcpy(mapa[0].cor, "Azul"); 
    mapa[0].tropas = 5;

    strcpy(mapa[1].nome, "Argentina");
    strcpy(mapa[1].cor, "Verde"); 
    mapa[1].tropas = 3;

    strcpy(mapa[2].nome, "Peru");
    strcpy(mapa[2].cor, "Azul"); 
    mapa[2].tropas = 2;

    strcpy(mapa[3].nome, "EUA");
    strcpy(mapa[3].cor, "Vermelho"); 
    mapa[3].tropas = 4;

    strcpy(mapa[4].nome, "Canada");
    strcpy(mapa[4].cor, "Verde"); 
    mapa[4].tropas = 3;
}

/**
 * @brief Libera a memória previamente alocada para o mapa.
 */
void liberarMemoria(Territorio* mapa) {
    printf("Liberando memoria do mapa...\n");
    free(mapa);
    mapa = NULL; 
}

/**
 * @brief Imprime na tela o menu de ações disponíveis.
 */
void exibirMenuPrincipal() {
    printf("\n--- MENU DE ACOES ---\n");
    printf("1. Atacar\n");
    printf("2. Verificar Missao\n");
    printf("0. Sair\n");
    printf("Escolha sua acao: ");
}

/**
 * @brief Mostra o estado atual de todos os territórios no mapa.
 */
void exibirMapa(const Territorio* mapa) {
    printf("\n------------------- MAPA ATUAL -------------------\n");
    printf("ID | NOME            | COR        | TROPAS\n");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        // Usamos '-' para alinhar o texto à esquerda
        printf("%-2d | %-15s | %-10s | %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("--------------------------------------------------\n");
}

/**
 * @brief Exibe a descrição da missão atual do jogador.
 */
void exibirMissao(int idMissao, const char* missoes[]) {
    printf("Sua Missao: [ %s ]\n", missoes[idMissao]);
}

/**
 * @brief Gerencia a interface para a ação de ataque.
 */
void faseDeAtaque(Territorio* mapa, const char* corJogador) {
    int idAtacante, idDefensor;
    
    printf("\n--- FASE DE ATAQUE ---\n");
    printf("Digite o ID (0-%d) do territorio ATACANTE: ", NUM_TERRITORIOS - 1);
    scanf("%d", &idAtacante);
    printf("Digite o ID (0-%d) do territorio DEFENSOR: ", NUM_TERRITORIOS - 1);
    scanf("%d", &idDefensor);
    limparBufferEntrada(); 

    // Validação de IDs
    if (idAtacante < 0 || idAtacante >= NUM_TERRITORIOS ||
        idDefensor < 0 || idDefensor >= NUM_TERRITORIOS) {
        printf("IDs invalidos! Ataque cancelado.\n");
        return; // Sai da função
    }

    // Chama a função de lógica, passando os endereços
    simularAtaque(&mapa[idAtacante], &mapa[idDefensor], corJogador);
} // <-- Verifique se essa chave } está no seu código

/**
 * @brief Executa a lógica de uma batalha entre dois territórios.
 */
void simularAtaque(Territorio* atacante, Territorio* defensor, const char* corJogador) {
    // --- Validações de Regras ---
    // 'strcmp' compara duas strings. Retorna 0 se forem iguais.
    if (strcmp(atacante->cor, corJogador) != 0) {
        printf("ATAQUE FALHOU: Voce nao domina %s.\n", atacante->nome);
        return;
    }
    if (atacante->tropas <= 1) {
        printf("ATAQUE FALHOU: %s precisa de mais de 1 tropa para atacar.\n", atacante->nome);
        return;
    }
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("ATAQUE FALHOU: Nao pode atacar seu proprio territorio (%s).\n", defensor->nome);
        return;
    }
    
    // --- Lógica da Batalha ---
    printf("\nBATALHA: %s (%s) ataca %s (%s)!\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    // Simulação dos dados
    int dadoAtaque = (rand() % 6) + 1; // Número de 1 a 6
    int dadoDefesa = (rand() % 6) + 1; // Número de 1 a 6
    printf("... Dados: Ataque [ %d ] vs Defesa [ %d ] ...\n", dadoAtaque, dadoDefesa);

    // Lógica do Nível Aventureiro: Empate favorece o atacante
    if (dadoAtaque >= dadoDefesa) {
        printf("VITORIA DO ATAQUE! %s perdeu 1 tropa.\n", defensor->nome);
        defensor->tropas--;

        // Se defensor perder todas as tropas, é conquistado
        if (defensor->tropas <= 0) {
            printf("CONQUISTA! %s tomou %s!\n", atacante->nome, defensor->nome);
            strcpy(defensor->cor, atacante->cor); // Muda a cor
            defensor->tropas = 1; // Move 1 tropa
            atacante->tropas--;   // Perde a tropa que moveu
        }
    } else {
        printf("VITORIA DA DEFESA! %s perdeu 1 tropa.\n", atacante->nome);
        atacante->tropas--; // Atacante perde 1 tropa
    }
} // <-- Verifique se essa chave } está no seu código

/**
 * @brief Sorteia e retorna um ID de missão aleatório.
 */
int sortearMissao(int totalMissoes) {
    return rand() % totalMissoes; // Retorna 0 ou 1
}

/**
 * @brief Verifica se o jogador cumpriu os requisitos de sua missão.
 */
int verificarVitoria(const Territorio* mapa, int idMissao, const char* corJogador) {
    
    // Missão 0: "DESTRUIR O EXERCITO VERDE"
    if (idMissao == 0) {
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, "Verde") == 0) {
                return 0; // Achou um território verde, missão não cumprida
            }
        }
        return 1; // Não achou nenhum verde, missão cumprida!
    }
    
    // Missão 1: "CONQUISTAR 3 TERRITORIOS"
    else if (idMissao == 1) {
        int contador = 0;
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                contador++;
            }
        }
        return (contador >= 3); // Retorna 1 (true) se tiver 3 ou mais, 0 (false) se não
    }

    return 0; // Caso de missão desconhecida
}

/**
 * @brief Função utilitária para limpar o buffer de entrada do teclado (stdin).
 */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
