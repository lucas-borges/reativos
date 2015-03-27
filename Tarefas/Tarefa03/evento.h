#include <Arduino.h>

void button_listen (int pin);
void timer_set (int timer_id, int ms);
void button_changed (int pin, int v);
void timer_expired (void);
void inicializa (void); //tive que trocar porque init() dava conflito com millis();
