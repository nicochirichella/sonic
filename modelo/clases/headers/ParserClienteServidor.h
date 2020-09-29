//
// Created by nicolas on 04/05/17.
//

#ifndef SONIC_PARSERCLIENTESERVIDOR_H
#define SONIC_PARSERCLIENTESERVIDOR_H

#include <string>
#include <fstream>
#include <jsoncpp/json/json.h>
#include "../headers/Constantes.h"
#include "../headers/JuegoCliente.h"
#include "../headers/JuegoServidor.h"

using namespace std;

class ParserClienteServidor{
private:
    Json::Value datosDeClienteServidor;
    int puerto;
    int cantidadClientes;
    bool tipoCliente;
    int modoDeJuego;
    string ip;
    string tipoString = "tipo";
    string puertoString = "puerto";
    string clientesString = "clientes";
    string ipServidorString = "ip_servidor";

public:
    ParserClienteServidor(string nombreArchivo);
    ~ParserClienteServidor();
    bool esArchivoServidor();
    bool esArchivoCliente();
    JuegoServidor* getServidor();
    JuegoCliente* getCliente();

private:
    void obtenerDatos();
    void crearSesionDefault();
    bool obtenerTipo();
    void obtenerIP();
    void obtenerCantidadClientes();
    void obtenerModo();
    void obtenerPuerto();

};
#endif //SONIC_PARSERCLIENTESERVIDOR_H
