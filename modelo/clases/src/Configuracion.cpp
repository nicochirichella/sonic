//
// Created by nicolas on 19/03/17.
//

#include <string>
#include "../headers/Log.h"
#include "../headers/Configuracion.h"
#include "../headers/Constantes.h"

Configuracion::Configuracion() {

    this->velocidadScroll=VEL_SCROLL;
    Log::loguear(constantes::logEnumModo::ALTO, "Configuracion::Constructor",
                 "Configuración creada con: VelScroll= "+std::to_string(this->velocidadScroll));

}

Configuracion::Configuracion(int velocidadScroll) {

    if(velocidadScroll>0){
        this->velocidadScroll=velocidadScroll;
    }else{
        this->velocidadScroll=VEL_SCROLL;
        Log::loguear(constantes::logEnumModo::BAJO, "Configuración::Constructor",
                     "La velocidad de Scroll debe ser un valor mayor a 0, se usa valor por defecto.");
    }

    Log::loguear(constantes::logEnumModo::ALTO, "Configuracion::Constructor",
                 "Configuración: VelScroll= "+std::to_string(this->velocidadScroll));
}

void Configuracion::setVelocidadScroll(int velocidadScroll) {
    if(velocidadScroll>0){
        this->velocidadScroll=velocidadScroll;
    }else{
        this->velocidadScroll=VEL_SCROLL;
    }

    Log::loguear(constantes::logEnumModo::ALTO, "Configuracion::Set",
                 "Configuración Seteada con: VelScroll= "+std::to_string(this->velocidadScroll));}

int Configuracion::getVelocidadScroll() {
    return this->velocidadScroll;
}

Configuracion::~Configuracion() {
    Log::loguear(constantes::logEnumModo::ALTO, "Configuracion::Destructor", "Configuracion destruida.");
}
