#include <evento.h>

#define LED_PIN 13
#define BUT_PIN  2

int i;

void timer_expired(int id){};
void button_changed (int pin, int v) {
    digitalWrite(LED_PIN, v);
    
}
 
void inicializa () {
    button_listen(BUT_PIN);
}

