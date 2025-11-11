// C++ code
//
#include <LiquidCrystal.h>

/*
Link do projeto (diferente da simulacao):
https://www.tinkercad.com/things/fuzK10t6Juq-pump-jmj/editel?returnTo=%2Fthings%2FfuzK10t6Juq-pump-jmj%3Fsharecode%3DZ_Ms8Wja-3LT8w22mTiTNjF7CL8jRZG00pj-otVT4d0
*/


const int trigPin = 9;
const int echoPin = 10;
const int motorPin = 6;
const float altura_total = 103; // em cm

const int rs_pin = 12;
const int enable_pin = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

int velocidadeMotor = 0;

//LiquidCrystal meu_lcd(12,11,5,4,3,2);
LiquidCrystal meu_lcd(rs_pin,enable_pin,d4,d5,d6,d7);



float duration, distance, nivel_atual;

float margem = 3.00;

int motorLigado = 0; 

void setup()
{

  // Setando os pinos
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(motorPin,INPUT);

  // O display LCD tem 16 colunas e 2 linhas
  meu_lcd.begin(16,2);
  
  meu_lcd.print("Status:");
  
  // Comunicacao serial
  Serial.begin(9600);
  
}

void loop()
{
  
  analogWrite(motorPin, velocidadeMotor);

  /* 
  Sensor de distância(HC-SR04)
    

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
  
  distance = (duration*0.0343)/2; 

  distance -= margem;


  if (distance < 0){ // guard para garantir que estamos trabalhando com niveis adequados de profundidda
  	distance = 0;
  }
  
  //nivel_atual = (1 - distance/altura_total)*100;

  
  if (nivel_atual < 0){
  	nivel_atual = 0;
  }
  
  
  /* 
  Ligar a bomba (motor CC)
  */

  if (motorLigado){ // O motor CC está ligado? Ajuste a velocidade da rotação a depender do nível atual da caixa d'água
  	
    if(nivel_atual >= 60 && nivel_atual < 75){
      velocidadeMotor = 128;
      
    }else if(nivel_atual >= 75){
      //analogWrite(motorPin,10);
      velocidadeMotor = 64;
    }
    
    if(nivel_atual >= 90){
      //analogWrite(motorPin,0);
      velocidadeMotor = 0; // Desligar motor CC
      motorLigado = 0;
    }
    
    
  
  }else{ // Caso contrário,ligue o motor CC fo
    if (nivel_atual <= 10){
      velocidadeMotor = 240; // ? rpm (? % do Duty Cicle) (? V de tensão média)
    	motorLigado = 1;
    }
    
  }
  
 
  
  /* 
  Mostrar informação no display LCD 
  */
  int nivel_formatado = (int)nivel_atual;
  meu_lcd.setCursor(0,1);
  meu_lcd.print(nivel_formatado);
  meu_lcd.print("% cheio");
  

  /*
  Comunicacao Serial
  */
  
  Serial.print("Vol:");
  
  Serial.print(nivel_formatado);
  
  Serial.println("% cheio");
  
    
  delay(100);
  
}