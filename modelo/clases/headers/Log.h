//
// Created by dami on 19/03/17.
//

#ifndef UNTITLED_LOG_H
#define UNTITLED_LOG_H

#include <string>

/* Recordar llamar al principio del main.cpp al metodo "iniciar" con el modo correspondiente
 * Ejemplo de uso para loguear:
 *  Log::loguear(constantes::logEnumModo::ALTO, "main.cpp", "mensaje1")
 */
class Log {
private:
    static int modo;
    static std::string pathArchivo;
    static int escritura;

    /* Escribe una linea al final del archivo. Si no existe el archivo lo crea. */
    static void escribirArchivo(std::string nombreArchivo, std::string linea);

    /* Devuelve el nombre del modo */
    static std::string stringModo(int modo);

    /* Devuelve un string con el timestamp formateado como YYYY-MM-DD_HH:mm:ss */
    static std::string getTimestamp();

    /* Devuelve el nombre del archivo completo con su path
     * Ejemplo: logs/log_ALTO_YYYY-MM-DD_hh:mm:ss */
    static std::string getPathArchivo();

public:
    /* Se tiene que llamar a este metodo al inicio del programa */
    static void iniciar(int modo, int escritura);

    /* Escribe en el archivo de log si corresponde. Por ejemplo, si la constante del modo log esta
     * seteada en BAJA y se quiere escribir algo en modo MEDIO o ALTO no se va a escribir.
     * El contexto es un string con el nombre del contexto en el que se ejecuta la funcion o codigo
     * que se quiere loguear. Linea es el mensaje a loguear */
    static void loguear(int modo, std::string contexto, std::string linea);
};


#endif //UNTITLED_LOG_H
