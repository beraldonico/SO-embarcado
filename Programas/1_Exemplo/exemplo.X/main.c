#include <xc.h>

void main(void) {
    /*
     * Configuração de portas:
     * 
     * Registradores :
     * 
     * TRIS - define a direção dos dados.
     * Se 0 é saida, se 1 é entrada. O padrão é 1
     * 
     * PORT - serve para fazer a leitura e a escrita de dados
     * 
     * LAT - serve para fazer a leitura e a escrtita de dados
     * sempre mantem a informação sobre a ultima escrtita de dados
     */
    
    // Define RD7 como saida de dados
    TRISDbits.RD7 = 0;
    
    // Escreve em RD7
    PORTDbits.RD7 = 1;
    
    return;
}
