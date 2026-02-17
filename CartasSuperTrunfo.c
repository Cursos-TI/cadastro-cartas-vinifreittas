// ==================================================================
// Super Trunfo - Países
// Tema 2.5 (extra) - Extensão
// Objetivo: Melhorar o código, adicionando funcionalidades, graficos e estrutura.
// ==================================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>

// Constantes
#define ARQUIVO_REGISTRO "registro.bin"

// Estilos de texto (ANSI)
#define NEGRITO "\033[1m"
#define NEGRITO_ITALICO_AZUL "\033[1;3;34m"
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
} Lista_de_cartas;



/* ========================== Utilitários ========================== */


// Retorna uma lista com todos os registros.
Lista_de_cartas listar_cartas() {
    Lista_de_cartas lista = {NULL, 0};;
    FILE *registro = fopen(ARQUIVO_REGISTRO, "rb");

    Carta temp;
    while (fread(&temp, sizeof(Carta), 1, registro)) {
        lista.cartas = realloc(lista.cartas, sizeof(Carta) * (lista.tamanho + 1));
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

// Exlui registro por código.
void excluir_dados(char codigo[10]) {
    Lista_de_cartas lista = listar_cartas();
    FILE *registro = fopen(ARQUIVO_REGISTRO, "wb");

    for (size_t i = 0; i < lista.tamanho; i++) {
        if (strcmp(lista.cartas[i].codigo, codigo) != 0) {
            fwrite(&lista.cartas[i], sizeof(Carta), 1, registro);
        }
    }
    free(lista.cartas);
    fclose(registro);
}

void limpar_tela() {
    system("cls || clear");
}



/* ========================== Funcionalidades ========================== */


// Cadastro.
void cadastrar_carta() {
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
    printf("\n %s Carta registrada com sucesso! %s \n", VERDE, RESET);
    sleep(2);
}

// Remoção.
void remover_carta() {
    char codigo[10];

    printf("\n\n");
    printf("Qual o código da carta? -> ");
    scanf("%s", codigo);

    excluir_dados(codigo);
    printf("\n %s Carta exluida com sucesso! %s \n", VERDE, RESET);
    sleep(2);
}

// Exibição
void exibir_carta(Carta dados, const char *modo) {

    if (strcmp(modo, "extendido") == 0) {
        printf("Cidade %s %s %s \n\n", NEGRITO, dados.codigo, RESET);

        printf("População: %u hab. \n", dados.populacao);
        printf("Área territorial: %.2f km² \n", dados.area);
        printf("PIB: %.2f bilhões \n", dados.pib);
        printf("N° de pontos turisticos: %i \n\n", dados.pontos_turisticos);

        printf("Densidade populacional: %.2f hab. por km² \n", dados.densidade_populacional);
        printf("PIB per capita: %.2f reais \n\n", dados.pib_per_capita);
        
    } else if (strcmp(modo, "simplificado") == 0) {
        char codigo[20];
        sprintf(codigo, "%s[%s]%s", AZUL, dados.codigo, RESET);

        printf("%-15s População: %u hab.\n", codigo, dados.populacao);
        printf("%-5s Área: %.2f km²\n", "", dados.area);
        printf("%-5s PIB: %.2f bi.\n", "", dados.pib);
        printf("%-5s Pontos turísticos: %i\n", "", dados.pontos_turisticos);
    }
}



/* ========================== Menus ========================== */


void iniciar_partida() {
    limpar_tela();
    printf("\n%s Em breve... %s\n", NEGRITO, RESET);
    sleep(2);
}

void gerenciar_cartas() {
    int escolha, rodando = 1;

    while(rodando) {
        Lista_de_cartas lista = listar_cartas();
        limpar_tela();

        printf("%sSistema de Gerenciamento de Cartas%s\n", NEGRITO, RESET);

        printf("\nTotal de cartas: %zu\n", lista.tamanho);

        if (lista.tamanho == 0) {
            printf("\n\nNenhuma carta cadastrada!\n");
        } else {
            for (size_t i = 0; i < lista.tamanho; i++) {
                printf("\n\n");
                exibir_carta(lista.cartas[i], "simplificado");
            }
        } 

        free(lista.cartas);

        printf("\n\n");
        printf(" 1 - Cadastrar carta \n"); 
        printf(" 2 - Remover carta \n");
        printf(" 3 - Voltar ao Menu principal \n");

        printf("\nEscolha sua opção -> ");
        scanf("%i", &escolha);

        switch (escolha) {
                case 1: cadastrar_carta(); break;
                case 2: remover_carta(); break;
                case 3: rodando = 0; break;
        }
    }
}

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    int escolha, rodando = 1;

    while(rodando) {
        limpar_tela();

        printf("%s #### #   # ##### #### #####    ##### ##### #   # #     # #### ##### %s\n", NEGRITO, RESET);
        printf("%s #    #   # #   # #    #   #      #   #   # #   # # #   # #    #   # %s\n", NEGRITO, RESET);
        printf("%s #### #   # ##### #### #####      #   ##### #   # #  #  # #### #   # %s\n", NEGRITO, RESET);
        printf("%s    # #   # #     #    #  #       #   #  #  #   # #   # # #    #   # %s\n", NEGRITO, RESET);
        printf("%s #### ##### #     #### #   #      #   #   # ##### #    ## #    ##### %s\n", NEGRITO, RESET);

        printf("\n%s=== MENU PRINCIPAL ===%s\n\n", NEGRITO_ITALICO_AZUL, RESET);

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
    printf("\n %s Fechando o programa... %s \n\n", VERMELHO, RESET);
    return 0;
} 
