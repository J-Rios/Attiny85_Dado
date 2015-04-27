// Attiny dado

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
                         
const int pinSinUso = 5;        //(pin1)
const int pinLeds1 = 4;         //(pin3)
const int pinLeds2 = 1;         //(pin6)
const int pinLeds3 = 0;         //(pin5)
const int pinLed4 = 3;          //(pin2)
const int boton = 2;            //(pin7)
const int tiempo = 2000;

volatile int wd_counter = 0;

ISR(WDT_vect)
{
    wd_counter++;
}

void Int0ISR()
{  
   detachInterrupt(INT0);   // desactivar INT0 (GIMSK &= ~(1<<INT0))
}

void setup ()
{
    // Pines de salida (leds)
    pinMode(pinLeds1, OUTPUT);
    pinMode(pinLeds2, OUTPUT);
    pinMode(pinLeds3, OUTPUT);
    pinMode(pinLed4, OUTPUT);
    
    // Pin de entrada (boton) y resistencia de pull-up interna activa
    pinMode(boton, INPUT);
    digitalWrite(boton, HIGH);
    
    // Establece modo de bajo consumo
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    
    randomSeed(micros()* analogRead(pinSinUso));
    
    animacion();
}

void loop()
{
    if(digitalRead(boton) == HIGH)
    {
        aDormir();   
        funcionalidad();
    }
}

void aDormir()
{ 
    attachInterrupt(0, Int0ISR, LOW);  // activar INT0 (GIMSK |= (1<<INT0))
    
    //sleep_mode(); // Sustituye a las tres siguientes lineas
    sleep_enable();  
    sleep_cpu();
    sleep_disable();
}

void funcionalidad()
{
    dado(random(1, 7));   // El limite maximo se excluye (7)
    wd_delay(tiempo);
    dado(0);
}

void dado(int num)
{
    if(num == 1)
        digitalWrite(pinLed4, LOW);
    else if(num == 2)
        digitalWrite(pinLeds1, LOW);
    else if(num == 3)
    {
        digitalWrite(pinLeds3, LOW);
        digitalWrite(pinLed4, LOW);
    }
    else if(num == 4)
    {
        digitalWrite(pinLeds1, LOW);
        digitalWrite(pinLeds3, LOW);
    }
    else if(num == 5)
    {
        digitalWrite(pinLeds1, LOW);
        digitalWrite(pinLeds3, LOW);
        digitalWrite(pinLed4, LOW);
    }
    else if(num == 6)
    {
        digitalWrite(pinLeds1, LOW);
        digitalWrite(pinLeds2, LOW);
        digitalWrite(pinLeds3, LOW);
    }
    else
    {
       digitalWrite(pinLeds1, HIGH);
       digitalWrite(pinLeds2, HIGH);
       digitalWrite(pinLeds3, HIGH);
       digitalWrite(pinLed4, HIGH);
    }
}

void animacion()
{
    dado(0);
    dado(1);
    wd_delay(500);
    dado(0);
    dado(2);
    wd_delay(500);
    dado(0);
    dado(3);
    wd_delay(500);
    dado(0);
    dado(4);
    wd_delay(500);
    dado(0);
    dado(5);
    wd_delay(500);
    dado(0);
    dado(6);
    wd_delay(500);
    dado(0);
}

void wd_delay(unsigned long t_ms) // OJO: LA SENSIBILIDAD SERA DE 15MS (UN CAMBIO MENOR A 15MS NO SE DETECTA Y, EL VALOR MINIMO SERA DE 15MS)
{
    int num_veces = 0;
    
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    
    if(t_ms == 15)
    {
        setup_watchdog(WDTO_15MS);
        sleep_mode();
    }
    else if(t_ms == 30)
    {
        setup_watchdog(WDTO_30MS);
        sleep_mode();
    }
    else if(t_ms == 60)
    {
        setup_watchdog(WDTO_60MS);
        sleep_mode();
    }
    else if(t_ms == 120)
    {
        setup_watchdog(WDTO_120MS);
        sleep_mode();
    }
    else if(t_ms == 250)
    {
        setup_watchdog(WDTO_250MS);
        sleep_mode();
    }
    else if(t_ms == 500)
    {
        setup_watchdog(WDTO_500MS);
        sleep_mode();
    }
    else if(t_ms == 1000)
    {
        setup_watchdog(WDTO_1S);
        sleep_mode();
    }
    else if(t_ms == 2000)
    {
        setup_watchdog(WDTO_2S);
        sleep_mode();
    }
    else if(t_ms == 4000)
    {
        setup_watchdog(WDTO_4S);
        sleep_mode();
    }
    else if(t_ms == 8000)
    {
        setup_watchdog(WDTO_8S);
        sleep_mode();
    }
    else
    {
        num_veces = t_ms/15;
        while(wd_counter < num_veces)
        {
            setup_watchdog(WDTO_15MS);
            sleep_mode();
        }
    }
    
    wdt_disable();
    wd_counter = 0; 
}

void setup_watchdog(int timerPrescaler)
{
    if ((timerPrescaler < 0) || (timerPrescaler > 10))
        timerPrescaler = WDTO_15MS;
        
    byte bb = timerPrescaler & 7;    
    if(timerPrescaler > 7)
        bb |= (1<<5); // Establece los bits necesarios
    
    MCUSR &= ~(1<<WDRF); // Limpia el flag de reset del watchdog
    WDTCR |= (1<<WDCE) | (1<<WDE); // Activa el watchdog
    WDTCR = bb; // Establece el Timeout del watchdog
    WDTCR |= _BV(WDIE); // Activa la interrupcion del watchdog
}
