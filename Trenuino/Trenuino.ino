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
float media(float distanciasTemporarias[]);
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
  
  Serial.begin(9600);       // Serial para acompanhar as medicoes (sera excluida posteriormente)

  pinMode(buttonStartPause, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonStartPause), StartPause, LOW);

  pinMode(buttonStop, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonStop), FuncaoParar, LOW);

   // Cartão SD
  pinMode(53, OUTPUT); // Segundo Japa, por algum precisa colocar isso, senão não funciona shield (vimos numa das referencias)

  pinMode(10, OUTPUT); // Como não estamos usando ethernet é necessário desativar esta porta, explicitamente
  digitalWrite(10, HIGH); // Para isso, deixe em alto para desativar

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Inicializando cartao SD...");

  if (!SD.begin(4)) {
    Serial.println("Inicializacao falhou!");
    return;
  }
  Serial.println("Inicializacao completa!");
  
}

void loop() {
  obterMedidas();
  escreveArquivo();
  delay(5000);
}

float media(float distanciasTemporarias[]) {
  float sum = 0;
  for(int j = 0; j < nMedidas; j++) {
    sum += distanciasTemporarias[j];
  }
  return (sum/nMedidas);
}

void obterMedidas() {
  float distanciasTemporarias[nMedidas];
  
  /* Girar o motor de passo e medir a distância */
  while(numeroPassos < 200) {
    /* Se estiver no estado de GIRANDO, efetua as medições */
    if(estadoExecucao == GIRANDO) {
    
      for(int j = 0; j < nMedidas; j++) {
        distanciasTemporarias[j] = medirDistancia(trigPin, echoPin);
        delay(50);
      }
    
      medidas[numeroPassos].distancia = media(distanciasTemporarias);
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
    /* Se o botão STOP for pressionado, retorna para posição inicial e seta estado de PARADO */
    else if (estadoExecucao == INTERROMPENDO) {
      myStepper.step((-1) * numeroPassos);
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

void escreveArquivo(){ // Função que escreve no arquivo as medidas + espaço + graus
    myFile = SD.open("medidas.csv", O_WRITE | O_CREAT | O_TRUNC); // Abre o arquivo medidas.txt
    if (myFile) { // Se conseguir abrir o arquivo medidas.txt
      for (int i=0; i< 200; i++){// loop que percorre as 200 posições do vetor e escreve no arquivo medida[i] grau[i]
         myFile.print(medidas[i].grau);
         myFile.print(",");
         myFile.println(medidas[i].distancia);
      }
      // close the file:
      Serial.println("Arquivo salvo com sucesso!");
      myFile.close();
    } else {
        // if the file didn't open, print an error:
        Serial.println("Erro ao abrir arquivo");
      }
}

