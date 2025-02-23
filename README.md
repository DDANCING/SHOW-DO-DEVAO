# Jogo Educacional com LVGL no ESP32

Este projeto apresenta um jogo educacional criado para rodar em um ESP32 com interface gráfica baseada na biblioteca LVGL (Light and Versatile Graphics Library). A proposta é incentivar o aprendizado por meio da interatividade, com perguntas e respostas que estimulam o desenvolvimento cognitivo e o aprendizado em uma experiência divertida e envolvente.

## Índice

1. [Objetivo](#objetivo)
2. [Justificativa](#justificativa)
3. [Ferramentas e Tecnologias](#ferramentas-e-tecnologias)
4. [Desenvolvimento do Projeto](#desenvolvimento-do-projeto)
5. [Impacto no Aprendizado](#impacto-no-aprendizado)
6. [Conclusão](#conclusão)
7. [Como Usar](#como-usar)

---

### Objetivo

O objetivo deste projeto é criar uma interface gráfica para o ESP32 que permita ao usuário interagir com um jogo educacional, respondendo perguntas e recebendo feedback visual e sonoro sobre seu desempenho. O jogo é finalizado quando uma resposta errada é escolhida, incentivando a concentração e a precisão na resposta correta.

### Justificativa

Com o uso de sistemas embarcados, a aplicação prática de interfaces gráficas permite que crianças e jovens se envolvam com jogos educacionais que estimulam o aprendizado em várias disciplinas, por meio de um formato interativo.

## Ferramentas e Tecnologias

### Hardware
- **ESP32**: Microcontrolador utilizado como plataforma para o jogo.
- **Case para Tela**: Proporciona estrutura para o display usado na interface gráfica.

### Software
- **Arduino IDE**: Ambiente de desenvolvimento para programação do ESP32.
- **LVGL**: Biblioteca gráfica utilizada para criar e gerenciar a interface do usuário.

## Desenvolvimento do Projeto

### Configuração do ESP32
O ESP32 é programado com lógica de jogo, na qual o usuário acumula pontos para cada resposta correta. Ao responder incorretamente, o jogo termina. Isso cria uma experiência de aprendizado com foco em precisão e conhecimento.

### Uso da LVGL
- **Interface Gráfica**: Criação da interface para que o usuário possa selecionar respostas e interagir com o jogo.
- **Botões de Resposta**: Permitem que o usuário escolha a resposta correta para cada pergunta.
- **Perguntas e Respostas**: As perguntas são apresentadas na tela junto com as respostas possíveis.
- **Navegação entre Telas**: Permite a transição entre perguntas e resultados.

## Impacto no Aprendizado

Este projeto proporciona benefícios educacionais, incluindo:
- **Desenvolvimento Mental**: Melhora habilidades de resolução de problemas, tomada de decisão e interpretação de informações.
- **Interatividade**: Jogos educacionais aumentam o envolvimento e a motivação, o que pode ser aplicado a várias áreas de ensino.

## Conclusão

- **Facilidade de Interação**: Busca-se aprimorar a interação entre o usuário e o jogo.
- **Inclusão e Acessibilidade**: Explorar maneiras de tornar o jogo acessível a pessoas com deficiência.
- **Potencial Educacional**: Reforçar o valor do projeto como ferramenta educacional.

---

## Como Usar

1. **Configuração do Hardware**: Conecte o ESP32 ao display e case apropriado.
2. **Upload do Código**: Use o Arduino IDE para fazer o upload do código no ESP32.
3. **Iniciar o Jogo**: Após o upload, o jogo será exibido no display. Utilize os botões para navegar e escolher as respostas.
4. **Interação**: Escolha uma resposta para cada pergunta. Se acertar, você ganha pontos; se errar, o jogo termina.
5. **Feedback Visual**: A interface fornecerá feedback sobre o desempenho, incentivando o aprendizado contínuo.

---

Este projeto ilustra a aplicação prática de jogos educacionais com ESP32 e LVGL, incentivando o aprendizado interativo e engajante.
