//
// Created by aurore on 07/06/17.
//

#include "../headers/EnemigoFinal.h"


int EnemigoFinal::getOriginalCoordenadaX() const {
    return originalCoordenadaX;
}

EnemigoFinal::EnemigoFinal(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
                           const std::string &rutaImagen, int indexZ, int ancho, int alto) : Enemigo(id, color,
                                                                                                     coordenadaX,
                                                                                                     coordenadaY, cropX,
                                                                                                     cropY, rutaImagen,
                                                                                                     indexZ, ancho,
                                                                                                     alto) {
    this->rutaImagen = "images/enemigoFinalbig.png";
    this->originalCoordenadaX = coordenadaX;
    this->originalCoordenadaY = coordenadaY;
    this->coordenadaY = 2200;
    this->setSprite();
    this->currentClip = &this->avanzarFinal[0];
    this->puntos= 1000;
}

int EnemigoFinal::getZonaDeMovimientoX() const {
    return zonaDeMovimientoX;
}

void EnemigoFinal::setZonaDeMovimientoX(int zonaDeMovimiento) {
    EnemigoFinal::zonaDeMovimientoX = zonaDeMovimiento;
}

void EnemigoFinal::setNewX(int escenarioAncho) {
    if (!this->isDirecionIzquierda() ){
        if (this->getCoordenadaX() + velocidad < this->getOriginalCoordenadaX() + zonaDeMovimientoX){
            this->setCoordenadaX(this->getCoordenadaX() + velocidad);
        } else {
            if (contadorQuieto > 0){
                if (contadorQuieto > 45){
                    contadorFuego = 0;
                }else if( contadorQuieto > 40){
                    contadorFuego = 1;
                }else if( contadorQuieto > 35){
                    contadorFuego = 2;
                }else if( contadorQuieto > 30){
                    contadorFuego = 3;
                }else {
                    contadorFuego = -1;
                }
                contadorQuieto --;
            }else{
                contadorQuieto = 50;
                this->setDirecionIzquierda(true);
                this->setCoordenadaX(this->getCoordenadaX() - 10);
            }
        }

    }else {
        if (this->getCoordenadaX() - velocidad > this->getOriginalCoordenadaX() - zonaDeMovimientoX){
            this->setCoordenadaX(this->getCoordenadaX() - velocidad);
        } else {
            if (contadorQuieto > 0){
                if (contadorQuieto > 45){
                    contadorFuego = 0;
                }else if( contadorQuieto > 40){
                    contadorFuego = 1;
                }else if( contadorQuieto > 35){
                    contadorFuego = 2;
                }else if( contadorQuieto > 30){
                    contadorFuego = 3;
                }else {
                    contadorFuego = -1;
                }
                contadorQuieto --;
            }else{
                contadorQuieto = 50;
                this->setDirecionIzquierda(false);
                this->setCoordenadaX(this->getCoordenadaX() + velocidad);
            }
        }
    }
}

void EnemigoFinal::setNewY() {
    if (this->isDireccionArriba()){
        if (this->getCoordenadaY() - 1 > this->originalCoordenadaY - zonaDeMovimientoY){
            this->setCoordenadaY(this->getCoordenadaY() - 1);
        } else {
            this->setDireccionArriba(false);
            this->setCoordenadaY(this->getCoordenadaY()  + 1);
        }
    }else {
        if (this->getCoordenadaY() + 1 < this->originalCoordenadaY +  zonaDeMovimientoY){
            this->setCoordenadaY(this->getCoordenadaY() + 1);
        } else {
            this->setDireccionArriba(true);
            this->setCoordenadaY(this->getCoordenadaY() - 1);
        }
    }
}

void EnemigoFinal::setCurrentClipAvanzarFinal(int frame){
    this->currentClip = &this->avanzarFinal[frame];
}

void EnemigoFinal::setSprite() {
    avanzarFinal[0].x = 0;
    avanzarFinal[0].y = 0;
    avanzarFinal[0].w = 200;
    avanzarFinal[0].h = 150;

    avanzarFinal[1].x = 207;
    avanzarFinal[1].y = 0;
    avanzarFinal[1].w = 200;
    avanzarFinal[1].h = 150;

    avanzarFinal[2].x = 447;
    avanzarFinal[2].y = 0;
    avanzarFinal[2].w = 200;
    avanzarFinal[2].h = 150;

    avanzarFinal[3].x = 676;
    avanzarFinal[3].y = 0;
    avanzarFinal[3].w = 200;
    avanzarFinal[3].h = 150;
/**********************************/

    avanzarFinal[4].x = 0;
    avanzarFinal[4].y = 168;
    avanzarFinal[4].w = 200;
    avanzarFinal[4].h = 150;

    avanzarFinal[5].x = 240;
    avanzarFinal[5].y = 168;
    avanzarFinal[5].w = 200;
    avanzarFinal[5].h = 150;

    avanzarFinal[6].x = 470;
    avanzarFinal[6].y = 168;
    avanzarFinal[6].w = 200;
    avanzarFinal[6].h = 150;

    avanzarFinal[7].x = 687;
    avanzarFinal[7].y = 168;
    avanzarFinal[7].w = 200;
    avanzarFinal[7].h = 150;

/**********************************/

    avanzarFinal[8].x = 0;
    avanzarFinal[8].y = 333;
    avanzarFinal[8].w = 200;
    avanzarFinal[8].h = 150;

    avanzarFinal[9].x = 240;
    avanzarFinal[9].y = 333;
    avanzarFinal[9].w = 200;
    avanzarFinal[9].h = 150;

    avanzarFinal[10].x = 455;
    avanzarFinal[10].y = 333;
    avanzarFinal[10].w = 200;
    avanzarFinal[10].h = 150;

    avanzarFinal[11].x = 672;
    avanzarFinal[11].y = 333;
    avanzarFinal[11].w = 200;
    avanzarFinal[11].h = 150;
}

/*
void EnemigoFinal::setSprite() {
    avanzarFinal[0].x = 0;
    avanzarFinal[0].y = 0;
    avanzarFinal[0].w = 130;
    avanzarFinal[0].h = 100;

    avanzarFinal[1].x = 130;
    avanzarFinal[1].y = 0;
    avanzarFinal[1].w = 130;
    avanzarFinal[1].h = 100;

    avanzarFinal[2].x = 280;
    avanzarFinal[2].y = 0;
    avanzarFinal[2].w = 130;
    avanzarFinal[2].h = 100;

    avanzarFinal[3].x = 420;
    avanzarFinal[3].y = 0;
    avanzarFinal[3].w = 130;
    avanzarFinal[3].h = 100;

    avanzarFinal[4].x = 0;
    avanzarFinal[4].y = 105;
    avanzarFinal[4].w = 130;
    avanzarFinal[4].h = 100;

    avanzarFinal[5].x = 148;
    avanzarFinal[5].y = 105;
    avanzarFinal[5].w = 130;
    avanzarFinal[5].h = 100;

    avanzarFinal[6].x = 293;
    avanzarFinal[6].y = 105;
    avanzarFinal[6].w = 130;
    avanzarFinal[6].h = 100;

    avanzarFinal[7].x = 425;
    avanzarFinal[7].y = 105;
    avanzarFinal[7].w = 130;
    avanzarFinal[7].h = 100;

    avanzarFinal[8].x = 0;
    avanzarFinal[8].y = 200;
    avanzarFinal[8].w = 130;
    avanzarFinal[8].h = 100;

    avanzarFinal[9].x = 140;
    avanzarFinal[9].y = 200;
    avanzarFinal[9].w = 130;
    avanzarFinal[9].h = 100;

    avanzarFinal[10].x = 280;
    avanzarFinal[10].y = 200;
    avanzarFinal[10].w = 130;
    avanzarFinal[10].h = 100;

    avanzarFinal[11].x = 420;
    avanzarFinal[11].y = 200;
    avanzarFinal[11].w = 130;
    avanzarFinal[11].h = 100;
}
*/

int EnemigoFinal::getZonaDeMovimientoY() const {
    return zonaDeMovimientoY;
}

void EnemigoFinal::setZonaDeMovimientoY(int zonaDeMovimientoY) {
    EnemigoFinal::zonaDeMovimientoY = zonaDeMovimientoY;
}

bool EnemigoFinal::isDireccionArriba() const {
    return direccionArriba;
}

void EnemigoFinal::setDireccionArriba(bool direccionArriba) {
    EnemigoFinal::direccionArriba = direccionArriba;
}

int EnemigoFinal::getOriginalCoordenadaY() const {
    return originalCoordenadaY;
}

int EnemigoFinal::getVida() const {
    return vida;
}

void EnemigoFinal::setVida(int vida) {
    EnemigoFinal::vida = vida;
}

bool EnemigoFinal::isInvincible() const {
    return invincible;
}

void EnemigoFinal::setInvincible(bool invincible) {
    EnemigoFinal::invincible = invincible;
}

int EnemigoFinal::getContadorFuego() const {
    return contadorFuego;
}

void EnemigoFinal::setContadorFuego(int contadorFuego) {
    EnemigoFinal::contadorFuego = contadorFuego;
}

int EnemigoFinal::getMuerto() const {
    return muerto;
}

void EnemigoFinal::setMuerto(int muerto) {
    EnemigoFinal::muerto = muerto;
}
