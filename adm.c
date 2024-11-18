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

void cadastroinvestidor() {
    char nome[maxcpf];
    char cpf[15];

    printf("Cadastro de novo Investidor: \n");
    printf("Digite o nome do Investidor: ");
    scanf("%s", nome);
    printf("Digite o CPF do Investidor: ");
    scanf("%s", cpf);

    FILE *file = fopen(usuariofile, "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de investidores.\n");
        return;
    }
    fprintf(file, "%s %s\n", nome, cpf);
    fclose(file);

    printf("Investidor cadastrado com sucesso!\n");
}

void cadastromoeda() {
    char nome[20];
    float valor;

    printf("Cadastro de Nova Criptomoeda: \n");
    printf("Digite o nome da Criptomoeda: ");
    scanf("%s", nome);
    printf("Digite o valor da Criptomoeda: ");
    scanf("%f", &valor);

    FILE *file = fopen(criptofile, "a");
    if (file == NULL) {
        printf("Erro ao abrir arquivo de Criptomoedas.\n");
        return;
    }
    fprintf(file, "%s %.2f\n", nome, valor);
    fclose(file);

    printf("Criptomoeda cadastrada com sucesso!\n");
}

void excluirinvestidor() {
    char nome[maxcpf];
    char cpf[15];
    char nomeremover[maxcpf];
    int encontrado = 0;

    printf("Excluir Investidor: \n");
    printf("Digite o nome do investidor que deseja excluir: ");
    scanf("%s", nomeremover);

    FILE *file = fopen(usuariofile, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL || temp == NULL) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }

    while (fscanf(file, "%s %s", nome, cpf) != EOF) {
        if (strcmp(nome, nomeremover) != 0) {
            fprintf(temp, "%s %s\n", nome, cpf);
        } else {
            encontrado = 1;
        }
    }
    fclose(file);
    fclose(temp);

    remove(usuariofile);
    rename("temp.txt", usuariofile);

    if (encontrado) {
        printf("Investidor removido com sucesso!\n");
    } else {
        printf("Investidor não encontrado.\n");
    }
}

void excluirmoeda() {
    char nome[20];
    char nomeremove[20];
    float valor;
    int encontrado = 0;

    printf("Excluir Criptomoeda: \n");
    printf("Digite o nome da Criptomoeda que deseja excluir: ");
    scanf("%s", nomeremove);

    FILE *file = fopen(criptofile, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL || temp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    while (fscanf(file, "%s %f", nome, &valor) != EOF) {
        if(strcmp(nome, nomeremove) != 0) {
            fprintf(temp, "%s %.2f\n", nome, valor);
        } else {
            encontrado = 1;
        }
    }
    fclose(file);
    fclose(temp);

    remove(criptofile);
    rename("temp.txt", criptofile);

    if(encontrado) {
        printf("Criptomoeda removida com sucesso!\n");
    } else {
        printf("Criptomoeda não encontrada.\n");
    }
}

void sair() {
    printf("Encerrando programa.\n");
}
// TERMINAR O ADM.C