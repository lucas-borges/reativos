#include <evento.h>

#define LED_PIN 13
#define BUT_ACC  2
#define BUT_DES  8

int intervalo=1000;
int estado_led=1;
int parar=0;

void timer_expired(int id){
  if(id==0){
    estado_led=!estado_led;
    digitalWrite(LED_PIN, estado_led);
    timer_set(0,intervalo);
  }
  else if (id==1){
    parar=0;
  }
};
void button_changed (int pin, int v) {
  if(pin==BUT_ACC && v==1){
    if(intervalo>=100)
       intervalo-=100;
    else
      intervalo=0;
    Serial.println("botao acelerador apertado ");
    flag_parada(BUT_ACC);
  }
  else if(pin==BUT_DES && v==1){
    intervalo+=100;
    Serial.println("botao desacelerador apertado ");
    flag_parada(BUT_DES);
  }
}
 
void inicializa () { //tive que trocar porque init() dava conflito com millis();
    button_listen(BUT_ACC);
    button_listen(BUT_DES);
    digitalWrite(LED_PIN,estado_led);
    timer_set(0,intervalo);
}

void flag_parada (int but){
   if (parar==0){
      parar=but;
      timer_set(1,500);
   }
   else if(parar!=but){
      digitalWrite(LED_PIN,LOW);
      while(1); 
  } 
}
