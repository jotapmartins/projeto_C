#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxcpf 12
#define maxsenha 7
#define admfile "adm.txt"
#define usuariofile "USUARIO.txt"
#define criptofile "cripto.txt"

void cadastraradm();
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
  printf("--------------------------------------------------------------");

  while (1) {
    printf("\n Menu Inicial \n");
    printf("1. Cadastrar Administrador\n");
    printf("2. Login\n");
    printf("3. Sair\n");
    printf("Escolha uma opção: ");
    printf("\n--------------------------------------------------------------\n");
    if (scanf("%d", &opcao) != 1) {
        printf("Erro na leitura da opção.\n");
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
    if (strlen(cpf) != 11) {
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

    fscanf(file, "%s %s", savecpf, savesenha);
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
        printf("--------------------------------------------");
        printf("\n Menu \n");
        printf("1. Cadastrar novo Investidor\n");
        printf("2. Cadastrar nova Criptomoeda\n");
        printf("3. Excluir Investidor\n");
        printf("4. Excluir Criptomoeda\n");
        printf("5. Consultar saldo de um investidor\n");
        printf("6. Consultar extrado de um investidor\n");
        printf("7. Atualizar cotação de criptomoedas\n");
        printf("8. Sair\n");
        printf("Escolha uma opção: ");
        printf("\n");
        if (scanf("%d", &opcao) != 1) {
            printf("Erro na leitura da opção.\n");
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
                consultarsaldo();
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

void cadastroinvestidor() {
  char cpf[12];
  char senha[20];
  float saldoReais = 0.0;

  printf("Digite o CPF para cadastro (somente números): ");
  scanf("%11s", cpf);
  printf("Digite a senha para cadastro: ");
  scanf("%19s", senha);


  FILE *file = fopen(usuariofile, "a");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo de cadastro.\n");
    return;
  }

  fprintf(file, "CPF:%s Senha:%s Saldo:%.2f\n", cpf, senha, saldoReais);

  fclose(file);
  printf("Cadastro realizado com sucesso!\n");
}

void cadastromoeda() {
    char nome[20];
    float cotacao_inicial, taxa_compra, taxa_venda;

    printf("Cadastro de Nova Criptomoeda: \n");
    printf("Digite o nome da Criptomoeda: ");
    if (scanf("%s", nome) != 1) {
        printf("Erro na leitura do nome da criptomoeda.\n");
        return;
    }

    printf("Digite a cotação inicial da Criptomoeda: ");
    if (scanf("%f", &cotacao_inicial) != 1) {
        printf("Erro na leitura da cotação inicial.\n");
        return;
    }

    printf("Digite a taxa de compra da Criptomoeda: ");
    if (scanf("%f", &taxa_compra) != 1) {
        printf("Erro na leitura da taxa de compra.\n");
        return;
    }

    printf("Digite a taxa de venda da Criptomoeda: ");
    if (scanf("%f", &taxa_venda) != 1) {
        printf("Erro na leitura da taxa de venda.\n");
        return;
    }

    FILE *file = fopen(criptofile, "a");
    if (file == NULL) {
        printf("Erro ao abrir arquivo de Criptomoedas.\n");
        return;
    }

    fprintf(file, "%s %.2f %.2f %.2f\n", nome, cotacao_inicial, taxa_compra, taxa_venda);
    fclose(file);

    printf("Criptomoeda cadastrada com sucesso!\n");
}


void excluirinvestidor() {
    char cpfremover[maxcpf];
    char linha[100];
    int encontrado = 0;

    printf("Excluir Investidor: \n");
    printf("Digite o CPF do investidor que deseja excluir: ");
    if (scanf("%s", cpfremover) != 1) {
        printf("Erro na leitura do CPF.\n");
        return;
    }

    FILE *file = fopen(usuariofile, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        if (file) fclose(file);
        if (temp) fclose(temp);
        return;
    }

    while (fgets(linha, sizeof(linha), file)) {
        char cpf[maxcpf];
        sscanf(linha, "CPF:%11s", cpf);

        if (strcmp(cpf, cpfremover) != 0) {
            fputs(linha, temp);
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
    char linha[100];
    char nomeremove[20];
    char nome[20];
    float cotacao, taxa_compra, taxa_venda;
    int encontrado = 0;

    printf("Excluir Criptomoeda: \n");
    printf("Digite o nome da Criptomoeda que deseja excluir: ");
    if (scanf("%s", nomeremove) != 1) {
        printf("Erro na leitura do nome da criptomoeda.\n");
        return;
    }

    FILE *file = fopen(criptofile, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        if (file) fclose(file);
        if (temp) fclose(temp);
        return;
    }

    while (fgets(linha, sizeof(linha), file)) {
        if (sscanf(linha, "%s %f %f %f", nome, &cotacao, &taxa_compra, &taxa_venda) == 4) {
            if (strcmp(nome, nomeremove) != 0) {
                fputs(linha, temp);
            } else {
                encontrado = 1;
            }
        }
    }

    fclose(file);
    fclose(temp);
    remove(criptofile);
    rename("temp.txt", criptofile);

    if (encontrado) {
        printf("Criptomoeda '%s' removida com sucesso!\n", nomeremove);
    } else {
        printf("Criptomoeda '%s' não encontrada.\n", nomeremove);
    }
}

void consultarsaldo() {
    char cpf[maxcpf];
    char linha[100];
    char cpfbuscado[maxcpf];
    float saldo;
    int encontrado = 0;

    printf("Consultar Saldo do Investidor:\n");
    printf("Digite o CPF do investidor: ");
    if (scanf("%s", cpf) != 1) {
        printf("Erro na leitura do CPF.\n");
        return;
    }

    FILE *file = fopen(usuariofile, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de usuários.\n");
        return;
    }

    while (fgets(linha, sizeof(linha), file)) {
        char senha[maxsenha];

        if (sscanf(linha, "CPF:%11s Senha:%6s Saldo:%f", cpfbuscado, senha, &saldo) == 3) {
            if (strcmp(cpf, cpfbuscado) == 0) {
                printf("CPF: %s\nSaldo disponível: R$%.2f\n", cpfbuscado, saldo);
                encontrado = 1;
                break;
            }
        }
    }

    fclose(file);

    if (!encontrado) {
        printf("Investidor com CPF '%s' não encontrado.\n", cpf);
    }
}

void consultarExtrato() {
    char cpf[maxcpf];
    char linha[200];
    int encontrado = 0;

    printf("Digite o CPF do investidor para consultar o extrato: ");
    if (scanf("%s", cpf) != 1) {
        printf("Erro na leitura do CPF.\n");
        return;
    }

    FILE *file = fopen("transacoes.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de transações.\n");
        return;
    }

    printf("\nExtrato de Transações para o CPF: %s\n", cpf);

    while (fgets(linha, sizeof(linha), file)) {
        char cpfAtual[maxcpf];
        char tipo[20], moeda[20], data[11];
        float quantidade, valor;

        if (sscanf(linha, "CPF:%11s / Tipo:%19s / Moeda:%19s / Quantidade:%f / Valor:%f / Data:%10s",
                   cpfAtual, tipo, moeda, &quantidade, &valor, data) == 6) {
            if (strcmp(cpf, cpfAtual) == 0) {
                printf("Tipo: %s | Moeda: %s | Quantidade: %.2f | Valor: R$%.2f | Data: %s\n",
                       tipo, moeda, quantidade, valor, data);
                encontrado = 1;
            }
        }
    }

    fclose(file);

    if (!encontrado) {
        printf("Nenhuma transação encontrada para o CPF '%s'.\n", cpf);
    }
}

void atualizarcotacao() {
    char linha[100];
    char nomecripto[20];
    char nome[20];
    float nova_cotacao;
    float cotacao, taxa_compra, taxa_venda;
    int encontrado = 0;

    printf("Atualizar Cotação de Criptomoeda: \n");
    printf("Digite o nome da Criptomoeda: ");
    if (scanf("%s", nomecripto) != 1) {
        printf("Erro na leitura do nome da criptomoeda.\n");
        return;
    }

    printf("Digite a nova cotação: ");
    if (scanf("%f", &nova_cotacao) != 1) {
        printf("Erro na leitura da nova cotação.\n");
        return;
    }

    FILE *file = fopen(criptofile, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        if (file) fclose(file);
        if (temp) fclose(temp);
        return;
    }

    while (fgets(linha, sizeof(linha), file)) {
        if (sscanf(linha, "%s %f %f %f", nome, &cotacao, &taxa_compra, &taxa_venda) == 4) {
            if (strcmp(nome, nomecripto) == 0) {
                fprintf(temp, "%s %.2f %.2f %.2f\n", nome, nova_cotacao, taxa_compra, taxa_venda);
                encontrado = 1;
            } else {
                fputs(linha, temp);
            }
        }
    }

    fclose(file);
    fclose(temp);
    remove(criptofile);
    rename("temp.txt", criptofile);

    if (encontrado) {
        printf("Cotação da Criptomoeda '%s' atualizada com sucesso!\n", nomecripto);
    } else {
        printf("Criptomoeda '%s' não encontrada.\n", nomecripto);
    }
}


void sair() {
    printf("Encerrando programa.\n");
}