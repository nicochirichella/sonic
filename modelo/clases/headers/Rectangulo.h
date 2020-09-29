#ifndef UNTITLED_RECTANGULO_H
#define UNTITLED_RECTANGULO_H


#include <iostream>
#include "Entidad.h"
#include <SDL.h>

class Rectangulo : public Entidad {
private:
    int ancho;
    int alto;
    int cropX;
    int cropY;

public:
/* Pre: Recibe el id, el color, la coordenada X, la coordenada Y, la ruta de la imagen, el index Z,
 * el ancho y el alto.
 *
 * Post: Queda inicializado el rectangulo con los valores recibidos por parametro.
 */
    Rectangulo(int id, std::string color, int coordenadaX, int coordenadaY, int cropX, int cropY,
               std::string rutaImagen, int indexZ, int ancho, int alto);

    int getAncho();

    int getAlto();

    void dibujar(SDL_Renderer *renderer, int x, int ventanaAncho, int y, int ventanaAlto);

    void setSpritesEntidad(int x, int y, int w, int h);

    int getCropX() const;

    void setCropX(int cropX);

    int getCropY() const;

    void setCropY(int cropY);

    void setAncho(int ancho);

    void setAlto(int alto);

    bool load(SDL_Renderer *);

    void setFill(SDL_Renderer *renderer);

};


#endif //UNTITLED_RECTANGULO_H
