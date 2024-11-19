#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxcpf 12
#define maxsenha 7
#define admfile "adm.txt"
#define usuariofile "USUARIO.txt"
#define criptofile "cripto.txt"
#define extratofile "extrato.txt"

void cadastraradm();
int loginadm();
void menuadm();
void cadastroinvestidor();
void cadastromoeda();
void excluirinvestidor();
void excluirmoeda();
void consultarSaldo();
void consultarExtrato();
void atualizarcotacao();
void sair();
void salvarinvestidor();
void salvausuario();
void atualizarinvestidor();

// Funções Implementadas
void atualizarcotacao() {
    printf("Atualizando cotação das criptomoedas...\n");
    // Adicione aqui a lógica de atualização
}

void consultarExtrato() {
    printf("Consultando extrato do investidor...\n");
    // Adicione aqui a lógica de consulta
}

void consultarSaldo() {
    printf("Consultando saldo do investidor...\n");
    // Adicione aqui a lógica de consulta
}

void excluirmoeda() {
    printf("Excluindo moeda...\n");
    // Adicione aqui a lógica de exclusão
}

void excluirinvestidor() {
    printf("Excluindo investidor...\n");
    // Adicione aqui a lógica de exclusão
}

void cadastromoeda() {
    printf("Cadastrando nova moeda...\n");
    // Adicione aqui a lógica de cadastro
}

void cadastroinvestidor() {
    printf("Cadastrando novo investidor...\n");
    // Adicione aqui a lógica de cadastro
}

int main() {
    int opcao;

    printf("Bem-vindo à área do administrador da FEI Crypto Exchange!\n");

    while (1) {
        printf("\n Menu Inicial \n");
        printf("1. Cadastrar Administrador\n");
        printf("2. Login\n");
        printf("3. Sair\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Erro na entrada. Tente novamente.\n");
            while(getchar() != '\n');  // Limpa o buffer de entrada
            continue;
        }

        switch (opcao) {
            case 1:
                cadastraradm();
                break;
            case 2:
                if (loginadm()) {
                    menuadm();
                } else {
                    printf("Falha no login. Tente novamente.\n");
                }
                break;
            case 3:
                printf("Saindo do sistema.\n");
                exit(0);
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    }
    return 0;
}

void cadastraradm() {
    char cpf[maxcpf];
    char senha[maxsenha];

    printf("Cadastro de Administrador: \n");
    printf("Digite o CPF: \n");
    if (scanf("%s", cpf) != 1) {
        printf("Erro na leitura do CPF.\n");
        return;
    }

    if (strlen(cpf) != 11){
        printf("CPF inválido. \n");
        return;
    }

    printf("Digite uma senha de 6 dígitos: ");
    if (scanf("%s", senha) != 1) {
        printf("Erro na leitura da senha.\n");
        return;
    }

    if (strlen(senha) != 6) {
        printf("Senha inválida. Deve conter 6 dígitos.\n");
        return;
    }

    FILE *file = fopen(admfile, "w");
    if (file == NULL) {
        printf("Erro ao criar o arquivo de administradores.\n");
        return;
    }
    fprintf(file, "%s %s\n", cpf, senha);
    fclose(file);

    printf("Administrador cadastrado com sucesso!\n");
}

int loginadm() {
    char cpf[maxcpf];
    char senha[maxsenha];
    char savecpf[maxcpf];
    char savesenha[maxsenha];

    FILE *file = fopen(admfile, "r");
    if (file == NULL) {
        printf("Nenhum administrador cadastrado. \n");
        return 0;
    }

    // Verificando o retorno de fscanf para garantir que os dados foram lidos corretamente
    if (fscanf(file, "%s %s", savecpf, savesenha) != 2) {
        printf("Erro ao ler os dados do administrador.\n");
        fclose(file);
        return 0;
    }

    fclose(file);

    printf("Login de Administrador: \n");
    printf("Digite seu CPF: ");
    if (scanf("%s", cpf) != 1) {
        printf("Erro na leitura do CPF.\n");
        return 0;
    }
    printf("Digite sua senha: ");
    if (scanf("%s", senha) != 1) {
        printf("Erro na leitura da senha.\n");
        return 0;
    }

    if (strcmp(cpf, savecpf) == 0 && strcmp(senha, savesenha) == 0) {
        printf("Login bem-sucedido!\n");
        return 1;
    } else {
        printf("CPF ou senha incorretos.\n");
        return 0;
    }
}

void menuadm() {
    int opcao;

    do {
        printf("\n Menu \n");
        printf("1. Cadastrar novo Investidor\n");
        printf("2. Cadastrar nova Criptomoeda\n");
        printf("3. Excluir Investidor\n");
        printf("4. Excluir Criptomoeda\n");
        printf("5. Consultar Saldo de Investidor\n");
        printf("6. Consultar Extrato de Investidor\n");
        printf("7. Atualizar Cotação das Criptomoedas\n");
        printf("8. Sair\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Erro na leitura da opção.\n");
            while(getchar() != '\n');
            continue;
        }

        switch (opcao) {
            case 1:
                cadastroinvestidor();
                break;
            case 2:
                cadastromoeda();
                break;
            case 3:
                excluirinvestidor();
                break;
            case 4:
                excluirmoeda();
                break;
            case 5:
                consultarSaldo();
                break;
            case 6:
                consultarExtrato();
                break;
            case 7:
                atualizarcotacao();
                break;
            case 8:
                sair();
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 8);
}

void sair() {
    printf("Encerrando programa.\n");
}

// Funções de manipulação de dados
void salvarinvestidor() {
    printf("Salvando investidor...\n");
    // Lógica para salvar o investidor
}

void salvausuario() {
    printf("Salvando usuário...\n");
    // Lógica para salvar o usuário
}

void atualizarinvestidor() {
    printf("Atualizando investidor...\n");
    // Lógica para atualizar dados do investidor
}
