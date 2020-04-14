#include <virtuabotixRTC.h>

// Declarando saidas do Rele

int Relay1 = 0;
int Relay2 = 1;

int Button = 13;

// HORARIO PARA LIGAR
int RHorOn = 6;
int RMinOn = 0;

// HORARIO PARA DESLIGAR
int RHorOff = 7;
int RMinOff = 30;

bool status = false;

bool force_shutdown = false;


#define clk      8
#define dat      9
#define rst     10

#define segL        13
#define minL        27
#define horL         2
#define d_semL       6
#define d_mesL      10
#define mesL         4
#define anoL      2020


// Declarando objetos
virtuabotixRTC myRTC(clk, dat, rst);

// Protótipo das Funções
void DS1302();
void week(int dayW);


void setup() {
   // Declarando Pinos para Rele
   pinMode(Button, INPUT_PULLUP);

   pinMode(Relay1, OUTPUT);
   pinMode(Relay2, OUTPUT);

   Serial.begin(9600);

   //myRTC.setDS1302Time(segL, minL, horL, d_semL, d_mesL, mesL, anoL);
}

void loop() {
   DS1302();


   // CONVERTENDO HORAS PARA MINUTOS
   int DS_min = (myRTC.hours * 60) + myRTC.minutes;
   int R_init = (RHorOn * 60) + RMinOn; 
   int R_end  = (RHorOff * 60) + RMinOff - 1; 

   if (digitalRead(Relay2) == LOW) {
      status = true;
   } else {
      status = false;
   }

   // Se ano for igual a 2000 então entra em looping dizendo que está com ano errado
   if (myRTC.year == 2000) {
      while (true) {
           digitalWrite(Relay1, LOW);
           delay(500);
           digitalWrite(Relay1, HIGH);
           delay(500);
      }
   }

   
   if (DS_min >= R_init && DS_min <= R_end && force_shutdown == false) {
      digitalWrite(Relay1, LOW);
      digitalWrite(Relay2, LOW);
   } else {
      digitalWrite(Relay1, HIGH);
      digitalWrite(Relay2, HIGH);
   }

   if (digitalRead(Button) == LOW && status == false) {
      digitalWrite(Relay1, LOW);
      digitalWrite(Relay2, LOW);
      status = true;
      while(digitalRead(Button) == LOW) {
         DS1302();
         Status();
         delay(1000);
      }
   }   

   if (digitalRead(Button) == LOW && status == true) {
      digitalWrite(Relay1, LOW);
      delay(500);
      digitalWrite(Relay1, HIGH);
      delay(500);
      
      force_shutdown = true;

      while (digitalRead(Button) == LOW) {
         digitalWrite(Relay1, LOW);
         delay(500);
         digitalWrite(Relay1, HIGH);
         delay(500);
      }
   }

   if (DS_min > R_end && force_shutdown == true) {
      force_shutdown = false;
   }

   Status();

   delay(1000);
}

void DS1302() {
   myRTC.updateTime(); // Faz leitura do DS1302

  Serial.print(" -> ");
  week(myRTC.dayofweek);
  Serial.print(" | ");
  Serial.print(myRTC.dayofmonth);
  Serial.print("/");
  Serial.print(myRTC.month);
  Serial.print("/");
  Serial.print(myRTC.year);
  Serial.print(" | ");
  if(myRTC.hours < 10) Serial.print("0");
  Serial.print(myRTC.hours);
  Serial.print(":");
  if(myRTC.minutes < 10) Serial.print("0");
  Serial.print(myRTC.minutes);
  Serial.print(":");
  if(myRTC.seconds < 10) Serial.print("0");
  Serial.println(myRTC.seconds);
}

void Status() {

   Serial.print(" -> Status: ");
   if (status == false){Serial.print("OFF");} else {Serial.print("ON ");}
   Serial.print(" | Rele 1: ");
   if (digitalRead(Relay1) == 0){Serial.print("ON ");} else {Serial.print("OFF");}
   Serial.print(" | Rele 2: ");
   if (digitalRead(Relay2) == 0){Serial.print("ON ");} else {Serial.print("OFF");}
   Serial.print(" | FORCE SHUTDOWN: ");
   if (force_shutdown == false){Serial.print("OFF");} else {Serial.print("ON ");}
   Serial.print(" | BOTAO: ");
   if (digitalRead(Button)){Serial.println("OFF");} else {Serial.println("ON ");}

}

void week(int dayW) {

   switch (dayW)
   {
    case 1: Serial.print("Dom"); break;
    case 2: Serial.print("Seg"); break;
    case 3: Serial.print("Ter"); break;
    case 4: Serial.print("Qua"); break;
    case 5: Serial.print("Qui"); break;
    case 6: Serial.print("Sex"); break;
    case 7: Serial.print("Sab"); break;
   }
}
