#include <xc.h>
#include <stdio.h>

/**
 * Bits de configuration:
 */
#pragma config FOSC = INTIO67  // Osc. interne, A6 et A7 comme IO.
#pragma config IESO = OFF      // Pas d'osc. au démarrage.
#pragma config FCMEN = OFF     // Pas de monitorage de l'oscillateur.

// Nécessaires pour ICSP / ICD:
#pragma config MCLRE = EXTMCLR // RE3 est actif comme master reset.
#pragma config WDTEN = OFF     // Watchdog inactif.
#pragma config LVP = OFF       // Single Supply Enable bits off.

/**
 * Point d'entrée du programme.
 */
void main(void) {
    char sinus[] = {15,19,23,27,29,30,29,27,23,19,15,10,6,2,0,0,0,2,6,10,14};
    unsigned char n;
    
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    TRISA = 0;

    while(1) {
        for (n = 0; n < 20; n++) {
            PORTA = sinus[n];
        }
    }
}
