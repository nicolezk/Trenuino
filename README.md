# Trenuino
Trabalho final da disciplina de Laboratório de Microcontroladores e Aplicações

## Proposta
O projeto a ser desenvolvido pelo grupo consiste em uma aplicação de um **controlador capaz de efetuar o mapeamento da planta de um cômodo residencial** (ou qualquer outro ambiente totalmente delimitado). 

### Material
 - Microcontrolador Arduíno;
 - Sensor ultrassônico de distância;
 - Motor de passo;
 - ULN2003APG Darlington Array;
 - Fonte de energia (5V/3A);
 - Shield Ethernet/SD Card;
 - Cartão SD.
 
### Funcionamento
O sensor ultrassônico, acoplado ao eixo do motor, será rotacionado em 360º, de modo que, a cada passo (estima-se um passo de aproximadamente 1,8º), serão efetuadas medições de distância. Dadas a medição média e o ângulo para cada passo, a aplicação será responsável por conectar todos os pontos obtidos e gerar uma imagem com a planta obtida. Por fim, será gerado um relatório sobre a **área** e o **perímetro** estimados do ambiente.

### To do
- [x] Adicionar a API do sensor ultrassônico
- [x] Adicionar a API do motor de passos
- [x] Salvar medidas em um arquivo .txt
- [x] Adicionar botões para start/pause e stop, implementando suas interrupções
- [x] Testar o sensor ultrassônico em cantos
- [x] Melhorar a estrutura física
- [x] Calcular a área e perímetro
- [x] Gerar arquivo de saída final

## Grupo
 - [Felipe Khoji Myose](https://github.com/felipekhoji) - 611026
 - [João Victor Pacheco](https://github.com/jovictorp1) - 594970
 - [Nicole Zafalon Kovacs](https://github.com/nicolezk1) - 558419
 - [Vinicius Ito Nagura](https://github.com/viniciusnagura) - 558478
