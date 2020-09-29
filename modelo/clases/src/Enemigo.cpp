//
// Created by aurore on 18/05/17.
//

#include "../headers/Enemigo.h"

Enemigo::Enemigo(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
                 const std::string &rutaImagen, int indexZ, int ancho, int alto) : Rectangulo(id, color, coordenadaX,
                                                                                              coordenadaY, cropX, cropY,
                                                                                              rutaImagen, indexZ, ancho,
                                                                                              alto) {

}

bool Enemigo::isVivo() const {
    return vivo;
}

void Enemigo::setVivo(bool vivo) {
    Enemigo::vivo = vivo;
}

int Enemigo::getIndexSprite() const {
    return indexSprite;
}

void Enemigo::setIndexSprite(int indexSprite) {
    Enemigo::indexSprite = indexSprite;
}

void Enemigo::setSprite() {

}

void Enemigo::setNewX(int escenarioAncho) {
    return;
}

void Enemigo::setNewY() {
    return;
}


void Enemigo::setCurrentClipAvanzar(int frame){
        this->currentClip = &this->avanzar[frame];
}

void Enemigo::dibujar(SDL_Renderer *renderer, int x, int ventanaAncho, int y, int ventanaAlto ) {

    if ( 0 < coordenadaX + x + this->getAncho() && coordenadaX + x < ventanaAncho) {
        dst->x = coordenadaX + x;
        dst->y = coordenadaY + y;
        SDL_Rect *newDst = dst;

        if (this->texture != NULL) {

            SDL_RenderCopyEx(renderer, this->texture, this->currentClip, newDst, 0.0, NULL, SDL_FLIP_NONE);
        } else {
            setFill(renderer);
        }
    }

}

int Enemigo::getPuntos() const {
    return puntos;
}

bool Enemigo::isDirecionIzquierda() const {
    return direcionIzquierda;
}

void Enemigo::setDirecionIzquierda(bool direcionIzquierda) {
    Enemigo::direcionIzquierda = direcionIzquierda;
}



