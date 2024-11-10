#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

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
int pontuacao = 0;  // Variável para pontuação
int perguntaAtual = 0;
bool gameOver = false; // Flag para verificar se o jogo acabou

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
  {"O que e polimorfismo?", {"Sobrecarga de Funcao", "Sobrecarga de Variavel", "Interface", "Classe Abstrata"}, 0},
  {"O que e um ponteiro?", {"Variavel que armazena endereco de memoria", "Variavel que armazena valor", "Variavel global", "Funcao"}, 0},
  {"O que faz o comando 'return'?", {"Finaliza uma funcao", "Inicia uma funcao", "Chama outra funcao", "Armazena um valor"}, 0},
  {"O que e recursao?", {"Chamada de funcao dentro dela mesma", "Estrutura de repeticao", "Classe", "Ponteiro"}, 0},
  {"O que e um banco de dados relacional?", {"Estrutura de Armazenamento", "Tipo de Sistema Operacional", "Sistema de Arquivos", "Aplicacao Web"}, 0},
  {"O que e SQL?", {"Lingua de Programacao", "Banco de Dados", "Sistema Operacional", "Estrutura de Dados"}, 0},
  {"O que significa API?", {"Interface de Programacao de Aplicacao", "Aplicativo", "Modelo de Dados", "Processador de Informacoes"}, 0},
  {"O que e JSON?", {"Formato de Dados", "Banco de Dados", "Modelo de Objeto", "Estrutura de Programacao"}, 0},
  {"O que e Git?", {"Controle de Versao", "Compilador", "Banco de Dados", "Sistema Operacional"}, 0},
  {"O que e um algoritmo?", {"Conjunto de passos para resolver um problema", "Comando de sistema", "Estrutura de dados", "Tipo de variavel"}, 0},
  {"O que e uma estrutura de dados?", {"Metodos para armazenar dados", "Codigos para compilar programas", "Comandos de sistema", "Tipos de operacoes aritmeticas"}, 0},
  {"O que e a linguagem C?", {"Lingua de Programacao", "Sistema Operacional", "Banco de Dados", "Modelo de Objeto"}, 0},
  {"O que e Python?", {"Lingua de Programacao", "Banco de Dados", "Estrutura de Dados", "Sistema Operacional"}, 0},
  {"O que e uma condicional?", {"Estrutura de Controle", "Bloco de Codigo", "Classe", "Variavel"}, 0},
  {"O que e o operador &&?", {"E Logico", "Ou Logico", "Comparacao", "Atribuicao"}, 0},
  {"O que faz o operador ==?", {"Compara dois valores", "Atribui um valor", "Inicia um loop", "Exibe um valor"}, 0},
  {"O que e a orientacao a objetos?", {"Paradigma de programacao", "Lingua de Programacao", "Estrutura de Dados", "Comando de Sistema"}, 0},
  {"O que significa o termo 'encapsulamento'?", {"Esconder detalhes internos", "Criar variaveis", "Definir funcoes", "Usar loops"}, 0},
  {"O que e um compilador?", {"Traduz o codigo para linguagem de maquina", "Exibe texto", "Armazena variaveis", "Executa o programa"}, 0},
  {"O que e uma constante?", {"Valor que nao muda", "Variavel", "Comando", "Ponteiro"}, 0},
  {"O que significa o operador '!='?", {"Diferente", "Atribuicao", "Maior", "Menor"}, 0},
  {"O que e um banco de dados NoSQL?", {"Banco de Dados Sem Estrutura Relacional", "Banco de Dados Relacional", "Aplicacao Web", "Interface de Programacao"}, 0},
  {"O que e um framework?", {"Conjunto de ferramentas para desenvolvimento", "Banco de Dados", "Sistema Operacional", "Biblioteca de Funcoes"}, 0},
  {"O que e o conceito de 'Thread'?", {"Execucao simultanea de tarefas", "Classe", "Metodo", "Ponteiro"}, 0},
  {"O que significa a sigla IDE?", {"Ambiente de Desenvolvimento Integrado", "Interface de Programacao de Aplicacao", "Banco de Dados", "Modelo de Programacao"}, 0},
  {"O que e um tipo de dado?", {"Especificacao do tipo de variavel", "Comando de sistema", "Estrutura de Controle", "Bloco de Codigo"}, 0},
  {"O que significa 'HTTP'?", {"Protocolo de Comunicacao", "Banco de Dados", "Sistema Operacional", "Ambiente de Desenvolvimento"}, 0},
  {"O que significa a sigla URL?", {"Localizador Uniforme de Recursos", "Lingua de Programacao", "Protocolo de Comunicacao", "Banco de Dados"}, 0},
  {"O que significa 'IP'?", {"Endereco de Protocolo de Internet", "Banco de Dados", "Protocolo de Comunicacao", "Ambiente de Desenvolvimento"}, 0},
  {"O que e o conceito de 'DNS'?", {"Sistema de Nomes de Dominio", "Sistema de Dados", "Protocolo de Comunicacao", "Banco de Dados"}, 0},
  {"O que e uma variavel global?", {"Variavel acessivel em todo o programa", "Variavel local", "Comando de sistema", "Ponteiro"}, 0},
  {"O que faz a funcao malloc?", {"Aloca memoria dinamicamente", "Libera memoria", "Executa um loop", "Compara dois valores"}, 0},
  {"O que e um banco de dados?", {"Sistema para armazenar e manipular dados", "Programa de sistema", "Comando de sistema", "Estrutura de Dados"}, 0},
  {"O que e SQL Injection?", {"Ataque para inserir comandos SQL", "Ataque a rede", "Ataque ao sistema operacional", "Ataque de phishing"}, 0},
  {"O que e uma transacao?", {"Conjunto de operacoes atômicas", "Variavel", "Comando", "Bloco de Codigo"}, 0},
  {"O que e uma view?", {"Consulta ao banco de dados", "Tabela", "Index", "Operacao de SQL"}, 0},
  {"O que e uma trigger?", {"Acao automatica no banco de dados", "Funcao", "Classe", "Estrutura de Repeticao"}, 0},
  {"O que significa 'CRUD'?", {"Criar, Ler, Atualizar, Deletar", "Comando, Repetir, Utilizar, Deletar", "Classe, Repetir, Unir, Deletar", "Criar, Relacionar, Utilizar, Definir"}, 0},
  {"O que e uma funcao anônima?", {"Funcao sem nome", "Funcao com parametros", "Funcao que retorna valor", "Funcao global"}, 0},
  {"O que significa 'AJAX'?", {"Tecnologia para requisições assíncronas", "Lingua de Programacao", "Banco de Dados", "Protocolo de Comunicacao"}, 0},
  {"O que e REST?", {"Estilo arquitetural para APIs", "Banco de Dados", "Linguagem de Programacao", "Sistema de Controle"}, 0},
  {"O que significa o termo 'session'?", {"Armazenamento temporario de dados", "Banco de Dados", "Funcao", "Estrutura de Dados"}, 0},
  {"O que e o comando 'git merge'?", {"Une branches", "Cria branch", "Deleta branch", "Exibe commits"}, 0},
  {"O que faz a funcao 'git pull'?", {"Atualiza o repositório local", "Cria novo repositório", "Adiciona arquivo", "Deleta repositório"}, 0},
  {"O que e um teste unitario?", {"Teste de funcao isolada", "Teste de integração", "Teste de banco de dados", "Teste de segurança"}, 0},
  {"O que significa a sigla OOP?", {"Programacao Orientada a Objetos", "Programacao de Objetos", "Estrutura de Dados", "Banco de Dados"}, 0},
  {"O que significa 'MVC'?", {"Modelo, Visita, Controlador", "Modelo, View, Controller", "Metodo, Variavel, Classe", "Modulo, Variavel, Comando"}, 1},
  {"O que e um banco de dados NoSQL?", {"Banco de Dados Sem Estrutura Relacional", "Banco de Dados Relacional", "Estrutura de Dados", "Sistema Operacional"}, 0},
  {"O que e uma fila?", {"Estrutura de Dados FIFO", "Estrutura de Dados LIFO", "Classe", "Operacao de Banco"}, 0},
  {"O que e uma pilha?", {"Estrutura de Dados LIFO", "Estrutura de Dados FIFO", "Funcao", "Classe"}, 0},
  {"O que e um arquivo binario?", {"Arquivo com dados não legíveis por humanos", "Arquivo de texto", "Comando de sistema", "Banco de Dados"}, 0},
  {"O que faz o comando 'chmod'?", {"Altera permissões de arquivos", "Altera o nome de arquivos", "Deleta arquivos", "Cria arquivos"}, 0},
  {"O que e uma chave primaria?", {"Identificador único de uma tabela", "Indice de uma tabela", "Variavel de classe", "Chave de criptografia"}, 0},
  {"O que e um join?", {"Operacao de união de tabelas", "Comando de banco de dados", "Chave de banco", "Operacao de sistema"}, 0},
  {"O que e um loop infinito?", {"Loop que nunca termina", "Loop com limite de repetições", "Loop que executa uma vez", "Loop com condição de parada"}, 0},
  {"O que significa o operador '<<'?", {"Deslocamento de bits para a esquerda", "Deslocamento de bits para a direita", "Comparacao", "Atribuicao de valor"}, 0},
  {"O que significa 'git clone'?", {"Cria uma copia de um repositório", "Deleta um repositório", "Atualiza um repositório", "Cria um commit"}, 0},
  {"O que e um debugger?", {"Ferramenta de analise de codigo", "Banco de dados", "Sistema operacional", "Bloco de codigo"}, 0},
{"O que faz a funcao strcmp?", {"Compara duas strings", "Compara dois numeros", "Inverte uma string", "Calcula o tamanho da string"}, 0},
{"O que e um buffer?", {"Espaco temporario de armazenamento", "Estrutura de dados", "Tipo de funcao", "Tipo de classe"}, 0},
{"O que e SSH?", {"Protocolo de comunicacao segura", "Banco de dados", "Comando de sistema", "Sistema operacional"}, 0},
{"O que faz o comando ls?", {"Lista arquivos e pastas", "Cria um arquivo", "Deleta uma pasta", "Exibe permissao de arquivo"}, 0},
{"O que e um array bidimensional?", {"Array com duas dimensoes", "Array com uma dimensao", "Bloco de codigo", "Tipo de variavel"}, 0},
{"O que e um endereco IP?", {"Endereco numerico unico de dispositivo", "Endereco de banco de dados", "Sistema de armazenamento", "Tipo de variavel"}, 0},
{"O que e o comando 'cd'?", {"Muda o diretorio atual", "Cria um diretorio", "Deleta um arquivo", "Exibe conteudo do diretorio"}, 0},
{"O que e um editor de texto?", {"Software para editar codigo", "Sistema de banco de dados", "Comando de sistema", "API de comunicacao"}, 0},
{"O que e um algoritmo de ordenacao?", {"Metodo para ordenar dados", "Estrutura de controle", "Comando de sistema", "Classe de objetos"}, 0},
{"O que faz a funcao scanf?", {"Le entrada do usuario", "Exibe dados", "Armazena variavel", "Compara valores"}, 0},
{"O que e RAM?", {"Memoria de acesso aleatorio", "Tipo de sistema", "Sistema de comunicacao", "Classe de objeto"}, 0},
{"O que e um sistema operacional?", {"Software que gerencia hardware", "Programa de banco de dados", "Estrutura de dados", "Bloco de codigo"}, 0},
{"O que e um codigo binario?", {"Representacao numerica em 0s e 1s", "Tipo de banco de dados", "Sistema de arquivos", "Tipo de variavel"}, 0},
{"O que e um namespace?", {"Espaco para organizacao de codigo", "Tipo de funcao", "Sistema operacional", "Classe de dados"}, 0},
{"O que e uma interface?", {"Contrato de metodos e propriedades", "Variavel global", "Tipo de funcao", "Bloco de repeticao"}, 0},
{"O que e um IDE?", {"Ambiente de desenvolvimento integrado", "Sistema de banco de dados", "Software de edicao de texto", "Tipo de classe"}, 0},
{"O que e UML?", {"Linguagem de modelagem unificada", "Sistema de comunicacao", "Tipo de banco de dados", "Classe de objetos"}, 0},
{"O que faz a funcao 'git status'?", {"Exibe estado atual do repositorio", "Cria um branch", "Deleta arquivos", "Une branches"}, 0},
{"O que e uma biblioteca de codigo?", {"Conjunto de codigos reutilizaveis", "Sistema de banco de dados", "Metodo de autenticacao", "Tipo de loop"}, 0},
{"O que e uma funcao de hash?", {"Funcao que gera valor unico para dados", "Funcao para executar loops", "Funcao de busca", "Funcao de comparacao"}, 0},
{"O que e uma chave estrangeira?", {"Referencia entre tabelas", "Identificador unico de tabela", "Classe de dados", "Metodo de ordenacao"}, 0},
{"O que e uma query?", {"Consulta ao banco de dados", "Tipo de variavel", "Estrutura de dados", "Bloco de codigo"}, 0},
{"O que e uma subconsulta?", {"Consulta dentro de outra consulta", "Tabela secundaria", "Banco de dados relacional", "Sistema de permissao"}, 0},
{"O que faz a funcao strcat?", {"Concatena duas strings", "Divide uma string", "Compara strings", "Inverte uma string"}, 0},
{"O que e um arquivo JSON?", {"Formato de dados em texto", "Arquivo binario", "Sistema de banco de dados", "Tipo de comando"}, 0},
{"O que e um byte?", {"Unidade basica de informacao", "Sistema de banco de dados", "Bloco de repeticao", "Comando de sistema"}, 0},
{"O que faz o comando mkdir?", {"Cria um diretorio", "Lista arquivos", "Deleta um arquivo", "Exibe permissoes"}, 0},
{"O que e um servidor web?", {"Software que entrega conteudo web", "Sistema de banco de dados", "Editor de texto", "Sistema de arquivos"}, 0},
{"O que e um banco de dados?", {"Sistema para armazenar dados", "Sistema de operacao", "Comando de sistema", "Classe de dados"}, 0},
{"O que e um metodo?", {"Funcao associada a um objeto", "Tipo de variavel", "Estrutura de controle", "Tipo de classe"}, 0},
{"O que e um repositório?", {"Armazenamento de codigo versionado", "Tipo de variavel", "Comando de sistema", "Classe de objeto"}, 0},
{"O que e autenticação?", {"Processo de verificacao de identidade", "Tipo de banco de dados", "Bloco de codigo", "Sistema operacional"}, 0},
{"O que e um DOM?", {"Modelo de objeto do documento", "Bloco de repeticao", "Tipo de variavel", "Metodo de ordenacao"}, 0},
{"O que e um arquivo XML?", {"Formato de dados estruturado", "Banco de dados relacional", "Sistema de arquivos", "Tipo de variavel"}, 0},
{"O que e um terminal?", {"Interface de linha de comando", "Sistema de banco de dados", "Classe de objetos", "Metodo de ordenacao"}, 0},
{"O que e um manipulador de eventos?", {"Codigo que responde a eventos", "Tipo de variavel", "Metodo de autenticação", "Bloco de codigo"}, 0},
{"O que e um processador?", {"Unidade central de processamento", "Tipo de sistema operacional", "Editor de texto", "Bloco de repeticao"}, 0},
{"O que e uma variavel local?", {"Variavel visivel so em uma funcao", "Variavel global", "Comando de sistema", "Tipo de funcao"}, 0},
{"O que e uma operacao logica?", {"Operacao entre valores booleanos", "Tipo de banco de dados", "Classe de objeto", "Tipo de variavel"}, 0},
{"O que e o protocolo FTP?", {"Protocolo de transferencia de arquivos", "Banco de dados", "Sistema operacional", "Estrutura de dados"}, 0},
{"O que e um fluxo de dados?", {"Sequencia de dados transmitidos", "Tipo de sistema", "Classe de objetos", "Metodo de ordenacao"}, 0},
{"O que e um widget?", {"Elemento de interface grafica", "Tipo de variavel", "Comando de sistema", "Classe de objeto"}, 0},
{"O que e uma classe abstrata?", {"Classe incompleta para heranca", "Classe completa", "Classe de dados", "Classe com metodos finais"}, 0},
{"O que e uma excecao?", {"Erro durante a execucao", "Erro de sintaxe", "Comando de sistema", "Metodo de autenticação"}, 0},
{"O que e uma macro?", {"Conjunto de instrucoes predefinidas", "Tipo de variavel", "Classe de dados", "Metodo de ordenacao"}, 0},
{"O que e o comando 'ps'?", {"Exibe processos em execucao", "Cria um processo", "Exclui um processo", "Modifica um processo"}, 0},
{"O que e um IP estatico?", {"Endereco IP fixo", "Endereco IP temporario", "Tipo de comando", "Classe de objeto"}, 0},
{"O que e uma topologia de rede?", {"Configuracao fisica da rede", "Classe de objetos", "Banco de dados", "Bloco de repeticao"}, 0},
{"O que e uma chave privada?", {"Codigo secreto para autenticacao", "Codigo publico de autenticacao", "Classe de dados", "Tipo de variavel"}, 0},
{"O que e um frontend?", {"Parte visivel da aplicacao", "Banco de dados", "Parte logica do sistema", "Bloco de codigo"}, 0},
{"O que e um backend?", {"Parte logica do sistema", "Parte visivel da aplicacao", "Banco de dados", "Tipo de variavel"}, 0},
{"O que faz a funcao free?", {"Libera memoria alocada", "Aloca memoria", "Executa loop", "Compara valores"}, 0},
{"O que e uma variavel global?", {"Variavel acessivel em todo o programa", "Variavel local", "Comando de sistema", "Ponteiro"}, 0},
{"O que e uma rede?", {"Conjunto de dispositivos interligados", "Banco de dados", "Classe de dados", "Bloco de repeticao"}, 0},
{"O que e uma API REST?", {"Interface para interacao com servicos", "Sistema de banco de dados", "Classe de objetos", "Metodo de ordenacao"}, 0},
{"O que e uma transacao?", {"Conjunto de operacoes no banco de dados", "Metodo de ordenacao", "Bloco de repeticao", "Classe de dados"}, 0},
{"O que e uma lista encadeada?", {"Estrutura onde cada elemento aponta para o proximo", "Estrutura de repeticao", "Classe de objetos", "Tipo de variavel"}, 0},
{"O que e um laço for?", {"Estrutura de repeticao com contador", "Classe de objetos", "Bloco de funcao", "Metodo de ordenacao"}, 0},
{"O que e um laço while?", {"Estrutura de repeticao condicional", "Classe de objetos", "Bloco de funcao", "Metodo de ordenacao"}, 0},
{"O que e a funcao strlen?", {"Calcula o comprimento da string", "Compara duas strings", "Inverte uma string", "Divide uma string"}, 0},
{"O que faz o operador modulo?", {"Retorna o resto da divisao", "Realiza divisao inteira", "Multiplica dois numeros", "Inverte um valor"}, 0},
{"O que e uma estrutura condicional?", {"Instrucao que altera fluxo com base em condicao", "Classe de dados", "Metodo de ordenacao", "Ponteiro"}, 0},
{"O que e uma chave primaria?", {"Identificador unico de tabela", "Referencia a outra tabela", "Classe de dados", "Bloco de repeticao"}, 0},
{"O que e um switch case?", {"Estrutura condicional para varios casos", "Bloco de repeticao", "Tipo de variavel", "Metodo de ordenacao"}, 0},
{"O que e uma biblioteca dinamica?", {"Biblioteca carregada em tempo de execucao", "Biblioteca estatica", "Classe de dados", "Metodo de ordenacao"}, 0},
{"O que e uma tabela hash?", {"Estrutura para armazenamento com chaves unicas", "Classe de objetos", "Metodo de ordenacao", "Bloco de repeticao"}, 0},
{"O que e a estrutura de dados pilha?", {"Estrutura que segue LIFO", "Estrutura que segue FIFO", "Classe de dados", "Bloco de repeticao"}, 0},
{"O que e a estrutura de dados fila?", {"Estrutura que segue FIFO", "Estrutura que segue LIFO", "Classe de dados", "Bloco de repeticao"}, 0},
{"O que e uma variavel estatica?", {"Variavel que retém valor entre execucoes", "Variavel de classe", "Tipo de variavel", "Metodo de ordenacao"}, 0},
{"O que e um arquivo executavel?", {"Arquivo que pode ser executado", "Arquivo de texto", "Banco de dados", "Classe de dados"}, 0},
{"O que e um sistema de arquivos?", {"Estrutura que organiza arquivos", "Sistema de banco de dados", "Bloco de codigo", "Classe de dados"}, 0},
{"O que e um worm?", {"Malware que se replica sozinho", "Software de seguranca", "Bloco de codigo", "Sistema de banco de dados"}, 0},
{"O que e o ciclo de vida de um software?", {"Etapas de desenvolvimento e manutencao", "Processo de ordenacao", "Tipo de variavel", "Bloco de repeticao"}, 0},
{"O que e o comando 'man'?", {"Mostra o manual de comandos", "Deleta arquivo", "Cria diretorio", "Muda permissoes"}, 0},
{"O que e uma vulnerabilidade?", {"Falha de seguranca em software", "Classe de dados", "Tipo de variavel", "Bloco de codigo"}, 0},
{"O que e uma VPN?", {"Rede privada virtual", "Banco de dados", "Comando de sistema", "Classe de dados"}, 0},
{"O que e uma variavel temporaria?", {"Variavel usada por curto periodo", "Variavel global", "Classe de dados", "Tipo de repeticao"}, 0},
{"O que e um pacote?", {"Conjunto de arquivos de software", "Banco de dados", "Comando de sistema", "Classe de dados"}, 0},
{"O que e um hotfix?", {"Correção rapida para erro critico", "Tipo de variavel", "Bloco de codigo", "Classe de objeto"}, 0},
{"O que e um backup?", {"Copia de seguranca de dados", "Banco de dados", "Classe de objeto", "Comando de sistema"}, 0},
{"O que é um IDE?", {"Ambiente de Desenvolvimento Integrado", "Sistema Operacional", "Banco de Dados", "Classe de dados"}, 0},
{"O que é um sistema operacional?", {"Software que gerencia hardware e software", "Banco de dados", "Aplicativo de edição de texto", "Bloco de código"}, 0},
{"O que é a linguagem HTML?", {"Linguagem de marcação para páginas web", "Linguagem de programação", "Banco de dados", "Sistema de arquivos"}, 0},
{"O que é um banco de dados relacional?", {"Banco de dados que usa tabelas para organizar dados", "Sistema de arquivos", "Bloco de código", "Classe de objetos"}, 0},
{"O que é SQL?", {"Linguagem de consulta para bancos de dados", "Linguagem de programação web", "Bloco de código", "Sistema operacional"}, 0},
{"O que é uma função recursiva?", {"Função que chama a si mesma", "Função que organiza dados", "Função que cria uma interface", "Função de banco de dados"}, 0},
{"O que é um firewall?", {"Sistema de segurança que monitora o tráfego de rede", "Sistema operacional", "Banco de dados", "Editor de código"}, 0},
{"O que é uma rede LAN?", {"Rede Local", "Rede de área ampla", "Banco de dados", "Bloco de código"}, 0},
{"O que é um servidor?", {"Computador que fornece serviços a outros computadores", "Software de segurança", "Bloco de código", "Classe de dados"}, 0},
{"O que é um algoritmo?", {"Sequência de instruções para resolver um problema", "Classe de dados", "Sistema de arquivos", "Bloco de código"}, 0},
{"O que é uma variável?", {"Espaço de memória para armazenar valores", "Função de banco de dados", "Sistema operacional", "Tipo de rede"}, 0},
{"O que é um loop infinito?", {"Laço de repetição que nunca termina", "Função que se auto-referencia", "Sistema de segurança", "Classe de dados"}, 0},
{"O que é um compilador?", {"Software que traduz código fonte para código máquina", "Banco de dados", "Sistema de arquivos", "Classe de objetos"}, 0},
{"O que é JavaScript?", {"Linguagem de programação para a web", "Banco de dados", "Sistema operacional", "Bloco de repetição"}, 0},
{"O que é um navegador web?", {"Software para acessar páginas da internet", "Banco de dados", "Sistema de arquivos", "Editor de código"}, 0},
{"O que é JSON?", {"Formato leve para troca de dados", "Sistema operacional", "Linguagem de marcação", "Editor de texto"}, 0},
{"O que é um IP?", {"Endereço único de um dispositivo na rede", "Banco de dados", "Classe de objeto", "Função matemática"}, 0},
{"O que é um DNS?", {"Sistema que traduz nomes de domínio para IP", "Banco de dados", "Classe de objeto", "Sistema de arquivos"}, 0},
{"O que é HTTPS?", {"Protocolo seguro de transferência de hipertexto", "Linguagem de programação", "Banco de dados", "Sistema operacional"}, 0},
{"O que é uma tag HTML?", {"Elemento usado para definir o layout da página", "Classe de dados", "Bloco de repetição", "Banco de dados"}, 0},
{"O que é CSS?", {"Linguagem para estilizar páginas web", "Sistema operacional", "Banco de dados", "Editor de código"}, 0},
{"O que é um framework?", {"Conjunto de ferramentas e bibliotecas para desenvolvimento", "Classe de objetos", "Sistema de arquivos", "Bloco de repetição"}, 0},
{"O que é REST?", {"Estilo de arquitetura para web services", "Banco de dados", "Editor de código", "Sistema operacional"}, 0},
{"O que é um commit no Git?", {"Salvar uma mudança no repositório", "Apagar um arquivo", "Mover um arquivo", "Editor de código"}, 0},
{"O que é o GitHub?", {"Plataforma para hospedagem de código", "Linguagem de programação", "Banco de dados", "Bloco de repetição"}, 0},
{"O que é o Docker?", {"Ferramenta para criar e gerenciar contêineres", "Banco de dados", "Sistema operacional", "Bloco de repetição"}, 0},
{"O que é machine learning?", {"Treinamento de máquinas para executar tarefas", "Banco de dados", "Sistema operacional", "Editor de código"}, 0},
{"O que é um array?", {"Estrutura de dados que armazena múltiplos valores", "Bloco de repetição", "Sistema de arquivos", "Classe de objetos"}, 0},
{"O que é uma chave estrangeira?", {"Campo que referencia uma chave primária em outra tabela", "Banco de dados", "Bloco de repetição", "Classe de dados"}, 0},
{"O que é um design responsivo?", {"Design que se adapta a diferentes tamanhos de tela", "Banco de dados", "Editor de código", "Sistema operacional"}, 0},
{"O que é CSS Flexbox?", {"Modelo de layout para distribuir espaço", "Sistema de arquivos", "Bloco de repetição", "Banco de dados"}, 0},
{"O que é um objeto?", {"Entidade com propriedades e métodos", "Bloco de repetição", "Banco de dados", "Classe de dados"}, 0},
{"O que é uma função anônima?", {"Função sem nome", "Função que se auto-referencia", "Função com parâmetros fixos", "Bloco de código"}, 0},
{"O que é um prompt em programação?", {"Solicitação de entrada do usuário", "Saída de dados", "Banco de dados", "Sistema de arquivos"}, 0},
{"O que é virtualização?", {"Execução de múltiplos sistemas operacionais em uma máquina", "Banco de dados", "Editor de código", "Sistema de arquivos"}, 0},
{"O que é uma chave composta?", {"Chave que combina dois ou mais campos", "Classe de dados", "Banco de dados", "Bloco de repetição"}, 0},
{"O que é uma estrutura de repetição 'for each'?", {"Laço que percorre cada item de uma coleção", "Classe de dados", "Banco de dados", "Editor de código"}, 0},
{"O que é um paradigma de programação?", {"Estilo de programação que define a estrutura de um programa", "Bloco de repetição", "Banco de dados", "Sistema de arquivos"}, 0},
{"O que é SQL Injection?", {"Ataque que explora falhas em consultas SQL", "Bloco de repetição", "Classe de dados", "Sistema de arquivos"}, 0},
{"O que é um cache?", {"Área de armazenamento temporário", "Classe de dados", "Sistema de arquivos", "Banco de dados"}, 0},
{"O que é o Java Virtual Machine (JVM)?", {"Máquina virtual que executa código Java", "Banco de dados", "Classe de dados", "Sistema de arquivos"}, 0},
{"O que é um vetor?", {"Estrutura de dados unidimensional", "Banco de dados", "Bloco de repetição", "Classe de dados"}, 0},
{"O que é um servidor de arquivos?", {"Servidor que armazena e compartilha arquivos", "Banco de dados", "Bloco de repetição", "Editor de código"}, 0},
{"O que é um compilador Just-In-Time (JIT)?", {"Compilador que traduz código durante a execução", "Banco de dados", "Classe de objetos", "Sistema de arquivos"}, 0},
{"O que é um namespace?", {"Espaço para organizar nomes de variáveis e funções", "Banco de dados", "Sistema operacional", "Editor de código"}, 0},
{"O que é um modelo de dados?", {"Representação estrutural de dados", "Classe de objetos", "Banco de dados", "Bloco de repetição"}, 0},
{"O que é um código fonte?", {"Código escrito por programadores antes da compilação", "Classe de objetos", "Banco de dados", "Sistema de arquivos"}, 0},
{"O que é um sistema distribuído?", {"Sistema com múltiplos componentes em redes diferentes", "Banco de dados", "Classe de objetos", "Editor de código"}, 0},
{"O que é um singleton?", {"Padrão de design para criar uma única instância de classe", "Bloco de repetição", "Banco de dados", "Classe de objetos"}, 0},
{"O que é um frontend?", {"Parte do software que interage com o usuário", "Banco de dados", "Classe de dados", "Editor de código"}, 0},
{"O que é um backend?", {"Parte do software que executa lógica e processa dados", "Bloco de repetição", "Banco de dados", "Editor de código"}, 0},
{"O que é um cookie?", {"Pequeno arquivo com informações de usuário", "Banco de dados", "Classe de objetos", "Sistema operacional"}, 0},
{"O que é uma aplicação monolítica?", {"Aplicação em que todos os componentes são unidos", "Sistema de arquivos", "Banco de dados", "Bloco de código"}, 0},
{"O que é uma aplicação microservices?", {"Aplicação composta por pequenos serviços independentes", "Classe de dados", "Banco de dados", "Editor de código"}, 0},
{"O que é uma API?", {"Interface para comunicação entre softwares", "Banco de dados", "Sistema operacional", "Classe de objetos"}, 0},
{"O que significa CPU?", {"Unidade Central de Processamento", "Banco de dados", "Classe de dados", "Editor de código"}, 0},
{"O que é um QR Code?", {"Código de barras bidimensional", "Banco de dados", "Sistema de arquivos", "Editor de código"}, 0},
{"O que é um SSD?", {"Unidade de armazenamento em estado sólido", "Banco de dados", "Bloco de repetição", "Classe de dados"}, 0},
{"O que é um DNS Spoofing?", {"Ataque que altera registros DNS", "Banco de dados", "Classe de dados", "Bloco de repetição"}, 0},
{"O que é um Trojan?", {"Software malicioso disfarçado de legítimo", "Banco de dados", "Classe de dados", "Editor de código"}, 0},
{"O que é a criptografia?", {"Processo de codificar informações", "Banco de dados", "Sistema de arquivos", "Editor de código"}, 0},
{"O que é a inteligência artificial?", {"Sistemas que simulam a inteligência humana", "Banco de dados", "Classe de dados", "Bloco de repetição"}, 0},
{"O que é um ataque DDoS?", {"Ataque de negação de serviço distribuído", "Classe de dados", "Banco de dados", "Editor de código"}, 0},
{"O que é a computação em nuvem?", {"Entrega de serviços de computação pela internet", "Bloco de repetição", "Classe de objetos", "Sistema operacional"}, 0},
{"O que é o Git?", {"Sistema de controle de versão distribuído", "Classe de dados", "Banco de dados", "Bloco de repetição"}, 0},
{"O que é o Git Commit?", {"Salvar mudanças no repositório local", "Banco de dados", "Classe de objetos", "Sistema de arquivos"}, 0},
{"O que é um IP dinâmico?", {"Endereço IP que muda periodicamente", "Banco de dados", "Classe de objetos", "Editor de código"}, 0},
{"O que é um loop 'while'?", {"Estrutura de repetição que continua enquanto condição for verdadeira", "Banco de dados", "Classe de objetos", "Bloco de repetição"}, 0},
{"O que é Big Data?", {"Grande volume de dados que não pode ser tratado por métodos tradicionais", "Banco de dados", "Classe de objetos", "Sistema operacional"}, 0},
{"O que é um editor de texto?", {"Software para escrever e editar texto", "Banco de dados", "Classe de dados", "Bloco de repetição"}, 0},
{"O que é XML?", {"Linguagem de marcação para estrutura de dados", "Banco de dados", "Classe de objetos", "Sistema operacional"}, 0},
{"O que é a codificação binária?", {"Sistema numérico com 0 e 1", "Banco de dados", "Classe de dados", "Bloco de repetição"}, 0},
{"O que é SSH?", {"Protocolo de acesso seguro a dispositivos remotos", "Banco de dados", "Classe de dados", "Editor de código"}, 0},
{"O que é um CMS?", {"Sistema de Gerenciamento de Conteúdo", "Banco de dados", "Classe de objetos", "Bloco de repetição"}, 0},
{"O que é um código de barras?", {"Representação gráfica de dados legíveis por máquinas", "Classe de objetos", "Banco de dados", "Sistema operacional"}, 0},
{"O que é um container?", {"Ambiente isolado para execução de aplicações", "Banco de dados", "Bloco de repetição", "Classe de objetos"}, 0},
{"O que é uma biblioteca em programação?", {"Coleção de código reutilizável", "Banco de dados", "Classe de dados", "Editor de código"}, 0},
{"O que é uma variável de ambiente?", {"Valor configurável acessível pelo sistema e aplicações", "Bloco de repetição", "Classe de dados", "Editor de código"}, 0},
{"O que é o padrão MVC?", {"Modelo de desenvolvimento Model-View-Controller", "Banco de dados", "Classe de dados", "Editor de código"}, 0},
{"O que é um 'framework web'?", {"Estrutura para o desenvolvimento de aplicações web", "Banco de dados", "Classe de dados", "Bloco de repetição"}, 0},
{"O que é a indexação de um banco de dados?", {"Organização para melhorar a velocidade das consultas", "Classe de dados", "Bloco de repetição", "Editor de código"}, 0},
{"O que é um teste unitário?", {"Teste que verifica o funcionamento de uma pequena parte do código", "Classe de objetos", "Banco de dados", "Editor de código"}, 0},
{"O que é escalabilidade?", {"Capacidade de um sistema de crescer sem perder eficiência", "Classe de dados", "Bloco de repetição", "Editor de código"}, 0},
{"O que é criptografia simétrica?", {"Criptografia que usa a mesma chave para cifrar e decifrar", "Classe de dados", "Banco de dados", "Sistema operacional"}, 0},
{"O que é um 'token' em programação?", {"Unidade lexical do código-fonte", "Classe de dados", "Banco de dados", "Sistema de arquivos"}, 0},
{"O que é o Vue.js?", {"Framework JavaScript para interfaces de usuário", "Classe de dados", "Banco de dados", "Bloco de repetição"}, 0},
{"O que é um template?", {"Modelo para criar estruturas semelhantes", "Classe de dados", "Banco de dados", "Editor de código"}, 0},
{"O que é Open Source?", {"Software com código aberto ao público", "Banco de dados", "Classe de objetos", "Editor de código"}, 0},
{"O que é um breakpoint?", {"Ponto de interrupção no código para depuração", "Classe de dados", "Bloco de repetição", "Banco de dados"}, 0},
{"O que é autenticação?", {"Processo de verificação de identidade", "Classe de objetos", "Banco de dados", "Bloco de repetição"}, 0},
{"O que é autorização?", {"Processo de concessão de permissões", "Classe de dados", "Banco de dados", "Bloco de repetição"}, 0},
{"O que é uma string?", {"Sequência de caracteres", "Classe de objetos", "Banco de dados", "Editor de código"}, 0},
{"O que é um código fonte aberto?", {"Código disponível para visualização e modificação", "Banco de dados", "Classe de dados", "Bloco de repetição"}, 0},
{"O que é um fork?", {"Cópia de um repositório para um novo projeto", "Classe de dados", "Banco de dados", "Editor de código"}, 0},
{"O que é um pull request?", {"Pedido para incluir alterações em um repositório", "Classe de objetos", "Banco de dados", "Sistema operacional"}, 0},
{"O que é uma solicitação POST?", {"Método de envio de dados ao servidor em HTTP", "Banco de dados", "Classe de dados", "Editor de código"}, 0},
{"O que é um booleano?", {"Tipo de dado com valores verdadeiro ou falso", "Classe de dados", "Banco de dados", "Bloco de repetição"}, 0},
{"O que é um objeto JSON?", {"Formato de dados baseado em pares chave-valor", "Banco de dados", "Classe de dados", "Sistema de arquivos"}, 0},
{"O que é um 'proxy'?", {"Intermediário entre cliente e servidor", "Classe de dados", "Banco de dados", "Bloco de repetição"}, 0},
{"O que é refatoração?", {"Melhorar a estrutura do código sem alterar o funcionamento", "Banco de dados", "Classe de dados", "Bloco de repetição"}, 0},
{"O que é um token JWT?", {"JSON Web Token para autenticação", "Classe de objetos", "Banco de dados", "Editor de código"}, 0},
{"O que é uma função 'pure function'?", {"Função que retorna sempre o mesmo resultado para os mesmos argumentos", "Classe de dados", "Banco de dados", "Editor de código"}, 0},
{"O que é um valor 'hard-coded'?", {"Valor fixo embutido no código-fonte", "Classe de objetos", "Banco de dados", "Bloco de repetição"}, 0},
{"O que é o protocolo FTP?", {"Protocolo de transferência de arquivos", "Classe de dados", "Banco de dados", "Sistema de arquivos"}, 0},
{"O que é a metodologia Scrum?", {"Estrutura ágil para gestão de projetos", "Classe de dados", "Banco de dados", "Bloco de repetição"}, 0},
{"O que é um Scrum Master?", {"Facilitador no processo de desenvolvimento Scrum", "Classe de dados", "Banco de dados", "Sistema operacional"}, 0},
{"O que é um DevOps?", {"Conjunto de práticas para integração e entrega contínua", "Classe de dados", "Banco de dados", "Bloco de repetição"}, 0},
{"O que é versionamento semântico?", {"Sistema de versão que indica alterações e compatibilidade", "Banco de dados", "Classe de dados", "Editor de código"}, 0},
{"O que é um 'host'?", {"Computador ou dispositivo em uma rede", "Classe de dados", "Banco de dados", "Sistema operacional"}, 0},
{"O que é uma função de hash?", {"Função que transforma dados em um valor de tamanho fixo", "Classe de dados", "Banco de dados", "Bloco de repetição"}, 0},
{"O que é 'binding' em programação?", {"Associação de uma variável a um valor ou função", "Banco de dados", "Classe de dados", "Bloco de repetição"}, 0},
{"O que é um 'cookie'?", {"Pequeno arquivo de dados armazenado pelo navegador", "Classe de dados", "Banco de dados", "Editor de código"}, 0},
{"O que é HTTPS?", {"Protocolo HTTP seguro com criptografia SSL/TLS", "Banco de dados", "Classe de dados", "Bloco de repetição"}, 0},
{"O que é IoT?", {"Internet das Coisas, interconexão de dispositivos", "Classe de dados", "Banco de dados", "Sistema operacional"}, 0},
{"O que é 'overhead'?", {"Sobrecarga que diminui a eficiência do sistema", "Classe de objetos", "Banco de dados", "Bloco de repetição"}, 0},
{"O que é 'caching'?", {"Armazenamento temporário de dados para acesso rápido", "Classe de dados", "Banco de dados", "Bloco de repetição"}, 0},
{"O que é um 'namespace'?", {"Espaço que agrupa identificadores únicos", "Classe de dados", "Banco de dados", "Sistema operacional"}, 0},
{"O que é a lógica de programação?", {"Conjunto de instruções e regras para resolver problemas com código", "Classe de dados", "Banco de dados", "Editor de código"}, 0},
{"O que é um 'viewport'?", {"Área visível de um site em uma tela", "Classe de dados", "Banco de dados", "Bloco de repetição"}, 0},
{"O que é uma sub-rede?", {"Divisão de uma rede maior em redes menores", "Classe de dados", "Banco de dados", "Bloco de repetição"}, 0},
{"O que é SQL?", {"Linguagem de consulta estruturada", "Classe de dados", "Banco de dados", "Editor de código"}, 0},
{"O que é HTTP?", {"Protocolo de transferência de hipertexto", "Classe de dados", "Banco de dados", "Editor de código"}, 0},
{"O que é renderização?", {"Processo de geração de imagem ou interface a partir de dados", "Banco de dados", "Classe de dados", "Editor de código"}, 0},
{"O que é memoização em programação?", {"Técnica de otimização que armazena os resultados de funções para evitar cálculos repetidos", "Método de compactação de dados", "Algoritmo de busca em largura", "Estrutura de dados dinâmica"}, 0},
{"Qual a complexidade temporal média da busca binária?", {"O(log n)", "O(n^2)", "O(1)", "O(n)"}, 0},
{"Em algoritmos de grafos, o que é um 'ciclo Hamiltoniano'?", {"Um ciclo que visita cada vértice exatamente uma vez", "Caminho que cobre todas as arestas do grafo", "Caminho de menor custo em um grafo ponderado", "Conjunto de ciclos mínimos"}, 0},
{"O que é um compilador Just-In-Time (JIT)?", {"Compilador que converte código em tempo de execução", "Compilador que gera código binário antes da execução", "Compilador que transforma bytecode em assembly", "Compilador para pré-processamento de dados"}, 0},
{"Qual é a função de uma árvore AVL?", {"Manter o balanceamento de uma árvore binária de busca", "Ordenar elementos em O(log n)", "Armazenar dados em uma estrutura de hash", "Implementar busca de caminhos em grafos"}, 0},
{"Qual é a função da normalização de um banco de dados?", {"Reduzir a redundância de dados e melhorar a consistência", "Aumentar a velocidade das consultas", "Dividir tabelas para otimizar consultas", "Armazenar dados não estruturados"}, 0},
{"O que é polimorfismo em orientação a objetos?", {"Capacidade de objetos assumirem várias formas", "Propriedade de encapsulamento de dados", "Técnica de reutilização de código", "Processo de herança de classes"}, 0},
{"Qual a função do protocolo OAuth?", {"Autenticação para acesso a recursos de terceiros sem expor credenciais", "Proteger comunicações entre dispositivos de rede", "Definir regras de firewall", "Compactar dados durante a transmissão"}, 0},
{"Qual é a função de um 'heap' em estruturas de dados?", {"Manter a estrutura de uma árvore binária parcialmente ordenada", "Implementar uma lista de prioridade", "Representar grafos com menor complexidade", "Armazenar dados temporários para cálculos"}, 0},
{"Em qual contexto é usada a técnica de 'backtracking'?", {"Exploração de soluções possíveis para problemas de decisão", "Compactação de dados em rede", "Validação de transações em blockchain", "Ordenação de dados em memória"}, 0},
{"Qual é a complexidade de tempo do algoritmo de ordenação 'Merge Sort'?", {"O(n log n)", "O(n^2)", "O(n)", "O(log n)"}, 0},
{"O que é o protocolo WebSocket?", {"Protocolo para comunicação bidirecional em tempo real entre cliente e servidor", "Protocolo de camada de aplicação para requisições HTTP", "Protocolo de transferência de arquivos", "Protocolo para criptografia de dados"}, 0},
{"O que é o padrão de projeto Singleton?", {"Garante que uma classe tenha apenas uma instância global", "Define uma interface para criar objetos sem especificar a classe exata", "Separa a construção de um objeto da sua representação", "Permite que diferentes classes implementem a mesma interface"}, 0},
{"Qual é a função de um 'semaphore' em sistemas operacionais?", {"Controlar o acesso a recursos compartilhados por múltiplas threads", "Executar processos em paralelo", "Aumentar a prioridade de uma thread específica", "Monitorar a memória utilizada por processos"}, 0},
{"O que é a técnica 'hashing' em estruturas de dados?", {"Mapear dados de tamanho variável para valores de tamanho fixo", "Organizar dados em uma lista encadeada", "Acessar dados em tempo constante", "Proteger dados em uma rede privada"}, 0},
{"Qual é a função da 'indexação invertida' em mecanismos de busca?", {"Permitir busca rápida de palavras em documentos", "Comprimir dados para armazenamento eficiente", "Classificar documentos em categorias", "Mapear dados em uma tabela hash"}, 0},
{"O que é um 'promise' em JavaScript?", {"Objeto que representa um valor que estará disponível no futuro", "Função para manipular o DOM", "API para manipulação de cookies", "Estrutura de dados para armazenar eventos"}, 0},
{"O que é uma 'Race Condition' em sistemas?", {"Condição onde múltiplas threads acessam recursos compartilhados sem sincronização adequada", "Processo de aumento de prioridade de uma thread", "Situação onde um processo de memória se torna inacessível", "Execução de comandos em tempo real"}, 0},
{"Qual é o objetivo do design responsivo?", {"Adaptar a interface de um site para diferentes tamanhos de tela", "Melhorar a segurança do site contra ataques", "Aumentar a velocidade de carregamento da página", "Permitir armazenamento local de dados"}, 0},
{"O que é a arquitetura de microsserviços?", {"Dividir uma aplicação em serviços pequenos e independentes", "Centralizar dados em um servidor único", "Usar apenas servidores com Linux", "Reduzir a latência de redes locais"}, 0},
{"Qual é a principal diferença entre TCP e UDP?", {"TCP é orientado à conexão e UDP não é", "UDP oferece mais segurança que TCP", "TCP não garante entrega de pacotes", "UDP é mais lento que TCP"}, 0},
{"O que significa uma 'chave primária composta'?", {"Chave que usa mais de um campo para garantir unicidade", "Chave única gerada automaticamente", "Chave usada apenas para indexação", "Chave que se repete em outras tabelas"}, 0},
{"Qual é o objetivo do algoritmo de Floyd-Warshall?", {"Encontrar o caminho mais curto entre todos os pares de vértices", "Determinar o ciclo mais longo em um grafo", "Ordenar os vértices de um grafo", "Identificar componentes fortemente conectados"}, 0},
{"O que significa 'AJAX'?", {"JavaScript assíncrono e XML", "Algoritmo JavaScript avançado", "API de autenticação para web", "Rede de armazenamento de dados XML"}, 0},
{"O que significa JSON?", {"JavaScript Object Notation", "JavaScript Open Network", "JavaScript Optimization Nullifier", "JavaScript Object Navigation"}, 0},
{"Qual a diferença entre pilha e fila?", {"Pilha é LIFO e fila é FIFO", "Fila é LIFO e pilha é FIFO", "Ambas são LIFO", "Ambas são FIFO"}, 0},
{"O que é um Webhook?", {"Método para receber dados automaticamente quando ocorrem eventos", "API para enviar dados a um servidor", "Função JavaScript para modificar o DOM", "Script para executar tarefas de rede"}, 0},
{"O que é uma vulnerabilidade 'Cross-Site Scripting' (XSS)?", {"Injeção de scripts maliciosos em um site", "Ataque que intercepta cookies de usuários", "Exploração de falhas de autenticação", "Acesso não autorizado ao servidor"}, 0},
{"O que é a técnica 'Minimax'?", {"Algoritmo para tomada de decisão em jogos de dois jogadores", "Método para ordenação de dados", "Estrutura de dados para listas encadeadas", "Técnica para compactação de dados"}, 0},
{"Qual a função do protocolo ARP?", {"Resolver endereços IP para endereços MAC", "Definir rotas para redes externas", "Controlar acesso a redes públicas", "Compactar dados de rede"}, 0},
{"Qual é a função de uma 'hash table'?", {"Armazenar dados associados a uma chave para acesso rápido", "Manter uma lista de dados ordenada", "Proteger dados durante a transferência", "Agrupar dados por categorias"}, 0},
{"Qual é o papel do DNS em redes?", {"Mapear nomes de domínio para endereços IP", "Verificar pacotes para segurança de rede", "Definir regras de firewall", "Controlar a largura de banda"}, 0},
{"O que é um 'pipeline' em desenvolvimento de software?", {"Sequência de etapas automáticas para entrega de software", "Ferramenta de compactação de código", "Método para analisar segurança de pacotes", "Protocolo de transferência de dados"}, 0},
{"O que é um 'pointer' em C++?", {"Variável que armazena o endereço de memória de outra variável", "Variável de dados constante", "Classe que contém métodos de ponteiro", "Função para incrementar variáveis"}, 0},
{"O que é 'polimorfismo'?", {"Capacidade de um objeto assumir várias formas", "Transformação de classes em objetos", "Forma de encapsulamento", "Separação de dados em módulos"}, 0},
{"Em redes, qual é o objetivo de um switch?", {"Distribuir pacotes entre dispositivos na mesma rede", "Conectar redes distintas", "Resolver nomes de domínio", "Bloquear acesso a portas específicas"}, 0},
{"O que é um 'deadlock' em sistemas operacionais?", {"Situação onde processos ficam presos aguardando recursos", "Bloqueio temporário de um arquivo", "Acesso simultâneo a uma variável", "Execução contínua de um loop"}, 0},
{"O que é SQL Injection?", {"Injeção de código SQL malicioso em consultas de banco de dados", "Erro ao compilar um código", "Problema na renderização de páginas", "Interrupção de serviços de rede"}, 0},
{"O que é uma API REST?", {"Interface de comunicação que segue princípios RESTful", "Script de autenticação", "Protocolo de rede", "Banco de dados estruturado"}, 0}
};

// Função para mostrar a pergunta e alternativas na tela
void exibirPergunta() {
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);

  // Exibe a pergunta
  tft.drawCentreString(perguntas[perguntaAtual].texto, 161, 56, FONT_SIZE);

  // Exibe as alternativas com mais espaço
  tft.drawCentreString(perguntas[perguntaAtual].alternativas[0], 160, 110, FONT_SIZE);
  tft.drawCentreString(perguntas[perguntaAtual].alternativas[1], 160, 145, FONT_SIZE);
  tft.drawCentreString(perguntas[perguntaAtual].alternativas[2], 160, 180, FONT_SIZE);
  tft.drawCentreString(perguntas[perguntaAtual].alternativas[3], 160, 215, FONT_SIZE);

  // Exibe a pontuação
  tft.drawString("Pontuacao: " + String(pontuacao), 200, 5, FONT_SIZE);
  tft.drawString("SHOW DO DEVAO ", 40, 5, FONT_SIZE);
}

// Função para verificar a resposta
void verificarResposta(int alternativaSelecionada) {
  if (alternativaSelecionada == perguntas[perguntaAtual].respostaCorreta) {
    pontuacao += 1000;  // Incrementa a pontuação se a resposta estiver correta
  } else {
    gameOver = true; // Define game over se a resposta estiver errada
  }
  
  if (!gameOver) {
    perguntaAtual = (perguntaAtual + 1) % (sizeof(perguntas) / sizeof(perguntas[0])); // Passa para a próxima pergunta
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

// Função para reiniciar o jogo
void reiniciarJogo() {
  pontuacao = 0;
  perguntaAtual = 0;
  gameOver = false;
  exibirPergunta();
}

void setup() {
  Serial.begin(115200);

  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  touchscreen.setRotation(1);

  tft.init();
  tft.setRotation(1);

  exibirPergunta();
}

void loop() {
  if (gameOver) {
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

