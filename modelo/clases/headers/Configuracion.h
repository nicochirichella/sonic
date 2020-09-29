//
// Created by nicolas on 19/03/17.
//

#ifndef UNTITLED_CONFIGURACION_H
#define UNTITLED_CONFIGURACION_H


#include "Ventana.h"

class Configuracion {
private:
    int velocidadScroll;
public:
    Configuracion();
    Configuracion(int velocidadScroll);

    void setVelocidadScroll(int velocidadScroll);

    int getVelocidadScroll();

    ~Configuracion();
};


#endif //UNTITLED_CONFIGURACION_H
