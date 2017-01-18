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

/* Parametros para API do sensor */
long duration;
int distance;

/* Pinos */
const int trigPin = 13;
const int echoPin = 12;
const int m1      = 8;
const int m2      = 9;
const int m3      = 10;
const int m4      = 11;

/* Inicializar a biblioteca do motor de passo */
Stepper myStepper(stepsPerRevolution, m1, m2, m3, m4);

void setup() {
  pinMode(trigPin, OUTPUT); // Saida da onda
  pinMode(echoPin, INPUT);  // Retorno da onda

  myStepper.setSpeed(10);   // Setar a velocidade do motor para 10 rpm 
  
  Serial.begin(9600);       // Serial para acompanhar as medicoes (sera excluida posteriormente)
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
  for(int i = 0; i < 200; i++) {
    myStepper.step(1);

    delay(50);
    
    for(int j = 0; j < nMedidas; j++) {
      distanciasTemporarias[j] = medirDistancia(trigPin, echoPin);
      
      // Printa a medicao
      Serial.print("Distance: ");
      Serial.println(distanciasTemporarias[j]);
      
      delay(50);
    }
    
    medidas[i].distancia = media(distanciasTemporarias);
    medidas[i].grau = (i + 1) * 1.8;
  }
  
  /* Voltar o motor para a posição inicial */
  myStepper.step(-200);
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

