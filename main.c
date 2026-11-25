#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* 26 (A-Z) + 676 (AA-ZZ) = 702 cidades */
#define MAXIMO_CIDADES 702
#define TAMANHO_NOME_CIDADE 50

char nomesCidades[MAXIMO_CIDADES][TAMANHO_NOME_CIDADE];
int matrizAdjacencia[MAXIMO_CIDADES][MAXIMO_CIDADES];
int quantidadeCidades = 0;

/* Gera o rotulo do vertice: A..Z, depois AA, AB, AC ... ZZ */
void rotulo(int indice, char *rotuloGerado) {
    if (indice < 26) {
        rotuloGerado[0] = 'A' + indice;
        rotuloGerado[1] = '\0';
    } else {
        int indiceAposZ = indice - 26;
        rotuloGerado[0] = 'A' + indiceAposZ / 26;
        rotuloGerado[1] = 'A' + indiceAposZ % 26;
        rotuloGerado[2] = '\0';
    }
}

/* Compara dois nomes ignorando maiusculas/minusculas */
int nomesIguais(const char *primeiroNome, const char *segundoNome) {
    while (*primeiroNome && *segundoNome) {
        if (tolower((unsigned char)*primeiroNome) != tolower((unsigned char)*segundoNome)) return 0;
        primeiroNome++;
        segundoNome++;
    }
    return *primeiroNome == *segundoNome;
}

/* Procura cidade pelo nome (sem diferenciar maiusculas); retorna indice ou -1 */
int buscarCidade(const char *nome) {
    for (int indiceCidade = 0; indiceCidade < quantidadeCidades; indiceCidade++)
        if (nomesIguais(nomesCidades[indiceCidade], nome)) return indiceCidade;
    return -1;
}

void lerTexto(const char *mensagem, char *textoLido) {
    printf("%s", mensagem);
    scanf(" %49[^\n]", textoLido);
}

/* 1. Cadastrar cidade (vertice) */
void cadastrarCidade() {
    char rotuloCidade[3];
    if (quantidadeCidades >= MAXIMO_CIDADES) { printf("Limite de %d cidades atingido.\n", MAXIMO_CIDADES); return; }
    char nome[TAMANHO_NOME_CIDADE];
    lerTexto("Nome da cidade: ", nome);
    int existente = buscarCidade(nome);
    if (existente != -1) {
        printf("Cidade ja cadastrada como '%s'.\n", nomesCidades[existente]);
        return;
    }
    strcpy(nomesCidades[quantidadeCidades], nome);
    rotulo(quantidadeCidades, rotuloCidade);
    printf("Cidade '%s' cadastrada como vertice %s.\n", nome, rotuloCidade);
    quantidadeCidades++;
}

/* 2. Inserir conexao (aresta) - nos dois sentidos */
void inserirConexao() {
    char primeiraCidade[TAMANHO_NOME_CIDADE], segundaCidade[TAMANHO_NOME_CIDADE];
    lerTexto("Cidade 1: ", primeiraCidade);
    lerTexto("Cidade 2: ", segundaCidade);
    int indicePrimeiraCidade = buscarCidade(primeiraCidade), indiceSegundaCidade = buscarCidade(segundaCidade);
    if (indicePrimeiraCidade == -1 || indiceSegundaCidade == -1) { printf("Cidade nao encontrada.\n"); return; }
    if (indicePrimeiraCidade == indiceSegundaCidade) { printf("Nao e possivel conectar uma cidade a ela mesma.\n"); return; }
    if (matrizAdjacencia[indicePrimeiraCidade][indiceSegundaCidade]) {
        printf("Essas cidades ja estao conectadas: %s <-> %s.\n", nomesCidades[indicePrimeiraCidade], nomesCidades[indiceSegundaCidade]);
        return;
    }
    matrizAdjacencia[indicePrimeiraCidade][indiceSegundaCidade] = 1;
    matrizAdjacencia[indiceSegundaCidade][indicePrimeiraCidade] = 1;
    printf("Conexao %s <-> %s inserida.\n", nomesCidades[indicePrimeiraCidade], nomesCidades[indiceSegundaCidade]);
}

/* 3. Remover conexao - nos dois sentidos */
void removerConexao() {
    char primeiraCidade[TAMANHO_NOME_CIDADE], segundaCidade[TAMANHO_NOME_CIDADE];
    lerTexto("Cidade 1: ", primeiraCidade);
    lerTexto("Cidade 2: ", segundaCidade);
    int indicePrimeiraCidade = buscarCidade(primeiraCidade), indiceSegundaCidade = buscarCidade(segundaCidade);
    if (indicePrimeiraCidade == -1 || indiceSegundaCidade == -1) { printf("Cidade nao encontrada.\n"); return; }
    if (!matrizAdjacencia[indicePrimeiraCidade][indiceSegundaCidade]) { printf("Essas cidades nao estao conectadas.\n"); return; }
    matrizAdjacencia[indicePrimeiraCidade][indiceSegundaCidade] = 0;
    matrizAdjacencia[indiceSegundaCidade][indicePrimeiraCidade] = 0;
    printf("Conexao %s <-> %s removida.\n", nomesCidades[indicePrimeiraCidade], nomesCidades[indiceSegundaCidade]);
}

/* 4. Exibir matriz de adjacencia */
void exibirMatriz() {
    char rotuloCidade[3];
    if (quantidadeCidades == 0) { printf("Nenhuma cidade cadastrada.\n"); return; }

    /* largura da coluna: 1 ate Z, 2 a partir de AA */
    int larguraColuna = (quantidadeCidades > 26) ? 2 : 1;

    printf("\n%*s", larguraColuna + 3, "");
    for (int indiceColuna = 0; indiceColuna < quantidadeCidades; indiceColuna++) {
        rotulo(indiceColuna, rotuloCidade);
        printf("%-*s ", larguraColuna, rotuloCidade);
    }
    printf("\n");

    for (int indiceSeparador = 0; indiceSeparador < (larguraColuna + 3) + (larguraColuna + 1) * quantidadeCidades; indiceSeparador++) printf("-");
    printf("\n");

    for (int indiceLinha = 0; indiceLinha < quantidadeCidades; indiceLinha++) {
        rotulo(indiceLinha, rotuloCidade);
        printf("%-*s | ", larguraColuna, rotuloCidade);
        for (int indiceColuna = 0; indiceColuna < quantidadeCidades; indiceColuna++) printf("%-*d ", larguraColuna, matrizAdjacencia[indiceLinha][indiceColuna]);
        printf("\n");
    }

    printf("\nLegenda:\n");
    for (int indiceCidade = 0; indiceCidade < quantidadeCidades; indiceCidade++) {
        rotulo(indiceCidade, rotuloCidade);
        printf("%s = %s\n", rotuloCidade, nomesCidades[indiceCidade]);
    }
}

/* 5. Grau de cada vertice */
void calcularGraus() {
    char rotuloCidade[3];
    if (quantidadeCidades == 0) { printf("Nenhuma cidade cadastrada.\n"); return; }
    for (int indiceCidade = 0; indiceCidade < quantidadeCidades; indiceCidade++) {
        int grau = 0;
        for (int indiceVizinho = 0; indiceVizinho < quantidadeCidades; indiceVizinho++) grau += matrizAdjacencia[indiceCidade][indiceVizinho];
        rotulo(indiceCidade, rotuloCidade);
        printf("%s (%s): %d conexao(oes)\n", rotuloCidade, nomesCidades[indiceCidade], grau);
    }
}

/* 6. Cidades isoladas */
void cidadesIsoladas() {
    char rotuloCidade[3];
    int encontrouCidadeIsolada = 0;
    for (int indiceCidade = 0; indiceCidade < quantidadeCidades; indiceCidade++) {
        int grau = 0;
        for (int indiceVizinho = 0; indiceVizinho < quantidadeCidades; indiceVizinho++) grau += matrizAdjacencia[indiceCidade][indiceVizinho];
        if (grau == 0) {
            if (!encontrouCidadeIsolada) printf("Cidades isoladas:\n");
            rotulo(indiceCidade, rotuloCidade);
            printf("%s (%s)\n", rotuloCidade, nomesCidades[indiceCidade]);
            encontrouCidadeIsolada = 1;
        }
    }
    if (!encontrouCidadeIsolada) printf("Nao ha cidades isoladas.\n");
}

int main() {
    int opcao;
    do {
        printf("\n=== CONEXOES ENTRE CIDADES ===\n");
        printf("1. Cadastrar cidade\n");
        printf("2. Inserir conexao\n");
        printf("3. Remover conexao\n");
        printf("4. Exibir matriz de adjacencia\n");
        printf("5. Calcular grau de cada cidade\n");
        printf("6. Identificar cidades isoladas\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1) break;

        switch (opcao) {
            case 1: cadastrarCidade(); break;
            case 2: inserirConexao();  break;
            case 3: removerConexao();  break;
            case 4: exibirMatriz();    break;
            case 5: calcularGraus();   break;
            case 6: cidadesIsoladas(); break;
            case 0: printf("Encerrando...\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}
