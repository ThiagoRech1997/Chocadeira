// Biblioteca referente a comunicacao I2C
#include <Wire.h>
// Biblioteca referente a BME280
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Variavel de identificacao do BME280
Adafruit_BME280 bme;
// Pinos do Relé
#define Lampada         8
#define Fita_LED        9
#define Ventilador      10
#define Ventilador_PWM  11

// Demais Pinos
#define PWM_Ventilador  3
#define Switch_Porta    4

// Variaveis globais
float temperatura;
float umidade;

void setup() {
  // Abre comunicacao com o bluetooth
  Serial.begin(9600);
  // Verifica se BME esta conectado
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  // Seta pinos dos relés como saída
  pinMode(Lampada,        OUTPUT);
  pinMode(Fita_LED,       OUTPUT);
  pinMode(Ventilador,     OUTPUT);
  pinMode(Ventilador_PWM, OUTPUT);
  // Demais pinos digitais
  pinMode(PWM_Ventilador, OUTPUT);
  pinMode(Switch_Porta,   INPUT);
}

void loop() {
  Ler_Dados_Sensor();
  Controla_Temperatura();
  Controla_Umidade();
  Luz_Ambiente();
  delay(1000);
}

void Ler_Dados_Sensor(){
  Serial.print("Temperatura: ");
  temperatura = bme.readTemperature();
  Serial.print(temperatura);
  Serial.println(" *C");

  Serial.print("Umidade: ");
  umidade = bme.readHumidity();
  Serial.print(umidade);
  Serial.println(" %");
}

void Controla_Temperatura(){
  if(temperatura < 37.5f){
    // Liga lampada de aquecimento e desliga ventilador de exaustão
    digitalWrite(Lampada, LOW);
    digitalWrite(Ventilador_PWM, HIGH);
  }else 
  if(temperatura == 37.5f){
    // Desliga ampada de aquecimento e ventilador de exaustão
    digitalWrite(Lampada, HIGH);
    digitalWrite(Ventilador_PWM, HIGH);
  }else
  if(temperatura > 37.5f){
    // Desliga ampada de aquecimento e liga ventilador de exaustão
    digitalWrite(Lampada, HIGH);
    digitalWrite(Ventilador_PWM, LOW);
  }
}

void Controla_Umidade(){
  if(umidade <= 60){
    // Liga Ventilador
    digitalWrite(Ventilador, LOW);
  }else {
    // Desliga Ventilador
    digitalWrite(Ventilador, HIGH);
  }
}

void Luz_Ambiente(){
  if(Switch_Porta == HIGH){
    digitalWrite(Fita_LED, LOW);
  }else{
    digitalWrite(Fita_LED, HIGH);
  }
}
