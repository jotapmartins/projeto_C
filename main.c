#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILENAME "USUARIO.txt"

// Função para cadastrar o usuário (CPF e senha)
void cadastrarUsuario() {
    char cpf[12];
    char senha[20];

    printf("Digite o CPF para cadastro (somente números): ");
    scanf("%11s", cpf);
    printf("Digite a senha para cadastro: ");
    scanf("%19s", senha);

    // Abrindo o arquivo para adicionar (modo "a" para não sobrescrever)
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de cadastro.\n");
        return;
    }

    // Escrevendo o CPF e a senha no arquivo
    fprintf(file, "CPF:%s Senha:%s\n", cpf, senha);

    fclose(file);
    printf("Cadastro realizado com sucesso!\n");
}

// Função para verificar o login
int efetuarLogin(char *cpf, char *senha) {
    char cpfCadastrado[12];
    char senhaCadastrada[20];
    char linha[50];

    // Abrindo o arquivo para leitura
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de login.\n");
        return 0;
    }

    // Lendo o CPF e a senha do arquivo linha por linha
    while (fgets(linha, sizeof(linha), file)) {
        // Verifica se a linha contém um CPF e uma senha
        if (sscanf(linha, "CPF:%11s Senha:%19s", cpfCadastrado, senhaCadastrada) == 2) {
            // Comparando o CPF e a senha inseridos com os dados cadastrados
            if (strcmp(cpf, cpfCadastrado) == 0 && strcmp(senha, senhaCadastrada) == 0) {
                fclose(file);
                return 1; // Login correto
            }
        }
    }

    fclose(file);
    return 0; // Login incorreto
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

int main() {
    int opcao;
    char cpf[12];
    char senha[20];

    printf("Bem-vindo à FEI Crypto Exchange!\n");
    printf("1. Cadastrar novo usuário\n");
    printf("2. Efetuar login\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    if (opcao == 1) {
        cadastrarUsuario();  // so pra saber
    } else if (opcao == 2) {
        printf("Insira o CPF: ");
        scanf("%11s", cpf);

        printf("Insira a senha: ");
        scanf("%19s", senha);

        if (efetuarLogin(cpf, senha)) {
            printf("Login realizado com sucesso! Bem-vindo à FEI Crypto Exchange!\n");
            menuPrincipal();
        } else {
            printf("CPF ou senha incorretos. Tente novamente.\n");
        }
    } else {
        printf("Opção inválida. Encerrando o programa.\n");
    }

    printf("escolha uma opção: ");
    
    // fim
    return 0;
}