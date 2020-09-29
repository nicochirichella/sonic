//
// Created by panasonic.
//

#ifndef UNTITLED_CAPA_H
#define UNTITLED_CAPA_H

#include <iostream>
#include <SDL.h>
#include "Dibujable.h"

class Capa : public Dibujable{
private:
    int alto;
    int ancho;
    int id;
    int originalX;

public:

    Capa(int id, std::string rutaImagen, int alto, int ancho, int indexZ, int x);
    ~Capa();
    void dibujar(SDL_Renderer *renderer, int x, int ventanaAncho, int y, int ventanaAlto);
    bool load(SDL_Renderer *renderer);
    void setCoordenadaX(int x);
};

#endif //UNTITLED_CAPA_H
