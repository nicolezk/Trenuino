// Trenuino
// Projeto Desenvolvido por:
// Felipe Khoji Myose
// Nicole Zafalon Kovacs
// Vinicius Ito Nagura

// Registro para medicoes
struct Medida{
  float grau;
  int distancia;
};

/ *
  Declaracao das variaveis
*/

// Uma unica medida, por enquanto (ainda sem motor)
Medida medida1;

// Parametros para API do sensor
long duration;
int distance;

// Pinos
const int trigPin = 13;
const int echoPin = 12;
   
   
void setup() {
  pinMode(trigPin, OUTPUT); // Saida da onda
  pinMode(echoPin, INPUT);  // Retorno da onda
  // Serial para acompanhar as medicoes (sera excluida posteriormente)
  Serial.begin(9600);
}

// API para medicao da distancia
// retirada de:
// http://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/http://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/
int medeDistancia(int trigPin, int echoPin){
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance= duration*0.034/2;
    // Prints the distance on the Serial Monitor
    return distance;
}


void loop() {
  // Por enquanto, esta efetuando apenas uma medicao
  medida1.distancia = medeDistancia(trigPin, echoPin);
  // Printa a medicao
  Serial.print("Distance: ");
  Serial.println(medida1.distancia);
  delay(500);
}
