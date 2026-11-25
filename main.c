#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* 26 (A-Z) + 676 (AA-ZZ) = 702 cidades */
#define MAX 702
#define TAM_NOME 50

char nomes[MAX][TAM_NOME];
int matriz[MAX][MAX];
int n = 0;

/* Gera o rotulo do vertice: A..Z, depois AA, AB, AC ... ZZ */
void rotulo(int idx, char *out) {
    if (idx < 26) {
        out[0] = 'A' + idx;
        out[1] = '\0';
    } else {
        int k = idx - 26;
        out[0] = 'A' + k / 26;
        out[1] = 'A' + k % 26;
        out[2] = '\0';
    }
}

/* Compara dois nomes ignorando maiusculas/minusculas */
int nomesIguais(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++;
        b++;
    }
    return *a == *b;
}

/* Procura cidade pelo nome (sem diferenciar maiusculas); retorna indice ou -1 */
int buscarCidade(const char *nome) {
    for (int i = 0; i < n; i++)
        if (nomesIguais(nomes[i], nome)) return i;
    return -1;
}

void lerTexto(const char *msg, char *dest) {
    printf("%s", msg);
    scanf(" %49[^\n]", dest);
}

/* 1. Cadastrar cidade (vertice) */
void cadastrarCidade() {
    char r[3];
    if (n >= MAX) { printf("Limite de %d cidades atingido.\n", MAX); return; }
    char nome[TAM_NOME];
    lerTexto("Nome da cidade: ", nome);
    int existente = buscarCidade(nome);
    if (existente != -1) {
        printf("Cidade ja cadastrada como '%s'.\n", nomes[existente]);
        return;
    }
    strcpy(nomes[n], nome);
    rotulo(n, r);
    printf("Cidade '%s' cadastrada como vertice %s.\n", nome, r);
    n++;
}

/* 2. Inserir conexao (aresta) - nos dois sentidos */
void inserirConexao() {
    char a[TAM_NOME], b[TAM_NOME];
    lerTexto("Cidade 1: ", a);
    lerTexto("Cidade 2: ", b);
    int i = buscarCidade(a), j = buscarCidade(b);
    if (i == -1 || j == -1) { printf("Cidade nao encontrada.\n"); return; }
    if (i == j) { printf("Nao e possivel conectar uma cidade a ela mesma.\n"); return; }
    if (matriz[i][j]) {
        printf("Essas cidades ja estao conectadas: %s <-> %s.\n", nomes[i], nomes[j]);
        return;
    }
    matriz[i][j] = 1;
    matriz[j][i] = 1;
    printf("Conexao %s <-> %s inserida.\n", nomes[i], nomes[j]);
}

/* 3. Remover conexao - nos dois sentidos */
void removerConexao() {
    char a[TAM_NOME], b[TAM_NOME];
    lerTexto("Cidade 1: ", a);
    lerTexto("Cidade 2: ", b);
    int i = buscarCidade(a), j = buscarCidade(b);
    if (i == -1 || j == -1) { printf("Cidade nao encontrada.\n"); return; }
    if (!matriz[i][j]) { printf("Essas cidades nao estao conectadas.\n"); return; }
    matriz[i][j] = 0;
    matriz[j][i] = 0;
    printf("Conexao %s <-> %s removida.\n", nomes[i], nomes[j]);
}

/* 4. Exibir matriz de adjacencia */
void exibirMatriz() {
    char r[3];
    if (n == 0) { printf("Nenhuma cidade cadastrada.\n"); return; }

    /* largura da coluna: 1 ate Z, 2 a partir de AA */
    int w = (n > 26) ? 2 : 1;

    printf("\n%*s", w + 3, "");
    for (int j = 0; j < n; j++) {
        rotulo(j, r);
        printf("%-*s ", w, r);
    }
    printf("\n");

    for (int k = 0; k < (w + 3) + (w + 1) * n; k++) printf("-");
    printf("\n");

    for (int i = 0; i < n; i++) {
        rotulo(i, r);
        printf("%-*s | ", w, r);
        for (int j = 0; j < n; j++) printf("%-*d ", w, matriz[i][j]);
        printf("\n");
    }

    printf("\nLegenda:\n");
    for (int i = 0; i < n; i++) {
        rotulo(i, r);
        printf("%s = %s\n", r, nomes[i]);
    }
}

/* 5. Grau de cada vertice */
void calcularGraus() {
    char r[3];
    if (n == 0) { printf("Nenhuma cidade cadastrada.\n"); return; }
    for (int i = 0; i < n; i++) {
        int grau = 0;
        for (int j = 0; j < n; j++) grau += matriz[i][j];
        rotulo(i, r);
        printf("%s (%s): %d conexao(oes)\n", r, nomes[i], grau);
    }
}

/* 6. Cidades isoladas */
void cidadesIsoladas() {
    char r[3];
    int achou = 0;
    for (int i = 0; i < n; i++) {
        int grau = 0;
        for (int j = 0; j < n; j++) grau += matriz[i][j];
        if (grau == 0) {
            if (!achou) printf("Cidades isoladas:\n");
            rotulo(i, r);
            printf("%s (%s)\n", r, nomes[i]);
            achou = 1;
        }
    }
    if (!achou) printf("Nao ha cidades isoladas.\n");
}

int main() {
    int op;
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
        if (scanf("%d", &op) != 1) break;

        switch (op) {
            case 1: cadastrarCidade(); break;
            case 2: inserirConexao();  break;
            case 3: removerConexao();  break;
            case 4: exibirMatriz();    break;
            case 5: calcularGraus();   break;
            case 6: cidadesIsoladas(); break;
            case 0: printf("Encerrando...\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 0);

    return 0;
}