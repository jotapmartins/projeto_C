#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILENAME "USUARIO.txt"

// valor inicial criptomoedas
float bitcoin_preco = 150000.0;
float ripple_preco = 5.0;
float ethereum_preco = 10000.0;

typedef struct {
  char tipo[10];       // "compra" ou "venda"
  char cripto[20];     // Nome da criptomoeda
  float quantidade;     // Quantidade comprada ou vendida
  float valor;          // Valor da transação
  char dataHora[20];   // Data e hora da transação
} Transacao;

//extrato
void registrarTransacao(const char *tipo, const char *cripto, float quantidade, float valor) {
    FILE *file = fopen("transacoes.txt", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de transações.\n");
        return;
    }

    // Obter a data e hora atual
    time_t agora;
    time(&agora);
    char *dataHora = ctime(&agora);
    dataHora[strcspn(dataHora, "\n")] = 0; // Remove a nova linha do final

    fprintf(file, "Tipo:%s Cripto:%s Quantidade:%.2f Valor:%.2f DataHora:%s\n",
            tipo, cripto, quantidade, valor);

    fclose(file);
}

typedef struct {
  float bitcoin;
  float ripple;
  float ethereum;
} CriptoSaldo;

// função de variação valor (-5% a +5%)
float gerarvariacao() {
  return ((rand() % 101) - 50) / 1000.0;
}

// função atualizar cotação
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

// função taxas na compra
float aplicartaxacompra(float valorcompra, int cripto) {
  float taxa = 0.0;
  switch (cripto) {
    case 1: //bitcoin
      taxa = valorcompra * 0.02;
      break;
    case 2: //ripple
      taxa = valorcompra * 0.01;
      break;
    case 3: //ethereum
      taxa = valorcompra * 0.01;
      break;
  }
  return valorcompra - taxa; //voltar valor após taxa
}

// função taxas na venda
float aplicartaxavenda(float valorvenda, int cripto) {
  float taxa = 0.0;
  switch (cripto) {
    case 1: //bitcoin
      taxa = valorvenda * 0.03;
      break;
    case 2: //ripple
      taxa = valorvenda * 0.02;
      break;
    case 3: //ethereum
      taxa = valorvenda * 0.01;
      break;
  }
  return valorvenda - taxa; //voltar valor após taxa
}

// Função para cadastrar o usuário (CPF, senha e saldo inicial)
void cadastrarUsuario() {
  char cpf[12];
  char senha[20];
  float saldoReais = 0.0; // Saldo inicial em reais

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

  // Escrevendo o CPF, a senha e o saldo no arquivo
  fprintf(file, "CPF:%s Senha:%s Saldo:%.2f\n", cpf, senha, saldoReais);

  fclose(file);
  printf("Cadastro realizado com sucesso!\n");
}

// Função para verificar o login
int efetuarLogin(char *cpf, char *senha, float *saldo) {
  char cpfCadastrado[12];
  char senhaCadastrada[20];
  char linha[50];

  // Abrindo o arquivo para leitura
  FILE *file = fopen(FILENAME, "r");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo de login.\n");
    return 0;
  }

  // Lendo o CPF, senha e saldo do arquivo linha por linha
  while (fgets(linha, sizeof(linha), file)) {
    // Verifica se a linha contém um CPF, uma senha e o saldo
    if (sscanf(linha, "CPF:%11s Senha:%19s Saldo:%f", cpfCadastrado,
               senhaCadastrada, saldo) == 3) {
      // Remover espaços em branco do final do CPF e da senha
      cpfCadastrado[strcspn(cpfCadastrado, "\n")] =
          '\0'; // Remove newline, se houver
      senhaCadastrada[strcspn(senhaCadastrada, "\n")] =
          '\0'; // Remove newline, se houver

      // Comparando o CPF e a senha inseridos com os dados cadastrados
      if (strcmp(cpf, cpfCadastrado) == 0 &&
          strcmp(senha, senhaCadastrada) == 0) {
        fclose(file);
        return 1; // Login correto
      }
    }
  }

  fclose(file);
  return 0; // Login incorreto
}

// Função para exibir o menu principal
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

// Função para executar a escolha do usuário
void escolhaOpcao(int opcao, float *saldoReais, CriptoSaldo *criptoSaldo, const char *cpf,
                  const char *senha) {
  switch (opcao) {
  case 1:
    printf("Saldo atual: %.2f\n", *saldoReais);
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
            printf("%s", linha); // Exibe cada linha do arquivo
        }
      printf("--------------------------------------------------------------------------------------\n");

        fclose(file);
        break;
    }
  case 3: { // Depositar
    float deposito;
    printf("Digite o valor a depositar: ");
    scanf("%f", &deposito);
    if (deposito > 0) {
      *saldoReais += deposito; // Adiciona o depósito ao saldo
      
      printf("\n");
      printf("Depósito realizado com sucesso!\n");
        printf("--------------------------------------\n");

      //registrar deposito no extrato
      registrarTransacao("deposito", "Reais", 0, deposito); //modificado

      // Atualiza o saldo no arquivo
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
    } else {
      printf("Valor de depósito inválido. Tente novamente.\n");
    }
    break;
  }
  case 4: { // Sacar
    float saque;
    printf("Digite o valor a sacar: ");
    scanf("%f", &saque);
    if (saque > 0 && saque <= *saldoReais) {
      *saldoReais -= saque; // Subtrai o saque do saldo
      
      printf("\n");
      printf("Saque realizado com sucesso!\n");
        printf("----------------------------------\n");

      //registrar saque no extrato
      registrarTransacao("saque", "Reais", 0, saque);//modificado

      // Atualiza o saldo no arquivo
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
    printf("Digite o valor em reais que deseja gastar: ");
    scanf("%f", &valorcompra);

    if (valorcompra > 0 && valorcompra <= *saldoReais) {
      float valorcomtaxa = aplicartaxacompra(valorcompra, escolhacripto);
      *saldoReais -= valorcompra; // subtrai o valor da compra do saldo

      switch (escolhacripto) {
      case 1: //compra bitcoin
        criptoSaldo->bitcoin += valorcomtaxa / bitcoin_preco;
        printf("Compra de BITCOIN realizada com sucesso! Saldo atual %.6f BTC.\n", criptoSaldo->bitcoin);
        registrarTransacao("compra", "Bitcoin", valorcomtaxa / bitcoin_preco, valorcompra);////////
          printf("------------------------------------------------------------------------------------------\n");
        break;
      case 2: //compra ripple
        criptoSaldo->ripple += valorcomtaxa / ripple_preco;
        printf("Compra de RIPPLE realizada com sucesso! Saldo atual %.6f XPR.\n", criptoSaldo->ripple);
        registrarTransacao("compra", "Ripple", valorcomtaxa / ripple_preco, valorcompra);////////
          printf("------------------------------------------------------------------------------------------\n");
        break;
      case 3: //compra ethereum
        criptoSaldo->ethereum += valorcomtaxa / ethereum_preco;
        printf("Compra de ETHEREUM realizada com sucesso! Saldo atual %.6f ETH.\n", criptoSaldo->ethereum);
        registrarTransacao("compra", "Ethereum", valorcomtaxa / ethereum_preco, valorcompra);////////
          printf("------------------------------------------------------------------------------------------\n");
        break;

      default:
        printf("Opção inválida.\n");
        *saldoReais += valorcompra; // reembolso em opção inválida
        break;
      }
    } else {
      printf("Saldo insuficiente.\n");
    }
    break;
    // Lógica para comprar criptomoedas

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
    case 1: //venda bitcoin
      if (quantidadevenda <= criptoSaldo->bitcoin) {
        valorvenda = quantidadevenda * bitcoin_preco;
        valorvenda = aplicartaxavenda(valorvenda, escolhacripto); //aplica taxa
        criptoSaldo->bitcoin -= quantidadevenda;
        *saldoReais += valorvenda;
        printf("Venda de BITCOIN realizada com sucesso! Valor recebido: R$ %.2f. Saldo atual: R$ %.2f\n", valorvenda, *saldoReais);
        registrarTransacao("venda", "Bitcoin", quantidadevenda, valorvenda);//////////
            printf("------------------------------------------------------------------------------------------\n");
      } else {
        printf("Quantidade de BITCOIN insuficiente.\n");
      }
      break;
    case 2: //venda ripple
      if (quantidadevenda <= criptoSaldo->ripple) {
        valorvenda = quantidadevenda * ripple_preco;
        valorvenda = aplicartaxavenda(valorvenda, escolhacripto); //aplica taxa
        criptoSaldo->ripple -= quantidadevenda;
        *saldoReais += valorvenda;
        printf("Venda de RIPPLE realizada com sucesso! Valor recebido: R$ %.2f. Saldo atual: R$ %.2f\n", valorvenda, *saldoReais);
        registrarTransacao("venda", "Ripple", quantidadevenda, valorvenda);/////////
            printf("------------------------------------------------------------------------------------------\n");
      } else {
        printf("Quantidade de RIPPLE insuficiente.\n");
      }
      break;
      case 3: //venda ethereum
        if (quantidadevenda <= criptoSaldo->ethereum) {
          valorvenda = quantidadevenda * ethereum_preco;
          valorvenda = aplicartaxavenda(valorvenda, escolhacripto); //aplica taxa
          criptoSaldo->ethereum -= quantidadevenda;
          *saldoReais += valorvenda;
          printf("Venda de Ethereum realizada com sucesso! Valor recebido: R$ %.2f. Saldo atual: R$ %.2f\n", valorvenda, *saldoReais);
          registrarTransacao("venda", "ETHEREUM", quantidadevenda, valorvenda);//////////
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
// Lógica para vender criptomoedas

  case 7:
    atualizarcotacao();
    // Lógica para atualizar cotação
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
  float saldoReais = 0.0; // Inicializa saldo
  CriptoSaldo criptoSaldo = {0, 0, 0}; //inicializa saldo de cripto

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

    // Passa o endereço de saldo para efetuarLogin
    if (efetuarLogin(cpf, senha, &saldoReais)) {
      printf("\n");
      printf("Login realizado com sucesso! Bem-vindo à FEI Crypto Exchange!\n");
      printf("------------------------------------------------------------------\n");
      do {
        menuPrincipal(saldoReais);
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        escolhaOpcao(opcao, &saldoReais, &criptoSaldo, cpf,
                     senha); // Chama a função para executar a opção escolhida

      } while (opcao !=
               8); // O loop continua até que a opção 8 (sair) seja escolhida
    } else {
      printf("CPF ou senha incorretos. Tente novamente.\n");
    }
  } else {
    printf("Opção inválida. Encerrando o programa.\n");
  }

  return 0;
}