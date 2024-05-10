//importando bibliotecas

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <dht.h>

//definindo os pinos dos sensores

#define dht_pin  8

#define LDR_PIN  2

//nomeando o sensor DHT

dht my_dht;

//criando variaveis locais

int LED_VERMELHA = 11;
int LED_AMARELA = 12;
int LED_VERDE = 13;
int som = 4;
int temperatura = 0x00;
int umidade = 0x00;

//criando variaveis para ajuste da luminosidade do photoresistor
const float GAMMA = 0.7;
const float RL10 = 50;

//criando o objeto do display

LiquidCrystal_I2C lcd(0x27, 2, 1);

//configurações iniciais
void setup(){
  pinMode(LED_VERMELHA,OUTPUT);
  pinMode(LED_AMARELA,OUTPUT);
  pinMode(LED_VERDE,OUTPUT);
  pinMode(som,OUTPUT);
  pinMode(A0,INPUT);
  lcd.begin(16,2);
  lcd.clear();
  lcd.backlight();
}

//inicio do looping
void loop(){

  //conversão do valor da luz retirado da documentação do photoresistor
  int analogValue = analogRead(A0);
  float voltage = analogValue / 1024. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));

  //leitura dos dados de temperatura e umidade do DHT
  my_dht.read22(dht_pin);
  temperatura = my_dht.temperature;
  umidade = my_dht.humidity;  
  
  
  //condições da intensidade da luz
  //luz ideal(baixa)
  if (lux < 800){
    digitalWrite(LED_VERMELHA,LOW);
    digitalWrite(LED_AMARELA,LOW);
    digitalWrite(LED_VERDE,HIGH);
    noTone(som);
  }
  //luz intermédia
  else if (800 <= lux && lux <= 950){
    digitalWrite(LED_VERDE,LOW);
    digitalWrite(LED_VERMELHA,LOW);
    digitalWrite(LED_AMARELA,HIGH);
    noTone(som);
    lcd.setCursor(0,0);
    lcd.print("Amb. a meia luz");
  }
  //luz de intensidade alta
  else{
    digitalWrite(LED_VERDE,LOW);
    digitalWrite(LED_AMARELA,LOW);
    digitalWrite(LED_VERMELHA,HIGH);
    lcd.setCursor(0,0);
    lcd.print("Amb. muito claro");
    tone(som,1000);
  }
  //reset display
  delay(1000);
  lcd.clear();

  //condição para o estado da temperatura
  //temperatura ideal
  if (temperatura >= 10 && temperatura <= 15){
    lcd.print(temperatura);
    lcd.print(" C ");
    lcd.setCursor(0,1);
    lcd.print("Temperatura ok");
    
  }
  //temperatura alta
  else if (temperatura > 15){
    lcd.print(temperatura);
    lcd.print(" C ");
    lcd.print("Temperatura");
    lcd.setCursor(0,1);
    lcd.print("Alta");
  }
  //temperatura baixa
  else{
    lcd.print(temperatura);
    lcd.print(" C ");
    lcd.print("Temperatura");
    lcd.setCursor(0,1);
    lcd.print("Baixa");
  }
  //reset display
  delay(1000);
  lcd.clear();

  //condição para mostrar o estado da umidade
  //umidade ideal
  if (umidade >= 50 && umidade <= 70){
    lcd.print(umidade);
    lcd.print("% ");
    lcd.setCursor(0,1);
    lcd.print("Umidade ok");
  }
  //umidade baixa
  else if (umidade < 50){
    lcd.print(umidade);
    lcd.print("% ");
    lcd.print("Umidade");
    lcd.setCursor(0,1);
    lcd.print("Baixa");
  }
  //umidade alta
  else{
    lcd.print(umidade);
    lcd.print("% ");
    lcd.print("Umidade");
    lcd.setCursor(0,1);
    lcd.print("Alta");
  }
  //reset display
  delay(1000);
  lcd.clear();
}
