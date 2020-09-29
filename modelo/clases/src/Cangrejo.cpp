//
// Created by aurore on 18/05/17.
//

#include "../headers/Cangrejo.h"

Cangrejo::Cangrejo(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
                   const std::string &rutaImagen, int indexZ, int ancho, int alto) : Enemigo(id, color, coordenadaX,
                                                                                             coordenadaY, cropX, cropY,
                                                                                             rutaImagen, indexZ, ancho,
                                                                                             alto) {
    this->rutaImagen = "images/cangrejo.png";
    this->originalCoordenadaX = coordenadaX;
    this->setSprite();
    this->currentClip = &this->avanzar[0];
    this->puntos= 100;
}

void Cangrejo::setNewX(int escenarioAncho) {
    if (!this->isDirecionIzquierda() ){
        if (this->getCoordenadaX() + 5 < this->getOriginalCoordenadaX() + zonaDeMovimiento){
            this->setCoordenadaX(this->getCoordenadaX() + 5);
        } else {
            this->setDirecionIzquierda(true);
            this->setCoordenadaX(this->getCoordenadaX() - 5);
        }

    }else {
        if (this->getCoordenadaX() - 5 > this->getOriginalCoordenadaX() - zonaDeMovimiento){
            this->setCoordenadaX(this->getCoordenadaX() - 5);
        } else {
            this->setDirecionIzquierda(false);
            this->setCoordenadaX(this->getCoordenadaX() + 5);
        }
    }
}

void Cangrejo::setNewY() {
    return;
}

int Cangrejo::getOriginalCoordenadaX() const {
    return originalCoordenadaX;
}

void Cangrejo::setOriginalCoordenadaX(int originalCoordenadaX) {
    Cangrejo::originalCoordenadaX = originalCoordenadaX;
}

int Cangrejo::getZonaDeMovimiento() const {
    return zonaDeMovimiento;
}

void Cangrejo::setZonaDeMovimiento(int zonaDeMovimiento) {
    Cangrejo::zonaDeMovimiento = zonaDeMovimiento;
}


void Cangrejo::setSprite() {
    avanzar[0].x = 0;
    avanzar[0].y = 0;
    avanzar[0].w = 60;
    avanzar[0].h = 60;

    avanzar[1].x = 60;
    avanzar[1].y = 0;
    avanzar[1].w = 60 ;
    avanzar[1].h = 60;

    avanzar[2].x = 0;
    avanzar[2].y = 0;
    avanzar[2].w = 60;
    avanzar[2].h = 60;

    avanzar[3].x = 60;
    avanzar[3].y = 0;
    avanzar[3].w = 60 ;
    avanzar[3].h = 60;
}

