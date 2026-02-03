#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>

// Desafio Super Trunfo - Países
// Tema 1 - Cadastro das cartas
// Objetivo: No nível novato você deve criar as cartas representando as cidades utilizando scanf para entrada de dados e printf para exibir as informações.

// Área para definição das variáveis para armazenar as propriedades das cartas.
typedef struct {
    char codigo[10];
    char estado;
    int cidade;
    int populacao;
    float area;
    float pib;
    int pontos_turisticos;
} carta;

// Função para guardar o registro.
void salvar_dados(carta dados) {
    FILE *fp = fopen("dados.bin", "ab"); 
    
    fwrite(&dados, sizeof(carta), 1, fp);
    fclose(fp);
}

int main() {
    carta dados;

    setlocale(LC_ALL, "Portuguese_Brazil");
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

    sprintf(dados.codigo, "%c%02i", dados.estado, dados.cidade);

    salvar_dados(dados);
    printf("\n \033[2;32m Carta registrada com sucesso! \033[0m \n");
    sleep(2);

    // Área para exibição dos dados da cidade.
    system("cls");

    printf("Sua cidade é %s \n\n", dados.codigo);

    printf("Possui uma população de %i; \n", dados.populacao);

    printf("com área territorial de %.2f km²; \n", dados.area);

    printf("PIB de %.2f bilhões; \n", dados.pib);

    printf("e N° de pontos turisticos de %i \n\n", dados.pontos_turisticos);

    return 0;
} 
