#include <xc.h>
#include <stdio.h>
#include "test.h"

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

char sinus[] = {15,19,23,27,29,30,29,27,23,19,15,10,6,2,0,0,0,2,6,10,14};

char generateur() {
    static unsigned short i=0;
    
    if (i == 21){
        i = 0;
    }
    
    return sinus[i++] ;
}

#ifndef TEST

/**
 * Point d'entrée du programme.
 */
void main(void) {
    unsigned char n, m;
        
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    TRISA = 0;
    
    while(1) {
        for (n = 0; n < 20; n++) {
            for (m = 0; m < 32; m++) {
                if (m < sinus[n]) {
                    PORTAbits.RA7 = 1;
                } else {
                    PORTAbits.RA7 = 0;
                }
            }
        }
    }
}
#endif

#ifdef TEST

int testGenerateur() {
    unsigned char i;
    int testsEnErreur = 0;
    
    testsEnErreur += assertEquals("GEN01", generateur(), 15);
    testsEnErreur += assertEquals("GEN02", generateur(), 19);
    
    for(i = 2; i < 20; i++) {
        generateur() ;
    }
    
    testsEnErreur += assertEquals("GEN03", generateur(), 14);
    testsEnErreur += assertEquals("GEN04", generateur(), 15);
    
    return testsEnErreur;    
}

/**
 * Point d'entrée des test unitaires.
 */
void main(void) {
    int testsEnErreur = 0;
    
    initialiseTests();
    
    printf("Lancement des tests\r\n");
    testsEnErreur += testGenerateur();
    printf("%d tests en erreur\r\n", testsEnErreur);

    while(1);
}
#endif