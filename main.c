#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int id;
    char nombreCanal[40];
    char rubro[30]; //los rubros de juegos a los que se dedica cada canal son (y estan escritos tal cual: “fantasia","creativos","accion","aventura","retro","construccion"
    int cantSuscriptores; //cant suscript total del canal
    int cantVistasSemestre; //cant de vistas que recibieron sus videos en el último semestre
} stYoutuber;

void ContNY(char * cont)
{
    printf("Desea continuar? (s/n)\n");
    fflush(stdin);
    *cont = getch();
}

///4.

typedef struct
{
    stYoutuber youtuber;
    struct nodoD* siguiente;
    struct nodoD* anterior;
} nodoD;

nodoD* inicListaD()
{
    return NULL;
}

nodoD* CrearNodoD(stYoutuber yt)
{
    nodoD * nuevo = (nodoD*)malloc(sizeof(nodoD));

    nuevo->youtuber = yt;
    nuevo->anterior = inicListaD();
    nuevo->siguiente = inicListaD();

    return nuevo;
}

///1.

typedef struct
{
    stYoutuber youtuber;
    struct nodoS* siguiente;
} nodoS;

nodoS* inicLista()
{
    return NULL;
}

nodoS* CrearNodo(stYoutuber yt)
{
    nodoS* nuevo = (nodoS*)malloc(sizeof(nodoS));

    nuevo->youtuber = yt;
    nuevo->siguiente = inicLista();

    return nuevo;
}

void agregarPpio(nodoS ** lista, nodoS* insertar)
{
    if(*lista == NULL)
    {
        *lista = insertar;
    }
    else
    {
        insertar->siguiente = *lista;
        *lista = insertar;
    }
}

void InsertarPorRubro(nodoS ** lista, nodoS* insertar)
{
    if(*lista == NULL)
    {
        *lista = insertar;
    }
    else if(strcmpi(((*lista)->youtuber.rubro), (insertar->youtuber.rubro)) > 0)
    {
        agregarPpio(lista, insertar);
    }
    else
    {
        nodoS* seg = (*lista)->siguiente;
        nodoS* ante = *lista;

        while(seg != NULL && strcmpi((seg->youtuber.rubro), (insertar->youtuber.rubro)) <= 0)
        {
            ante = seg;
            seg = seg->siguiente;
        }

        ante->siguiente = insertar;
        insertar->siguiente = seg;
    }
}

void PasarDeArchivoToListaSimple(nodoS ** lista)
{
    FILE * buf = fopen("archivoYoutubers.bin", "rb");
    nodoS* nuevoNodo;
    stYoutuber aux;

    if(buf != NULL)
    {
        while(fread(&aux, sizeof(stYoutuber), 1, buf) > 0)
        {
            nuevoNodo = CrearNodo(aux);
            InsertarPorRubro(lista, nuevoNodo);
        }
        fclose(buf);
    }
    else
    {
        printf("\n|X| ERROR AL ABRIR EL ARCHIVO |X|\n");
    }
}

///2.

void MostrarYoutuber(stYoutuber p)
{
    printf("\n ***************************** YOUTUBER *********************************** \n");
    printf("Id del Youtuber..............................: %d ", p.id);
    printf("\nNombre del Canal...........................: %s ", p.nombreCanal);
    printf("\nRubro del Canal (tipo de juegos)...........: %s ", p.rubro);
    printf("\nCantidad de Suscriptores en miles..........: %d ", p.cantSuscriptores);
    printf("\nCantidad de vistas del semestre en miles..: %d \n\n", p.cantVistasSemestre);
}

void MostrarListaSimpleRecursiva(nodoS* lista)
{
    if(lista == NULL)
    {
        printf("\n*final*\n");
    }
    else
    {
        MostrarYoutuber(lista->youtuber);
        MostrarListaSimpleRecursiva(lista->siguiente);
    }
}

///3.

nodoS * BuscarPorIdRecursiva(nodoS* lista, int id)
{
    if(lista != NULL && lista->youtuber.id != id)
    {
        lista = BuscarPorIdRecursiva(lista->siguiente, id);
    }

    return lista;
}

void ModificarCantVistas(nodoS** lista, int id, int nuevoDato)
{
    nodoS* buscado = BuscarPorIdRecursiva(*lista, id);

    buscado->youtuber.cantVistasSemestre = nuevoDato;
}

///4.

nodoD* agregarPpioDoble(nodoD * lista, nodoD * agregar)
{
    if(lista == NULL)
    {
        lista = agregar;
    }
    else
    {
        agregar->siguiente = lista;
        lista->anterior = agregar;
        lista = agregar;
    }
    return lista;
}

void BorrarNodo(nodoS ** lista, nodoS * eliminar)
{
    if(*lista != NULL)
    {
        nodoS* aBorrar;
        if((*lista) == eliminar)
        {
            aBorrar = *lista;
            *lista = (*lista)->siguiente;
            free(aBorrar);
        }
        else
        {
            nodoS* seg = (*lista)->siguiente;
            nodoS* ante = *lista;

            while(seg != NULL && seg != eliminar)
            {
                ante = seg;
                seg = seg->siguiente;
            }

            if(seg == eliminar)
            {
                aBorrar = seg;
                ante->siguiente = seg->siguiente;
                free(aBorrar);
            }
        }
    }
}

nodoD* PasarToListaDobleBorrandoElNodo(nodoS** lista, char rubroBorrar[])
{
    nodoD* listaDoble = inicListaD();
    nodoD* auxD;

    if(*lista != NULL)
    {
        if(strcmp((*lista)->youtuber.rubro, rubroBorrar) == 0)
        {
            auxD = CrearNodoD((*lista)->youtuber);

            listaDoble = agregarPpioDoble(listaDoble, auxD);

            BorrarNodo(lista, *lista);
        }

        nodoS* seg = (*lista)->siguiente;
        nodoS* ante = (*lista);

        while(seg != NULL)
        {
            if(strcmp(seg->youtuber.rubro, rubroBorrar) == 0)
            {
                auxD = CrearNodoD(seg->youtuber);

                listaDoble = agregarPpioDoble(listaDoble, auxD);

                BorrarNodo(lista, seg);
            }

            ante = seg;
            seg = seg->siguiente;
        }
    }

    return listaDoble;
}

///5.

nodoD * BuscarNodoFinal(nodoD * lista)
{
    while(lista->siguiente != NULL)
    {
        lista = lista->siguiente;
    }
    return lista;
}

void MostrarListaDobleInversa(nodoD * lista)
{
    nodoD* ultimo = BuscarNodoFinal(lista);
    while(ultimo != NULL)
    {
        MostrarYoutuber(ultimo->youtuber);
        ultimo = ultimo->anterior;
    }
}

//6.

nodoD* BuscarMayorSubs(nodoD * listaD, nodoD* mayorSubs)
{
    if(listaD != NULL)
    {
        if(listaD->youtuber.cantSuscriptores > mayorSubs->youtuber.cantSuscriptores)
        {
            mayorSubs = BuscarMayorSubs(listaD->siguiente, listaD);
        }
        else
        {
            mayorSubs = BuscarMayorSubs(listaD->siguiente, mayorSubs);
        }
    }
    return mayorSubs;
}

float CalcularPlusMayorNroSubs(nodoD * lista)
{
    nodoD* mayorSubs = BuscarMayorSubs(lista->siguiente, lista);

    float bono = (mayorSubs->youtuber.cantSuscriptores) * 0.2;

    return bono;
}

/// ####################### MAIN ##################### ///

int main()
{
    char cont = 's';
    int numSw;

    //1.
    nodoS* lista = inicLista();

    //3.
    int idModificar, cantVistasModificadas;

    //4. y 5.
    nodoD* listaD = inicListaD();

    //6.
    float bono;

    while(cont == 's')
    {
        printf(">>> Escriba el numero del ejercicio: #");
        scanf("%d", &numSw);

        switch(numSw)
        {
        case 1: //1.
            {
                PasarDeArchivoToListaSimple(&lista);

                MostrarListaSimpleRecursiva(lista); //2.

                break;
            }

        case 2: //2.
            {
                MostrarListaSimpleRecursiva(lista);

                break;
            }

        case 3: //3.
            {
                printf("\nEscriba el id del youtuber a modificar: #");
                scanf("%d", &idModificar);
                printf("\nEscriba la nueva cantidad de vistas del youtuber a modificar: #");
                scanf("%d", &cantVistasModificadas);

                ModificarCantVistas(&lista, idModificar, cantVistasModificadas);

                printf("\n\n### LISTA CON DATO MODIFICADO ###\n\n");

                MostrarListaSimpleRecursiva(lista);

                break;
            }

        case 4: //4.
            {
                listaD = PasarToListaDobleBorrandoElNodo(&lista, "fantasia");

                printf("\n> LISTA DOBLE SOLO DE FANTASIA: \n");
                MostrarListaDobleInversa(listaD);

                printf("\n> LISTA SIMPLE CON FANTASIA BORRADO: \n");
                MostrarListaSimpleRecursiva(lista);

                break;
            }

        case 5: //5.
            {
                printf("\n> LISTA DOBLE SOLO DE FANTASIA: \n");
                MostrarListaDobleInversa(listaD);

                break;
            }

        case 6: //6.
            {
                bono = CalcularPlusMayorNroSubs(listaD);
                printf("El bono otorgado equivale a %.2f\n", bono);

                break;
            }

        default:
            {
                printf("\n|X| ESCRIBA UN EJERCICIO VALIDO |X|\n");

                break;
            }
        }

        ContNY(&cont);
    }

    return 0;
}
