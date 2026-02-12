#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>

// Desafio Super Trunfo - Países
// Tema 2 - Adicionado mais propriedades
// Objetivo: No nível aventureiro, as funcionalidade do nível novato se mantém, mas agora o sistema calcula novas propriedades pras cartas, como densidade populacional e PIB per capita.

// Área para definição das variáveis para armazenar as propriedades das cartas.
typedef struct {
    char codigo[10];
    char estado;
    int cidade;
    int populacao;
    float area;
    float pib;
    int pontos_turisticos;
    float densidade_populacional;
    float pib_per_capita;
} Carta;

// Função para guardar o registro.
void salvar_dados(Carta dados) {
    FILE *registro = fopen("registro.bin", "ab"); 
    
    fwrite(&dados, sizeof(Carta), 1, registro);
    fclose(registro);
}

int main() {
    Carta dados;

    setlocale(LC_ALL, "pt_BR.UTF-8");
    system("cls");

    printf("Bem-vindo ao sistema de cadastro de cartas!\n");
    printf("Para o cadastro de sua carta, preencha os dados solicitados a seguir...\n");

    // Área para entrada de dados.
    printf("\n Letra do Estado (A á H): ");
    scanf("%c", &dados.estado);

    printf("\n Numero da cidade (01 á 04): ");
    scanf("%i", &dados.cidade);

    printf("\n População: ");
    scanf("%i", &dados.populacao);

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
    printf("\n \033[2;32m Carta registrada com sucesso! \033[0m \n");
    sleep(2);


    // Área para exibição dos dados da cidade.
    system("cls");

    printf("Sua cidade é %s \n\n", dados.codigo);

    printf("Possui uma população de %i habitantes; \n", dados.populacao);

    printf("Área territorial de %.2f km²; \n", dados.area);

    printf("PIB de %.2f bilhões; \n", dados.pib);

    printf("N° de pontos turisticos de %i; \n\n", dados.pontos_turisticos);

    printf("Densidade populacional de %.2f habitantes por km²; \n", dados.densidade_populacional);

    printf("PIB per capita de %.2f reais. \n\n", dados.pib_per_capita);

    return 0;
} 
