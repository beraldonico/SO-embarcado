#include <xc.h>

void main(void) {
    /*
     * Configura��o de portas:
     * 
     * Registradores :
     * 
     * TRIS - define a dire��o dos dados.
     * Se 0 � saida, se 1 � entrada. O padr�o � 1
     * 
     * PORT - serve para fazer a leitura e a escrita de dados
     * 
     * LAT - serve para fazer a leitura e a escrtita de dados
     * sempre mantem a informa��o sobre a ultima escrtita de dados
     */
    
    // Define RD7 como saida de dados
    TRISDbits.RD7 = 0;
    
    // Escreve em RD7
    PORTDbits.RD7 = 1;
    
    return;
}
