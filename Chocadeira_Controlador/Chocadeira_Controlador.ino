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
float MAX_Temperatura = 37.8;
float MIN_Temperatura = 37.4;

bool lampada = false;
bool fita_led = false;
bool ventilador = false;
bool ventilador_pwm = false;

void setup() {
  // Abre comunicacao com o bluetooth
  Serial.begin(9600);

  Serial.println(MAX_Temperatura);
  Serial.println(MIN_Temperatura);
  
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
  //Desliga Reles
  digitalWrite(Lampada, HIGH);
  digitalWrite(Ventilador_PWM, HIGH);
  digitalWrite(Ventilador, HIGH);
  digitalWrite(Fita_LED, HIGH);
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
  Serial.println("Controle da Temperatura");
  if(lampada && temperatura > MAX_Temperatura){
    digitalWrite(Lampada, HIGH);
    lampada = false;
    digitalWrite(Ventilador_PWM, LOW);
    ventilador_pwm = true;
    Serial.println("Quente");
  }
  if(!lampada && ventilador_pwm && temperatura <= MAX_Temperatura){
    digitalWrite(Lampada, HIGH);
    lampada = false;
    digitalWrite(Ventilador_PWM, HIGH);
    ventilador_pwm = false;
    Serial.println("Morno");
  }
  if(!lampada && !ventilador_pwm && temperatura < MIN_Temperatura){
    digitalWrite(Lampada, LOW);
    lampada = true;
    digitalWrite(Ventilador_PWM, HIGH);
    ventilador_pwm = false;
    Serial.println("Frio");
  }
}

void Controla_Umidade(){
  Serial.println("Controle da Umidade");
  if(umidade <= 60){
    // Liga Ventilador
    Serial.println("Umidificando");
    digitalWrite(Ventilador, LOW);
    ventilador = true;
  }else {
    // Desliga Ventilador
    Serial.println("Desumidificando");
    digitalWrite(Ventilador, HIGH);
    ventilador = false;
  }
}

void Luz_Ambiente(){
  //if(Switch_Porta == HIGH){ //Troca pra HIGH quando instalar o Switch
    digitalWrite(Fita_LED, LOW);
  //}else{
    //digitalWrite(Fita_LED, HIGH);
  //}
}
