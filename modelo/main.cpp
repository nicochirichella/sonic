

#include <string>
#include <iostream>
#include "clases/headers/Entidad.h"
#include "clases/headers/Juego.h"
#include "clases/headers/Log.h"
#include "clases/headers/Constantes.h"
#include "clases/headers/JuegoCliente.h"
#include "clases/headers/JuegoServidor.h"
#include "clases/headers/ParserClienteServidor.h"

using namespace std;

int main(int argc, char *args[]) {

    Log::iniciar(constantes::logEnumModo::BAJO, constantes::logEnumEscritura::CONSOLA);

    if (argc == 1) {
        Log::loguear(constantes::logEnumModo::ALTO, "MAIN", "ERROR: Se necesita el nombre del archivo Json de configuración para iniciar");
    }else if (argc>2) {

        Log::loguear(constantes::logEnumModo::ALTO, "MAIN", "ERROR: La función sólo acepta un parámetro. (Nombre de archivo Json de configuración");
        }else{

        string textoLog="Se pasó por parámetro el nombre de archivo: ";
        string nombreArchivo=args[1];

        Log::loguear(constantes::logEnumModo::ALTO, "MAIN", textoLog+nombreArchivo);
        ParserClienteServidor jsonParser = ParserClienteServidor(nombreArchivo);

        if(jsonParser.esArchivoServidor()){

            JuegoServidor* juegoServidor = jsonParser.getServidor();
            juegoServidor->iniciarServidor();
            juegoServidor->aceptarConexiones();
            juegoServidor->~JuegoServidor();

        }else if(jsonParser.esArchivoCliente()){
            JuegoCliente* juegoCliente = jsonParser.getCliente();
            juegoCliente->InitSDL();
            juegoCliente->~JuegoCliente();

        }else{
            Log::loguear(constantes::logEnumModo::ALTO, "MAIN", "ERROR INTERNO");
        }

    }

    return 0;
}