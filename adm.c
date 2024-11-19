#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxcpf 12
#define maxsenha 7
#define admfile "adm.txt"
#define usuariofile "USUARIO.txt"
#define criptofile "cripto.txt"

void cadastraradm();  // Corrigido o nome para cadastraradm
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
    if (scanf("%d", &opcao) != 1) {
        printf("Erro na leitura da opção.\n");
        continue;  // Ignorar a opção inválida e continuar
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
        printf("\n Menu \n");
        printf("1. Cadastrar novo Investidor\n");
        printf("2. Cadastrar nova Criptomoeda\n");
        printf("3. Excluir Investidor\n");
        printf("4. Excluir Criptomoeda\n");
        printf("5. Consultar saldo de um investidor\n");
        printf("6. Consultar extrado de um investidor\n");
        printf("7. Sair\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Erro na leitura da opção.\n");
            continue;  // Ignorar a opção inválida e continuar
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
                printf("saldo do investidor");
                break;
            case 6:
                printf("extrato do investidor");
                break;
            case 7:
                sair();
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 7);
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

  //salvando o cpf, senha e saldo
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

    // Armazenando os dados no arquivo
    fprintf(file, "%s %.2f %.2f %.2f\n", nome, cotacao_inicial, taxa_compra, taxa_venda);
    fclose(file);

    printf("Criptomoeda cadastrada com sucesso!\n");
}


void excluirinvestidor() {
    char cpfremover[maxcpf];
    char linha[100]; // Para armazenar uma linha do arquivo
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
        sscanf(linha, "CPF:%11s", cpf); // Extrai o CPF da linha

        // Verifica se o CPF é o que deve ser removido
        if (strcmp(cpf, cpfremover) != 0) {
            fputs(linha, temp); // Copia a linha para o arquivo temporário
        } else {
            encontrado = 1;
        }
    }

    fclose(file);
    fclose(temp);

    // Substitui o arquivo original pelo temporário
    remove(usuariofile);
    rename("temp.txt", usuariofile);

    if (encontrado) {
        printf("Investidor removido com sucesso!\n");
    } else {
        printf("Investidor não encontrado.\n");
    }
}

void excluirmoeda() {
    char linha[100];             // Para armazenar a linha lida do arquivo
    char nomeremove[20];         // Nome da moeda a ser removida
    char nome[20];               // Nome da moeda atual no arquivo
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

    // Percorre o arquivo linha por linha
    while (fgets(linha, sizeof(linha), file)) {
        // Tenta extrair os dados da linha
        if (sscanf(linha, "%s %f %f %f", nome, &cotacao, &taxa_compra, &taxa_venda) == 4) {
            // Verifica se o nome é diferente do que será excluído
            if (strcmp(nome, nomeremove) != 0) {
                fputs(linha, temp); // Escreve a linha no arquivo temporário
            } else {
                encontrado = 1; // Indica que a moeda foi encontrada
            }
        }
    }

    fclose(file);
    fclose(temp);

    // Substitui o arquivo original pelo temporário
    remove(criptofile);
    rename("temp.txt", criptofile);

    if (encontrado) {
        printf("Criptomoeda '%s' removida com sucesso!\n", nomeremove);
    } else {
        printf("Criptomoeda '%s' não encontrada.\n", nomeremove);
    }
}


void sair() {
    printf("Encerrando programa.\n");
}