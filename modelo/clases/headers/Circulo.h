//
// Created by nicolas on 17/03/17.
//

#ifndef UNTITLED_CIRCULO_H
#define UNTITLED_CIRCULO_H

#include "Entidad.h"


class Circulo: public Entidad
{
private:
    int radio;
    int cropX;
    int cropY;
public:
    /* Pre: Recibe el id, el color, la coordenada X, la coordenada Y, la ruta de la imagen, el index Z y el radio.
     *
     * Post: Queda inicializado el Circulo con los valores recibidos por parametro.
     */
    Circulo(int id, std::string color, int coordenadaX , int coordenadaY, int cropX, int cropY, std::string rutaImagen, int indexZ, int radio );

    int getRadio();

    void dibujar(SDL_Renderer *renderer, int x, int ventanaAncho, int y, int ventanaAlto);

    void setSpritesEntidad(int x, int y, int w, int h);

    int getCropX() const;

    void setCropX(int cropX);

    int getCropY() const;

    void setCropY(int cropY);

    bool load(SDL_Renderer *);

    void setFill(SDL_Renderer *renderer);

    int getAncho();

    int getAlto();
};

#endif //UNTITLED_CIRCULO_H
