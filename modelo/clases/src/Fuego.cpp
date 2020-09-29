//
// Created by aurore on 10/06/17.
//

#include "../headers/Fuego.h"

Fuego::Fuego(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
             const std::string &rutaImagen, int indexZ, int ancho, int alto, bool direcionIzquierda) : Enemigo(id, color, coordenadaX,
                                                                                       coordenadaY, cropX, cropY,
                                                                                       rutaImagen, indexZ, ancho,
                                                                                       alto) {
    this->rutaImagen = "images/fuego.png";
    this->coordenadaY = 2200;
    this->coordenadaX = 150;
    this->setSprite();
    this->currentClip = &this->avanzar[1];
    this->direcionIzquierda = direcionIzquierda;

}
void Fuego::setNewX(){
    if (this->movimientoLateral){
        if (direcionIzquierda){
            while(this->getCoordenadaX() - 5 > coordenadaXOrigen - zonaDeMovimiento ){
                this->setCoordenadaX(this->getCoordenadaX() - 5 );
                return;
            }
            this->setCoordenadaY(2200);
            movimientoLateral = false;
            movimientoIniciado = false;
        }else{
            while( this->getCoordenadaX() + 5 < coordenadaXOrigen + zonaDeMovimiento ){
                this->setCoordenadaX(this->getCoordenadaX() + 5 );
                return;
            }
            this->setCoordenadaY(2200);
            movimientoLateral = false;
            movimientoIniciado = false;
        }
    }
}

void Fuego::setNewY(int yEnemigoFinal, int xEnemigoFinal, bool direcionIzquierda){
    if (movimientoBajando){
        movimientoLateral = false;
        if (!movimientoIniciado){
            this->setCoordenadaY(yEnemigoFinal + 75);
            if (direcionIzquierda){
                this->setCoordenadaX(xEnemigoFinal + 53);
                coordenadaXOrigen = xEnemigoFinal + 53;
            }else{
                this->setCoordenadaX(xEnemigoFinal + 98);
                coordenadaXOrigen = xEnemigoFinal + 98;
            }
            movimientoIniciado = true;
        }
        while (this->getCoordenadaY() + 10 < 620){
            this->setCoordenadaY(this->getCoordenadaY() + 10);
            return;
        }
        movimientoLateral = true;
        movimientoBajando = false;
        movimientoIniciado = false;
    }
}

void Fuego::setSprite(){
    avanzar[0].x = 0;
    avanzar[0].y = 0;
    avanzar[0].w = 29;
    avanzar[0].h = 60;

    avanzar[1].x = 32;
    avanzar[1].y = 0;
    avanzar[1].w = 29 ;
    avanzar[1].h = 60;

    avanzar[2].x = 0;
    avanzar[2].y = 0;
    avanzar[2].w = 25;
    avanzar[2].h = 60;

    avanzar[3].x = 25;
    avanzar[3].y = 0;
    avanzar[3].w = 25 ;
    avanzar[3].h = 60;
}

void Fuego::setMovimientoBajando(bool movimientoBajando) {
    Fuego::movimientoBajando = movimientoBajando;
}
