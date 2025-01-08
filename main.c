#include <stdio.h>
#include "funciones.h"

int main (int argc, char *argv[]) {

    int opc=0, opc2=0;
    do{
        opc = menu();
        switch(opc){
            case 1:
                createFactura();
                line();
                break;
            case 2:
                readFactura();
                line();
                break;
            case 3:
                editFactura();
                line();
                break;
            default:
                printf("Opcion no valida\n");
                break;
        }
        printf("Desea otra opcion? \n1.Si\t2.No\n");
        scanf("%d", &opc2);
        line();
        if(opc2 == 2){
            printf("\nSaliendo....\n");
        }
    }while(opc2 != 2);

    return 0;
}