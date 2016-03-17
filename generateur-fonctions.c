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

static unsigned short i=0;

void reinitialiseGenerateur() {
    i = 0;
}

char generateur() {
    
    if (i == 21){
        i = 0;
    }
    
    return sinus[i++] ;
}

char prochainFlanc() {
    static char pas = 0;
    static char valeurGeneree;
    
    switch(pas) {
        case 0:
            valeurGeneree = generateur();
            pas = 1;
            return valeurGeneree;

        case 1:
        default:
            pas = 0;
            return 30 - valeurGeneree;
    }
}

#ifndef TEST

void low_priority interrupt interruptionsBassePriorite() {
    unsigned int p;
    if (INTCONbits.TMR0IF) {
        INTCONbits.TMR0IF = 0;
        p = prochainFlanc() << 7;
        TMR0 = 65535 - p;
        if (PORTAbits.RA7) {
            PORTAbits.RA7 = 0;
        } else {
            PORTAbits.RA7 = 1;
        }
    }
}

/**
 * Point d'entrée du programme.
 */
void main(void) {
    unsigned char n, m;
    
    // Réglage de l'horloge:
    OSCCONbits.IRCF = 6;   // 8 MHz
    OSCTUNEbits.PLLEN = 0; // Pas de multiplicateur de fréquence.
    
    // Réglage du temporisateur 0:
    T0CONbits.T0CS = 0;     // Source: FOSC / 4
    T0CONbits.T08BIT = 0;   // Temporisateur de 16 bits.
    T0CONbits.PSA = 1;      // Pas de diviseur de fréquence.
    T0CONbits.TMR0ON = 1;   // Active le temporisateur.

    // Réglage des interruptions:
    RCONbits.IPEN = 1;      // Active les niveaux d'interruption.
    INTCONbits.GIEH = 1;    // Active les interruptions.
    INTCONbits.GIEL = 1;    // Active les interruptions de basse priorité.
    
    INTCONbits.TMR0IE = 1;  // Active interruptions du T0...
    INTCONbits.TMR0IF = 0;  // Abaissement du drapeau.
    INTCON2bits.TMR0IP = 0; // ... de basse priorité.

    // Réglage des ports I/O:
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    TRISA = 0;
    
    while(1);
    
}
#endif

#ifdef TEST

int testGenerateur() {
    unsigned char i;
    int testsEnErreur = 0;
    
    reinitialiseGenerateur();
    
    testsEnErreur += assertEquals("GEN01", generateur(), 15);
    testsEnErreur += assertEquals("GEN02", generateur(), 19);
    
    for(i = 2; i < 20; i++) {
        generateur() ;
    }
    
    testsEnErreur += assertEquals("GEN03", generateur(), 14);
    testsEnErreur += assertEquals("GEN04", generateur(), 15);
    
    return testsEnErreur;    
}

int testProchainFlanc() {
    int testsEnErreur = 0;

    reinitialiseGenerateur();
    
    testsEnErreur += assertEquals("PF001a", prochainFlanc(), 15);
    testsEnErreur += assertEquals("PF001b", prochainFlanc(), 15);

    testsEnErreur += assertEquals("PF002a", prochainFlanc(), 19);
    testsEnErreur += assertEquals("PF002b", prochainFlanc(), 11);

    testsEnErreur += assertEquals("PF003a", prochainFlanc(), 23);
    testsEnErreur += assertEquals("PF003b", prochainFlanc(), 7);
    
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
    testsEnErreur += testProchainFlanc();
    printf("%d tests en erreur\r\n", testsEnErreur);

    while(1);
}
#endif