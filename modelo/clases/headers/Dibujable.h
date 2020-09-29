//
// Created by mariano on 3/25/17.
//

#ifndef UNTITLED_DIBUJABLE_H
#define UNTITLED_DIBUJABLE_H

#include <SDL_render.h>
#include <string>
#include <string>
/**
 * Representa cualquier cosa dibujable en la pantalla
 */
class Dibujable {
protected:
    int indexZ;
    int id;
    std::string rutaImagen;
    SDL_Rect *dst;
    SDL_Texture* texture;





public:
    Dibujable(int indexZ, int id, std::string rutaImagen);
    ~Dibujable();


    virtual void dibujar(SDL_Renderer *renderer, int x, int ventanaAncho, int y, int ventanaAlto);

    int getId();

    void setIndexZ(int indexZ);

    int getIndexZ();

    virtual bool load(SDL_Renderer *renderer);

    std::string getRutaImagen();


    /*
     * Para que el sort() sepa como ordenar.
     * Ordena de menor a mayor indexZ, entonces, los dibujables de menor indexZ
     * se van a dibujar primero, quedando adelante de ellos los de mayor indexZ.
     */
    static bool sortDibujable(Dibujable* instanciaA, Dibujable* instanciaB);

    virtual void setCoordenadaX(int x);


};


#endif //UNTITLED_DIBUJABLE_H
