#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <stdlib.h>

TFT_eSPI tft = TFT_eSPI();

// Touchscreen pins
#define XPT2046_IRQ 36   // T_IRQ
#define XPT2046_MOSI 32  // T_DIN
#define XPT2046_MISO 39  // T_OUT
#define XPT2046_CLK 25   // T_CLK
#define XPT2046_CS 33    // T_CS

SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define FONT_SIZE 2

// Variáveis globais
int x, y, z;
int pontuacao = 100; // Começa com 1000 para permitir dobrar
int perguntaAtual = 0;
bool gameOver = false; // Flag para verificar se o jogo acabou
bool vitoria = false;  // Flag para verificar se o jogador venceu

// Estrutura para armazenar perguntas e alternativas
struct Pergunta {
  String texto;
  String alternativas[4];
  int respostaCorreta;  // Índice da alternativa correta
};

Pergunta perguntas[] = {
{"O que e uma variavel?", {"Armazenamento", "Funcao", "Loop", "Classe"}, 0},
  {"O que e um loop?", {"Funcao", "Estrutura de Repeticao", "Variavel", "Classe"}, 1},
  {"O que e uma funcao?", {"Armazenamento", "Classe", "Bloco de Codigo", "Loop"}, 2},
  {"O que e um array?", {"Variavel", "Estrutura de Dados", "Bloco de Codigo", "Funcao"}, 1},
  {"O que e um operador aritmetico?", {"Somar", "Subtrair", "Multiplicar", "Todas as alternativas"}, 3},
  {"O que faz a funcao printf?", {"Exibe texto", "Executa loop", "Armazena dado", "Define uma variavel"}, 0},
  {"O que e uma classe?", {"Objeto", "Tipo de Variavel", "Modelo de Objeto", "Tipo de Funcao"}, 2},
  {"O que e um objeto?", {"Instancia de Funcao", "Instancia de Classe", "Comando", "Variavel"}, 1},
  {"O que e heranca em orientacao a objetos?", {"Metodo", "Classe filha", "Atributo", "Interface"}, 1},
  {"O que e polimorfismo?", {"Classe Abstrata", "Sobrecarga de Variavel", "Sobrecarga de Funcao", "Interface"}, 2},
  {"O que e um ponteiro?", {"Variavel que armazena valor", "Variavel global", "Variavel que armazena endereco de memoria", "Funcao"}, 2},
  {"O que faz o comando 'return'?", {"Finaliza uma funcao", "Chama outra funcao", "Inicia uma funcao", "Armazena um valor"}, 0},
  {"O que e recursao?", {"Estrutura de repeticao", "Ponteiro", "Classe", "Chamada de funcao dentro dela mesma"}, 3},
  {"O que e um banco de dados relacional?", {"Aplicacao Web", "Sistema de Arquivos", "Tipo de Sistema Operacional", "Estrutura de Armazenamento"}, 3},
  {"O que e SQL?", {"Estrutura de Dados", "Sistema Operacional", "Banco de Dados", "Lingua de Programacao"}, 3},
  {"O que significa API?", {"Processador de Informacoes", "Modelo de Dados", "Aplicativo", "Interface de Programacao de Aplicacao"}, 3},
  {"O que e JSON?", {"Estrutura de Programacao", "Modelo de Objeto", "Banco de Dados", "Formato de Dados"}, 3},
  {"O que e Git?", {"Sistema Operacional", "Compilador", "Banco de Dados", "Controle de Versao"}, 3},
  {"O que e um algoritmo?", {"Estrutura de dados", "Tipo de variavel", "Comando de sistema", "Conjunto de passos para resolver um problema"}, 3},
  {"O que e uma estrutura de dados?", {"Metodos para armazenar dados", "Tipos de operacoes aritmeticas", "Codigos para compilar programas", "Comandos de sistema"}, 0},
  {"O que e a linguagem C?", {"Lingua de Programacao", "Sistema Operacional", "Banco de Dados", "Modelo de Objeto"}, 0},
  {"O que e Python?", {"Lingua de Programacao", "Estrutura de Dados", "Banco de Dados", "Sistema Operacional"}, 0},
  {"O que e uma condicional?", {"Classe", "Bloco de Codigo", "Variavel", "Estrutura de Controle"}, 3},
  {"O que e o operador &&?", {"Comparacao", "Atribuicao", "Ou Logico", "E Logico"}, 3},
  {"O que faz o operador ==?", {"Compara dois valores", "Exibe um valor", "Inicia um loop", "Atribui um valor"}, 0},
  {"O que e a orientacao a objetos?", {"Lingua de Programacao", "Comando de Sistema", "Estrutura de Dados", "Paradigma de programacao"}, 3},
  {"O que significa o termo 'encapsulamento'?", {"Criar variaveis", "Usar loops", "Definir funcoes", "Esconder detalhes internos"}, 3},
  {"O que e um compilador?", {"Traduz o codigo para linguagem de maquina", "Executa o programa", "Armazena variaveis", "Exibe texto"}, 0},
  {"O que e uma constante?", {"Ponteiro", "Comando", "Variavel", "Valor que nao muda"}, 3},
  {"O que significa o operador '!='?", {"Maior", "Atribuicao", "Menor", "Diferente"}, 3},
  {"O que e um banco de dados NoSQL?", {"Aplicacao Web", "Banco de Dados Relacional", "Interface de Programacao", "Banco de Dados Sem Estrutura Relacional"}, 3},
  {"O que e um framework?", {"Banco de Dados", "Sistema Operacional", "Conjunto de ferramentas para desenvolvimento", "Biblioteca de Funcoes"}, 2},
  {"O que e o conceito de 'Thread'?", {"Classe", "Metodo", "Ponteiro", "Execucao simultanea de tarefas"}, 3},
  {"O que significa a sigla IDE?", {"Banco de Dados", "Modelo de Programacao", "Interface de Programacao de Aplicacao", "Ambiente de Desenvolvimento Integrado"}, 3},
  {"O que e um tipo de dado?", {"Comando de sistema", "Especificacao do tipo de variavel", "Bloco de Codigo", "Estrutura de Controle"}, 1},
  {"O que significa 'HTTP'?", {"Banco de Dados", "Protocolo de Comunicacao", "Sistema Operacional", "Ambiente de Desenvolvimento"}, 1},
  {"O que significa a sigla URL?", {"Banco de Dados", "Lingua de Programacao", "Protocolo de Comunicacao", "Localizador Uniforme de Recursos"}, 3},
  {"O que significa 'IP'?", {"Banco de Dados", "Ambiente de Desenvolvimento", "Protocolo de Comunicacao", "Endereco de Protocolo de Internet"}, 3},
  {"O que e o conceito de 'DNS'?", {"Sistema de Nomes de Dominio", "Sistema de Dados", "Protocolo de Comunicacao", "Banco de Dados"}, 0},
  {"O que e uma variavel global?", {"Comando de sistema", "Variavel acessivel em todo o programa", "Variavel local", "Ponteiro"}, 1},
  {"O que faz a funcao malloc?", {"Libera memoria", "Executa um loop", "Aloca memoria dinamicamente", "Compara dois valores"}, 2},
  {"O que e um banco de dados?", {"Programa de sistema", "Sistema para armazenar e manipular dados", "Estrutura de Dados", "Comando de sistema"}, 1},
  {"O que e SQL Injection?", {"Ataque a rede", "Ataque ao sistema operacional", "Ataque de phishing", "Ataque para inserir comandos SQL"}, 3},
  {"O que e uma transacao?", {"Bloco de Codigo", "Comando", "Variavel", "Conjunto de operacoes atomicas"}, 3},
  {"O que e uma view?", {"Consulta ao banco de dados", "Tabela", "Index", "Operacao de SQL"}, 0},
  {"O que e uma trigger?", {"Estrutura de Repeticao", "Funcao", "Classe", "Acao automatica no banco de dados"}, 3},
  {"O que significa 'CRUD'?", {"Classe, Repetir, Unir, Deletar", "Criar, Relacionar, Utilizar, Definir", "Comando, Repetir, Utilizar, Deletar", "Criar, Ler, Atualizar, Deletar"}, 3},
  {"O que e uma funcao anonima?", {"Funcao que retorna valor", "Funcao sem nome", "Funcao global", "Funcao com parametros"}, 1},
  {"O que significa 'AJAX'?", {"Lingua de Programacao", "Banco de Dados", "Tecnologia para requisicoes assincronas", "Protocolo de Comunicacao"}, 2},
  {"O que e REST?", {"Estilo arquitetural para APIs", "Linguagem de Programacao", "Banco de Dados", "Sistema de Controle"}, 0},
  {"O que significa o termo 'session'?", {"Banco de Dados", "Estrutura de Dados", "Funcao", "Armazenamento temporario de dados"}, 3},
  {"O que e o comando 'git merge'?", {"Deleta branch", "Une branches", "Exibe commits", "Cria branch"}, 1},
  {"O que faz a funcao 'git pull'?", {"Atualiza o repositorio local", "Adiciona arquivo", "Cria novo repositorio", "Deleta repositorio"}, 0},
  {"O que e um teste unitario?", {"Teste de seguranca", "Teste de banco de dados", "Teste de integracao", "Teste de funcao isolada"}, 3},
  {"O que significa a sigla OOP?", {"Banco de Dados", "Estrutura de Dados", "Programacao Orientada a Objetos", "Programacao de Objetos"}, 2},
  {"O que significa 'MVC'?", {"Modelo, View, Controller", "Modelo, Visita, Controlador", "Metodo, Variavel, Classe", "Modulo, Variavel, Comando"}, 0},
  {"O que e uma fila?", {"Classe", "Estrutura de Dados FIFO", "Operacao de Banco", "Estrutura de Dados LIFO"}, 1},
  {"O que e uma pilha?", {"Estrutura de Dados LIFO", "Estrutura de Dados FIFO", "Funcao", "Classe"}, 0},
  {"O que e um arquivo binario?", {"Banco de Dados", "Arquivo de texto", "Arquivo com dados nao legiveis por humanos", "Comando de sistema"}, 2},
  {"O que faz o comando 'chmod'?", {"Cria arquivos", "Deleta arquivos", "Altera permissoes de arquivos", "Altera o nome de arquivos"}, 2},
  {"O que e uma chave primaria?", {"Indice de uma tabela", "Identificador unico de uma tabela", "Chave de criptografia", "Variavel de classe"}, 1},
  {"O que e um join?", {"Operacao de sistema", "Comando de banco de dados", "Operacao de uniao de tabelas", "Chave de banco"}, 2},
  {"O que e um loop infinito?", {"Loop que nunca termina", "Loop com condicao de parada", "Loop que executa uma vez", "Loop com limite de repeticoes"}, 0},
  {"O que significa o operador '<<'?", {"Atribuicao de valor", "Comparacao", "Deslocamento de bits para a direita", "Deslocamento de bits para a esquerda"}, 3},
  {"O que significa 'git clone'?", {"Atualiza um repositorio", "Cria um commit", "Deleta um repositorio", "Cria uma copia de um repositorio"}, 3},
  {"O que e um debugger?", {"Banco de dados", "Bloco de codigo", "Sistema operacional", "Ferramenta de analise de codigo"}, 3},
  {"O que e o conceito de Heranca em programacao?", {"Permite reutilizar codigo de uma classe", "Chama funcoes de outra classe", "Remove uma classe do projeto", "Altera valores de uma classe"}, 0},
  {"O que significa FIFO em estruturas de dados?", {"First In First Out", "First In Last Out", "Fast In Fast Out", "Fast Input Output"}, 0},
  {"O que faz o comando ls em sistemas baseados em Unix?", {"Lista arquivos e diretorios", "Move arquivos", "Apaga arquivos", "Renomeia arquivos"}, 0},
  {"O que e um endereco IP?", {"Identificador unico de uma rede ou dispositivo", "Protocolo de comunicacao", "Codigo de uma pagina web", "Sistema operacional"}, 0},
  {"O que faz o comando cd em sistemas baseados em Unix?", {"Altera o diretorio atual", "Cria um novo arquivo", "Deleta um diretorio", "Lista arquivos no diretorio atual"}, 0},
  {"O que significa LIFO em estruturas de dados?", {"Last In First Out", "Last Input First Output", "List of Items for Operation", "Logical Input File Organizer"}, 0},
  {"O que significa DRY em desenvolvimento de software?", {"Dont Repeat Yourself", "Develop Rapidly", "Do it Right Yesterday", "Duplicate Resources Yield"}, 0},
  {"O que e um array?", {"Estrutura para armazenar varios valores", "Bloco de codigo repetido", "Funcao para criar objetos", "Classe que manipula dados"}, 0},
  {"O que e um cache?", {"Armazenamento temporario de dados", "Espaco reservado no banco de dados", "Metodo de compressao de dados", "Bloco de codigo para repeticao"}, 0},
  {"O que e uma chave estrangeira em um banco de dados?", {"Relaciona tabelas diferentes", "Define valores unicos", "Remove duplicatas", "Define indices"}, 0},
  {"O que e uma pilha em estruturas de dados?", {"Estrutura que segue o principio LIFO", "Bloco de codigo para execucao", "Classe para armazenar dados", "Estrutura que segue o principio FIFO"}, 0},
  {"O que e um loop while?", {"Estrutura de controle que repete enquanto uma condicao for verdadeira", "Estrutura que executa uma unica vez", "Classe que inicializa um objeto", "Funcao que chama outra funcao"}, 0},
  {"O que significa ORM em desenvolvimento?", {"Object Relational Mapping", "Object Retrieval Method", "Operational Resource Management", "Open Relational Mode"}, 0},
  {"O que faz o comando echo no terminal?", {"Imprime uma mensagem no terminal", "Apaga arquivos no sistema", "Lista diretorios disponiveis", "Executa scripts salvos"}, 0},
  {"O que significa HTTP?", {"Hypertext Transfer Protocol", "High Transfer Technology Protocol", "Hyperlink Text Platform", "Host Transfer Tool Protocol"}, 0},
  {"O que e um pacote em programacao?", {"Conjunto de classes ou modulos relacionados", "Bloco de codigo executavel", "Variavel global de projeto", "Script de inicializacao de programa"}, 0},
  {"O que e um objeto?", {"Instancia de uma classe", "Metodo que manipula dados", "Bloco de controle para repeticao", "Estrutura para armazenar chaves"}, 0},
  {"O que faz o comando mkdir?", {"Cria um novo diretorio", "Renomeia um diretorio", "Apaga um diretorio", "Move arquivos para um diretorio"}, 0},
  {"O que e uma API RESTful?", {"Interface baseada em principios REST", "Sistema de comunicacao por sockets", "Ferramenta para desenvolver APIs", "Protocolo de transferencia de dados"}, 0},
  {"O que significa DNS?", {"Domain Name System", "Data Network Structure", "Direct Name System", "Distributed Node Server"}, 0},
  {"O que e um pacote npm?", {"Biblioteca ou modulo para projetos Node.js", "Script de inicializacao para APIs", "Sistema de controle de versoes", "Banco de dados para projetos web"}, 0},
  {"O que faz a funcao console.log() em JavaScript?", {"Exibe mensagens no console", "Adiciona dados ao navegador", "Cria logs de erro", "Executa um arquivo JavaScript"}, 0},
  {"O que significa CRUD?", {"Create, Read, Update, Delete", "Create, Replace, Update, Debug", "Copy, Read, Use, Delete", "Classify, Replace, Use, Debug"}, 0},
  {"O que e uma transacao em banco de dados?", {"Conjunto de operacoes que devem ser realizadas como uma unidade", "Comando para alterar tabelas", "Metodo para buscar dados", "Chave para unir tabelas"}, 0},
  {"O que e um middleware?", {"Funcao intermediaria entre requisicoes e respostas", "Estrutura para armazenar variaveis", "Metodo para compilar codigo", "Classe para criar objetos"}, 0},
  {"O que faz o operador typeof em JavaScript?", {"Retorna o tipo de uma variavel", "Compara dois valores", "Executa funcoes em cadeia", "Remove chaves de um objeto"}, 0},
  {"O que e o conceito de Heranca em programacao?", {"Remove uma classe do projeto", "Altera valores de uma classe", "Permite reutilizar codigo de uma classe", "Chama funcoes de outra classe"}, 2},
  {"O que significa FIFO em estruturas de dados?", {"Fast Input Output", "First In First Out", "First In Last Out", "Fast In Fast Out"}, 1},
  {"O que faz o comando ls em sistemas baseados em Unix?", {"Apaga arquivos", "Renomeia arquivos", "Move arquivos", "Lista arquivos e diretorios"}, 3},
  {"O que e um endereco IP?", {"Codigo de uma pagina web", "Sistema operacional", "Identificador unico de uma rede ou dispositivo", "Protocolo de comunicacao"}, 2},
  {"O que faz o comando cd em sistemas baseados em Unix?", {"Lista arquivos no diretorio atual", "Cria um novo arquivo", "Deleta um diretorio", "Altera o diretorio atual"}, 3},
  {"O que significa LIFO em estruturas de dados?", {"Logical Input File Organizer", "Last Input First Output", "Last In First Out", "List of Items for Operation"}, 2},
  {"O que significa DRY em desenvolvimento de software?", {"Duplicate Resources Yield", "Develop Rapidly", "Do it Right Yesterday", "Dont Repeat Yourself"}, 3},
  {"O que e um array?", {"Classe que manipula dados", "Estrutura para armazenar varios valores", "Funcao para criar objetos", "Bloco de codigo repetido"}, 1},
  {"O que e um cache?", {"Bloco de codigo para repeticao", "Metodo de compressao de dados", "Armazenamento temporario de dados", "Espaco reservado no banco de dados"}, 2},
  {"O que e uma chave estrangeira em um banco de dados?", {"Relaciona tabelas diferentes", "Remove duplicatas", "Define indices", "Define valores unicos"}, 0},
  {"O que e uma pilha em estruturas de dados?", {"Classe para armazenar dados", "Estrutura que segue o principio FIFO", "Bloco de codigo para execucao", "Estrutura que segue o principio LIFO"}, 3},
  {"O que e um loop while?", {"Funcao que chama outra funcao", "Estrutura de controle que repete enquanto uma condicao for verdadeira", "Estrutura que executa uma unica vez", "Classe que inicializa um objeto"}, 1},
  {"O que significa ORM em desenvolvimento?", {"Object Retrieval Method", "Object Relational Mapping", "Operational Resource Management", "Open Relational Mode"}, 1},
  {"O que faz o comando echo no terminal?", {"Lista diretorios disponiveis", "Executa scripts salvos", "Apaga arquivos no sistema", "Imprime uma mensagem no terminal"}, 3},
  {"O que significa HTTP?", {"High Transfer Technology Protocol", "Host Transfer Tool Protocol", "Hypertext Transfer Protocol", "Hyperlink Text Platform"}, 2},
  {"O que e um pacote em programacao?", {"Classe que manipula dados", "Conjunto de classes ou modulos relacionados", "Variavel global de projeto", "Bloco de codigo executavel"}, 1},
  {"O que e um objeto?", {"Metodo que manipula dados", "Bloco de controle para repeticao", "Estrutura para armazenar chaves", "Instancia de uma classe"}, 3},
  {"O que faz o comando mkdir?", {"Renomeia um diretorio", "Move arquivos para um diretorio", "Apaga um diretorio", "Cria um novo diretorio"}, 3},
  {"O que e uma API RESTful?", {"Sistema de comunicacao por sockets", "Interface baseada em principios REST", "Protocolo de transferencia de dados", "Ferramenta para desenvolver APIs"}, 1},
  {"O que significa DNS?", {"Data Network Structure", "Direct Name System", "Distributed Node Server", "Domain Name System"}, 3},
  {"O que e um pacote npm?", {"Biblioteca ou modulo para projetos Node.js", "Script de inicializacao para APIs", "Sistema de controle de versoes", "Banco de dados para projetos web"}, 0},
  {"O que faz a funcao console.log() em JavaScript?", {"Cria logs de erro", "Adiciona dados ao navegador", "Executa um arquivo JavaScript", "Exibe mensagens no console"}, 3},
  {"O que significa CRUD?", {"Classify, Replace, Use, Debug", "Create, Replace, Update, Debug", "Copy, Read, Use, Delete", "Create, Read, Update, Delete"}, 3},
  {"O que e uma transacao em banco de dados?", {"Metodo para buscar dados", "Chave para unir tabelas", "Conjunto de operacoes que devem ser realizadas como uma unidade", "Comando para alterar tabelas"}, 2},
  {"O que e um middleware?", {"Estrutura para armazenar variaveis", "Classe para criar objetos", "Metodo para compilar codigo", "Funcao intermediaria entre requisicoes e respostas"}, 3},
  {"O que faz o operador typeof em JavaScript?", {"Retorna o tipo de uma variavel", "Executa funcoes em cadeia", "Remove chaves de um objeto", "Compara dois valores"}, 0},
};

// Número de perguntas
int numPerguntas = sizeof(perguntas) / sizeof(perguntas[0]);
int ordemPerguntas[sizeof(perguntas) / sizeof(perguntas[0])];

void quebrarTexto(String texto, int larguraMaxima, int x, int y, int espacoLinha) {
  int comprimentoTexto = texto.length();
  String linhaAtual = "";

  for (int i = 0; i < comprimentoTexto; i++) {
    linhaAtual += texto[i];

    // Verifica se o comprimento da linha excede o limite ou se é o final do texto
    if (tft.textWidth(linhaAtual) > larguraMaxima || i == comprimentoTexto - 1) {
      tft.drawCentreString(linhaAtual, x, y, FONT_SIZE); // Exibe a linha
      y += espacoLinha; // Move para a próxima linha
      linhaAtual = "";  // Reseta a linha
    }
  }
}

// Função para embaralhar a ordem das perguntas
void embaralharPerguntas() {
  for (int i = 0; i < numPerguntas; i++) {
    ordemPerguntas[i] = i;
  }
  for (int i = 0; i < numPerguntas; i++) {
    int j = random(0, numPerguntas);
    int temp = ordemPerguntas[i];
    ordemPerguntas[i] = ordemPerguntas[j];
    ordemPerguntas[j] = temp;
  }
}

// Função para mostrar a pergunta e alternativas na tela
void exibirPergunta() {
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);

  // Exibe a pergunta com quebra de linha
  quebrarTexto(perguntas[ordemPerguntas[perguntaAtual]].texto, SCREEN_WIDTH - 20, SCREEN_WIDTH / 2, 40, 20);

  // Exibe as alternativas
  tft.drawCentreString(perguntas[ordemPerguntas[perguntaAtual]].alternativas[0], 160, 110, FONT_SIZE);
  tft.drawCentreString(perguntas[ordemPerguntas[perguntaAtual]].alternativas[1], 160, 145, FONT_SIZE);
  tft.drawCentreString(perguntas[ordemPerguntas[perguntaAtual]].alternativas[2], 160, 180, FONT_SIZE);
  tft.drawCentreString(perguntas[ordemPerguntas[perguntaAtual]].alternativas[3], 160, 215, FONT_SIZE);

  // Exibe a pontuação
  tft.drawString("Pontuacao: " + String(pontuacao), 200, 5, FONT_SIZE);
  tft.drawString("SHOW DO DEVAO ", 40, 5, FONT_SIZE);
}


// Função para verificar a resposta
void verificarResposta(int alternativaSelecionada) {
  if (alternativaSelecionada == perguntas[ordemPerguntas[perguntaAtual]].respostaCorreta) {
    pontuacao *= 2; // Dobra a pontuação se a resposta estiver correta
  } else {
    gameOver = true; // Define game over se a resposta estiver errada
  }

  if (pontuacao >= 1000000) {
    vitoria = true;
    exibirVitoria();
  } else if (!gameOver) {
    perguntaAtual = (perguntaAtual + 1) % numPerguntas; // Passa para a próxima pergunta
    exibirPergunta();
  } else {
    exibirGameOver();
  }
}

// Função para exibir a tela de Game Over
void exibirGameOver() {
  tft.fillScreen(TFT_RED);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.drawCentreString("GAME OVER", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 10, 2);
  tft.drawCentreString("Pontuacao Final: " + String(pontuacao), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 10, 2);
  tft.drawCentreString("Toque para reiniciar", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50, 2);
}

// Função para exibir a tela de vitória
void exibirVitoria() {
  tft.fillScreen(TFT_GREEN);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.drawCentreString("PARABENS!", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 10, 2);
  tft.drawCentreString("Voce ganhou 1 MILHAO!", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 10, 2);
  tft.drawCentreString("Toque para reiniciar", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50, 2);
}

// Função para reiniciar o jogo
void reiniciarJogo() {
  pontuacao = 1000;
  perguntaAtual = 0;
  gameOver = false;
  vitoria = false;
  embaralharPerguntas();
  exibirPergunta();
}

void setup() {
  Serial.begin(115200);

  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  touchscreen.setRotation(1);

  tft.init();
  tft.setRotation(1);

  embaralharPerguntas();
  exibirPergunta();
}

void loop() {
  if (gameOver || vitoria) {
    if (touchscreen.tirqTouched() && touchscreen.touched()) {
      reiniciarJogo(); // Reinicia o jogo ao tocar na tela
    }
  } else {
    if (touchscreen.tirqTouched() && touchscreen.touched()) {
      TS_Point p = touchscreen.getPoint();
      x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
      y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
      z = p.z;

      // Verifica qual botão foi pressionado com base nas coordenadas
      if (x > 100 && x < 220) {
        if (y > 105 && y < 125) verificarResposta(0);    // Primeira alternativa
        else if (y > 140 && y < 160) verificarResposta(1); // Segunda alternativa
        else if (y > 175 && y < 195) verificarResposta(2); // Terceira alternativa
        else if (y > 210 && y < 230) verificarResposta(3); // Quarta alternativa
      }
      
      delay(1000);
    }
  }
}

