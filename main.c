#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILENAME "USUARIO.txt"

//valor moedas
float bitcoin_preco = 150000.0;
float ripple_preco = 5.0;
float ethereum_preco = 10000.0;

typedef struct {
  char tipo[10];     
  char cripto[20];     
  float quantidade;    
  float valor;          
  char dataHora[20];   
} Transacao;

//extrato
void registrarTransacao(const char *tipo, const char *cripto, float quantidade, float valor) {
    FILE *file = fopen("transacoes.txt", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de transações.\n");
        return;
    }

  // data
  time_t agora = time(NULL);
  struct tm *dataAtual = localtime(&agora);
  char data[11]; 

  strftime(data, sizeof(data), "%d/%m/%Y", dataAtual);

  fprintf(file, "Tipo: %s / Moeda: %s / Quantidade: %.2f / Valor: %.2f / Data: %s\n",
          tipo, cripto, quantidade, valor, data);

  fclose(file);
}

typedef struct {
  float bitcoin;
  float ripple;
  float ethereum;
} CriptoSaldo;

// atualizar cotação
float gerarvariacao() {
  return ((rand() % 101) - 50) / 1000.0;
}

void atualizarcotacao() {
  float variacaobitcoin = gerarvariacao();
  float variacaoripple = gerarvariacao();
  float variacaoethereum = gerarvariacao();

  bitcoin_preco *= (1 + variacaobitcoin);
  ripple_preco *= (1 + variacaoripple);
  ethereum_preco *= (1 + variacaoethereum);

  printf("Cotação atualizada:\n");
  printf("\n");
  printf("BTC: %.2f (variação: %.2f%%)\n", bitcoin_preco, variacaobitcoin * 100);
  printf("XRP: %.2f (variação: %.2f%%)\n", ripple_preco, variacaoripple * 100);
  printf("ETH: %.2f (variação: %.2f%%)\n", ethereum_preco, variacaoethereum * 100);
  printf("--------------------------------------\n");
}

//taxas
float aplicartaxacompra(float valorcompra, int cripto) {
  float taxa = 0.0;
  switch (cripto) {
    case 1:
      taxa = valorcompra * 0.02;
      break;
    case 2:
      taxa = valorcompra * 0.01;
      break;
    case 3:
      taxa = valorcompra * 0.01;
      break;
  }
  return valorcompra - taxa;
}

float aplicartaxavenda(float valorvenda, int cripto) {
  float taxa = 0.0;
  switch (cripto) {
    case 1:
      taxa = valorvenda * 0.03;
      break;
    case 2:
      taxa = valorvenda * 0.02;
      break;
    case 3:
      taxa = valorvenda * 0.01;
      break;
  }
  return valorvenda - taxa;
}

//cadastro
void cadastrarUsuario() {
  char cpf[12];
  char senha[20];
  float saldoReais = 0.0;

  printf("Digite o CPF para cadastro (somente números): ");
  scanf("%11s", cpf);
  printf("Digite a senha para cadastro: ");
  scanf("%19s", senha);

  
  FILE *file = fopen(FILENAME, "a");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo de cadastro.\n");
    return;
  }

  //salvando o cpf, senha e saldo
  fprintf(file, "CPF:%s Senha:%s Saldo:%.2f\n", cpf, senha, saldoReais);

  fclose(file);
  printf("Cadastro realizado com sucesso!\n");
}

//verificar o login
int efetuarLogin(char *cpf, char *senha, float *saldo) {
  char cpfCadastrado[12];
  char senhaCadastrada[20];
  char linha[50];

  //lendo
  FILE *file = fopen(FILENAME, "r");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo de login.\n");
    return 0;
  }

  // Lendo um por um
  while (fgets(linha, sizeof(linha), file)) {
    if (sscanf(linha, "CPF:%11s Senha:%19s Saldo:%f", cpfCadastrado,
               senhaCadastrada, saldo) == 3) {
      // limpando para ler cpf e senha  
      cpfCadastrado[strcspn(cpfCadastrado, "\n")] =
          '\0';
      senhaCadastrada[strcspn(senhaCadastrada, "\n")] =
          '\0';

      //comparação
      if (strcmp(cpf, cpfCadastrado) == 0 &&
          strcmp(senha, senhaCadastrada) == 0) {
        fclose(file);
        return 1;
      }
    }
  }
  fclose(file);
  return 0;
}

void menuPrincipal(float saldoReais) {
  printf("Opções Disponíveis:\n");
  printf("\n");
  printf("1. Consultar saldo\n");
  printf("2. Consultar extrato\n");
  printf("3. Depositar\n");
  printf("4. Sacar\n");
  printf("5. Comprar Criptomoedas\n");
  printf("6. Vender Criptomoedas\n");
  printf("7. Atualizar cotação\n");
  printf("8. Sair\n");
}

// escolha do usuario
void escolhaOpcao(int opcao, float *saldoReais, CriptoSaldo *criptoSaldo, const char *cpf,
                  const char *senha) {
  switch (opcao) {
  case 1:
    printf("Saldo atual: R$ %.2f\n", *saldoReais);
    printf("----------------------\n");
    break;
    case 2: {
      printf("\n");
        printf("Extrato de transações:\n");
      printf("\n");
        FILE *file = fopen("transacoes.txt", "r");
        if (file == NULL) {
            printf("Nenhuma transação registrada ainda.\n");
            break;
        }

        char linha[256];
        while (fgets(linha, sizeof(linha), file)) {
            printf("%s", linha); 
        }
      printf("--------------------------------------------------------------------------------------\n");

        fclose(file);
        break;
    }
  case 3: {
    float deposito;
    printf("Digite o valor a depositar: R$ ");
    scanf("%f", &deposito);
    if (deposito > 0) {
      *saldoReais += deposito;

      printf("\n");
      printf("Depósito realizado com sucesso!\n");
        printf("--------------------------------------\n");

      // add no extrato
      registrarTransacao("deposito", "Reais", 0, deposito);

      FILE *file = fopen(FILENAME, "r+");
      if (file != NULL) {
        char linha[50];
        long pos;
        while (fgets(linha, sizeof(linha), file)) {
          // Encontra a linha correspondente
          if (strstr(linha, cpf) != NULL) {
            pos = ftell(file);
            fseek(file, pos - strlen(linha), SEEK_SET);
            fprintf(file, "CPF:%s Senha:%s Saldo:%.2f\n", cpf, senha, *saldoReais);
            break;
          }
        }
        fclose(file);
      }
    } else {
      printf("Valor de depósito inválido. Tente novamente.\n");
    }
    break;
  }
  case 4: {
    float saque;
    printf("Digite o valor a sacar: R$ ");
    scanf("%f", &saque);
    if (saque > 0 && saque <= *saldoReais) {
      *saldoReais -= saque;

      printf("\n");
      printf("Saque realizado com sucesso!\n");
        printf("----------------------------------\n");

      //add no extrato
      registrarTransacao("saque", "Reais", 0, saque);//modificado

      // Atualiza o saldo
      FILE *file = fopen(FILENAME, "r+");
      if (file != NULL) {
        char linha[50];
        long pos;
        while (fgets(linha, sizeof(linha), file)) {
          // Encontra a linha correspondente ao usuário
          if (strstr(linha, cpf) != NULL) {
            // Armazena a posição atual do arquivo
            pos = ftell(file);
            // Retorna ao início da linha para sobrescrever
            fseek(file, pos - strlen(linha), SEEK_SET);
            fprintf(file, "CPF:%s Senha:%s Saldo:%.2f\n", cpf, senha, *saldoReais);
            break;
          }
        }
        fclose(file);
      }
    } else if (saque > *saldoReais) {
      printf(
          "Saldo insuficiente para o saque de R$ %.2f. Saldo atual: R$ %.2f\n",
          saque, *saldoReais);
    } else {
      printf("Valor de saque inválido. Tente novamente.\n");
    }
    break;
  }
  case 5: {
    int escolhacripto;
    float valorcompra;
    printf("\n");
    printf("Digite o número referente a criptomoeda que você deseja comprar.\n");
    printf("\n");
    printf("1. BITCOIN - R$ %.2f\n", bitcoin_preco);
    printf("2. RIPPLE - R$ %.2f\n", ripple_preco);
    printf("3. ETHEREUM - R$ %.2f\n", ethereum_preco);
    scanf("%d", &escolhacripto);
    printf("Digite o valor em reais que deseja investir: R$ ");
    scanf("%f", &valorcompra);

    if (valorcompra > 0 && valorcompra <= *saldoReais) {
      float valorcomtaxa = aplicartaxacompra(valorcompra, escolhacripto);
      *saldoReais -= valorcompra;

      switch (escolhacripto) {
      case 1:
        criptoSaldo->bitcoin += valorcomtaxa / bitcoin_preco;
        printf("Compra de BITCOIN realizada com sucesso! Saldo atual %.6f BTC.\n", criptoSaldo->bitcoin);
        registrarTransacao("compra", "Bitcoin", valorcomtaxa / bitcoin_preco, valorcompra);//add no extrato
          printf("------------------------------------------------------------------------------------------\n");
        break;
      case 2:
        criptoSaldo->ripple += valorcomtaxa / ripple_preco;
        printf("Compra de RIPPLE realizada com sucesso! Saldo atual %.6f XPR.\n", criptoSaldo->ripple);
        registrarTransacao("compra", "Ripple", valorcomtaxa / ripple_preco, valorcompra);//add no extrato
          printf("------------------------------------------------------------------------------------------\n");
        break;
      case 3:
        criptoSaldo->ethereum += valorcomtaxa / ethereum_preco;
        printf("Compra de ETHEREUM realizada com sucesso! Saldo atual %.6f ETH.\n", criptoSaldo->ethereum);
        registrarTransacao("compra", "Ethereum", valorcomtaxa / ethereum_preco, valorcompra);//add no extrato
          printf("------------------------------------------------------------------------------------------\n");
        break;

      default:
        printf("Opção inválida.\n");
        *saldoReais += valorcompra;// opçao invalida dinheiro volta
        break;
      }
    } else {
      printf("Saldo insuficiente.\n");
    }
    break;

  }
  case 6: {
    int escolhacripto;
    float quantidadevenda;
    printf("\n");
    printf("Qual Criptomoeda deseja vender?\n");
    printf("1. BITCOIN - R$ %2f\n", bitcoin_preco);
    printf("2. RIPPLE - R$ %2f\n", ripple_preco);
    printf("3. ETHEREUM - R$ %2f\n", ethereum_preco);
    scanf("%d", &escolhacripto);
    printf("Digite a quantidade que deseja vender: ");
    scanf("%f", &quantidadevenda);

    float valorvenda = 0.0;

    switch (escolhacripto) {
    case 1:
      if (quantidadevenda <= criptoSaldo->bitcoin) {
        valorvenda = quantidadevenda * bitcoin_preco;
        valorvenda = aplicartaxavenda(valorvenda, escolhacripto); //aplica taxa
        criptoSaldo->bitcoin -= quantidadevenda;
        *saldoReais += valorvenda;
        printf("Venda de BITCOIN realizada com sucesso! Valor recebido: R$ %.2f. Saldo atual: R$ %.2f\n", valorvenda, *saldoReais);
        registrarTransacao("venda", "Bitcoin", quantidadevenda, valorvenda);//add extrato
            printf("------------------------------------------------------------------------------------------\n");
      } else {
        printf("Quantidade de BITCOIN insuficiente.\n");
      }
      break;
    case 2:
      if (quantidadevenda <= criptoSaldo->ripple) {
        valorvenda = quantidadevenda * ripple_preco;
        valorvenda = aplicartaxavenda(valorvenda, escolhacripto); //aplica taxa
        criptoSaldo->ripple -= quantidadevenda;
        *saldoReais += valorvenda;
        printf("Venda de RIPPLE realizada com sucesso! Valor recebido: R$ %.2f. Saldo atual: R$ %.2f\n", valorvenda, *saldoReais);
        registrarTransacao("venda", "Ripple", quantidadevenda, valorvenda);//add extrato
            printf("------------------------------------------------------------------------------------------\n");
      } else {
        printf("Quantidade de RIPPLE insuficiente.\n");
      }
      break;
      case 3:
        if (quantidadevenda <= criptoSaldo->ethereum) {
          valorvenda = quantidadevenda * ethereum_preco;
          valorvenda = aplicartaxavenda(valorvenda, escolhacripto); //aplica taxa
          criptoSaldo->ethereum -= quantidadevenda;
          *saldoReais += valorvenda;
          printf("Venda de Ethereum realizada com sucesso! Valor recebido: R$ %.2f. Saldo atual: R$ %.2f\n", valorvenda, *saldoReais);
          registrarTransacao("venda", "ETHEREUM", quantidadevenda, valorvenda);// add extrato
            printf("------------------------------------------------------------------------------------------\n");
        } else {
          printf("Quantidade de ETHEREUM insuficiente.\n");
        }
        break;
      default:
        printf("Opção inválida.\n");
        break;
      }
    break;
  }

  case 7:
    atualizarcotacao();
    printf("\n");
    break;
  case 8:
    printf("Encerrando programa!\n");
    break;
  default:
    printf("Opção inválida! Tente novamente.\n");
    break;
  }
}

int main() {
  int opcao;
  char cpf[12];
  char senha[20];
  float saldoReais = 0.0;//saldo zero
  CriptoSaldo criptoSaldo = {0, 0, 0};

  printf("Bem-vindo à FEI Crypto Exchange!\n");
  printf("---------------------------------\n");
  printf("1. Cadastrar novo usuário\n");
  printf("2. Efetuar login\n");
  printf("\n");
  printf("Escolha uma opção: ");
  scanf("%d", &opcao);

  if (opcao == 1) {
    cadastrarUsuario();
  } else if (opcao == 2) {
    printf("\n");
    printf("Insira o CPF: ");
    scanf("%11s", cpf);

    printf("Insira a senha: ");
    scanf("%19s", senha);

    if (efetuarLogin(cpf, senha, &saldoReais)) {
      printf("\n");
      printf("Login realizado com sucesso! Bem-vindo à FEI Crypto Exchange!\n");
      printf("------------------------------------------------------------------\n");
      do {
        menuPrincipal(saldoReais);
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        escolhaOpcao(opcao, &saldoReais, &criptoSaldo, cpf, senha);

      } while (opcao !=
               8);
    } else {
      printf("CPF ou senha incorretos. Tente novamente.\n");
    }
  } else {
    printf("Opção inválida. Encerrando o programa.\n");
  }

  return 0;
}