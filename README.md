# Trenuino
Trabalho final da disciplina de Laboratório de Microcontroladores e Aplicações

## Proposta
O projeto a ser desenvolvido pelo grupo consiste em uma aplicação de um **controlador capaz de efetuar o mapeamento da planta de um cômodo residencial** (ou qualquer outro ambiente totalmente delimitado). 

### Material
 - microcontrolador Arduíno
 - sensor ultrassônico de distância
 - motor de passo. 

### Funcionamento
O sensor ultrassônico, acoplado ao eixo do motor, será rotacionado em 360º, de modo que, a cada passo (estima-se um passo de aproximadamente 0,088º), será efetuada medições de distância. Dadas a medição média e o ângulo para cada passo, a aplicação será responsável por conectar todos os pontos obtidos e gerar uma imagem com a planta obtida. Por fim, será gerado um relatório sobre a **área** e o **perímetro** estimados do ambiente.
