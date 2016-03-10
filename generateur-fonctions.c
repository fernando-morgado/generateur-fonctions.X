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
 * Fonction qui transmet un caractère à la EUSART.
 * Il s'agit de l'implémentation d'une fonction système qui est
 * appelée par <code>printf</code>.
 * Cette implémentation envoie le caractère à la UART. Si un terminal
 * est connecté aux sorties RX / TX, il affichera du texte.
 * @param data Le code ASCII du caractère à afficher.
*/
void putch(char data) {
    while( ! TX1IF);
    TXREG1 = data;
}

/**
 * Initialise la sortie 1 de la EUSART.
 * Si le UP est à 1MHz, le Virtual Terminal dans Proteus
 * doit être configuré comme suit:
 * - 1200 bauds.
 * - Transmission 8 bits.
 * - Bit de stop activé.
 */
void initialiseEUSART() {
    // Pour une fréquence de 1MHz, ceci donne 1200 bauds:
    SPBRG = 12;
    SPBRGH = 0;
    // Configure RC6 et RC7 comme entrées digitales, pour que
    // la EUSART puisse en prendre le contrôle:
    TRISCbits.RC6 = 1;
    TRISCbits.RC7 = 1;
    
    // Configure la EUSART:
    // (BRGH et BRG16 sont à leur valeurs par défaut)
    // (TX9 est à sa valeur par défaut)
    RCSTAbits.SPEN = 1;  // Active la EUSART.
    TXSTAbits.SYNC = 0;  // Mode asynchrone.
    TXSTAbits.TXEN = 1;  // Active l'émetteur.
}

char generateur() {
    char sinus[] = {15,19,23,27,29,30,29,27,23,19,15,10,6,2,0,0,0,2,6,10,14};
    static unsigned short i=0;
    
    //PORTA = sinus[i];
  //  i++;        
    
    return sinus[i++] ;
}


/**
 * Point d'entrée du programme.
 */
void main(void) {
    unsigned char n;
    unsigned char i;
    
    initialiseEUSART();

    if (generateur() == 15) {
        printf("Ok 1");
    }
    if (generateur() == 19) {
        printf("Ok 2");
    }
    for(i=2;i<21;i++)
    {
        generateur() ;
    }
    if (generateur() == 14) {
        printf("Ok 21"); 
    }
    
    while(1);
}
