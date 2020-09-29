//
// Created by nicolas on 19/03/17.
//

#include <string>
#include "../headers/Log.h"
#include "../headers/Ventana.h"
#include "../headers/Constantes.h"


Ventana::Ventana() {
    this->alto = ALTO_VENTANA;
    this->ancho = ANCHO_VENTANA;

    Log::loguear(constantes::logEnumModo::ALTO, "Ventana::Constructor",
                 "Ventana creada con Ancho:"+std::to_string(this->ancho)+" , Alto: "+std::to_string(this->alto));
}

Ventana::Ventana(int ancho, int alto) {
    if (alto>0) {
        this->alto = alto;
    } else {
        this->alto = ALTO_VENTANA;
    }
    if (ancho>0) {
        this->ancho = ancho;
    } else {
        this->ancho = ANCHO_VENTANA;
    }

    Log::loguear(constantes::logEnumModo::ALTO, "Ventana::Constructor",
                 "Ventana creada con Ancho:"+std::to_string(this->ancho)+" , Alto: "+std::to_string(this->alto));
}

int Ventana::getAlto() {
    return this->alto;
}

int Ventana::getAncho() {
    return this->ancho;
}

void Ventana::setDimension(int ancho, int alto) {

    if (alto>0) {
        this->alto = alto;
    } else {
        this->alto = ALTO_VENTANA;
    }
    if (ancho>0) {
        this->ancho = ancho;
    } else {
        this->ancho = ANCHO_VENTANA;
    }

    Log::loguear(constantes::logEnumModo::ALTO, "Ventana::Set",
                 "Ventana seteada con Ancho:"+std::to_string(this->ancho)+" , Alto: "+std::to_string(this->alto));
}


Ventana::~Ventana() {
    Log::loguear(constantes::logEnumModo::ALTO, "Ventana::Destructor", "Ventana destruida.");
}