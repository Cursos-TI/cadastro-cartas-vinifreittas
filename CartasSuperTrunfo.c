// ==================================================================
// Super Trunfo - Países
// Tema 6.5 (extra) - Implementação do modo partida e melhorias
// Objetivo: Implementar o modo partida e funcionalidades adjacentes, e melhorar o código.
// ==================================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <time.h>
#include <stdarg.h>

// Constantes
#define ARQUIVO_REGISTRO "registro.bin"

// Estilos de texto (ANSI)
#define NEGRITO "\033[1m"
#define ITALICO "\033[3m"
#define AZUL "\033[1;34m"
#define VERDE "\033[1;32m"
#define VERMELHO "\033[1;31m"
#define RESET "\033[0m"

// Extrutura que representa uma carta no jogo.
typedef struct {
    char codigo[10];
    char estado;
    int cidade;
    unsigned int populacao;
    float area;
    float pib;
    int pontos_turisticos;
    float densidade_populacional;
    float pib_per_capita;
} Carta;

// Estrutura para lista de cartas.
typedef struct {
    Carta* cartas;
    size_t tamanho;
    size_t capacidade;
} Lista_de_cartas;

// Codificação para o modo de exibição da carta.
typedef enum {
    COMPLETO,
    SIMPLIFICADO
} Modo_exibicao;

// Codificação para o resultado da competição.
typedef enum {
    VITORIA,
    DERROTA,
    EMPATE,
    ERRO
} Resultado_competicao;



/* ========================== Utilitários ========================== */


Lista_de_cartas inicializar() {
    return (Lista_de_cartas) {NULL, 0, 0};
}

void alocar_espaco(Lista_de_cartas *lista, size_t quantidade) {
    lista->capacidade += quantidade;
    lista->cartas = realloc(lista->cartas, sizeof(Carta) * lista->capacidade);
}

void finalizar(Lista_de_cartas *lista) {
    free(lista->cartas);
    *lista = (Lista_de_cartas) {NULL, 0, 0};
}

// Retorna uma lista com todos os registros.
Lista_de_cartas listar_cartas() {
    Lista_de_cartas lista = inicializar();
    FILE *registro = fopen(ARQUIVO_REGISTRO, "rb");

    Carta temp;
    while (fread(&temp, sizeof(Carta), 1, registro)) {
        alocar_espaco(&lista, 1);
        lista.cartas[lista.tamanho] = temp;
        lista.tamanho++;
    }
    fclose(registro);
    return lista;
}

// Salva um registro.
void salvar_dados(Carta dados) {
    FILE *registro = fopen(ARQUIVO_REGISTRO, "ab"); 
    fwrite(&dados, sizeof(Carta), 1, registro);
    fclose(registro);
}

// Exlui registro por código, reescrevendo o registro.
void excluir_dados(char codigo[10], const Lista_de_cartas *lista) {
    FILE *registro = fopen(ARQUIVO_REGISTRO, "wb");

    for (size_t i = 0; i < lista->tamanho; i++) {
        if (strcmp(lista->cartas[i].codigo, codigo) != 0) {
            fwrite(&lista->cartas[i], sizeof(Carta), 1, registro);
        }
    }
    fclose(registro);
}

// Embaralha lista de cartas.
void embaralhar_lista(Lista_de_cartas *lista) {
    if (!lista || lista->tamanho <= 1) return;

    for (size_t i = 0; i < lista->tamanho; i++){
        size_t aleatorio = (size_t) rand() % lista->tamanho;

        Carta temp = lista->cartas[aleatorio];  
        lista->cartas[aleatorio] = lista->cartas[i];
        lista->cartas[i] = temp;
    } 
}

// Porciona lista de cartas.
Lista_de_cartas porcionar_lista(Lista_de_cartas lista, int numero_de_porcoes, int porcao) {
    Lista_de_cartas mao = inicializar();

    int tamanho_ajustado = (lista.tamanho % 2) ? lista.tamanho - 1 : lista.tamanho; // Garante que a lista tenha um tamanho par.

    size_t cartas_por_porcao = (size_t) tamanho_ajustado / numero_de_porcoes;
    alocar_espaco(&mao, cartas_por_porcao);
    
    int indice_inicial = (cartas_por_porcao) * (porcao - 1);
    memcpy(mao.cartas, lista.cartas + indice_inicial, sizeof(Carta) * cartas_por_porcao); // Copia o bloco de cartas pra mao.
    
    mao.tamanho = cartas_por_porcao;
    return mao;
}

void imprimir_mensagem(char *msg, char *formatação) {
    printf("\n %s%s%s \n", formatação, msg, RESET); 
    sleep(2);
}

void limpar_tela() {
    system("cls || clear");
}



/* ========================== Funcionalidades ========================== */


void cadastrar_carta(void) {
    Carta dados;
    limpar_tela();

    printf("Bem-vindo ao sistema de cadastro de cartas!\n");
    printf("Para o cadastro de sua carta, preencha os dados solicitados a seguir...\n");

    printf("\n Letra do Estado (A á H): ");
    scanf(" %c", &dados.estado);

    printf("\n Numero da cidade (01 á 04): ");
    scanf("%i", &dados.cidade);

    printf("\n População: ");
    scanf("%u", &dados.populacao);

    printf("\n Área territorial (em km²): ");
    scanf("%f", &dados.area);

    printf("\n PIB (em bilhões): ");
    scanf("%f", &dados.pib);

    printf("\n N° de pontos turisticos: ");
    scanf("%i", &dados.pontos_turisticos);

    dados.densidade_populacional = (float) dados.populacao / dados.area;
    dados.pib_per_capita = (float) (dados.pib * 1000000000) / dados.populacao;
    sprintf(dados.codigo, "%c%02i", dados.estado, dados.cidade);

    salvar_dados(dados);
    imprimir_mensagem("Carta registrada com sucesso!", VERDE);
}

void remover_carta(Lista_de_cartas *lista) {
    char codigo[10];

    printf("\nQual o código da carta? -> ");
    scanf("%s", codigo);

    excluir_dados(codigo, lista);
    imprimir_mensagem("Carta exluida com sucesso!", VERDE);
}

void exibir_carta(const Carta dados, Modo_exibicao modo) {
    switch (modo) {
        case COMPLETO:
            printf("Cidade %s[%s]%s \n\n", AZUL, dados.codigo, RESET);

            printf("População: %u hab. \n", dados.populacao);
            printf("Área territorial: %.2f km² \n", dados.area);
            printf("PIB: %.2f bilhões \n", dados.pib);
            printf("N° de pontos turisticos: %i \n\n", dados.pontos_turisticos);

            printf("Densidade populacional: %.2f hab. por km² \n", dados.densidade_populacional);
            printf("PIB per capita: %.2f reais \n\n", dados.pib_per_capita);
            break;
        
        case SIMPLIFICADO:
            char codigo[20];
            sprintf(codigo, "%s[%s]%s", AZUL, dados.codigo, RESET);

            printf("%-15s População: %u hab.\n", codigo, dados.populacao);
            printf("%-5s Área: %.2f km²\n", "", dados.area);
            printf("%-5s PIB: %.2f bi.\n", "", dados.pib);
            printf("%-5s Pontos turísticos: %i\n", "", dados.pontos_turisticos);
            break;
    }
}

void distribuir_cartas(int qtd_jogadores, ...) {
    va_list args;
    va_start(args, qtd_jogadores);

    limpar_tela();
    
    Lista_de_cartas lista = listar_cartas();
    if (lista.tamanho < qtd_jogadores) {
        imprimir_mensagem("Cartas insuficientes!", NEGRITO);
        return;
    } else if (qtd_jogadores < 2) {
        imprimir_mensagem("Jogadores insuficientes!", NEGRITO);
        return;
    }
    
    embaralhar_lista(&lista);

    printf("Cartas distribuídas!\n"); 
    for (int i = 1; i <= qtd_jogadores; i++) {
        Lista_de_cartas *jogador = va_arg(args, Lista_de_cartas *);

        *jogador = porcionar_lista(lista, qtd_jogadores, i);
        printf("Player %i recebeu %zu cartas.\n", i, jogador->tamanho);
    }
    va_end(args);

    finalizar(&lista);
    sleep(2);
}

Resultado_competicao competir_atributo(int atributo, Carta carta, Carta oponente) {
    int p1, p2;
    limpar_tela();
    
    switch (atributo) {
            case 1: // População.
                printf("População: %u  VS  %u", carta.populacao, oponente.populacao);
                p1 = carta.populacao > oponente.populacao;
                p2 = carta.populacao < oponente.populacao; break;
            
            case 2: // Área
                printf("Área: %.2f  VS  %.2f", carta.area, oponente.area);
                p1 = carta.area > oponente.area; 
                p2 = carta.area < oponente.area; break;
            
            case 3: // PIB
                printf("PIB: %.2f  VS  %.2f", carta.pib, oponente.pib);
                p1 = carta.pib > oponente.pib;
                p2 = carta.pib < oponente.pib; break;

            case 4: // Pontos turisticos
                printf("Pontos turisticos: %i  VS  %i", carta.pontos_turisticos, oponente.pontos_turisticos);
                p1 = carta.pontos_turisticos > oponente.pontos_turisticos;
                p2 = carta.pontos_turisticos < oponente.pontos_turisticos; break;
            
            case 5: // Densidade populacional
                printf("Densidade populacional: %.2f  VS  %.2f", carta.densidade_populacional, oponente.densidade_populacional);
                p1 = carta.densidade_populacional < oponente.densidade_populacional;
                p2 = carta.densidade_populacional > oponente.densidade_populacional; break;

            case 6: // PIB per capita
                printf("PIB per capita: %.2f  VS  %.2f", carta.pib_per_capita, oponente.pib_per_capita);
                p1 = carta.pib_per_capita > oponente.pib_per_capita; 
                p2 = carta.pib_per_capita < oponente.pib_per_capita; break;

            default:
                imprimir_mensagem("\nAtributo inválido!", NEGRITO);
                return ERRO;
    }

    if (p1) {
        imprimir_mensagem("\nVocê ganhou a partida!", VERDE); return VITORIA;
    } else if (p2) {
        imprimir_mensagem("\nO oponente ganhou a partida!", VERMELHO); return DERROTA;
    } else {
        imprimir_mensagem("\nHouve empate!", NEGRITO); return EMPATE;
    }
}

void transferir_carta(Lista_de_cartas *devedor, Lista_de_cartas *receptor) {
    Carta temp = devedor->cartas[0];

    // Devedor perde a primeira carta.
    for (size_t i = 0; i < (devedor->tamanho - 1); i++) {
        devedor->cartas[i] = devedor->cartas[i + 1];
    }
    alocar_espaco(devedor, -1);
    devedor->tamanho--;

    // Receptor ganha
    alocar_espaco(receptor, 1);
    receptor->cartas[receptor->tamanho] = temp;
    receptor->tamanho++;
}



/* ========================== Menus ========================== */


void iniciar_partida() {

    // Distribuição das cartas
    Lista_de_cartas player1 = inicializar();
    Lista_de_cartas player2 = inicializar();

    distribuir_cartas(2, &player1, &player2);

    // Rodada
    int atributo, rodada = 1;
    while (player1.tamanho && player2.tamanho) {
        Carta atual = player1.cartas[(rodada - 1) % player1.tamanho];
        Carta atual_inimigo = player2.cartas[(rodada - 1) % player2.tamanho];

        limpar_tela();

        printf("%s Rodada %i (Placar P1 %i x P2 %i) %s\n", NEGRITO, rodada, player1.tamanho, player2.tamanho, RESET);

        printf("\n");
        exibir_carta(atual, COMPLETO);
        printf("\n");

        printf(" 1 - População \n");
        printf(" 2 - Área \n");
        printf(" 3 - PIB \n");
        printf(" 4 - Pontos turisticos \n");
        printf(" 5 - Densidade populacional \n");
        printf(" 6 - PIB per capita \n");

        printf("\nEscolha uma atributo para competir -> ");
        scanf("%i", &atributo);
        
        // Competição
        Resultado_competicao resultado = competir_atributo(atributo, atual, atual_inimigo);
        if (resultado == ERRO) { continue; }
        if (resultado == EMPATE) { rodada++; continue; }

        (resultado == VITORIA) ? transferir_carta(&player2, &player1) : transferir_carta(&player1, &player2);
        rodada++;
    }

    finalizar(&player1); 
    finalizar(&player2);
}

void gerenciar_cartas() {

    int escolha, rodando = 1;
    while(rodando) {
        Lista_de_cartas lista = listar_cartas();
        limpar_tela();

        printf("%sSistema de Gerenciamento de Cartas%s\n", NEGRITO, RESET);

        printf("\nTotal de cartas: %zu\n\n\n", lista.tamanho);

        if (!lista.tamanho) {
            printf("Nenhuma carta cadastrada!\n\n\n");
        } else {
            for (size_t i = 0; i < lista.tamanho; i++) {
                exibir_carta(lista.cartas[i], SIMPLIFICADO);
                printf("\n\n");
            }
        }
        finalizar(&lista);

        printf(" 1 - Cadastrar carta \n");
        printf(" 2 - Remover carta \n");
        printf(" 3 - Voltar ao Menu principal \n");

        printf("\nEscolha sua opção -> ");
        scanf("%i", &escolha);

        switch (escolha) {
            case 1: cadastrar_carta(); break;
            case 2: remover_carta(&lista); break;
            case 3: rodando = 0; break;
        }
    }
}

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    srand(time(NULL));

    int escolha, rodando = 1;
    while(rodando) {
        limpar_tela();

        printf("%s #### #   # ##### #### #####    ##### ##### #   # #     # #### ##### %s\n", NEGRITO, RESET);
        printf("%s #    #   # #   # #    #   #      #   #   # #   # # #   # #    #   # %s\n", NEGRITO, RESET);
        printf("%s #### #   # ##### #### #####      #   ##### #   # #  #  # #### #   # %s\n", NEGRITO, RESET);
        printf("%s    # #   # #     #    #  #       #   #  #  #   # #   # # #    #   # %s\n", NEGRITO, RESET);
        printf("%s #### ##### #     #### #   #      #   #   # ##### #    ## #    ##### %s\n", NEGRITO, RESET);

        printf("\n%s%s=== MENU PRINCIPAL ===%s\n\n", AZUL, ITALICO, RESET);

        printf(" 1 - Jogar (PvP) \n"); 
        printf(" 2 - Gerenciar cartas \n");
        printf(" 3 - Sair \n");

        printf("\nEscolha sua opção -> ");
        scanf("%i", &escolha);

        switch (escolha) {
            case 1: iniciar_partida(); break;
            case 2: gerenciar_cartas(); break;
            case 3: rodando = 0; break;
        }
    }
    imprimir_mensagem("Fechando o programa...\n", VERMELHO);
    return 0;
} 
