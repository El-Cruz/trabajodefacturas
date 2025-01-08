struct Producto
{
    char nombre[50];
    int cantidad;
    float precio;
};

struct Factura{
    char nombre[50];
    int cedula;
    int numProd;
    struct Producto productos[5];
    float total;
    int estado;
};

int menu();
void line();
void save(struct Factura *factura);
void readCadena(char *cadena, int numcaracteres);
void createFactura();
void readFactura();
void editFactura();
void deleteFactura(int cedula);
int findFacturaByCedula(int cedula);


