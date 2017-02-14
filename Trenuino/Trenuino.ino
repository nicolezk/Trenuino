/* Trenuino
 * Projeto Desenvolvido por:
 *  Felipe Khoji Myose
 *  João Victor Pacheco
 *  Nicole Zafalon Kovacs
 *  Vinicius Ito Nagura
 *  
 * Referências:
 *  Motor:
 *    https://www.arduino.cc/en/Reference/Stepper
 *    https://www.arduino.cc/en/Tutorial/StepperOneRevolution
 *  Sensor ultrassônico:
 *    http://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/http://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/
 *  Cartão SD:
 *    https://www.arduino.cc/en/Main/ArduinoEthernetShield
 *    https://www.arduino.cc/en/Tutorial/ReadWrite
 *    https://www.arduino.cc/en/Tutorial/Files
 *  Funcão qsort()
 *    https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm
*/

#include <Stepper.h>
#include <SPI.h>
#include <SD.h>

#define nMedidas 5
#define stepsPerRevolution 200

#define PARADO        0
#define GIRANDO       1
#define PAUSADO       2
#define INTERROMPENDO 30


/* Cabeçalho */
void obterMedidas();
float medirDistancia(int trigPin, int echoPin);
//float media(float distanciasTemporarias[]);
float mediana(float distanciasTemporarias[]);
void escreveArquivo();

/*Variável para criar arquivo*/
File myFile;

/* Registro para medicoes */
struct Medida{
  float grau;
  float distancia;
};

/* Declaracao das variaveis globais */
Medida medidas[200];

/*  Variáveis de estado de execução (relacionado aos botões)
    Estado de Execucao 0: Parado (Stopped), 1: Executando (Started), 2: Pausado (Paused)
    Numero de Passos: conta quantos passos já foram dados
*/
int volatile estadoExecucao = 0;
int volatile numeroPassos = 0;

/* Parametros para API do sensor */
long duration;
float distance;

/* Pinos */
/* NÃO USAR PINOS 11, 12, 13 e 53 Pois são utilizados pelo shield. O 10 precisa ficar desativado em HIGH */

const int trigPin           = 22;
const int echoPin           = 24;
const int m1                = 31;
const int m2                = 33;
const int m3                = 35;
const int m4                = 37;
const int buttonStartPause  = 3;
const int buttonStop        = 2;

/* Inicializar a biblioteca do motor de passo */
Stepper myStepper(stepsPerRevolution, m1, m2, m3, m4);

// Interrupção para botão Start/Pause
void StartPause() {
  if(estadoExecucao == GIRANDO)
    estadoExecucao = 2;
  else
    if(estadoExecucao == PARADO || estadoExecucao == PAUSADO)
      estadoExecucao = 1;      
}

// Interrupção para botão Stop
void FuncaoParar() {
  if(estadoExecucao == GIRANDO || estadoExecucao == PAUSADO) {
    estadoExecucao = INTERROMPENDO;
  }
}

void setup() {
  pinMode(trigPin, OUTPUT); // Saida da onda
  pinMode(echoPin, INPUT);  // Retorno da onda

  myStepper.setSpeed(10);   // Setar a velocidade do motor para 10 rpm 
  
  Serial.begin(9600);       // Monitor Serial para acompanhar as medicoes 

  pinMode(buttonStartPause, INPUT_PULLUP); // Botão Start/Pause
  attachInterrupt(digitalPinToInterrupt(buttonStartPause), StartPause, LOW);

  pinMode(buttonStop, INPUT_PULLUP); // Botão Stop
  attachInterrupt(digitalPinToInterrupt(buttonStop), FuncaoParar, LOW);

   // Os Comandos a seguir são relativos a implementação da Shield de Cartão SD
  pinMode(53, OUTPUT); // Necessário ser setado esse pino para o funcionamento da shield 

  pinMode(10, OUTPUT); // Como não estamos usando ethernet é necessário desativar esta porta da Shield, explicitamente 
  digitalWrite(10, HIGH); // Para isso, deixe em alto para desativar

  while (!Serial) {
    ; // Espera a porta serial conectar. Necessario apenas para portas usb nativas
  }
  Serial.print("Inicializando cartao SD...");
  // Caso exista algum erro e o cartão SD não inicialize, escreve uma mensagem de erro no Monitor Serial
  if (!SD.begin(4)) {
    Serial.println("Inicializacao falhou!");
    return;
  }
  // Caso o cartão SD inicialize com sucesso, escreve uma mensagem de aviso no Monitor Serial.
  Serial.println("Inicializacao completa!");
  
}

void loop() {
  obterMedidas();
  escreveArquivo();
  delay(5000);
}

// Foi optado por não utilizar a função a seguir, pois segundo análise feita nos dados, a mediana geraria valores mais precisos.
/*
float media(float distanciasTemporarias[]) {
  float sum = 0;
  for(int j = 0; j < nMedidas; j++) {
    sum += distanciasTemporarias[j];
  }
  return (sum/nMedidas);
}
*/

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

//Função que calcula a mediana das distâncias obtidas em cada grau medido.
float mediana(float distanciasTemporarias[]) {
  float m;
  int aux[nMedidas];
  
  for(int i = 0; i < nMedidas; i++) {
    aux[i] = distanciasTemporarias[i] * 100;
  }
  
  qsort(aux, nMedidas, sizeof(aux[0]), cmpfunc);
  
  if(nMedidas % 2 == 1)
    m = aux[(nMedidas/2) + 1];
  else {
    m = (aux[(nMedidas/2)] + aux[(nMedidas/2) + 1]) / 2;
  }
      
  return m / 100;
}

void obterMedidas() {
  float distanciasTemporarias[nMedidas];
  
  /* Girar o motor de passo e medir a distância */
  while(numeroPassos < 200) {
    /* Se estiver no estado de GIRANDO, efetua as medições */
    if(estadoExecucao == GIRANDO) {
    
      for(int j = 0; j < nMedidas; j++) {
        distanciasTemporarias[j] = medirDistancia(trigPin, echoPin);
        delay(100);
      }
    
      medidas[numeroPassos].distancia = mediana(distanciasTemporarias);
      medidas[numeroPassos].grau = (numeroPassos) * 1.8;

      // Printa a medicao
      Serial.print("Grau: ");
      Serial.print(medidas[numeroPassos].grau);
      Serial.print(" e Medida: ");
      Serial.println(medidas[numeroPassos].distancia);

      myStepper.step(1);

      delay(50);

      numeroPassos++;
    }
    /* Se o botão STOP for pressionado, retorna para posição inicial e seta estado de PARADO e preenche com 0 as posições do vetor */
    else if (estadoExecucao == INTERROMPENDO) {
      myStepper.step((-1) * numeroPassos);
      for (int i = 0; i < stepsPerRevolution; i++) {
        medidas[i].grau = 0;
        medidas[i].distancia = 0;
      }
      numeroPassos = 0;
      estadoExecucao = 0;
    }
  }
  
  /* Para a execução e volta o motor para a posição inicial caso atinja 360 graus */
  estadoExecucao = PARADO;
  myStepper.step(-200);
  numeroPassos = 0;
}

float medirDistancia(int trigPin, int echoPin){
    // Reseta o trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Seta o trigPin para HIGH por 10 micro-segundos
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Lê o echoPin, retorna o travel time da onda sonora em micro-segundos
    duration = pulseIn(echoPin, HIGH);
    // Calcula a distância
    distance= duration*0.034/2;

    return distance;
}

void escreveArquivo(){ // Função que escreve no arquivo as medidas + virgula + graus:
    myFile = SD.open("medidas.csv", O_WRITE | O_CREAT | O_TRUNC); // Abre o arquivo medidas.csv
    if (myFile) { // Se conseguir abrir o arquivo medidas.csv:
      for (int i=0; i< 200; i++){ // loop que percorre as 200 posições do vetor e escreve no arquivo as medidas
         myFile.print(medidas[i].grau);
         myFile.print(",");
         myFile.println(medidas[i].distancia);
      }
      // Fechando o Arquivo:
      Serial.println("Arquivo salvo com sucesso!");
      myFile.close();
    } else {
        // Se o arquivo não abrir, printa uma mensagem de erro:
        Serial.println("Erro ao abrir arquivo");
      }
}

