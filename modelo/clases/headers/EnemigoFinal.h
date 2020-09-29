//
// Created by aurore on 07/06/17.
//

#ifndef SONIC_ENEMIGOFINAL_H
#define SONIC_ENEMIGOFINAL_H


#include "Enemigo.h"

const int MOVING_ANIMATION_FRAMES_FINAL = 12;

class EnemigoFinal : public Enemigo {
private :
    int originalCoordenadaX;
    int originalCoordenadaY;
    int zonaDeMovimientoX = 180;
    int zonaDeMovimientoY = 6;
    bool direccionArriba = true;
    int contadorQuieto = 50;
    int contadorFuego = -1;
    SDL_Rect avanzarFinal[MOVING_ANIMATION_FRAMES_FINAL];
    int vida = 5;
    int muerto = 10;
    int velocidad = 25;
public:
    int getMuerto() const;

    void setMuerto(int muerto);

private:
    bool invincible = false;

public:
    int getContadorFuego() const;

    void setContadorFuego(int contadorFuego);

    bool isInvincible() const;

    void setInvincible(bool invincible);

    int getOriginalCoordenadaY() const;

    int getVida() const;

    void setVida(int vida);

    EnemigoFinal(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
                 const std::string &rutaImagen, int indexZ, int ancho, int alto);

    int getOriginalCoordenadaX() const;

    void setNewX(int escenarioAncho);

    void setNewY();

    void setSprite();

    int getZonaDeMovimientoX() const;

    void setZonaDeMovimientoX(int zonaDeMovimiento);

    bool isDireccionArriba() const;

    void setDireccionArriba(bool direccionArriba);

    int getZonaDeMovimientoY() const;

    void setZonaDeMovimientoY(int zonaDeMovimientoY);

    virtual void setCurrentClipAvanzarFinal(int frame);

};


#endif //SONIC_ENEMIGOFINAL_H




