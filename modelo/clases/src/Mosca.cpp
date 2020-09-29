//
// Created by aurore on 18/05/17.
//

#include "../headers/Mosca.h"

Mosca::Mosca(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
             const std::string &rutaImagen, int indexZ, int ancho, int alto) : Enemigo(id, color, coordenadaX,
                                                                                       coordenadaY, cropX, cropY,
                                                                                       rutaImagen, indexZ, ancho,
                                                                                       alto) {
    this->rutaImagen = "images/mosca.png";
    this->setSprite();
    this->currentClip = &this->avanzar[0];
    this->puntos= 500;
}

void Mosca::setNewX(int escenarioAncho) {
    if(this->isDirecionIzquierda()){
        if (this->getCoordenadaX() - 5 > 0){
            this->setCoordenadaX( this->getCoordenadaX() - 5);
        }else{
            this->setDirecionIzquierda(false);
            this->setCoordenadaX( this->getCoordenadaX() + 5);
        }
    }else{
        if(this->getCoordenadaX() + 5 + this->getAncho() < escenarioAncho){
            this->setCoordenadaX( this->getCoordenadaX() + 5);
        }else{
            this->setDirecionIzquierda(true);
            this->setCoordenadaX( this->getCoordenadaX() - 5);
        }
    }


}

void Mosca::setNewY() {
    return;
}



void Mosca::setSprite() {
    avanzar[0].x = 0;
    avanzar[0].y = 0;
    avanzar[0].w = 75;
    avanzar[0].h = 60;

    avanzar[1].x = 75;
    avanzar[1].y = 0;
    avanzar[1].w = 75 ;
    avanzar[1].h = 60;

    avanzar[2].x = 150;
    avanzar[2].y = 0;
    avanzar[2].w = 75 ;
    avanzar[2].h = 60;

    avanzar[3].x = 225;
    avanzar[3].y = 0;
    avanzar[3].w = 75 ;
    avanzar[3].h = 60;
}

