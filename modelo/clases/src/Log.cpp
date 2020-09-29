//
// Created by dami on 19/03/17.
//


#include "../headers/Log.h"
#include <string>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include "../headers/Constantes.h"
int Log::modo;
std::string Log::pathArchivo;
int Log::escritura;

void Log::loguear(int modoLog, std::string contexto, std::string mensaje) {

    bool escribir = false;
    switch (modo) {
        case (constantes::logEnumModo::ALTO):
            escribir = true;
            break;
        case (constantes::logEnumModo::MEDIO):
            if (modoLog == constantes::logEnumModo::MEDIO || modoLog == constantes::logEnumModo::BAJO) {
                escribir = true;
            }
            break;
        case (constantes::logEnumModo::BAJO):
            if (modoLog == constantes::logEnumModo::BAJO) {
                escribir = true;
            }
            break;
        default:
            escribir = false;
    }

    if (escribir) {
        std::string prefijo = "[ " + getTimestamp() + " MODO: " + stringModo(modoLog) + " CONTEXTO: " + contexto + " ]\n";
        std::string linea = prefijo + mensaje +"\n";

        switch (escritura) {
            case constantes::logEnumEscritura::ARCHIVO:
                Log::escribirArchivo(pathArchivo, linea);
                break;
            case constantes::logEnumEscritura::CONSOLA:
                std::cout << linea << std::endl<< std::endl;
                break;
            case constantes::logEnumEscritura::ARCHIVOYCONSOLA:
                Log::escribirArchivo(pathArchivo, linea);
                std::cout << linea << std::endl<< std::endl;
                break;
        }
    }
}

std::string Log::getTimestamp() {
    time_t ahora = time(0);
    struct tm tiempoStruct;
    char buffer[80];
    tiempoStruct = *localtime(&ahora);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d_%X", &tiempoStruct);
    return buffer;
}

void Log::escribirArchivo(std::string nombreArchivo, std::string linea) {
    std::ofstream archivo;
    archivo.open(nombreArchivo + ".log", std::ios::out | std::ios::app);
    if (archivo.fail()) {
        std::cout << "fail to open the log archive\n";
    }
    archivo << linea << std::endl;
    archivo.close();
}

std::string Log::stringModo(int modo) {
    switch (modo) {
        case (constantes::logEnumModo::ALTO):
            return "Alto";

        case (constantes::logEnumModo::MEDIO):
            return "Medio";

        case (constantes::logEnumModo::BAJO):
            return "Bajo";

        default:
            return "Nada";
    }
}

void Log::iniciar(int modo, int escritura) {
    Log::modo = modo;
    Log::escritura = escritura;
    Log::pathArchivo = Log::getPathArchivo();

}

std::string Log::getPathArchivo() {
    std::string rutaLog = "logs/";
    std::string timestampString = getTimestamp();
    std::string nombreArchivoConPath = rutaLog + "log_" + stringModo(modo) + "_" + getTimestamp();

    return nombreArchivoConPath;
}



