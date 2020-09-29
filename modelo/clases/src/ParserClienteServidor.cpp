//
// Created by nicolas on 04/05/17.
//

#include <fstream>
#include "../headers/ParserClienteServidor.h"
#include "../headers/Constantes.h"

#include <string>

using namespace std;


ParserClienteServidor::ParserClienteServidor(std::string nombreArchivo) {
    ifstream archivoDeConfiguracion(FILES_PATH + nombreArchivo);
    Json::Reader reader;

    if (!archivoDeConfiguracion) {
        Log::loguear(constantes::logEnumModo::BAJO, "ParserClienteServidor::Constructor",
                     "No se ha podido abrir el archivo de configuración Cliente-Servidor, se crea sesion Cliente Local.");
        this->crearSesionDefault();
    } else {

        bool parseOk = reader.parse(archivoDeConfiguracion, this->datosDeClienteServidor);

        if (!parseOk || !this->datosDeClienteServidor.isMember("conexion")) {
            Log::loguear(constantes::logEnumModo::MEDIO, "ParserClienteServidor::Constructor",
                         "No se ha podido abrir el archivo de configuración Cliente-Servidor, se crea sesion Cliente Local.");
            Log::loguear(constantes::logEnumModo::MEDIO, "ParserClienteServidor::Constructor",
                         reader.getFormattedErrorMessages());

            this->crearSesionDefault();
        } else {
            this->obtenerDatos();
        }
    }

    Log::loguear(constantes::logEnumModo::ALTO, "ParserClienteServidor::Constructor",
                 "Se abrió el archivo de configuración Cliente-Servidor correctamente.");
}

ParserClienteServidor::~ParserClienteServidor() {

    Log::loguear(constantes::logEnumModo::ALTO, "ParserClienteServidor::Destructor",
                 "ParserClienteServidor destruido.");
}


bool ParserClienteServidor::esArchivoServidor() {

    return !this->tipoCliente;
}

bool ParserClienteServidor::esArchivoCliente() {

    return this->tipoCliente;
}

JuegoServidor *ParserClienteServidor::getServidor() {

    if (this->esArchivoServidor()) {

        return new JuegoServidor(this->puerto, this->cantidadClientes, this->modoDeJuego);
    }

    return NULL;
}

JuegoCliente *ParserClienteServidor::getCliente() {

    if (this->esArchivoCliente()) {

        char *ipChar;
        const char *ipConstantChar = this->ip.c_str();
        ipChar = (char *) ipConstantChar;
        return new JuegoCliente(this->puerto, ipChar);
    }

    return NULL;
}

void ParserClienteServidor::crearSesionDefault() {
    this->ip = "127.0.0.1";
    this->puerto = 8888;
    this->cantidadClientes = 0;
    this->tipoCliente = true;
    Log::loguear(constantes::logEnumModo::MEDIO, "ParserClienteServidor::obtenerPuerto",
                 "La IP fue cargado con valor por defecto: " + this->ip);
    Log::loguear(constantes::logEnumModo::MEDIO, "ParserClienteServidor::obtenerPuerto",
                 "El puerto fue cargado con valor por defecto: " + to_string(this->puerto));

    return;
}

void ParserClienteServidor::obtenerDatos() {

    if (this->obtenerTipo()) {

        if (this->esArchivoCliente()) {

            this->obtenerIP();

        } else {

            this->obtenerCantidadClientes();
            this->obtenerModo();
        }

        this->obtenerPuerto();

    }

    return;
}

bool ParserClienteServidor::obtenerTipo() {

    if (!this->datosDeClienteServidor["conexion"].isMember(this->tipoString)) {
        Log::loguear(constantes::logEnumModo::MEDIO, "ParserClienteServidor::obtenerTipo",
                     "No se encuentra el campo TIPO en el archivo, se creará sesion Cliente Local.");
        this->crearSesionDefault();
        return false;

    }

    Json::Value tipoJson = this->datosDeClienteServidor["conexion"];

    if (!tipoJson[this->tipoString].isString()) {
        Log::loguear(constantes::logEnumModo::MEDIO, "ParserClienteServidor::obtenerTipo",
                     "No se encuentra el TIPO de CONEXION en el archivo o posee un formato erróneo, se creará sesion Cliente Local.");

        this->crearSesionDefault();
        return false;

    } else {
        string tipo = tipoJson[this->tipoString].asString();
        if (tipo == "cliente") {
            Log::loguear(constantes::logEnumModo::MEDIO, "ParserClienteServidor::obtenerTipo",
                         "Se cargarán los datos de sesion Cliente.");

            this->tipoCliente = true;
            return true;
        } else if (tipo == "servidor") {
            Log::loguear(constantes::logEnumModo::MEDIO, "ParserClienteServidor::obtenerTipo",
                         "Se cargarán los datos de sesion Servidor.");

            this->tipoCliente = false;
            return true;
        }
        Log::loguear(constantes::logEnumModo::MEDIO, "ParserClienteServidor::obtenerTipo",
                     "No se encuentra el TIPO de CONEXION posee un valor erróneo, se creará sesion Cliente Local.");
        this->crearSesionDefault();
        return false;
    }
}

void ParserClienteServidor::obtenerIP() {

    if (!this->datosDeClienteServidor["conexion"].isMember(this->ipServidorString)) {
        Log::loguear(constantes::logEnumModo::MEDIO, "ParserClienteServidor::obtenerIP",
                     "No se encuentra el campo IP en el archivo, se carga valor por defecto (local)");
        this->ip = "127.0.0.1";
        return;

    }

    Json::Value ipJson = this->datosDeClienteServidor["conexion"];

    if (!ipJson[this->ipServidorString].isString()) {
        Log::loguear(constantes::logEnumModo::MEDIO, "ParserClienteServidor::obtenerIP",
                     "La IP posee un formato erróneo, se carga valor por defecto (local)");

        this->ip = "127.0.0.1";
        return;

    } else {
        string ip = ipJson[this->ipServidorString].asString();
//        if (puerto >0) {
        this->ip = ip;
        Log::loguear(constantes::logEnumModo::MEDIO, "ParserClienteServidor::obtenerIP",
                     "La IP fue cargada exitosamente." + this->ip);
        return;

        //      }else {
        /*Log::loguear(constantes::logEnumModo::MEDIO, "ParserClienteServidor::obtenerIP",
                     "El puerto debe ser un valor mayor a 0, se carga valor por defecto (8888).");
        this->ip="127.0.0.1";
        return;*/
        //    }
    }

}

void ParserClienteServidor::obtenerModo() {
    if (  !this->datosDeClienteServidor.isMember("modo")) {
        Log::loguear(constantes::logEnumModo::BAJO, "ParserClienteServidor::obtenerModo",
                     "No se encuentra el campo MODO en el archivo, se carga valor por defecto (Individual)");
        this->modoDeJuego = constantes::modoDeJuego::INDIVIUAL;
        return;

    }
    if (!this->datosDeClienteServidor["modo"].isString()) {
        Log::loguear(constantes::logEnumModo::BAJO, "ParserClienteServidor::obtenerCantidadClientes",
                     "El modo de juego posee un formato erróneo, se carga valor por defecto (Individual)");
        this->modoDeJuego = constantes::modoDeJuego::INDIVIUAL;
        return;
    } else {
        string modo = this->datosDeClienteServidor["modo"].asString();
        if (modo =="equipo"){
            this->modoDeJuego = constantes::modoDeJuego::EQUIPO;
            return;

        }else if (modo == "individual") {
            this->modoDeJuego = constantes::modoDeJuego::INDIVIUAL;
        } else if (modo == "colaborativo"){
            this->modoDeJuego = constantes::modoDeJuego::COLABORATIVO;
        }else{
            Log::loguear(constantes::logEnumModo::BAJO, "ParserClienteServidor::obtenerCantidadClientes",
                         "El modo de juego posee un valor erróneo, se carga valor por defecto (Individual)");
            this->modoDeJuego = constantes::modoDeJuego::INDIVIUAL;
            return;
        }

    }

    return;
}


void ParserClienteServidor::obtenerCantidadClientes() {

    if (!this->datosDeClienteServidor["conexion"].isMember(this->clientesString)) {
        Log::loguear(constantes::logEnumModo::MEDIO, "ParserClienteServidor::obtenerCantidadClientes",
                     "No se encuentra el campo CLIENTES en el archivo, se carga valor por defecto (1)");
        this->cantidadClientes = 1;
        return;

    }

    Json::Value cantidadJson = this->datosDeClienteServidor["conexion"];

    if (!cantidadJson[this->clientesString].isInt()) {
        Log::loguear(constantes::logEnumModo::MEDIO, "ParserClienteServidor::obtenerCantidadClientes",
                     "La cantidad de Clientes aceptados posee un formato erróneo, se carga valor por defecto (1)");

        this->cantidadClientes = 1;
        return;

    } else {
        int cantidad = cantidadJson[this->clientesString].asInt();
        if (cantidad > 0) {
            this->cantidadClientes = cantidad;
            Log::loguear(constantes::logEnumModo::MEDIO, "ParserClienteServidor::obtenerCantidadClientes",
                         "La cantidad de Clientes aceptada fue cargada exitosamente. Valor: " +
                         to_string(this->cantidadClientes));
            return;

        } else {
            Log::loguear(constantes::logEnumModo::MEDIO, "ParserClienteServidor::obtenerCantidadClientes",
                         "La cantidad de Clientes aceptada debe ser un valor mayor a 0, se carga valor por defecto (1).");
            this->cantidadClientes = 1;
            return;
        }
    }
}

void ParserClienteServidor::obtenerPuerto() {

    if (!this->datosDeClienteServidor["conexion"].isMember(this->puertoString)) {
        Log::loguear(constantes::logEnumModo::MEDIO, "ParserClienteServidor::obtenerPuerto",
                     "No se encuentra el campo PUERTO en el archivo, se carga valor por defecto (8888)");
        this->puerto = 8888;
        return;

    }

    Json::Value puertoJson = this->datosDeClienteServidor["conexion"];

    if (!puertoJson[this->puertoString].isInt()) {
        Log::loguear(constantes::logEnumModo::MEDIO, "ParserClienteServidor::obtenerPuerto",
                     "El puerto posee un formato erróneo, se carga valor por defecto (8888)");

        this->puerto = 8888;
        return;

    } else {
        int puerto = puertoJson[this->puertoString].asInt();
        if (puerto > 0) {
            this->puerto = puerto;
            Log::loguear(constantes::logEnumModo::MEDIO, "ParserClienteServidor::obtenerPuerto",
                         "El puerto fue cargado exitosamente.Valor: " + to_string(this->puerto));
            return;

        } else {
            Log::loguear(constantes::logEnumModo::MEDIO, "ParserClienteServidor::obtenerPuerto",
                         "El puerto debe ser un valor mayor a 0, se carga valor por defecto (8888).");
            this->puerto = 8888;
            return;
        }
    }

}

