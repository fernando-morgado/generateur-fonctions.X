#include<xc.h>
#include<stdio.h>

#ifdef TEST

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
 * Si le UP est à 1MHz, le "Virtual Terminal" dans Proteus
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

/**
 * Initialise la console de test.
 */
void initialiseTests() {
    initialiseEUSART();
}

/**
 * Vérifie si la valeur obtenue est égale à la valeur attendue.
 * @param value Valeur obtenue.
 * @param expectedValue Valeur attendue.
 * @param testId Identifiant du test.
 * @return 1 si le test échoue.
 */
int assertEquals(const char *testId, int value, int expectedValue) {
    if (value != expectedValue) {
        printf("%s: Valeur obtenue %d - Valeur attendue %d\r\n", testId, value, expectedValue);
        return 1;
    }
    return 0;
}
#endif