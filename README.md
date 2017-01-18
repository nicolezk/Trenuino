# Trenuino
Trabalho final da disciplina de Laboratório de Microcontroladores e Aplicações

## Proposta
O projeto a ser desenvolvido pelo grupo consiste em uma aplicação de um **controlador capaz de efetuar o mapeamento da planta de um cômodo residencial** (ou qualquer outro ambiente totalmente delimitado). 

### Material
 - microcontrolador Arduíno;
 - sensor ultrassônico de distância;
 - motor de passo;
 - ULN2003APG Darlington Array;
 - Fonte de energia (5V/3A).

### Funcionamento
O sensor ultrassônico, acoplado ao eixo do motor, será rotacionado em 360º, de modo que, a cada passo (estima-se um passo de aproximadamente 1,8º), serão efetuadas medições de distância. Dadas a medição média e o ângulo para cada passo, a aplicação será responsável por conectar todos os pontos obtidos e gerar uma imagem com a planta obtida. Por fim, será gerado um relatório sobre a **área** e o **perímetro** estimados do ambiente.

### TODO List
[X] Adicionar a API do sensor ultrassônico
[X] Adicionar a API do motor de passos
[] Salvar medidas em um arquivo .txt
[] Adicionar um botão para medir (e pausar) e implementar sua interrupção
[] Testar o sensor ultrassônico em cantos
[] Melhorar a estrutura física
[] Identificar a figura geométrica
[] Calcular a área
[] Gerar arquivo de saída final

## Grupo
Felipe Khoji Myose
João Victor Pacheco
Nicole Zafalon Kovacs
Vinicius Ito Nagura
