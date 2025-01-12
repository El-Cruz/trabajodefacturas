#include <stdio.h>
#include <string.h>
#include "funciones.h"

int menu(){
    int opcion;
    printf("1. Crear Factura\n");
    printf("2. Ver Facturas\n");
    printf("3. Editar Factura\n");
    printf("Opcion: ");
    scanf("%d", &opcion);
    return opcion;
}

void line(){
    printf("\n");
    for(int i = 0; i < 30; i++){
        printf("=");
    }
    printf("\n\n");
}

void save(struct Factura *factura){ //aqui se guarda la factura en un archivo
    FILE *file;
    file = fopen("facturas", "ab+");
    if(file == NULL){
        printf("Error al abrir el archivo\n");
        return;
    }else{
        fwrite(factura, sizeof(struct Factura), 1, file);
        printf("Factura guardada\n");
    }
    fclose(file);
}
void readCadena(char *cadena, int numcaracteres){
    fflush(stdin);
    fgets(cadena, numcaracteres, stdin);
    int len = strlen(cadena)-1;
    cadena[len] = '\0';
}
void createFactura(){
    struct Factura factura;
    factura.total = 0;
    printf("Nombre del Cliente: ");
    readCadena(factura.nombre, 50);
    printf("Cédula: ");
    scanf("%d", &factura.cedula);
    printf("Numero de productos: ");
    scanf("%d", &factura.numProd);
    for(int i = 0; i < factura.numProd; i++ ){
        printf("Nombre del producto %d: ", i+1);
        readCadena(factura.productos[i].nombre, 50);
        printf("Cantidad del producto %d: ", i+1);
        scanf("%d", &factura.productos[i].cantidad);
        printf("Precio del producto %d: ", i+1);
        scanf("%f", &factura.productos[i].precio);
        factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
    }
    factura.estado = 0;
    save(&factura);
}

void readFactura(){
    struct Factura factura;
    int opc;
    FILE *file;
    file = fopen("facturas", "rb");

    if(file == NULL){
        printf("Error al abrir el archivo\n");
        return;
    }
    printf("Cedula\t\tNombre\t\tTotal\n");
    while(fread(&factura, sizeof(struct Factura), 1, file)){
        printf("%d\t\t%s\t\t%.2f\n", factura.cedula, 
                                    factura.nombre, 
                                    factura.total);
    }
    printf("\nDesea ver el detalle de una Factura\n1.Si\t2.No\n");
    scanf("%d", &opc);
    if(opc == 1){
        printf("Ingrese la cedula del cliente: ");
        int cedula;
        scanf("%d", &cedula);
        printf("\n");
        findFacturaByCedula(cedula);
        printf("\nDesea eliminar esta Factura\n1.Si\t2.No\n");
        scanf("%d", &opc);
        if(opc == 1){
            deleteFactura(cedula);
        }
    }

    fclose(file);
}

int findFacturaByCedula(int cedula){
    struct Factura factura;
    int f=0, pos=0;
    FILE *file;
    file = fopen("facturas", "rb");
    if(file == NULL){
        printf("Error al abrir el archivo\n");
        return -1;
    }
    while(fread(&factura, sizeof(struct Factura), 1, file)){
        if(factura.cedula == cedula){
            f=1;
            printf("Factura encontrada\n");
            printf("Nombre: %s\n", factura.nombre);
            printf("Cedula: %d\n", factura.cedula);
            printf("Productos:\n");
            printf("Nombre\t\tCantidad\tPrecio\n");
            for(int i = 0; i < factura.numProd; i++){
                printf("%s\t\t%d\t\t%.2f\n", factura.productos[i].nombre,
                                            factura.productos[i].cantidad,
                                            factura.productos[i].precio);
            }
            printf("Total: %.2f\n", factura.total);
            break;
        }
        pos= ftell(file);
    }
    if(f==0){
        pos=-1;
        printf("Factura no encontrada\n");
    }
    fclose(file);
    return pos;
}

void editFactura() {
    int cedula;
    printf("Ingrese la cedula del cliente de la factura a editar: ");
    scanf("%d", &cedula);

    struct Factura factura;
    FILE *file = fopen("facturas", "rb+");
    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    int found = 0;
    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        if (factura.cedula == cedula) {
            found = 1;
            printf("Factura encontrada. Ingrese los nuevos datos:\n");
            printf("Nombre del Cliente: ");
            readCadena(factura.nombre, 50);
            printf("Numero de productos: ");
            scanf("%d", &factura.numProd);
            factura.total = 0;
            for (int i = 0; i < factura.numProd; i++) {
                printf("Nombre del producto %d: ", i + 1);
                readCadena(factura.productos[i].nombre, 50);
                printf("Cantidad del producto %d: ", i + 1);
                scanf("%d", &factura.productos[i].cantidad);
                printf("Precio del producto %d: ", i + 1);
                scanf("%f", &factura.productos[i].precio);
                factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
            }
            fseek(file, -sizeof(struct Factura), SEEK_CUR);
            fwrite(&factura, sizeof(struct Factura), 1, file);
            printf("Factura actualizada\n");
            break;
        }
    }

    if (!found) {
        printf("Factura no encontrada\n");
    }

    fclose(file);
}

void deleteFactura(int cedula) {
    struct Factura factura;
    FILE *file = fopen("facturas", "rb");
    FILE *tempFile = fopen("temp", "wb");

    if (file == NULL || tempFile == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    int found = 0;
    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        if (factura.cedula != cedula) {
            fwrite(&factura, sizeof(struct Factura), 1, tempFile);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    

    if (found) {
        remove("facturas");
        rename("temp", "facturas");
        printf("Factura eliminada\n");
    } else {
        remove("temp");
        printf("Factura no encontrada\n");
    }
}

