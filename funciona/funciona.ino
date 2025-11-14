// C++ code
//
#include <LiquidCrystal.h>




const int trigPin = 7;
const int echoPin = 6;
const int motorPin = 11;
float altura_total = 15.40; // em cm

const int rs_pin = 1;
const int enable_pin = 0;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

//LiquidCrystal meu_lcd(12,11,5,4,3,2);
LiquidCrystal meu_lcd(rs_pin,enable_pin,d4,d5,d6,d7);



float duration, distance, nivel_atual;

float margem = 7.68;

int motorLigado = 0; 

void setup()
{

  // Setando os pinos
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(motorPin,INPUT);

  // O display LCD tem 16 colunas e 2 linhas
  meu_lcd.begin(16,2);
  
  
  // Comunicacao serial
  Serial.begin(9600);
}

void loop()
{
  meu_lcd.print("Nivel atual (%):");
  //analogWrite(motorPin,10);
  
  //analogWrite(motorPin, 240);

  /* Funcionamento do sonar (HC-SR04)
    

    Inicialmente, o pino 'trig', responsavel pela emissao do sinal de som, não emite nada. Portanto, 'trigPin' é 0 (LOW).

    Durante toda a execucao, a cada 2 microsegundos, o sensor emite um sinal (portanto, 'triPin' é HIGH) de 10 microsegundos de duracao, equivalente a 8 pulsos ultrassonicos.

    Quando o pulso emitido encontra uma superficie, ele é rebatido de volta para o reciver do sensor.

    Para pergamos a duracao que o sinal leva para ir ao obstaculo e voltar, usamos do pino 'echoPin' e da funcao pulseIn.

    O pino 'echoPin' eh mantido em estado de HIGH quando os pulsos sao emitidos.

    A funcao pulseIn tem 2 parametros: um pino que queremos ouvir e um estado (HIGH ou LOW). 

    Em 'pulseIn(echoPin, HIGH)', o microcontrolador espera o echoPin ficar HIGH (no caso ele ja esta), começa a contabilizar o tempo até echoPin ficar LOW novamente (ou seja, quando o receiver recebe o pulso rebatido no obstaculo). Quando isso ocorre, o tempo para ser contado.

    Portanto, temos a duracao do pulso indo e voltando do obstaculo.

    Por meio de:

    distancia = (duracao*velocidade)/2

    Temos a distancia do sensor ao obstáculo.
  */
  digitalWrite(trigPin, LOW);
  
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  
  delayMicroseconds(10);
  
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  
  distance = ((duration*0.0343)/2);
  Serial.print("distancia_original: ");
  float distance_margin = distance - margem;
  Serial.print(distance);
  Serial.print(" alt: ");
  
  //distance -= margem;
 

  distance = abs(distance);
  
 
  
  //nivel_atual = (1 - (distance/altura_total))*100;
  nivel_atual = abs((1 - abs((distance_margin/(altura_total-margem))))*100);

  //nivel_atual
  

  
  //MOTOR
  if (motorLigado){ // o motor CC esta ligado? faca isso
  	
    if(nivel_atual >= 60 && nivel_atual < 75){
      analogWrite(motorPin,64);
      
    }else if(nivel_atual >= 75){
      analogWrite(motorPin,10);
    }
    
    if(nivel_atual >= 80){
      analogWrite(motorPin,0);
      motorLigado = 0;
    }
    
    
  }else{ // puts ta desligado? faca isso
    if (nivel_atual <= 10){
      
      	analogWrite(motorPin,100);// os pinos aguentam 2 V?
    	motorLigado = 1;
    }
    
  }
  
 
  
  /* LCD */
  int nivel_formatado = (int)nivel_atual;
  meu_lcd.setCursor(0,1);
  meu_lcd.print(nivel_formatado);
  //meu_lcd.print(nivel_atual);
  //meu_lcd.print(" cm");
  

  /* Comunicacao Serial*/
  
  //Serial.print("Vol:");
  
  //Serial.print(nivel_formatado);
  
  //Serial.print(nivel_formatado);
  Serial.print(distance_margin);
  Serial.print(" total: ");
  Serial.print(altura_total);
  Serial.print(" - ");
 // Serial.println("% cheio");
  Serial.print(nivel_atual);
  Serial.print(" ; Status do motor:");
  Serial.println(motorLigado);

  
 
    
  delay(250);
  meu_lcd.clear();
}