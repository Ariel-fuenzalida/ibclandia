#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* APARTADO DE ESTRUCTURAS */

struct ticket {
    int codUnico;
    int valor;
    int fechaVencimiento;
    char *estado;
    char *tipoTicket;
};

struct cliente {
    int edad;
    float altura;
    struct ticket **entradas;
    int cantEntradas;
};

struct nodoCliente {
    struct cliente *datos;
    struct nodoCliente *sig;
};

struct atraccion {
    char *nombre;
    char *estado;
    struct nodoCliente *headActivo;
    int capMax;
    int visitasTotales;
    int edadMinima;
    float alturaMinima;
    struct nodoCliente *headEspera;
    struct nodoCliente *ultimoEspera;
    float duracion;
};

struct nodoAtraccion {
    struct atraccion *datos;
    struct nodoAtraccion *sig;
};

struct zonaTematica {
    char *nombre;
    struct nodoAtraccion *head;
    int activos;
};

struct nodoZonaTematica {
    struct zonaTematica *datos;
    struct nodoZonaTematica *izq, *der;
};

struct Parque {
    struct nodoZonaTematica *raiz;
    int clientesActivos;
    int recaudacion;
    int capacidadMaxima;
};

/*PROTOTIPOS DE FUNCIONES*/

/* APARTADO DE TICKETS */

struct cliente *ticketUtilizado(struct cliente *cliente);
int ticketContado(int *codigos, int cant, int codigo);
struct ticket *vendido(struct cliente *cliente);
int validarFecha(struct ticket *ticket, int fechaActual);
int anularTicket(struct cliente *cliente, int codigoTicket);
int entradasUtilizadas(struct nodoCliente *Clientes);
int registrarEntradaParque(struct cliente *cliente, struct Parque *parque, int fechaActual);
int registrarSalida(struct Parque *parque);
int eliminarTicket(struct cliente *cliente, int codigoTicket);
int montoRecaudacion(struct nodoCliente *Clientes, struct Parque *parque);


/* CREACIÓN Y ENLACE DE NODOS Y DATOS */

struct nodoCliente *crearNodoCliente(struct cliente *cliente);
struct cliente *crearCliente(int edad, float Estatura, int CantEntradas);
struct zonaTematica *crearZona(char *nombre);
struct nodoZonaTematica *crearNodoZona(struct zonaTematica *zonaTematica);
struct nodoZonaTematica *insertarZona(struct nodoZonaTematica *raiz, struct zonaTematica *zonaTematica);
struct nodoAtraccion *crearNodoAtraccion(struct atraccion *atraccion);
struct atraccion *crearAtraccion(char *nombreAtraccion, int capacidad, int tiempo, char *estado, int edadMinima, float AlturaMinima);
int enlazarAtraccion(struct nodoAtraccion **ListaAtraccion, struct atraccion *nuevaAtraccion);
int enlazarCliente(struct nodoCliente **listaClientes, struct cliente *cliente);
struct ticket *crearTciket(int codigo, int valor, int fechaVencimiento, char* estado, char *tipoTicket);
int agregarTicket(struct cliente *cliente, struct ticket *ticket, int posicion);


/* APARTADO DE ZONAS TEMÁTICAS*/

struct zonaTematica *buscarZonaTematica(struct nodoZonaTematica *raiz, char *nombre);

/* APARTADO DE ATRACCIONES */

int registrarAtraccion(struct nodoZonaTematica *raiz, char *NombreZonaTematica, char *nombreAtraccion, char *estado, int capacidad, int tiempo, int edadMinima, float AlturaMinima);
struct atraccion *buscarAtraccion(struct zonaTematica *zonaTematica, char *nombre);
int cambiarEstadoAtraccion(struct Parque *parque, char *nombreZona, char *nombreAtraccion, char *nuevoEstado);
int modificarAtraccion(struct Parque *parque, char *nombreZona, char *nombreAtraccion, char *nuevoEstado, int nuevaCapacidad, int nuevoTiempo, float NuevaAltura, int NuevaEdad);
void estadoAtraccion(struct nodoZonaTematica *raiz, char *NombreZonaTematica, char *NombreAtraccion);
int reinicioDeAtraccion(struct atraccion *atraccion);
struct atraccion *buscarAtraccionEspera(struct nodoAtraccion *atraccion);
struct atraccion *atraccionConMasEspera(struct nodoZonaTematica *zonasTematicas);
struct atraccion *buscarAtraccionVisitada(struct nodoAtraccion *atraccion);
struct atraccion *atraccionMasVisitada(struct nodoZonaTematica *zonasTematicas);
int ocupacionAtraccion(struct atraccion *atraccion);
struct atraccion *atraccionMasOcupadaZona(struct zonaTematica *zona);
void atraccionesNoOperativas(struct nodoZonaTematica *raiz);
int eliminarAtraccion(struct nodoZonaTematica *raiz, char *nombreZona, char *nombreAtraccion);
float tiempoEsperaEstimado(struct atraccion *atraccion);
void desordenArbol(struct nodoZonaTematica *raiz, struct atraccion *arreglo[], int *contador);
void rankingAtracciones(struct nodoZonaTematica *raiz);


/* APARTADO DE CLIENTES */

int ponerEnFila(struct atraccion *atraccion, struct cliente *cliente);
struct cliente *sacarDeFila(struct nodoCliente **HeadEspera, struct nodoCliente **ultimoEspera);
int contarClientesEnAtraccion(struct atraccion *atraccion);
int agregarClienteActivo(struct atraccion *atraccion, struct cliente *cliente);
int registrarEnAtraccion(struct atraccion *atraccion, struct cliente *cliente);
struct cliente *buscarCliente(struct nodoCliente *cliente, int codigo);
int contarClientesLista(struct nodoCliente *head);
int eliminarCliente(struct nodoCliente **listaClientes, int codigoTicket);
int contarVisitantes(struct nodoCliente *Clientes);


/* APARTADO DE MENÚS */

void menuAtracciones(struct Parque *parque);
void menuTickets(struct Parque *parque);
void menuClientes(struct Parque *parque);
void menuZonas(struct Parque *parque);


struct nodoCliente *listaGlobalClientes = NULL;



/*  ----- DEFINICIÓN DE FUNCIONES ------*/



/* APARTADO DE TICKETS*/

struct cliente *ticketUtilizado(struct cliente *cliente)
{
    int i;

    if (cliente == NULL) return NULL;

    for (i = 0; i < cliente->cantEntradas; i++)
    {
        if (cliente->entradas[i] != NULL)
        {
            if (strcmp(cliente->entradas[i]->estado, "utilizada") == 0)
            {
                return cliente;
            }
        }
    }

    return NULL;
}
int ticketContado(int *codigos, int cant, int codigo)
{
    int i;
    for (i = 0; i < cant; i++)
    {
        if (codigos[i] == codigo) return 1;
    }
    return 0;
}

struct ticket *vendido(struct cliente *cliente)
{
    int i;
    if (cliente == NULL)
    {
        return NULL;
    }
    for (i = 0; i < cliente->cantEntradas; i++)
    {
        if (cliente->entradas[i] != NULL )
        {
            if ((strcmp(cliente->entradas[i]->estado, "activa") == 0 || strcmp(cliente->entradas[i]->estado, "utilizada") == 0))
                return cliente->entradas[i];
        }
    }
    return NULL;
}

int validarFecha(struct ticket *ticket, int fechaActual)
{
    char *NuevoEstado;
    if (ticket == NULL ) return 0;
    if (strcmp(ticket->estado, "activa") != 0) return 0;
    if (fechaActual > ticket->fechaVencimiento)
    {
        NuevoEstado = (char *) malloc((strlen("vencida") + 1) * sizeof(char));
        strcpy(NuevoEstado, "vencida");
        ticket->estado = NuevoEstado;
        return 0;
    }
    return 1;
}

int anularTicket(struct cliente *cliente, int codigoTicket){
    int i;
    char *nuevoEstado;
    if (cliente == NULL) return 0;
    for (i = 0; i < cliente -> cantEntradas; i++){
        if (cliente -> entradas[i] != NULL && cliente -> entradas[i] -> codUnico == codigoTicket){
            if (strcmp(cliente -> entradas[i] -> estado, "activa") != 0) return 0;

            nuevoEstado = (char*) malloc((strlen("anulada") + 1) * sizeof(char));
            strcpy(nuevoEstado, "anulada");
            cliente -> entradas[i] -> estado = nuevoEstado;
            return 1;
        }
    }
    return 0;
}

int entradasUtilizadas(struct nodoCliente *Clientes)
{
    struct nodoCliente *rec;
    int cantCodigos = 0;
    int i;
    int *codigos;
    rec = Clientes;
    codigos = (int *) malloc(1000 * sizeof(int));
    while (rec != NULL)
    {
        for (i = 0; i < rec->datos->cantEntradas; i++)
        {
            if (rec->datos->entradas[i] != NULL)
            {
              if (strcmp(rec->datos->entradas[i]->estado, "utilizada") == 0)
              {
                  if (ticketContado(codigos,cantCodigos,rec->datos->entradas[i]->codUnico) == 0)
                  {
                      codigos[cantCodigos] = rec->datos->entradas[i]->codUnico;
                      cantCodigos++;
                  }
              }
            }
          }
        rec = rec->sig;
    }
    return cantCodigos;
}

int registrarEntradaParque(struct cliente *cliente, struct Parque *parque, int fechaActual)
{
    int i;
    char *NuevoEstado;
    if (cliente == NULL || parque == NULL) return 0;
    for (i = 0; i < cliente->cantEntradas; i++)
    {
        if (cliente->entradas[i] != NULL && validarFecha(cliente->entradas[i], fechaActual) == 1)
        {
            NuevoEstado = (char *) malloc((strlen("utilizada") + 1) * sizeof(char));
            strcpy(NuevoEstado, "utilizada");
            cliente->entradas[i]->estado = NuevoEstado;
            parque->clientesActivos++;
            return 1;
        }
    }
    return 0;
}

int registrarSalida (struct Parque *parque)
{
    if (parque == NULL) return 0;
    if (parque->clientesActivos == 0) return 0;
    parque->clientesActivos --;
    return 1;
}

int eliminarTicket(struct cliente *cliente, int codigoTicket){
    int i;
    if (cliente == NULL) return 0;
    for (i = 0; i < cliente -> cantEntradas; i++){
        if (cliente -> entradas[i] != NULL && cliente -> entradas[i] -> codUnico == codigoTicket){
            cliente -> entradas[i] = NULL;
            return 1;
        }
    }
    return 0;
}

int montoRecaudacion(struct nodoCliente *clientes, struct Parque *parque)
{
    struct nodoCliente *rec;
    int cantCodigos = 0;
    int i;
    int *codigos;
    rec = clientes;
    codigos = (int *) malloc(1000 * sizeof(int));
    parque->recaudacion = 0;
    while (rec != NULL)
    {
        for (i = 0; i < rec->datos->cantEntradas; i++)
        {
            if (rec->datos->entradas[i] != NULL && vendido(rec->datos) != NULL)
            {
                if (ticketContado(codigos,cantCodigos,rec->datos->entradas[i]->codUnico) == 0)
                {
                    codigos[cantCodigos] = rec->datos->entradas[i]->codUnico;
                    cantCodigos++;
                    parque->recaudacion += rec->datos->entradas[i]->valor;
                }
            }
        }
        rec = rec->sig;
    }
    return cantCodigos;
}

/* CREACIÓN Y ENLACE DE NODOS Y DATOS */

struct ticket *crearTciket(int codigo, int valor, int fechaVencimiento, char* estado, char *tipoTicket)
{
    struct ticket *ticket;
    char *est;
    char *tipo;
    if (estado == NULL || tipoTicket == NULL) return NULL;
    ticket = (struct ticket *)malloc(sizeof(struct ticket));
    est = (char*)malloc((strlen(estado)+1)*sizeof(char));
    tipo = (char*)malloc((strlen(tipoTicket)+1)*sizeof(char));
    strcpy(est, estado);
    strcpy(tipo, tipoTicket);
    ticket->codUnico = codigo;
    ticket->valor = valor;
    ticket->estado = est;
    ticket->tipoTicket = tipo;
    ticket->fechaVencimiento = fechaVencimiento;
    return ticket;
}

int agregarTicket(struct cliente *cliente, struct ticket *ticket, int posicion)
{
    if (cliente == NULL || ticket == NULL) return 0;
    if (posicion < 0 || posicion >= cliente->cantEntradas) return 0;

    cliente->entradas[posicion] = ticket;
    return 1;
}

struct nodoCliente *crearNodoCliente(struct cliente *cliente)
{
    struct nodoCliente *nuevo;
    if (cliente == NULL) return NULL;
    nuevo = (struct nodoCliente *) malloc(sizeof(struct nodoCliente));
    nuevo->datos = cliente;
    nuevo->sig = NULL;
    return nuevo;
}

struct cliente *crearCliente(int edad, float Estatura, int CantEntradas)
{
    struct cliente *cliente;
    int i;
    cliente = (struct cliente*)malloc(sizeof(struct cliente));
    cliente->edad = edad;
    cliente->altura = Estatura;
    cliente->cantEntradas = CantEntradas;
    cliente->entradas = (struct ticket**)malloc(CantEntradas * sizeof(struct ticket*));
    for (i = 0; i < CantEntradas; i++)
    {
        cliente->entradas[i] = NULL;
    }
    return cliente;
}

int enlazarCliente (struct nodoCliente **listaClientes, struct cliente *cliente)
{
    struct nodoCliente *nodo;
    struct nodoCliente *rec;
    if (listaClientes == NULL || cliente == NULL)
    {
        return 0;
    }
    nodo = crearNodoCliente(cliente);
    if (*listaClientes == NULL)
    {
        *listaClientes = nodo;
        return 1;
    }
    rec = *listaClientes;
    while (rec->sig != NULL) {
        rec = rec->sig;
    }
    rec->sig = nodo;
    return 1;
}

struct nodoZonaTematica *crearNodoZona(struct zonaTematica *zonaTematica)
{
    struct nodoZonaTematica *nodo;
    nodo = (struct nodoZonaTematica *) malloc(sizeof(struct nodoZonaTematica));
    nodo->datos = zonaTematica;
    nodo->der = NULL;
    nodo->izq = NULL;
    return nodo;
}

struct zonaTematica *crearZona(char *nombre)
{
    struct zonaTematica *zona;
    char *Nombre;
    Nombre = (char *) malloc((strlen(nombre) + 1) * sizeof(char));
    zona = (struct zonaTematica *) malloc(sizeof(struct zonaTematica));
    strcpy(Nombre, nombre);
    zona->nombre = Nombre;
    zona->head = NULL;
    zona->activos = 0;
    return zona;
}

struct nodoZonaTematica *insertarZona(struct nodoZonaTematica *raiz, struct zonaTematica *zonaTematica)
{
    if (zonaTematica == NULL) return raiz;
    if (raiz == NULL) return crearNodoZona(zonaTematica);

    if (strcmp(zonaTematica->nombre, raiz->datos->nombre) < 0)
    {
        raiz->izq = insertarZona(raiz->izq, zonaTematica);
    }
    else if (strcmp(zonaTematica->nombre, raiz->datos->nombre) > 0)
    {
        raiz->der = insertarZona(raiz->der, zonaTematica);
    }
    return raiz;
}

struct nodoAtraccion *crearNodoAtraccion(struct atraccion *atraccion)
{
    struct nodoAtraccion *nuevo;
    nuevo = (struct nodoAtraccion *) malloc(sizeof(struct nodoAtraccion));
    nuevo->datos = atraccion;
    nuevo->sig = NULL;
    return nuevo;
}

struct atraccion *crearAtraccion(char *nombreAtraccion, int capacidad, int tiempo, char *estado, int edadMinima, float AlturaMinima)
{
    struct atraccion *atraccion;
    char *nombre;
    char *Estado;

    atraccion = (struct atraccion *) malloc(sizeof(struct atraccion));
    nombre = (char *) malloc((strlen(nombreAtraccion) + 1) * sizeof(char));
    Estado = (char *) malloc((strlen(estado) + 1) * sizeof(char));

    strcpy(nombre, nombreAtraccion);
    strcpy(Estado, estado);
    atraccion->nombre = nombre;
    atraccion->estado = Estado;
    atraccion->capMax = capacidad;
    atraccion->edadMinima = edadMinima;
    atraccion->alturaMinima = AlturaMinima;
    atraccion->duracion = tiempo;
    atraccion->headActivo = NULL;
    atraccion->headEspera = NULL;
    atraccion->visitasTotales = 0;
    atraccion->ultimoEspera = NULL;
    return atraccion;
}

int enlazarAtraccion(struct nodoAtraccion **ListaAtraccion, struct atraccion *nuevaAtraccion)
{
    struct nodoAtraccion *nodo = (struct nodoAtraccion *) malloc(sizeof(struct nodoAtraccion));
    struct nodoAtraccion *rec;
    nodo->datos = nuevaAtraccion;
    nodo->sig = NULL;

    if (*ListaAtraccion == NULL)
    {
        *ListaAtraccion = nodo;
        return 1;
    }

    rec = *ListaAtraccion;
    while (rec->sig != NULL)
    {
        rec = rec->sig;
    }
    rec->sig = nodo;
    return 1;
}

/* APARTADO DE ZONAS TEMÁTICAS*/

struct zonaTematica *buscarZonaTematica(struct nodoZonaTematica *raiz, char *nombre)
{
    int comp;
    if (raiz == NULL) return NULL;
    comp = strcmp(nombre, raiz->datos->nombre);
    if (comp == 0) return raiz->datos;
    if (comp < 0) return buscarZonaTematica(raiz->izq, nombre);
    return buscarZonaTematica(raiz->der, nombre);
}

/* APARTADO DE ATRACCIONES */

int registrarAtraccion(struct nodoZonaTematica *raiz, char *NombreZonaTematica, char *nombreAtraccion, char *estado, int capacidad, int tiempo, int edadMinima, float AlturaMinima)
{
    struct zonaTematica *zona = buscarZonaTematica(raiz, NombreZonaTematica);
    struct atraccion *nueva;
    if (zona == NULL) return 0;

    nueva = crearAtraccion(nombreAtraccion, capacidad, tiempo, estado, edadMinima, AlturaMinima);
    return enlazarAtraccion(&(zona->head), nueva);
}

struct atraccion *buscarAtraccion(struct zonaTematica *zonaTematica, char *nombre)
{
    struct nodoAtraccion *rec = zonaTematica->head;
    while (rec != NULL)
    {
        if (strcmp(rec->datos->nombre, nombre) == 0) return rec->datos;
        rec = rec->sig;
    }
    return NULL;
}

int cambiarEstadoAtraccion(struct Parque *parque, char *nombreZona, char *nombreAtraccion, char *nuevoEstado)
{
    struct zonaTematica *zonaBuscada;
    struct atraccion *atraccionBuscada;
    char *cambioEstado;
    int operativa1 = 0;
    int operativa2 = 0;

    if (parque == NULL) return 0;
    zonaBuscada = buscarZonaTematica(parque->raiz, nombreZona);
    if (zonaBuscada == NULL) return 0;
    atraccionBuscada = buscarAtraccion(zonaBuscada, nombreAtraccion);
    if (atraccionBuscada == NULL) return 0;

    if (strcmp(atraccionBuscada->estado, "operativa") == 0) operativa1 = 1;

    cambioEstado = (char *)malloc((strlen(nuevoEstado)+1) *sizeof(char));
    strcpy(cambioEstado, nuevoEstado);
    atraccionBuscada->estado = cambioEstado;
    if (strcmp(atraccionBuscada->estado, "operativa") == 0) operativa2 = 1;

    if (operativa1 == 1 && operativa2 == 0)
    {
        zonaBuscada->activos  --;
    }
    else if (operativa1 == 0 && operativa2 == 1)
    {
        zonaBuscada->activos  ++;
    }
    return 1;
}

int modificarAtraccion (struct Parque *parque, char *nombreZona, char *nombreAtraccion, char *nuevoEstado, int nuevaCapacidad, int nuevoTiempo, float NuevaAltura, int NuevaEdad)
{
    struct zonaTematica *zonaBuscada;
    struct atraccion *atraccionBuscada;
    char *estadoNuevo;
    int operativa1 = 0;
    int operativa2 = 0;
    if (parque == NULL || nombreZona == NULL || nombreAtraccion == NULL || nuevoEstado == NULL ) return 0;
    zonaBuscada = buscarZonaTematica(parque->raiz, nombreZona);
    if (zonaBuscada == NULL) return 0;
    atraccionBuscada = buscarAtraccion(zonaBuscada, nombreAtraccion);
    if (atraccionBuscada == NULL) return 0;
    if (strcmp(atraccionBuscada->estado, "operativa") == 0) operativa1 = 1;

    estadoNuevo = (char *)malloc((strlen(nuevoEstado)+1) *sizeof(char));
    strcpy(estadoNuevo,nuevoEstado);
    atraccionBuscada->estado = estadoNuevo;
    atraccionBuscada->capMax = nuevaCapacidad;
    atraccionBuscada->alturaMinima = NuevaAltura;
    atraccionBuscada->duracion = nuevoTiempo;
    atraccionBuscada->edadMinima = NuevaEdad;
    if (strcmp(atraccionBuscada->estado, "operativa") == 0) operativa2 = 1;

    if (operativa1 == 1 && operativa2 == 0)
    {
        zonaBuscada->activos  --;
    }
    else if (operativa1 == 0 && operativa2 == 1)
    {
        zonaBuscada->activos  ++;
    }
    return 1;
}

void estadoAtraccion (struct nodoZonaTematica *raiz, char *NombreZonaTematica, char *NombreAtraccion)
{
    struct zonaTematica *zonaBuscada;
    struct atraccion *atraccionBuscada;

    if (raiz == NULL) {
        printf("Error: No hay zonas registradas en el parque.\n");
        return;
    }
    zonaBuscada = buscarZonaTematica(raiz, NombreZonaTematica);
    if (zonaBuscada == NULL) {
        printf("Error: La Zona Tematica '%s' no existe en el parque.\n", NombreZonaTematica);
        return;
    }
    atraccionBuscada = buscarAtraccion(zonaBuscada, NombreAtraccion);
    if (atraccionBuscada == NULL) {
        printf("Error: La atraccion '%s' no existe dentro de la zona '%s'.\n", NombreAtraccion, NombreZonaTematica);
        return;
    }
    printf("\n--- DATOS DE LA ATRACCIÓN ---\n");
    printf("Nombre: %s\n", atraccionBuscada->nombre);
    printf("Estado: %s\n", atraccionBuscada->estado);
    printf("Duracion ciclo de juego: %.2f minutos\n", atraccionBuscada->duracion);
    printf("Capacidad maxima: %d personas por ciclo\n", atraccionBuscada->capMax);
    printf("Visitas totales: %d\n", atraccionBuscada->visitasTotales);
}

int reinicioDeAtraccion (struct atraccion *atraccion)
{
    int numeroClientes = 0;
    struct cliente *cliente;
    while (atraccion->headEspera != NULL && numeroClientes < atraccion->capMax)
    {
        cliente = sacarDeFila(&(atraccion->headEspera), &(atraccion->ultimoEspera));
        if (cliente != NULL)
        {
            if (registrarEnAtraccion(atraccion, cliente) != 0) {
                numeroClientes++;
            }
        }
    }
    return 1;
}

struct atraccion *buscarAtraccionVisitada(struct nodoAtraccion *atraccion)
{
    struct nodoAtraccion *rec;
    struct atraccion *atraccionMasVisitada;
    if (atraccion == NULL) return NULL;
    rec = atraccion;
    atraccionMasVisitada = NULL;
    while (rec != NULL)
    {
        if (atraccionMasVisitada == NULL || atraccionMasVisitada->visitasTotales < rec->datos->visitasTotales)
        {
            atraccionMasVisitada = rec->datos;
        }
        rec = rec->sig;
    }
    return atraccionMasVisitada;
}

struct atraccion *atraccionMasVisitada(struct nodoZonaTematica *zonasTematicas)
{
    struct atraccion *izq;
    struct atraccion *der;
    struct atraccion *act;
    struct atraccion *mayor;

    if (zonasTematicas == NULL) return NULL;

    izq = atraccionMasVisitada(zonasTematicas -> izq);
    der = atraccionMasVisitada(zonasTematicas -> der);
    act = buscarAtraccionVisitada(zonasTematicas -> datos -> head);

    mayor = act;

    if (izq != NULL){
        if (mayor == NULL || izq -> visitasTotales > mayor -> visitasTotales){
            mayor = izq;
        }
    }
    if (der != NULL){
        if (mayor == NULL || der -> visitasTotales > mayor -> visitasTotales){
            mayor = der;
        }
    }
    return mayor;
}

struct atraccion *buscarAtraccionEspera(struct nodoAtraccion *atraccion)
{
    struct nodoAtraccion *rec;
    struct atraccion *atraccionBuscada;
    if (atraccion == NULL) return NULL;
    rec = atraccion;
    atraccionBuscada = NULL;
    while (rec != NULL)
    {
        if (atraccionBuscada == NULL || contarClientesLista(rec->datos->headEspera) > contarClientesLista(atraccionBuscada->headEspera))
        {
            atraccionBuscada = rec->datos;
        }
        rec = rec->sig;
    }
    return atraccionBuscada;
}

struct atraccion *atraccionConMasEspera(struct nodoZonaTematica *zonasTematicas)
{
    struct atraccion *izq;
    struct atraccion *der;
    struct atraccion *act;
    struct atraccion *mayor;

    if (zonasTematicas == NULL) return NULL;

    izq = atraccionConMasEspera(zonasTematicas->izq);
    act = buscarAtraccionEspera(zonasTematicas->datos->head);
    der = atraccionConMasEspera(zonasTematicas->der);

    mayor = act;

    if (izq != NULL)
    {
        if (mayor == NULL || contarClientesLista(izq->headEspera) > contarClientesLista(mayor->headEspera))
        {
            mayor = izq;
        }
    }
    if (der != NULL)
    {
        if (mayor == NULL || contarClientesLista(der->headEspera) > contarClientesLista(mayor->headEspera))
        {
            mayor = der;
        }
    }
    return mayor;
}

int ocupacionAtraccion(struct atraccion *atraccion)
{
    int total = 0;
    if (atraccion == NULL) return 0;
    total += contarClientesLista(atraccion->headActivo);
    total += contarClientesLista(atraccion->headEspera);
    return total;
}

struct atraccion *atraccionMasOcupadaZona(struct zonaTematica *zona)
{
    struct nodoAtraccion *rec;
    struct atraccion *mayor;

    if (zona == NULL) return NULL;

    rec = zona->head;
    mayor = NULL;

    while (rec != NULL)
    {
        if (rec->datos != NULL)
        {
            if (mayor == NULL || ocupacionAtraccion(rec->datos) > ocupacionAtraccion(mayor))
            {
                mayor = rec->datos;
            }
        }
        rec = rec->sig;
    }
    return mayor;
}

void atraccionesNoOperativas(struct nodoZonaTematica *raiz) {
    struct nodoAtraccion *rec;
    if (raiz == NULL) return;

    atraccionesNoOperativas(raiz->izq);

    rec = raiz->datos->head;
    while (rec != NULL) {
        if (strcmp(rec->datos->estado, "operativa") != 0) {
            printf("  [ZONA: %s] Atracción: %s | Estado: %s\n",
                   raiz->datos->nombre, rec->datos->nombre, rec->datos->estado);
        }
        rec = rec->sig;
    }
    atraccionesNoOperativas(raiz->der);
}

int eliminarAtraccion(struct nodoZonaTematica *raiz, char *nombreZona, char *nombreAtraccion){
    struct zonaTematica *zona;
    struct nodoAtraccion *rec;
    struct nodoAtraccion *anterior;

    zona = buscarZonaTematica(raiz, nombreZona);
    if (zona == NULL) return 0;

    rec = zona -> head;
    anterior = NULL;

    while (rec != NULL){
        if (strcmp(rec -> datos -> nombre, nombreAtraccion) == 0){
            if (anterior == NULL){
                zona -> head = rec -> sig;
            }
            else{
                anterior -> sig = rec -> sig;
            }
            rec -> datos = NULL;
            rec -> sig = NULL;
            return 1;
        }
        anterior = rec;
        rec = rec -> sig;
    }
    return 0;
}

float tiempoEsperaEstimado(struct atraccion *atraccion) {
    int personasEnFila;
    int vueltasNecesarias;
    if (atraccion == NULL || atraccion->headEspera == NULL) return 0.0;
    personasEnFila = contarClientesLista(atraccion->headEspera);
    vueltasNecesarias = (personasEnFila / atraccion->capMax) + 1;
    return vueltasNecesarias * atraccion->duracion;
}

void desordenArbol(struct nodoZonaTematica *raiz, struct atraccion *arreglo[], int *contador) {
    struct nodoAtraccion *rec;
    if (raiz == NULL) return;
    desordenArbol(raiz->izq, arreglo, contador);

    rec = raiz->datos->head;
    while (rec != NULL) {
        arreglo[*contador] = rec->datos;
        (*contador)++;
        rec = rec->sig;
    }
    desordenArbol(raiz->der, arreglo, contador);
}

void rankingAtracciones(struct nodoZonaTematica *raiz) {
    struct atraccion *listaPlana[500];
    int totalAtracciones = 0;
    int i, j;
    struct atraccion *temp;

    desordenArbol(raiz, listaPlana, &totalAtracciones);

    if (totalAtracciones == 0) {
        printf("No hay atracciones registradas en el parque.\n");
        return;
    }

    for (i = 0; i < totalAtracciones - 1; i++) {
        for (j = 0; j < totalAtracciones - i - 1; j++) {
            if (listaPlana[j]->visitasTotales < listaPlana[j + 1]->visitasTotales) {
                temp = listaPlana[j];
                listaPlana[j] = listaPlana[j + 1];
                listaPlana[j + 1] = temp;
            }
        }
    }

    printf("\n=== RANKING: ATRACCIONES MÁS VISITADAS DEL PARQUE ===\n");
    for (i = 0; i < totalAtracciones; i++) {
        printf("%d. %s | Visitas Totales: %d\n", i + 1, listaPlana[i]->nombre, listaPlana[i]->visitasTotales);
    }
}

/* APARTADO DE CLIENTES */


int ponerEnFila(struct atraccion *atraccion, struct cliente *cliente) {
    struct nodoCliente *nuevoNodo;

    if (cliente->edad < atraccion->edadMinima || cliente->altura < atraccion->alturaMinima) {
        printf("El cliente no cumple con los requisitos mínimos de la atracción.\n");
        return 0;
    }

    if (strcmp(atraccion->estado, "operativa") != 0) {
        printf("La atracción está cerrada.\n");
        return 0;
    }

    nuevoNodo = (struct nodoCliente *) malloc(sizeof(struct nodoCliente));
    nuevoNodo->datos = cliente;
    nuevoNodo->sig = NULL;

    if (atraccion->headEspera == NULL) {
        atraccion->headEspera = nuevoNodo;
        atraccion->ultimoEspera = nuevoNodo;
    } else {
        atraccion->ultimoEspera->sig = nuevoNodo;
        atraccion->ultimoEspera = nuevoNodo;
    }
    return 1;
}

struct cliente *sacarDeFila (struct nodoCliente **HeadEspera, struct nodoCliente **ultimoEspera)
{
    struct cliente *retorno;
    if (*HeadEspera == NULL) return NULL;
    retorno = (*HeadEspera)->datos;
    *HeadEspera = (*HeadEspera)->sig;
    if (*HeadEspera == NULL) {
        *ultimoEspera = NULL;
    }
    return retorno;
}

int contarClientesEnAtraccion(struct atraccion *atraccion )
{
    struct nodoCliente *rec;
    int contador = 0;
    rec = atraccion->headActivo;
    while (rec != NULL)
    {
        contador ++;
        rec = rec->sig;
    }
    return contador;
}

int agregarClienteActivo (struct atraccion *atraccion, struct cliente *cliente)
{
    struct nodoCliente *nuevo;
    if (cliente->edad < atraccion->edadMinima || cliente->altura < atraccion->alturaMinima)
    {
        return 0;
    }
    nuevo = crearNodoCliente(cliente);
    nuevo->sig = atraccion->headActivo;
    atraccion->headActivo = nuevo;
    return 1;
}

int registrarEnAtraccion (struct atraccion *atraccion, struct cliente *cliente)
{
    int clientesActivos;
    if (strcmp(atraccion->estado, "operativa") != 0)
    {
        return 0;
    }
    clientesActivos = contarClientesEnAtraccion(atraccion);
    if (clientesActivos >= atraccion->capMax)
    {
        ponerEnFila(atraccion, cliente);
        printf("Cliente en fila de espera\n");
        return 2;
    }
    else
    {
        agregarClienteActivo(atraccion, cliente);
        atraccion->visitasTotales++;
        printf("Cliente en atraccion\n");
        return 1;
    }
}

struct cliente *buscarCliente(struct nodoCliente *cliente, int codigo)
{
    struct nodoCliente *rec;
    int i;
    rec = cliente;
    while (rec != NULL)
    {
        for (i = 0; i < rec->datos->cantEntradas; i++)
        {
            if (rec->datos->entradas[i] != NULL)
            {
                if (rec->datos->entradas[i]->codUnico == codigo) return rec->datos;
            }
        }
        rec = rec->sig;
    }
    return NULL;
}

int contarClientesLista(struct nodoCliente *head)
{
    struct nodoCliente *rec;
    int contador = 0;
    rec = head;
    while (rec != NULL)
    {
        contador++;
        rec = rec->sig;
    }
    return contador;
}

int eliminarCliente(struct nodoCliente **listaClientes, int codigoTicket){
    struct nodoCliente *rec;
    struct nodoCliente *anterior;

    if (listaClientes == NULL || *listaClientes == NULL) return 0;
    rec = *listaClientes;
    anterior = NULL;

    while (rec != NULL){
        if (buscarCliente(rec, codigoTicket) != NULL){
          if (anterior == NULL){
            *listaClientes = rec -> sig;
          }
          else{
            anterior -> sig = rec -> sig;
          }
          rec -> datos = NULL;
          rec -> sig = NULL;
          return 1;
        }
        anterior = rec;
        rec = rec -> sig;
    }
    return 0;
}

int contarVisitantes(struct nodoCliente *clientes)
{
    struct nodoCliente *rec = clientes;
    int contador = 0;
    while (rec != NULL)
    {
        if (ticketUtilizado(rec->datos) != NULL) contador++;
        rec = rec->sig;
    }
    return contador;
}

/* APARTADO DE MENUS */

void menuAtracciones(struct Parque *parque) {
    int opcion, capacidad, tiempo, edadMin;
    float altMin;
    char nombreZona[100], nombreAtra[100], estado[50];
    struct zonaTematica *zonita;
    struct atraccion *atr;

    do {
        printf("\n  --- ADMINISTRAR ATRACCIONES ---  \n");
        printf("1 - Registrar Nueva Atracción\n");
        printf("2 - Modificar Estado\n");
        printf("3 - Modificar Datos\n");
        printf("4 - Consultar Estado/Datos\n");
        printf("5 - Eliminar\n");
        printf("6 - Ejecutar Ciclo de Juego (Reiniciar/Vaciar la Fila)\n");
        printf("7 - Consultar Tiempo de Espera Estimado\n");
        printf("8 - Volver\n");
        printf("Opción: ");
        scanf("%d", &opcion);
        getchar();

        if (opcion == 1) {
            printf("Nombre de la Zona Temática donde pertenece: ");
            fgets(nombreZona, sizeof(nombreZona), stdin); nombreZona[strcspn(nombreZona, "\n")] = 0;
            printf("Nombre de la Atracción: ");
            fgets(nombreAtra, sizeof(nombreAtra), stdin); nombreAtra[strcspn(nombreAtra, "\n")] = 0;
            printf("Capacidad maxima por ciclo de juego: "); scanf("%d", &capacidad);
            printf("Duracion del ciclo de juego (minutos): "); scanf("%d", &tiempo); getchar();
            printf("Estado inicial : ");
            fgets(estado, sizeof(estado), stdin); estado[strcspn(estado, "\n")] = 0;
            printf("Edad minima requerida: "); scanf("%d", &edadMin);
            printf("Altura minima requerida (metros): "); scanf("%f", &altMin);

            if (registrarAtraccion(parque->raiz, nombreZona, nombreAtra, estado, capacidad, tiempo, edadMin, altMin)) {
                printf("Atracción registrada exitosamente.\n");
            } else {
                printf("Error: No se encontro la Zona Tematica.\n");
            }
        }
        else if (opcion == 2) {
            printf("Nombre de la Zona: ");
            fgets(nombreZona, sizeof(nombreZona), stdin); nombreZona[strcspn(nombreZona, "\n")] = 0;
            printf("Nombre de la Atracción: ");
            fgets(nombreAtra, sizeof(nombreAtra), stdin); nombreAtra[strcspn(nombreAtra, "\n")] = 0;
            printf("Nuevo Estado (operativa/mantenimiento/fuera de servicio): ");
            fgets(estado, sizeof(estado), stdin); estado[strcspn(estado, "\n")] = 0;

            if (cambiarEstadoAtraccion(parque, nombreZona, nombreAtra, estado)) {
                printf("Estado actualizado exitosamente\n");
            } else {
                printf("Error al cambiar estado. Verifique los nombres.\n");
            }
        }
        else if (opcion == 3) {
            printf("Nombre de la Zona: ");
            fgets(nombreZona, sizeof(nombreZona), stdin); nombreZona[strcspn(nombreZona, "\n")] = 0;
            printf("Nombre de la Atracción atr modificar: ");
            fgets(nombreAtra, sizeof(nombreAtra), stdin); nombreAtra[strcspn(nombreAtra, "\n")] = 0;
            printf("Nuevo Estado: "); fgets(estado, sizeof(estado), stdin); estado[strcspn(estado, "\n")] = 0;
            printf("Nueva Capacidad: "); scanf("%d", &capacidad);
            printf("Nueva Duración: "); scanf("%d", &tiempo);
            printf("Nueva Altura Mínima: "); scanf("%f", &altMin);
            printf("Nueva Edad Mínima: "); scanf("%d", &edadMin);

            if (modificarAtraccion(parque, nombreZona, nombreAtra, estado, capacidad, tiempo, altMin, edadMin)) {
                printf("Atracción modificada exitosamente\n");
            } else {
                printf("Error al modificar. Verifique los datos.\n");
            }
        }
        else if (opcion == 4) {
            printf("Nombre de la Zona: ");
            fgets(nombreZona, sizeof(nombreZona), stdin);
            nombreZona[strcspn(nombreZona, "\n")] = 0;
            printf("Nombre de la Atracción: ");
            fgets(nombreAtra, sizeof(nombreAtra), stdin);
            nombreAtra[strcspn(nombreAtra, "\n")] = 0;
            estadoAtraccion(parque->raiz, nombreZona, nombreAtra);
        }
        else if (opcion == 5) {
            printf("Nombre de la Zona: ");
            fgets(nombreZona, sizeof(nombreZona), stdin); nombreZona[strcspn(nombreZona, "\n")] = 0;
            printf("Nombre de la Atracción que desea eliminar: ");
            fgets(nombreAtra, sizeof(nombreAtra), stdin); nombreAtra[strcspn(nombreAtra, "\n")] = 0;

            if (eliminarAtraccion(parque->raiz, nombreZona, nombreAtra)) {
                printf("Atracción '%s' eliminada del sistema exitosamente\n", nombreAtra);
            } else {
                printf("Error: No se encontro la atraccion o la zona especificada.\n");
            }
        }
        else if (opcion == 6) {
            printf("Nombre de la Zona: ");
            fgets(nombreZona, sizeof(nombreZona), stdin); nombreZona[strcspn(nombreZona, "\n")] = 0;
            printf("Nombre de la Atracción: ");
            fgets(nombreAtra, sizeof(nombreAtra), stdin); nombreAtra[strcspn(nombreAtra, "\n")] = 0;

            zonita = buscarZonaTematica(parque->raiz, nombreZona);
            atr = buscarAtraccion(zonita, nombreAtra);
            if (atr != NULL) {
                reinicioDeAtraccion(atr);
                printf("Ciclo ejecutado, fila de espera avanzada hacia clientes activos.\n");
            } else {
                printf("Error: Atracción no encontrada.\n");
            }
        }
        else if (opcion == 7) {
            printf("Nombre de la Zona: ");
            fgets(nombreZona, sizeof(nombreZona), stdin); nombreZona[strcspn(nombreZona, "\n")] = 0;
            printf("Nombre de la Atracción: ");
            fgets(nombreAtra, sizeof(nombreAtra), stdin); nombreAtra[strcspn(nombreAtra, "\n")] = 0;

            zonita = buscarZonaTematica(parque->raiz, nombreZona);
            atr = buscarAtraccion(zonita, nombreAtra);
            if (atr != NULL) {
                printf("Tiempo de espera estimado: %.2f minutos.\n", tiempoEsperaEstimado(atr));
            } else {
                printf("Error: Atracción no encontrada.\n");
            }
        }
    } while (opcion != 8);
}

void menuTickets(struct Parque *parque) {
    int opcion, codigo, valor, fecha;
    char estado[50], tipo[50];
    struct ticket *tito;

    do {
        printf("\n  --- ADMINISTRAR TICKETS ---  \n");
        printf("1 - Crear e Inicializar un Ticket Basico\n");
        printf("2 - Volver\n");
        printf("Opcion: ");
        scanf("%d", &opcion);
        getchar();

        if (opcion == 1) {
            printf("Ingrese Código Único numérico: "); scanf("%d", &codigo);
            printf("Ingrese Valor ($): "); scanf("%d", &valor);
            printf("Ingrese Fecha Vencimiento (AAAAMMDD): "); scanf("%d", &fecha); getchar();
            printf("Estado: "); fgets(estado, sizeof(estado), stdin); estado[strcspn(estado, "\n")] = 0;
            printf("Tipo de Ticket: "); fgets(tipo, sizeof(tipo), stdin); tipo[strcspn(tipo, "\n")] = 0;

            tito = crearTciket(codigo, valor, fecha, estado, tipo);
            if (tito != NULL) {
                printf("Ticket %d creado exitosamente.\n", tito->codUnico);
            } else {
                printf("Error al crear el ticket.\n");
            }
        }
    } while (opcion != 2);
}

void menuClientes(struct Parque *parque) {
    int opcion, edad, cantT, i, codT, fechaHoy;
    float altura;
    char tipoT[50];
    struct cliente *clnt;
    struct ticket *tito;

    do {
        printf("\n  --- ADMINISTRAR CLIENTES ---  \n");
        printf("1 - Registrar Cliente y simular compra de Tickets\n");
        printf("2 - Ingresar cliente al acceso principal\n");
        printf("3 - Procesar salida de un Visitante del Parque\n");
        printf("4 - Eliminar/retirar cliente del registro\n");
        printf("5 - Volver\n");
        printf("Opcion: ");
        scanf("%d", &opcion);
        getchar();

        if (opcion == 1) {
            printf("Edad del cliente: "); scanf("%d", &edad);
            printf("Altura (metros): "); scanf("%f", &altura);
            printf("¿Cuantos tickets va a comprar?: "); scanf("%d", &cantT);

            clnt = crearCliente(edad, altura, cantT);

            for(i = 0; i < cantT; i++) {
                printf("\n  --- Configurar Ticket %d/%d ---  \n", i+1, cantT);
                printf("Codigo unico numerico: "); scanf("%d", &codT);
                printf("Tipo de ticket: "); getchar();
                fgets(tipoT, sizeof(tipoT), stdin); tipoT[strcspn(tipoT, "\n")] = 0;

                tito = crearTciket(codT, 10000, 20261231, "activa", tipoT);
                agregarTicket(clnt, tito, i);
            }
            enlazarCliente(&listaGlobalClientes, clnt);
            printf("\nCliente creado y enlazado en el registro exitosamente.\n");
        }
        else if (opcion == 2) {
            printf("Ingrese la Fecha de hoy para control de vigencia (AAAAMMDD): "); scanf("%d", &fechaHoy);

            if (listaGlobalClientes != NULL) {
                if (registrarEntradaParque(listaGlobalClientes->datos, parque, fechaHoy)) {
                    printf("Ingreso autorizado, ticket cambiado a 'utilizada'.\n");
                    printf("Clientes activos actuales en IBCLandia: %d\n", parque->clientesActivos);
                } else {
                    printf("Acceso denegado: Ticket vencido o ya utilizado.\n");
                }
            } else {
                printf("Error: No hay clientes registrados en el sistema para procesar acceso.\n");
            }
        }
        else if (opcion == 3) {
            if (registrarSalida(parque)) {
                printf("Visitante salio del recinto. Clientes activos: %d\n", parque->clientesActivos);
            } else {
                printf("No hay clientes dentro del parque actualmente.\n");
            }
        }
        else if (opcion == 4) {
            printf("Ingrese el codigo del ticket del cliente que desea eliminar: ");
            scanf("%d", &codT);

            if (eliminarCliente(&listaGlobalClientes, codT)) {
                printf("Cliente asociado al ticket %d ha sido eliminado del registro exitosamente.\n", codT);
            } else {
                printf("Error: No se encontro ningun cliente con ese codigo de ticket.\n");
            }
        }
    } while (opcion != 5);
}

void menuZonas(struct Parque *parque){
    int opcion;
    char nombre[100];
    struct zonaTematica *nuevaZona;
    struct zonaTematica *buscada;

    do{
        printf("\n  --- ADMINISTRAR ZONAS ---  \n");
        printf("1 - Agregar Zona Tematica \n");
        printf("2 - Buscar Zona Tematica\n");
        printf("3 - Volver\n");
        printf("Opcion: ");
        scanf("%d", &opcion);
        getchar();

        if (opcion == 1){
            printf("Ingrese nombre de la nueva Zona Tematica: ");
            fgets(nombre, sizeof(nombre), stdin);
            nombre[strcspn(nombre, "\n")] = 0;

            nuevaZona = crearZona(nombre);
            parque->raiz = insertarZona(parque->raiz, nuevaZona);
            printf("Zona '%s' agregada e insertada exitosamente.\n", nombre);
        }
        else if (opcion == 2){
            printf("Ingrese el nombre de la zona a buscar: ");
            fgets(nombre, sizeof(nombre), stdin);
            nombre[strcspn(nombre, "\n")] = 0;

            buscada = buscarZonaTematica(parque->raiz, nombre);
            if(buscada != NULL) {
                printf("Zona encontrada: %s (Atracciones operativas activas: %d)\n", buscada->nombre, buscada->activos);
            } else {
                printf("La zona '%s' no existe.\n", nombre);
            }
        }
        else if (opcion == 3) {
            printf("\nVolviendo al menu principal\n");
        }
        else {
            printf("\nOpcion inválida.\n");
        }

    } while(opcion != 3);
}

int main(){
    struct Parque parque;
    int opcion;
    struct atraccion *masVisitada;
    struct atraccion *mayorEspera;
    int totalTicketsVendidos;

    parque.raiz = NULL;
    parque.clientesActivos = 0;
    parque.recaudacion = 0;
    parque.capacidadMaxima = 12000;

    do{
        printf("\n========================================\n");
        printf("||        SISTEMA GLOBAL IBCLANDIA     ||\n");
        printf("========================================\n");
        printf("1 - Administrar Zonas Tematicas\n");
        printf("2 - Administrar Atracciones\n");
        printf("3 - Administrar Tickets\n");
        printf("4 - Administrar Clientes\n");
        printf("5 - Reporte del día\n");
        printf("6 - Salir del programa\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        if (opcion == 1){
            menuZonas(&parque);
        }
        else if (opcion == 2){
            menuAtracciones(&parque);
        }
        else if (opcion == 3){
            menuTickets(&parque);
        }
        else if (opcion == 4){
            menuClientes(&parque);
        }
        else if (opcion == 5){
            rankingAtracciones(parque.raiz);
            printf("\n--- REPORTE DEL DÍA ---\n");
            totalTicketsVendidos = montoRecaudacion(listaGlobalClientes, &parque);
            printf("   - Recaudación: \n");
            printf("   - Cantidad Total de Tickets Vendidos: %d tickets.\n", totalTicketsVendidos);
            printf("   - Monto Total Recaudado: $%d.\n", parque.recaudacion);

            masVisitada = atraccionMasVisitada(parque.raiz);
            if(masVisitada) printf("Atracción mas visitada: %s (%d visitas)\n", masVisitada->nombre, masVisitada->visitasTotales);

            mayorEspera = atraccionConMasEspera(parque.raiz);
            if(mayorEspera) printf("Atracción con mas cola de espera en el parque: %s\n", mayorEspera->nombre);

            printf("Atracciones cerradas o no operativas actualmente:\n");
            atraccionesNoOperativas(parque.raiz);
        }
        else if (opcion == 6){
            printf("Saliendo del sistema de IBCLandia.\n");
        }
        else{
            printf("Opcion invÁlida.\n");
        }
    } while (opcion != 6);
    return 0;
}
