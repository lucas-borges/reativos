#include "evento.h"
#define LED_PIN 13

int but_state[13];
unsigned long but_old[13];
unsigned long timer_ini[2];
int timer [2];

void button_listen (int pin) {
	if(pin<LED_PIN && pin>=0){
		pinMode(pin, INPUT);
		but_state[pin]=0;
	}
	else {
		Serial.println("Erro ao tentar definir botao invalido");
	}
}

void timer_set (int timer_id, int ms) {
    if(ms>0 && timer_id<2 && timer_id>=0){
		timer[timer_id]=ms;             // timer deve expirar após “ms” milisegundos
		timer_ini[timer_id]=millis();
	}
	else
		Serial.println("Erro ao tentar definir timer invalido");
}

/* Callbacks */

void button_changed (int pin, int v);  // notifica que “pin” mudou para “v”
void timer_expired (int timer_id);             // notifica que o timer expirou

/* Programa principal: */

void setup () {
	int i;
	Serial.begin(9600);
	for (i=0;i<13;i++){			// inicializa vetor de estados
		but_state[i]=-1;
		but_old[i] = millis();
	}
	for (i=0; i<2; i++){
		timer[i]=-1;
	}
    pinMode(LED_PIN,OUTPUT);                   // inicialização da API
    inicializa();                 // inicialização do usuário
								//tive que trocar porque init() dava conflito com millis();
}

void loop () {
	for(int i=0;i<13;i++){
	    if(but_state[i]!=-1){
			unsigned long but_now=millis();
			if(but_state[i]!=digitalRead(i) && but_now>=but_old[i]+250){
				but_state[i]=!but_state[i];
				button_changed(i,but_state[i]);    // notifica o usuário
			}
		}
    }
    for(int i=0; i<2; i++){
		if(timer[i]!=-1){
			if(millis()-timer_ini[i]>=timer[i]){
				timer[i]=-1;
				timer_expired(i);
			}
		}
	}	// notifica o usuário
}
