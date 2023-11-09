#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_COMPRAS 50
#define MAX_VENTAS 50


struct MercaditoLibre
{
    struct NodoProveedores *headProveedores;
    struct NodoClientes *headClientes;
    struct listaProductos *productos;
    struct NodoBodegas *headBodegas;
    struct Inventario *datosInventario;
};

struct listaProductos
{
    int cantProductos;
    struct NodoProductos *abbProducto;
};

struct NodoProductos
{
    struct Productos *datosProducto;
    struct NodoProductos *izq, *der;
};

struct NodoProveedores
{
    struct Proveedores *datosProveedores;
    struct NodoProveedores *sig;

};

struct NodoClientes
{
    struct Cliente *datosCliente;
    struct NodoClientes *sig;

};

struct NodoBodegas
{
    struct Bodega *datosBodega;
    struct NodoBodegas *sig;

};

struct Proveedores
{
    int id;
    int vendido;
    int numProductos;
    char *nombre;
    struct Compra **ventasAlaEmpresa;

};

struct Cliente
{
    int id;
    char *nombre;
    int gastado;
    int numProductos;
    struct Compra **compras;
};

struct Bodega{
    int id;
    int capacidadProductos;
    char *direccion;
    struct Inventario **stockDeBodega;

};

//el struct inventario tiene un id ya que use el mismo struct tanto para el stock general//
//a la vez que para el de cada bodega por separado asi podremos diferenciarlos //

struct Inventario{
    int id;
    struct NodoProductos **headProductos;

};

struct Productos{
    int id;
    char *nombre;
    int precio;
    int stock;
    struct Flujo *flujoProducto;

};

//use un struct para el flujo asi podemos llevar la cant de compras y ventas de cada producto//
//a la vez que el flujo total del producto//

struct Flujo{
    int cantComprada;
    int cantVendida;
    int comprasTotales;
};

//el arreglo productosComprados es un arreglo con los id de cada producto de la compra//
struct Compra
{
    int codigoBoleta;
    int precioProducto;
    int idProducto;
    int cantProducto;
    char *nombre;
    char *fecha;
    char **productosComprados;

};

//aqui van las funciones//

int contarDigitos(int numero)
{
    int contador = 0;
    while (numero != 0) {
        numero = numero / 10;
        contador++;
    }
    return contador;
}

void convertirAMayusculas(char *cadena)
{
    int i = 0;
    while (cadena[i])
    {
        cadena[i] = toupper(cadena[i]);
        i++;
    }
}

int comprobarCliente (struct MercaditoLibre *mercado, int idIngresado)
{
    struct NodoClientes *rec = mercado->headClientes;
    while (rec!=NULL)
    {
        if (rec->datosCliente->id == idIngresado) return 1; //existe el cliente
        rec=rec->sig;
    }
    return 0; //no existe el cliente
}

int Comprobarproveedor (struct MercaditoLibre *mercado, int idIngresado)
{
    struct NodoProveedores *rec = mercado->headProveedores;
    while (rec!=NULL)
    {
        if (rec->datosProveedores->id == idIngresado) return 1; //existe el proveedor
        rec = rec->sig;
    }
    return 0; //no existe el proveedor
}

struct Cliente *crearCliente(struct MercaditoLibre *mercado)
{
    struct Cliente *nuevo;
    char nombre[50];
    nuevo=(struct Cliente *)malloc(sizeof(struct Cliente));

    if (nuevo == NULL)
    {
        printf("Error: No se pudo asignar memoria para el cliente.\n");
        exit(EXIT_FAILURE);
    }

    printf("Ingrese el ID del cliente: ");
    scanf("%d",&(nuevo->id));

    if(comprobarCliente(mercado,nuevo->id)==1)
    {
        printf("El cliente ya existe.\n");
        return NULL;
    }


    printf("Ingrese nombre del cliente: ");
    scanf("%s",nombre);

    nuevo->nombre=(char *)malloc(strlen(nombre)*sizeof(char));
    if (nuevo->nombre == NULL) {
        printf("Error: No se pudo asignar memoria para el nombre del cliente.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(nuevo->nombre,nombre);
    nuevo->gastado=0;
    nuevo->numProductos=0;
    nuevo->compras = (struct compra**)malloc(MAX_COMPRAS * sizeof(struct Compra*));


    return nuevo;
}

struct NodoClientes *crearNodo (struct Cliente *nueva)
{
    struct NodoClientes *nuevo=NULL;
    if(nueva != NULL)
    {
        nuevo=(struct NodoClientes *)malloc(sizeof(struct NodoClientes));
        if (nuevo == NULL) {
            printf("Error: No se pudo asignar memoria para el nodo del cliente.\n");
            exit(EXIT_FAILURE);
        }
        nuevo->datosCliente=nueva;
        nuevo->sig=NULL;
    }

    return nuevo;
}

void agregarCliente (struct MercaditoLibre *mercado,struct NodoClientes **head)
{
    struct NodoClientes *nuevo=NULL;
    struct NodoClientes *rec=NULL;
    nuevo=crearNodo(crearCliente(mercado));

    if(nuevo==NULL)return;

    if(*head == NULL)
    {
        *head=nuevo;
        printf("Cliente agregado con exito.\n\n");
        return;
    }

    else
    {
        rec=*head;
        while(rec->sig!=NULL)
        {
            rec=rec->sig;
        }

        rec->sig=nuevo;
        printf("Cliente agregado con exito.\n\n");
        return;
    }
}

struct Proveedores *crearProveedor(struct MercaditoLibre *mercado)
{
    struct Proveedores *nuevo;
    char buffer[50];
    nuevo=(struct Proveedores *)malloc(sizeof(struct Proveedores));

    if (nuevo == NULL)
    {
        printf("Error: No se pudo asignar memoria para el proveedor.\n");
        exit(EXIT_FAILURE);
    }

    printf("Ingrese el ID del proveedor: ");
    scanf("%d",&(nuevo->id));

    if(Comprobarproveedor(mercado,nuevo->id)==1)
    {
        printf("El proveedor ya existe.\n");
        return NULL;
    }


    printf("Ingrese nombre del proveedor: ");
    scanf("%s",buffer);

    nuevo->nombre=(char *)malloc(strlen(buffer)*sizeof(char));
    if (nuevo->nombre == NULL) {
        printf("Error: No se pudo asignar memoria para el nombre del proveedor.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(nuevo->nombre,buffer);
    nuevo->vendido=0;
    nuevo->numProductos=0;
    nuevo->ventasAlaEmpresa = (struct compra**)malloc(MAX_COMPRAS * sizeof(struct compra*));


    return nuevo;
}

struct NodoProveedores *nuevoNodo (struct Proveedores *nueva)
{
    struct NodoProveedores *nuevo=NULL;
    if(nueva != NULL)
    {
        nuevo=(struct NodoProveedores *)malloc(sizeof(struct NodoProveedores));
        if (nuevo == NULL) {
            printf("Error: No se pudo asignar memoria para el nodo del proveedor.\n");
            exit(EXIT_FAILURE);
        }
        nuevo->datosProveedores=nueva;
        nuevo->sig=NULL;
    }

    return nuevo;
}

void agregarProveedor (struct MercaditoLibre *mercado,struct NodoProveedores **head)
{
    struct NodoProveedores *nuevo=NULL;
    struct NodoProveedores *rec=NULL;
    nuevo=crearNodo(crearProveedor(mercado));

    if(nuevo==NULL)return;

    if(*head == NULL)
    {
        *head=nuevo;
        printf("Proveedor agregado con exito.\n\n");
        return;
    }

    else
    {
        rec=*head;
        while(rec->sig!=NULL)
        {
            rec=rec->sig;
        }

        rec->sig=nuevo;
        printf("Proveedor agregado con exito.\n\n");
        return;
    }
}

void eliminarCliente (struct MercaditoLibre *mercado, int idIngresado)
{
    if (mercado->headClientes == NULL)
    {
        printf("La lista de clientes esta vacia. No hay clientes para eliminar.\n\n");
        return;
    }

    if (comprobarCliente(mercado,idIngresado) == 0) //uso la función ya hecha para comprobar si el cliente NO se encuetra
    {
        printf("El cliente no se encuentra registrado.\n\n");//En caso de que el cliente no exista
        return;
    }

    struct NodoClientes *rec = mercado->headClientes;

    if(rec->datosCliente == NULL) return; //en caso de que no existan datos del cliente

    if (rec->datosCliente->id == idIngresado) //si es el head el cliente a eliminar
    {
        mercado->headClientes = rec->sig;
        printf("el cliente fue quitado con exito\n\n");
        return;
    }

    while(rec->sig != NULL)
    {
        if (rec->sig->datosCliente->id == idIngresado)
        {
            rec->sig= rec->sig->sig;
            printf("el cliente fue quitado con exito\n\n");
            return;
        }
        rec=rec->sig;
    }

}

void eliminarProveedor (struct MercaditoLibre *mercado, int idIngresado)
{
    if (mercado->headProveedores == NULL)
    {
        printf("La lista de proveedores esta vacia. No hay proveedores para eliminar.\n\n");
        return;
    }

    if (Comprobarproveedor(mercado,idIngresado) == 0) //uso la función ya hecha para comprobar si el cliente NO se encuetra
    {
        printf("El proveedor no se encuentra registrado.\n\n");//En caso de que el cliente no exista
        return;
    }

    struct NodoProveedores *rec = mercado->headProveedores;

    if(rec->datosProveedores == NULL) return; //en caso de que no existan datos del cliente

    if (rec->datosProveedores->id == idIngresado) //si es el head el cliente a eliminar
    {
        mercado->headProveedores = rec->sig;
        printf("el proveedor fue quitado con exito\n\n");
        return;
    }

    while(rec->sig != NULL)
    {
        if (rec->sig->datosProveedores->id == idIngresado)
        {
            rec->sig= rec->sig->sig;
            printf("el proveedor fue quitado con exito\n\n");
            return;
        }
        rec=rec->sig;
    }

}

void modificarCliente (struct MercaditoLibre *mercado, int idIngresado)
{
    if (mercado->headClientes == NULL)
    {
        printf("La lista de clientes esta vacia. No hay clientes para modificar.\n\n");
        return;
    }

    char nuevoNombre[50];
    struct NodoClientes *rec = mercado->headClientes;
    struct Cliente *clienteEncontrado = NULL;

    while (rec != NULL)
    {
        if (rec->datosCliente->id == idIngresado)
        {
            clienteEncontrado = rec->datosCliente;
            convertirAMayusculas(clienteEncontrado->nombre);
            printf("    ## CLIENTE ENCONTRADO ##    \n");
            printf(" -------------------------------\n");
            printf("|%-*s|%-*s|\n",+ 15, "Nombre",+ 15, "ID");
            printf(" -------------------------------\n");
            printf("|%-*s|%-*d|\n", -15, clienteEncontrado->nombre, -15, clienteEncontrado->id);
            printf(" -------------------------------\n");
            printf("\n");

            printf("Ingrse el nuevo nombre: ");
            scanf("%s",nuevoNombre);
            clienteEncontrado->nombre = (char*)malloc(strlen(nuevoNombre) + 1); // Asignar memoria para el nuevo nombre
            if (clienteEncontrado->nombre != NULL)
            {
                strcpy(clienteEncontrado->nombre, nuevoNombre);
                printf("Cliente con ID %d modificado correctamente.\n\n", idIngresado);
                return;
            }
            else
            {
                printf("Error: No se pudo asignar memoria para el nombre del producto.\n\n");
                return;
            }
        }
        rec = rec->sig;
    }

    if (clienteEncontrado == NULL)
    {
        printf("El cliente no se encuentra registrado.\n\n");
        return;
    }

}

void modificarProveedor (struct MercaditoLibre *mercado, int idIngresado)
{
    if (mercado->headProveedores == NULL)
    {
        printf("La lista de proveedores esta vacia. No hay proveedores para modificar.\n\n");
        return;
    }

    char nuevoNombre[50];
    struct NodoProveedores *rec = mercado->headProveedores;
    struct  Proveedores *proveedorEncontrado = NULL;

    while (rec != NULL)
    {
        if (rec->datosProveedores->id == idIngresado)
        {
            proveedorEncontrado = rec->datosProveedores;
            convertirAMayusculas(proveedorEncontrado->nombre);
            printf("   ## PROVEEDOR ENCONTRADO ##  \n");
            printf(" -------------------------------\n");
            printf("|%-*s|%-*s|\n",+ 15, "Nombre",+ 15, "ID");
            printf(" -------------------------------\n");
            printf("|%-*s|%-*d|\n", -15, proveedorEncontrado->nombre, -15, proveedorEncontrado->id);
            printf(" -------------------------------\n");
            printf("\n");

            printf("Ingrse el nuevo nombre: ");
            scanf("%s",nuevoNombre);
            proveedorEncontrado->nombre = (char*)malloc(strlen(nuevoNombre) + 1); // Asignar memoria para el nuevo nombre
            if (proveedorEncontrado->nombre != NULL)
            {
                strcpy(proveedorEncontrado->nombre, nuevoNombre);
                printf("Proveedor con ID %d modificado correctamente.\n\n", idIngresado);
                return;
            }
            else
            {
                printf("Error: No se pudo asignar memoria para el nombre del producto.\n\n");
                return;
            }
        }
        rec = rec->sig;
    }

    if (proveedorEncontrado == NULL)
    {
        printf("El proveedor no se encuentra registrado.\n\n");
        return;
    }

}

void modificarProducto(struct NodoProductos* nodo, int idProducto)
{
    char nuevoNombre[50];
    int nuevoPrecio,nuevoStock;

    if (nodo == NULL)
    {
        printf("Producto con ID %d no encontrado. No se puede modificar.\n\n", idProducto);
        return;
    }

    if (idProducto < nodo->datosProducto->id)
    {
        modificarProducto(nodo->izq, idProducto);
    }
    else if (idProducto > nodo->datosProducto->id)
    {
        modificarProducto(nodo->der, idProducto);
    }
    else
    {
        int opcion;
        // Modificar el precio y el nombre del producto
        printf("\n                 ## PRODUCTO ENCONTRADO ##\n");
        printf(" ----------------------------------------------------------------\n");
        printf("|%-*s|%-*s|%-*s|%-*s|\n",+ 15, "Nombre",+ 15, "ID",+ 15, "Precio",+ 15, "Stock");
        printf(" ----------------------------------------------------------------\n");
        printf("|%-*s|%-*d|%-*d|%-*d|\n", -15, nodo->datosProducto->nombre, -15, nodo->datosProducto->id, -15, nodo->datosProducto->precio, -15, nodo->datosProducto->stock);
        printf(" ----------------------------------------------------------------\n\n");

        printf(" ------------------------ \n");
        printf("|Que desea modificar     |\n");
        printf("|------------------------|\n");
        printf("|1 Nombre                |\n");
        printf("|------------------------|\n");
        printf("|2 Precio                |\n");
        printf("|------------------------|\n");
        printf("|3 Stock                 |\n");
        printf(" ------------------------\n\n");
        printf("Ingrese su respuesta: ");
        scanf("%d", &opcion);
        printf("\n");

        if(opcion == 1)
        {
            printf("Ingrse el nuevo nombre: ");
            scanf("%s",nuevoNombre);
            nodo->datosProducto->nombre = (char*)malloc(strlen(nuevoNombre) + 1); // Asignar memoria para el nuevo nombre
            if (nodo->datosProducto->nombre != NULL)
            {
                strcpy(nodo->datosProducto->nombre, nuevoNombre);
                printf("Producto con ID %d modificado correctamente.\n\n", idProducto);
                return;
            }
            else
            {
                printf("Error: No se pudo asignar memoria para el nombre del producto.\n\n");
                return;
            }
        }

        else if (opcion == 2)
        {
            printf("Ingrese nuevo precio: ");
            scanf("%d",&nuevoPrecio);
            nodo->datosProducto->precio = nuevoPrecio;
            printf("Producto con ID %d modificado correctamente.\n\n", idProducto);
        }

        else if (opcion == 3)
        {
            printf("Ingrese nuevo stock: ");
            scanf("%d",&nuevoStock);
            nodo->datosProducto->stock = nuevoStock;
            printf("Producto con ID %d modificado correctamente.\n\n", idProducto);
        }
        else
        {
            printf("Error: opcion invalida.\n\n");
            return;
        }
    }
}

void modificarProductoabb(struct MercaditoLibre* mercado, int idProducto)
{
    modificarProducto(mercado->productos->abbProducto, idProducto);
}

void agregarCant (struct NodoProductos *abb, int idIngresado)
{

    if (abb != NULL && abb->datosProducto != NULL)
    {
        agregarCant(abb->izq, idIngresado);
        if (abb->datosProducto->id == idIngresado)
        {
            printf("\nProducto con ID %d encontrado.\n",idIngresado);
            int cantidad=0;
            printf("Ingrese la cantidad que desea agregar: ");
            scanf("%d",&cantidad);
            abb->datosProducto->stock += cantidad;
            printf("\nCantidad del producto con ID %d y nombre %s incrementada a %d.\n\n", idIngresado,abb->datosProducto->nombre, abb->datosProducto->stock);
            return;
        }
        agregarCant(abb->der, idIngresado);
    }
    else
    {
        return;
    }
}

void mostrarClientes(struct NodoClientes *headClientes)
{
    if (headClientes==NULL)
    {
        printf("\nNo hay clientes para mostrar.\n\n");
        return;
    }
    printf("   ## LISTA DE CLIENTES ##\n");
    printf(" -------------------------------\n");
    printf("|%-*s|%-*s|\n",+ 15, "Nombre",+ 15, "ID");
    printf(" -------------------------------\n");
    struct NodoClientes *rec = headClientes;
    while(rec != NULL)
    {
        printf("|%-*s|%-*d|\n", -15, rec->datosCliente->nombre, -15, rec->datosCliente->id);
        rec = rec->sig;
    }
    printf(" -------------------------------\n");
    printf("\n");
}

void mostrarProveedores(struct NodoProveedores *headProveedores)
{
    if(headProveedores == NULL)
    {
        printf("\nNo hay proveedores para mostrar.\n\n");
        return;
    }
    printf("   ## LISTA DE PROVEEDORES ##\n");
    printf(" -------------------------------\n");
    printf("|%-*s|%-*s|\n",+ 15, "Nombre",+ 15, "ID");
    printf(" -------------------------------\n");
    struct NodoProveedores *rec = headProveedores;
    while(rec != NULL)
    {
        printf("|%-*s|%-*d|\n", -15, rec->datosProveedores->nombre, -15, rec->datosProveedores->id);
        rec = rec->sig;
    }
    printf(" -------------------------------\n");
    printf("\n");
}

void mostrarComprasClientes(struct MercaditoLibre *mercado, int idCliente)
{
    struct NodoClientes *rec = mercado->headClientes;

    if (rec == NULL)
    {
        printf("La lista de clientes esta vacia.\n\n");
        return;
    }

    while (rec != NULL)
    {
        if(rec->datosCliente->id == idCliente)
        {
            printf("Cliente ID: %d\n", rec->datosCliente->id);
            printf("Nombre cliente: %s\n\n",rec->datosCliente->nombre);
            if (rec->datosCliente->numProductos > 0)
            {
                printf("                    ## LISTA DE PRODUCTOS ##\n");
                printf(" ----------------------------------------------------------------\n");
                printf("|%-*s|%-*s|%-*s|%-*s|\n",+ 15, "Nombre",+ 15, "ID",+ 15, "Precio",+ 15, "Cantidad ");
                printf(" ----------------------------------------------------------------\n");
                for (int i = 0; i < rec->datosCliente->numProductos; i++)
                {
                    printf("|%-*s|%-*d|%-*d|%-*d|\n", -15, rec->datosCliente->compras[i]->nombre, -15, rec->datosCliente->compras[i]->idProducto, -15, rec->datosCliente->compras[i]->precioProducto, -15, rec->datosCliente->compras[i]->cantProducto);
                }
                printf(" ----------------------------------------------------------------\n");
                printf("\n");
                printf(" --------------------------------------------------\n");
                printf("| MONTO TOTAL GASTADO DEL CLIENTE: %-15d |\n", rec->datosCliente->gastado);
                printf(" --------------------------------------------------\n\n");
                return;
            }
            else
            {
                printf("El cliente no ha realizado compras.\n\n");
                return;
            }
        }
        rec = rec->sig;
    }

    printf("El cliente no esta registrado.\n\n");
    return;

}

void mostrarProductosRecursivo (struct NodoProductos *abb)
{
    if (abb==NULL)
    {
        return;
    }
    mostrarProductosRecursivo(abb->izq);
    // Luego mostramos el producto en este nodo
    printf("|%-*s|%-*d|%-*d|%-*d|\n", -15, abb->datosProducto->nombre, -15, abb->datosProducto->id, -15, abb->datosProducto->precio, -15, abb->datosProducto->stock);
    // Finalmente, recorremos el subárbol derecho
    mostrarProductosRecursivo(abb->der);
}

void mostrarProductos(struct NodoProductos *abb)
{
    if (abb==NULL)
    {
        printf("NO hay productos disponibles.\n\n");
        return;
    }

    // Encabezado del cuadrado
    printf("                    ## LISTA DE PRODUCTOS ##\n");
    printf(" ----------------------------------------------------------------\n");
    printf("|%-*s|%-*s|%-*s|%-*s|\n",+ 15, "Nombre",+ 15, "ID",+ 15, "Precio",+ 15, "Stock");
    printf(" ----------------------------------------------------------------\n");

    // Iterar sobre los productos y mostrar los detalles en un cuadrado simétrico
    mostrarProductosRecursivo(abb);
    printf(" ----------------------------------------------------------------\n");
    printf("\n");

}

void mostrarClienteQueMasGasto(struct MercaditoLibre *mercado)
{
    struct NodoClientes *rec = mercado->headClientes;

    if (rec == NULL) {
        printf("La lista de clientes esta vacia.\n\n");
        return;
    }

    struct Cliente *clienteMayor = rec->datosCliente;
    while (rec != NULL) {
        if (rec->datosCliente->gastado > clienteMayor->gastado)
        {
            clienteMayor = rec->datosCliente;
        }
        rec = rec->sig;
    }
    printf("## CLIENTE QUE MAS HA GASTADO ##\n");
    printf(" -------------------------------\n");
    printf("|%-*s|%-*s|\n",+ 15, "Nombre",+ 15, "ID");
    printf(" -------------------------------\n");
    printf("|%-*s|%-*d|\n", -15, clienteMayor->nombre, -15, clienteMayor->id);
    printf(" -------------------------------\n");
    printf("\n");
    printf(" --------------------------------------------------\n");
    printf("| MONTO TOTAL GASTADO DEL CLIENTE: %-15d |\n",clienteMayor->gastado);
    printf(" --------------------------------------------------\n\n");
    return;
}
/*
int ProveedorMayorMonto(struct MercaditoLibre *mercado)
{
    struct NodoProveedores *rec = mercado->headProveedores;
    int idProveedor = -1; //inicializamos en negativo para el caso de no haber proveedor//
    int MayorMonto = 0;
    while (rec!= NULL)
    {
        int MontoTotal = 0;
        if (rec->datosProveedores->ventasAlaEmpresa != NULL)
        {
            for (int i = 0; i < rec->datosProveedores->pLibreVentas;i++)
            {
                MontoTotal += rec->datosProveedores->ventasAlaEmpresa[i]->monto;
            }
        }
        if (MontoTotal > MayorMonto)
        {
            MayorMonto = MontoTotal;
            idProveedor = rec->datosProveedores->id;
        }
        rec = rec->sig;
    }
    return idProveedor;
}*/

struct NodoProductos* buscarProducto (struct NodoProductos *abb, int id)
{
    if (abb == NULL || abb->datosProducto == NULL) return NULL;

    if (id < abb->datosProducto->id)
    {
        return buscarProducto(abb->izq, id);
    }
    else if (id > abb->datosProducto->id)
    {
        return buscarProducto(abb->der, id);
    }
    else
    {
        return abb;
    }
}

void agregarCarrito(struct MercaditoLibre *mercado, int idCliente, int idProducto, int cant)
{
    struct NodoClientes *rec = mercado->headClientes;

    if (rec == NULL) {
        printf("La lista de clientes esta vacia.\n\n");
        return;
    }

    while (rec != NULL) {
        if (rec->datosCliente->id == idCliente)
        {
            struct NodoProductos *productoEncontrado = buscarProducto(mercado->productos->abbProducto, idProducto);

            if (productoEncontrado != NULL)
            {
                rec->datosCliente->gastado += (productoEncontrado->datosProducto->precio * cant);
                rec->datosCliente->numProductos+=1;
                if (rec->datosCliente->numProductos <= MAX_COMPRAS)
                {
                    int indice = rec->datosCliente->numProductos - 1;
                    rec->datosCliente->compras[indice] = (struct Compra*)malloc(sizeof(struct Compra));
                    if (rec->datosCliente->compras[indice] == NULL)
                    {
                        printf("Error: No se pudo asignar memoria para la compra.\n");
                        return;
                    }
                    else
                    {
                        rec->datosCliente->compras[indice]->nombre = (char*)malloc(strlen(productoEncontrado->datosProducto->nombre) + 1);
                        if (rec->datosCliente->compras[indice]->nombre == NULL)
                        {
                            printf("Error: No se pudo asignar memoria para el nombre del producto.\n");
                            return;
                        }
                        else
                        {
                            rec->datosCliente->compras[indice]->idProducto = 0;
                            rec->datosCliente->compras[indice]->precioProducto = 0;
                            rec->datosCliente->compras[indice]->cantProducto=0;
                        }
                    }
                    strcpy(rec->datosCliente->compras[indice]->nombre, productoEncontrado->datosProducto->nombre);
                    rec->datosCliente->compras[indice]->idProducto = idProducto;
                    rec->datosCliente->compras[indice]->precioProducto = productoEncontrado->datosProducto->precio;
                    rec->datosCliente->compras[indice]->cantProducto=cant;
                    printf("Producto agregado al carrito correctamente.\n");
                    printf("TOTAL GASTADO POR CLIENTE = %d\n\n", rec->datosCliente->gastado);
                    return;
                }
                else
                {
                    printf("Error: El carrito del cliente está lleno. No se pueden agregar más productos.\n\n");
                    return;
                }
            }
            else
            {
                printf("Producto no encontrado.\n\n");
                return;
            }
        }
        rec = rec->sig;
    }

    printf("Cliente no encontrado.\n\n");
    return;
}

void venderProducto(struct MercaditoLibre *mercado, int idIngresado)
{
    int producto, cant = 0;

    struct NodoProductos *recP = mercado->productos->abbProducto;
    struct NodoClientes *recC = mercado->headClientes;
    struct Cliente *clienteEncontrado = NULL;

    while (recC != NULL)
    {
        if (recC->datosCliente->id == idIngresado)
        {
            clienteEncontrado = recC->datosCliente;
            break;
        }
        recC = recC->sig;
    }

    if (clienteEncontrado == NULL)
    {
        printf("El cliente no se encuentra registrado.\n\n");
        return;
    }
    printf("## CLIENTE ENCONTRADO ##\n");
    convertirAMayusculas(clienteEncontrado->nombre);
    printf("NOMBRE CLIENTE : %s\n\n",clienteEncontrado->nombre);

    printf("Ingrese el ID del producto: \n");
    scanf("%d", &producto);
    printf("\n");

    struct NodoProductos *productoEncontrado = buscarProducto(recP, producto);
    if (productoEncontrado != NULL)
    {
        if(productoEncontrado->datosProducto->stock == 0)
        {
            printf("\n        PRODUCTO AGOTADO           \n");
            printf("CONSULTE SOBRE LLEGADA DE NUEVO STOCK\n");
            printf("     TELEFONO: +56947901390\n\n");
            return;
        }
        printf("                    ## PRODUCTO ENCONTRADO ##\n");
        printf(" ----------------------------------------------------------------\n");
        printf("|%-*s|%-*s|%-*s|%-*s|\n",+ 15, "Nombre",+ 15, "ID",+ 15, "Precio",+ 15, "Stock");
        printf(" ----------------------------------------------------------------\n");
        printf("|%-*s|%-*d|%-*d|%-*d|\n", -15, productoEncontrado->datosProducto->nombre, -15, productoEncontrado->datosProducto->id, -15, productoEncontrado->datosProducto->precio, -15, productoEncontrado->datosProducto->stock);
        printf(" ----------------------------------------------------------------\n\n");

        do
        {
            printf("Ingrese la cantidad que desea comprar: \n");
            scanf("%d", &cant);
            if(cant==0)
            {
                printf("No se ha vendido ningun producto\n\n");
                return;
            }
            if (cant > productoEncontrado->datosProducto->stock || cant <= 0)
            {
                printf("La cantidad a comprar no es valida o supera el stock disponible.\n");
            }

        } while (cant > productoEncontrado->datosProducto->stock || cant < 0);

        productoEncontrado->datosProducto->stock -= cant;
        if(productoEncontrado->datosProducto->stock==0)
        {
            printf("\n\n\n                   !! PRECAUCION !!            \n");
            printf("            PRODUCTO AGOTADO PARA VENTAS FUTURAS \n");
            printf("POR FAVOR CONTACTARSE CON UN PROVEEDOR PARA REABASTECER EL STOCK\n\n\n");
        }

        if(productoEncontrado->datosProducto->stock<=10 && productoEncontrado->datosProducto->stock >=1)
        {
            printf("\n\n\n                      !! PRECAUCION !!            \n");
            printf("                   QUEDAN POCAS UNIDADES  \n");
            printf("POR FAVOR CONTACTARSE CON UN PROVEEDOR PARA REABASTECER EL STOCK\n\n\n");
        }

        if (recC->datosCliente != NULL)
        {
            agregarCarrito (mercado, idIngresado ,productoEncontrado->datosProducto->id,cant);
            return;
        }
        else
        {
            printf("Error: No existe el cliente.\n\n");
        }
    }
    else
    {
        printf("Producto no encontrado.\n\n");
    }
}


struct NodoProductos * insertarProductos(struct NodoProductos* abb, struct Productos* nuevoProducto)
{
    if (abb == NULL)
    {
        struct NodoProductos* nuevoNodo = (struct NodoProductos*)malloc(sizeof(struct NodoProductos));
        nuevoNodo->datosProducto = nuevoProducto;
        nuevoNodo->izq = NULL;
        nuevoNodo->der = NULL;
        return nuevoNodo;
    }

    if (nuevoProducto->id < abb->datosProducto->id)
    {
        abb->izq = insertarProductos(abb->izq, nuevoProducto);
    }
    else if (nuevoProducto->id > abb->datosProducto->id)
    {
        abb->der = insertarProductos(abb->der, nuevoProducto);
    }

    return abb;
}

void agregarProducto(struct MercaditoLibre *mercado)
{
    struct Productos *nuevoProducto = (struct Productos*)malloc(sizeof(struct Productos));
    nuevoProducto->nombre = (char*)malloc(30 * sizeof(char));
    int prov=0;
    if (nuevoProducto == NULL)
    {
        printf("Error de memoria");
        return;
    }

    if (mercado->headProveedores==NULL)
    {
        printf("No hay proveedores para comprar.\n\n");
        return;
    }
    else
    {
        mostrarProveedores(mercado->headProveedores);
        printf("\nIngrese el ID del proveedor al que se le comprara: \n");
        scanf("%d", &prov);
        struct NodoProveedores *recP=mercado->headProveedores;
        struct Proveedores *proveedorEncontrado = NULL;
        while (recP != NULL)
        {
            if (recP->datosProveedores->id == prov)
            {
                proveedorEncontrado = recP->datosProveedores;
                break;
            }
            recP = recP->sig;
        }
        if (proveedorEncontrado == NULL)
        {
            printf("El proveedor no se encuentra registrado.\n\n");
            return;
        }
        convertirAMayusculas(proveedorEncontrado->nombre);
        printf("NOMBRE PROVEEDOR : %s\n",proveedorEncontrado->nombre);
    }

    printf("\nIngrese el ID del producto: ");
    scanf("%d", &(nuevoProducto->id));

    if (buscarProducto(mercado->productos->abbProducto, nuevoProducto->id) != NULL)
    {
        agregarCant(mercado->productos->abbProducto,nuevoProducto->id);
        return;
    }
    printf("Ingrese el nombre del producto: ");
    scanf("%s", nuevoProducto->nombre);
    printf("Ingrese el precio del producto: ");
    scanf("%d",&(nuevoProducto->precio));
    printf("Ingrese el stock del producto: ");
    scanf("%d",&(nuevoProducto->stock));
    if(nuevoProducto->stock == 0)
    {
        printf("\n## PRODUCTOS CON STOCK 0 NO PUEDEN SER AGREGADOS ##\n\n");
        return;
    }

    mercado->productos->abbProducto = insertarProductos(mercado->productos->abbProducto, nuevoProducto);

    printf("\nProducto agregado con exito\n\n");
    return;
}

struct NodoProductos* encontrarNodoMinimo(struct NodoProductos* nodo)
{
    struct NodoProductos* actual = nodo;
    while (actual->izq != NULL) {
        actual = actual->izq;
    }
    return actual;
}

struct NodoProductos* eliminarNodoABB(struct NodoProductos* nodo, int idProducto)
{
    if (nodo == NULL)
    {
        return nodo;
    }

    if (idProducto < nodo->datosProducto->id)
    {
        nodo->izq = eliminarNodoABB(nodo->izq, idProducto);
    }
    else if (idProducto > nodo->datosProducto->id)
    {
        nodo->der = eliminarNodoABB(nodo->der, idProducto);
    }
    else
    {
        // Nodo sin hijos o con un solo hijo
        if (nodo->izq == NULL)
        {
            struct NodoProductos* temp = nodo->der;
            return temp;
        }
        else if (nodo->der == NULL)
        {
            struct NodoProductos* temp = nodo->izq;
            return temp;
        }

        // Nodo con dos hijos
        struct NodoProductos* temp = encontrarNodoMinimo(nodo->der);
        nodo->datosProducto->id = temp->datosProducto->id;
        nodo->datosProducto->nombre = temp->datosProducto->nombre;
        nodo->datosProducto->precio = temp->datosProducto->precio;
        nodo->der = eliminarNodoABB(nodo->der, temp->datosProducto->id);
    }
    return nodo;
}

void eliminarProducto(struct MercaditoLibre* mercado, int idProducto)
{
    mercado->productos->abbProducto = eliminarNodoABB(mercado->productos->abbProducto, idProducto);
    printf("Producto con ID %d eliminado correctamente.\n\n", idProducto);
}

int calcularTotalGastado(struct MercaditoLibre *mercado)
{
    int totalGastado = 0;
    struct NodoClientes *rec = mercado->headClientes;

    while (rec != NULL) {
        totalGastado += rec->datosCliente->gastado;
        rec = rec->sig;
    }

    return totalGastado;
}


int main()
{
    struct MercaditoLibre *mercado = (struct MercaditoLibre *)malloc(sizeof(struct MercaditoLibre));
    mercado->headClientes = NULL;
    mercado->headProveedores = NULL;
    mercado->productos = (struct listaProductos *)malloc(sizeof(struct listaProductos));
    mercado->productos->abbProducto = NULL;

    int opcion1, opcion2,opcion3;
    int id, precio;
    char nombre[50];

    do
    {
        printf("#####################################\n");
        printf("## Hola, elige lo que deseas hacer ##\n");
        printf("## ------------------------------- ##\n");
        printf("## Opcion 1: Agregar               ##\n");
        printf("## ------------------------------- ##\n");
        printf("## Opcion 2: Quitar                ##\n");
        printf("## ------------------------------- ##\n");
        printf("## Opcion 3: Modificar             ##\n");
        printf("## ------------------------------- ##\n");
        printf("## Opcion 4: Mostrar               ##\n");
        printf("## ------------------------------- ##\n");
        printf("## Opcion 5: Vender                ##\n");
        printf("## ------------------------------- ##\n");
        printf("## Opcion 6: Informe               ##\n");
        printf("## ------------------------------- ##\n");
        printf("## Opcion 0: Salir                 ##\n");
        printf("#####################################\n\n");
        printf("Ingrese su respuesta: \n");
        scanf("%d", &opcion1);

        switch (opcion1)
        {
            case 1:
                system("cls");
                printf(" ------------------------ \n");
                printf("|Que desea agregar       |\n");
                printf("|------------------------|\n");
                printf("|1 Clientes              |\n");
                printf("|------------------------|\n");
                printf("|2 Proveedores           |\n");
                printf("|------------------------|\n");
                printf("|3 Comprar Productos     |\n");
                printf("|------------------------|\n");
                printf("|4 Volver                |\n");
                printf(" ------------------------ \n");
                printf("Ingrese su respuesta: ");
                scanf("%d", &opcion2);
                printf("\n");

                if (opcion2 == 1)
                {
                    system("cls");
                    agregarCliente(mercado,&(mercado->headClientes));
                }
                else if (opcion2 == 2)
                {
                    system("cls");
                    agregarProveedor(mercado,&(mercado->headProveedores));
                }
                else if (opcion2 == 3)
                {
                    system("cls");
                    agregarProducto(mercado);
                }
                else if (opcion2 == 4)
                {
                    system("cls");
                }
                else
                {
                    system("cls");
                }
                break;

            case 2:
                system("cls");
                printf(" ------------------------ \n");
                printf("|Que desea quitar        |\n");
                printf("|------------------------|\n");
                printf("|1 Clientes              |\n");
                printf("|------------------------|\n");
                printf("|2 Proveedores           |\n");
                printf("|------------------------|\n");
                printf("|3 Productos             |\n");
                printf("|------------------------|\n");
                printf("|4 Volver                |\n");
                printf(" ------------------------ \n");
                printf("Ingrese su respuesta: ");
                scanf("%d", &opcion2);
                printf("\n");

                if (opcion2 == 1)
                {
                    system("cls");
                    printf("\nIngrese el id del cliente que desea quitar: ");
                    scanf("%d", &id);
                    eliminarCliente(mercado, id);
                }
                else if (opcion2 == 2)
                {
                    system("cls");
                    printf("\nIngrese el id del proveedor que desea quitar: ");
                    scanf("%d", &id);
                    eliminarProveedor(mercado,id);
                }
                else if (opcion2 == 3)
                {
                    system("cls");
                    printf("\nIngrese el id del producto que desea eliminar: ");
                    scanf("%d", &id);
                    eliminarProducto(mercado,id);
                }
                else if (opcion2 == 4)
                {
                    system("cls");
                }
                else
                {
                    system("cls");
                }
                break;

            case 3:
                system("cls");
                printf(" ------------------------ \n");
                printf("|Que desea modificar     |\n");
                printf("|------------------------|\n");
                printf("|1 Clientes              |\n");
                printf("|------------------------|\n");
                printf("|2 Proveedores           |\n");
                printf("|------------------------|\n");
                printf("|3 Productos             |\n");
                printf("|------------------------|\n");
                printf("|4 Volver                |\n");
                printf(" ------------------------ \n");
                printf("Ingrese su respuesta: ");
                scanf("%d", &opcion2);
                printf("\n");

                if (opcion2 == 1)
                {
                    system("cls");
                    printf("\nIngrese el id del cliente que desea modificar: ");
                    scanf("%d", &id);
                    if (comprobarCliente(mercado,id)==0)
                    {
                        printf("El cliente no existe.\n\n");
                    }
                    else
                    {
                        modificarCliente(mercado, id);
                    }
                }
                else if (opcion2 == 2)
                {
                    system("cls");
                    printf("\nIngrese el id del proveedor que desea modificar: ");
                    scanf("%d", &id);
                    if (Comprobarproveedor(mercado,id)==0)
                    {
                        printf("El proveedor no existe.\n\n");
                    }
                    else
                    {
                        modificarProveedor(mercado, id);
                    }
                }
                else if (opcion2 == 3)
                {
                    system("cls");
                    printf("\nIngrese el id del producto que desea modificar: ");
                    scanf("%d", &id);
                    if(buscarProducto(mercado->productos->abbProducto,id)!=NULL)
                    {
                        modificarProductoabb(mercado,id);
                    }
                    else
                    {
                        printf("El producto no existe.\n\n");
                    }
                }
                else if (opcion2 == 4)
                {
                    system("cls");
                }
                else
                {
                    system("cls");
                }
                break;

            case 4:
                system("cls");
                printf(" ------------------------ \n");
                printf("|Que desea ver           |\n");
                printf("|------------------------|\n");
                printf("|1 Clientes              |\n");
                printf("|------------------------|\n");
                printf("|2 Proveedores           |\n");
                printf("|------------------------|\n");
                printf("|3 Productos             |\n");
                printf("|------------------------|\n");
                printf("|4 Volver                |\n");
                printf(" ------------------------ \n");
                printf("Ingrese su respuesta: ");
                scanf("%d", &opcion2);
                printf("\n");

                if (opcion2 == 1)
                {
                    system("cls");
                    printf(" ------------------------ \n");
                    printf("|Que desea ver           |\n");
                    printf("|------------------------|\n");
                    printf("|1 Lista de clientes     |\n");
                    printf("|------------------------|\n");
                    printf("|2 Compras de un cliente |\n");
                    printf(" ------------------------ \n");
                    printf("Ingrese su respuesta: ");
                    scanf("%d", &opcion3);
                    printf("\n");
                    if(opcion3 == 1)
                    {
                        system("cls");
                        mostrarClientes(mercado->headClientes);
                    }
                    else if (opcion3 == 2)
                    {
                        system("cls");
                        if(mercado->headClientes == NULL)
                        {
                            printf("La lista de clientes esta vacia.\n\n");
                        }
                        else
                        {
                            printf("Ingrese el id del cliente: ");
                            scanf("%d",&id);
                            mostrarComprasClientes(mercado,id);
                        }
                    }
                }
                else if (opcion2 == 2)
                {
                    system("cls");
                    mostrarProveedores(mercado->headProveedores);
                }
                else if (opcion2 == 3)
                {
                    system("cls");
                    mostrarProductos(mercado->productos->abbProducto);
                }
                else if (opcion2 == 4)
                {
                    system("cls");
                }
                else
                {
                    system("cls");
                }
                break;

            case 5:
                system("cls");
                if(mercado->headClientes == NULL)
                {
                    printf("La lista de clientes esta vacia.\n\n");
                }
                else
                {
                    printf("Ingrese el ID del cliente: ");
                    scanf("%d",&id);
                    venderProducto(mercado,id);
                }
                break;

            case 6:
                system("cls");
                mostrarClienteQueMasGasto(mercado);
                break;

            case 0:
                system("cls");
                printf("## GRACIAS POR PREFERIRNOS ##");
                break;


            default:
                system("cls");
                printf("\nOpcion no valida. Por favor, elija una opcion valida.\n\n");
                break;
        }

    } while (opcion1 != 0);

    return 0;
}