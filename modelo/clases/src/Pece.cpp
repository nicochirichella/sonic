//
// Created by aurore on 18/05/17.
//

#include "../headers/Pece.h"

Pece::Pece(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
           const std::string &rutaImagen, int indexZ, int ancho, int alto) : Enemigo(id, color, coordenadaX,
                                                                                     coordenadaY, cropX, cropY,
                                                                                     rutaImagen, indexZ, ancho,
                                                                                     alto) {
    this->rutaImagen = "images/pece.png";
    this->originalCoordenadaY = coordenadaY;
    this->setSprite();
    this->currentClip = &this->avanzar[0];
    this->puntos= 200;
}

void Pece::setNewX(int escenarioAncho) {
    return;
}

void Pece::setSprite() {
    avanzar[0].x = 0;
    avanzar[0].y = 0;
    avanzar[0].w = 50;
    avanzar[0].h = 60;

    avanzar[1].x = 0;
    avanzar[1].y = 60;
    avanzar[1].w = 50 ;
    avanzar[1].h = 60;

    avanzar[2].x = 0;
    avanzar[2].y = 0;
    avanzar[2].w = 50;
    avanzar[2].h = 60;

    avanzar[3].x = 0;
    avanzar[3].y = 60;
    avanzar[3].w = 50 ;
    avanzar[3].h = 60;
}


void Pece::setNewY() {
    if (this->isDirectionArriba()){
        if (this->getCoordenadaY() - 5 > this->getOriginalCoordenadaY() - zonaDeMovimiento){
            this->setCoordenadaY(this->getCoordenadaY() - 5);
        } else {
            this->setDirectionArriba(false);
            this->setCoordenadaY(this->getCoordenadaY()  + 5);
        }
    }else {
        if (this->getCoordenadaY() + 5 < this->getOriginalCoordenadaY() +  zonaDeMovimiento){
            this->setCoordenadaY(this->getCoordenadaY() + 5);
        } else {
            this->setDirectionArriba(true);
            this->setCoordenadaY(this->getCoordenadaY() - 5);
        }
    }
}

int Pece::getOriginalCoordenadaY() const {
    return originalCoordenadaY;
}

void Pece::setOriginalCoordenadaY(int originalCoordenadaY) {
    Pece::originalCoordenadaY = originalCoordenadaY;
}

int Pece::getZonaDeMovimiento() const {
    return zonaDeMovimiento;
}

void Pece::setZonaDeMovimiento(int zonaDeMovimiento) {
    Pece::zonaDeMovimiento = zonaDeMovimiento;
}

bool Pece::isDirectionArriba() const {
    return directionArriba;
}

void Pece::setDirectionArriba(bool directionArriba) {
    Pece::directionArriba = directionArriba;
}

