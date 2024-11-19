#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILENAME "USUARIO.txt"
#define MAX_USUARIOS 10
#define CRIPTO_FILE "cripto_saldo.txt"

float bitcoin_preco = 150000.0;
float ripple_preco = 5.0;
float ethereum_preco = 10000.0;

typedef struct {
    char tipo[10]; // Tipo da transação: compra/venda
    char cripto[20]; // Nome da criptomoeda
    float quantidade; // Quantidade de criptomoeda comprada/vendida
    float valor; // Valor da transação
    char dataHora[20]; // Data e hora da transação
} Transacao;

typedef struct {
    float bitcoin;
    float ripple;
    float ethereum;
} CriptoSaldo;

typedef struct {
    char cpf[12];
    char senha[20];
    float saldoReais;
    CriptoSaldo criptoSaldo;
} Usuario;

// Função para gerar variação nas cotações das criptomoedas
float gerarvariacao() {
    return ((rand() % 101) - 50) / 1000.0;
}

// Atualizar cotação das criptomoedas
void atualizarcotacao() {
    float variacaobitcoin = gerarvariacao();
    float variacaoripple = gerarvariacao();
    float variacaoethereum = gerarvariacao();

    bitcoin_preco *= (1 + variacaobitcoin);
    ripple_preco *= (1 + variacaoripple);
    ethereum_preco *= (1 + variacaoethereum);

    printf("Cotação atualizada:\n");
    printf("BTC: %.2f (variação: %.2f%%)\n", bitcoin_preco, variacaobitcoin * 100);
    printf("XRP: %.2f (variação: %.2f%%)\n", ripple_preco, variacaoripple * 100);
    printf("ETH: %.2f (variação: %.2f%%)\n", ethereum_preco, variacaoethereum * 100);
    printf("--------------------------------------\n");
}

// Função para registrar transações no extrato
void registrarTransacao(const char *tipo, const char *cripto, float quantidade, float valor) {
    FILE *file = fopen("transacoes.txt", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de transações.\n");
        return;
    }

    time_t agora = time(NULL);
    struct tm *dataAtual = localtime(&agora);
    char data[11]; 

    strftime(data, sizeof(data), "%d/%m/%Y", dataAtual);

    fprintf(file, "Tipo: %s / Moeda: %s / Quantidade: %.2f / Valor: %.2f / Data: %s\n",
            tipo, cripto, quantidade, valor, data);

    fclose(file);
}

// Função para ler e contar os usuários cadastrados
int contarUsuarios() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        return 0;
    }

    int contador = 0;
    char linha[50];
    while (fgets(linha, sizeof(linha), file)) {
        contador++;
    }
    fclose(file);
    return contador;
}

// Função para cadastrar um novo usuário
void cadastrarUsuario() {
    int totalUsuarios = contarUsuarios();

    if (totalUsuarios >= MAX_USUARIOS) {
        printf("Limite máximo de usuários atingido.\n");
        return;
    }

    char cpf[12];
    char senha[20];
    float saldoReais = 0.0;

    printf("Digite o CPF para cadastro (somente números): ");
    if (scanf("%11s", cpf) != 1) {
        printf("Erro ao ler CPF.\n");
        return;
    }

    printf("Digite a senha para cadastro: ");
    if (scanf("%19s", senha) != 1) {
        printf("Erro ao ler senha.\n");
        return;
    }

    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de cadastro.\n");
        return;
    }

    fprintf(file, "CPF:%s Senha:%s Saldo:%.2f\n", cpf, senha, saldoReais);

    fclose(file);
    printf("Cadastro realizado com sucesso!\n");
}

// Função para efetuar login do usuário
int efetuarLogin(char *cpf, char *senha, float *saldo, CriptoSaldo *criptoSaldo) {
    char cpfCadastrado[12];
    char senhaCadastrada[20];
    char linha[50];

    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de login.\n");
        return 0;
    }

    while (fgets(linha, sizeof(linha), file)) {
        if (sscanf(linha, "CPF:%11s Senha:%19s Saldo:%f", cpfCadastrado, senhaCadastrada, saldo) == 3) {
            cpfCadastrado[strcspn(cpfCadastrado, "\n")] = '\0';
            senhaCadastrada[strcspn(senhaCadastrada, "\n")] = '\0';

            if (strcmp(cpf, cpfCadastrado) == 0 && strcmp(senha, senhaCadastrada) == 0) {
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);
    return 0;
}

// Função para atualizar saldo no arquivo
void atualizarSaldoNoArquivo(const char *cpf, const char *senha, float novoSaldo) {
    FILE *file = fopen(FILENAME, "r+");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de usuários.\n");
        return;
    }

    char linha[50];
    long pos;
    while (fgets(linha, sizeof(linha), file)) {
        char cpfCadastrado[12], senhaCadastrada[20];
        float saldo;

        // Separa as informações do arquivo
        if (sscanf(linha, "CPF:%11s Senha:%19s Saldo:%f", cpfCadastrado, senhaCadastrada, &saldo) == 3) {
            if (strcmp(cpf, cpfCadastrado) == 0 && strcmp(senha, senhaCadastrada) == 0) {
                pos = ftell(file);
                fseek(file, pos - strlen(linha), SEEK_SET);

                // Atualiza o saldo no arquivo
                fprintf(file, "CPF:%s Senha:%s Saldo:%.2f\n", cpf, senha, novoSaldo);
                break;
            }
        }
    }

    fclose(file);
}

// Função para exibir o menu principal
void menuPrincipal() {
    printf("Opções Disponíveis:\n");
    printf("1. Consultar saldo\n");
    printf("2. Consultar extrato\n");
    printf("3. Depositar\n");
    printf("4. Sacar\n");
    printf("5. Comprar Criptomoedas\n");
    printf("6. Vender Criptomoedas\n");
    printf("7. Atualizar cotação\n");
    printf("8. Sair\n");
}

// Função que executa as opções escolhidas pelo usuário
void escolhaOpcao(int opcao, float *saldoReais, CriptoSaldo *criptoSaldo, const char *cpf,
                  const char *senha) {
    switch (opcao) {
    case 1:
        printf("Saldo atual: R$ %.2f\n", *saldoReais);
        break;
    case 2:
        printf("Extrato de transações:\n");
        FILE *file = fopen("transacoes.txt", "r");
        if (file == NULL) {
            printf("Nenhuma transação registrada ainda.\n");
        } else {
            char linha[256];
            while (fgets(linha, sizeof(linha), file)) {
                printf("%s", linha);
            }
            fclose(file);
        }
        break;
    case 3: {
        float deposito;
        printf("Digite o valor a depositar: R$ ");
        if (scanf("%f", &deposito) == 1 && deposito > 0) {
            *saldoReais += deposito;
            registrarTransacao("deposito", "Reais", 0, deposito);
            atualizarSaldoNoArquivo(cpf, senha, *saldoReais);
        }
        break;
    }
    case 4: {
        float saque;
        printf("Digite o valor a sacar: R$ ");
        if (scanf("%f", &saque) == 1 && saque <= *saldoReais && saque > 0) {
            *saldoReais -= saque;
            registrarTransacao("saque", "Reais", 0, saque);
            atualizarSaldoNoArquivo(cpf, senha, *saldoReais);
        }
        break;
    }
    case 5:
        // Lógica para comprar criptomoedas
        break;
    case 6:
        // Lógica para vender criptomoedas
        break;
    case 7:
        atualizarcotacao();
        break;
    case 8:
        printf("Saindo...\n");
        break;
    default:
        printf("Opção inválida!\n");
        break;
    }
}

int main() {
    srand(time(NULL));

    int escolha;
    char cpf[12], senha[20];
    float saldoReais = 0.0;
    CriptoSaldo criptoSaldo = {0.0, 0.0, 0.0};

    printf("Bem-vindo ao sistema de gerenciamento de criptomoedas!\n");
    printf("Digite seu CPF: ");
    scanf("%11s", cpf);

    printf("Digite sua senha: ");
    scanf("%19s", senha);

    if (!efetuarLogin(cpf, senha, &saldoReais, &criptoSaldo)) {
        printf("Login falhou. Verifique seu CPF e senha.\n");
        return 0;
    }

    do {
        menuPrincipal();
        printf("Escolha uma opção: ");
        scanf("%d", &escolha);
        escolhaOpcao(escolha, &saldoReais, &criptoSaldo, cpf, senha);
    } while (escolha != 8);

    return 0;
}
