#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxcpf 12
#define maxsenha 7
#define admfile "adm.txt"
#define usuariofile "USUARIO.txt"
#define criptofile "cripto.txt"

void cadastraradmin();
int loginadm();

void menuadm();
void cadastroinvestidor();
void cadastromoeda();
void excluirinvestidor();
void excluirmoeda();
void sair();

int main() {
  int opcao;

  printf("Bem-vindo à área do administrador da FEI Crypto Exchange!\n");

  while (1) {
    printf("\n Menu Inicial \n");
    printf("1. Cadastrar Administrador\n");
    printf("2. Login\n");
    printf("3. Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
      case 1:
          cadastraradmin();
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
          printf("Opção inválida. Tente novamente,\n");
    }
  }
  return 0;
}

void cadastraradm() {
    char cpf[maxcpf];
    char senha[maxsenha];

    printf("Cadastro de Administrador: \n");
    printf("Digite o CPF: \n");
    scanf("%s", cpf);
    if (strlen(cpf) != 11){
    printf("CPF inválido. \n");
    return;
    }

    printf("Digite uma senha de 6 dígitos: ");
    scanf("%s", senha);
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

    fscanf(file, "%s %s", savecpf, savesenha);
    fclose(file);

    printf("Login de Administrador: \n");
    printf("Digite seu CPF: ");
    scanf("%s", cpf);
    printf("Digite sua senha: ");
    scanf("%s", senha);

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

    do{
        printf("\n Menu \n");
        printf("1. Cadastrar novo Investidor\n");
        printf("2. Cadastrar nova Criptomoeda\n");
        printf("3. Excluir Investidor\n");
        printf("4. Excluir Criptomoeda\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

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
                sair();
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 5);
}