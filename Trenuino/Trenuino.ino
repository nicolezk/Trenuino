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
*/

#include <Stepper.h>

#define nMedidas 5
#define stepsPerRevolution 200

#define PARADO        0
#define GIRANDO       1
#define PAUSADO       2
#define INTERROMPENDO 30


/* Cabeçalho */
void obterMedidas();
int medirDistancia(int trigPin, int echoPin);
int media(int distanciasTemporarias[]);

/* Registro para medicoes */
struct Medida{
  float grau;
  int distancia;
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
int distance;

/* Pinos */
const int trigPin           = 13;
const int echoPin           = 12;
const int m1                = 8;
const int m2                = 9;
const int m3                = 10;
const int m4                = 11;
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
}

void loop() {
  obterMedidas();
  delay(5000);
}

int media(int distanciasTemporarias[]) {
  int sum = 0;
  for(int j = 0; j < nMedidas; j++) {
    sum += distanciasTemporarias[j];
  }
  return (sum/nMedidas);
}

void obterMedidas() {
  int distanciasTemporarias[nMedidas];
  
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

int medirDistancia(int trigPin, int echoPin){
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

