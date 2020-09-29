//
// Created by nicolas on 17/03/17.
//

#ifndef UNTITLED_ENTIDAD_H
#define UNTITLED_ENTIDAD_H


#include <string>
#include <SDL.h>
#include "Dibujable.h"
#include <SDL_image.h>
#include "../headers/Log.h"
#include "../headers/Constantes.h"

class Entidad : public Dibujable
{
protected:
    std::string color;
    int coordenadaX;
    int coordenadaXOrigen;
    int coordenadaY;
    int tipo = 0;
public:
    int getCoordenadaXOrigen() const;

    int getTipo() const;

    void setTipo(int tipo);

    Entidad(int id, std::string color, int coordenadaX, int coordenadaY, std::string rutaImagen, int indexZ);

    std::string getColor();

    virtual void dibujar(SDL_Renderer *renderer, int x, int ventanaAncho, int y, int ventanaAlto);

    virtual bool load(SDL_Renderer *renderer);

    Uint32 colorEntidad(SDL_Renderer* renderer);

    void setCoordenadaX(int coordenadaX);

    void setCoordenadaY(int coordenadaY);

    int getCoordenadaX();

    int getCoordenadaY();

    virtual int getAncho();

    virtual int getAlto();

    int getTipo();


};

#endif //UNTITLED_ENTIDAD_H
