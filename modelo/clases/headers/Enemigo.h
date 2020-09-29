//
// Created by aurore on 18/05/17.
//

#ifndef SONIC_ENEMIGO_H
#define SONIC_ENEMIGO_H

#include "Rectangulo.h"

const int MOVING_ANIMATION_FRAMES = 4;

class Enemigo : public Rectangulo{

private:
    bool vivo = true;
    int indexSprite = 0;

protected:
    int puntos = 0;
    SDL_Rect avanzar[MOVING_ANIMATION_FRAMES];
    SDL_Rect* currentClip;
    bool direcionIzquierda = true;


public:
    void setDirecionIzquierda(bool direcionIzquierda);

    bool isDirecionIzquierda() const;

    int getPuntos() const;

    const SDL_Rect *getAvanzar() const;

    bool isVivo() const;

    void setVivo(bool vivo);

    int getIndexSprite() const;

    void setIndexSprite(int indexSprite);

    Enemigo(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
            const std::string &rutaImagen, int indexZ, int ancho, int alto);

    void setSprite();

    virtual void setNewX(int escenarioAncho);

    virtual void setNewY();

    virtual void setCurrentClipAvanzar(int frame);

    void dibujar(SDL_Renderer *renderer, int x, int ventanaAncho, int y, int ventanaAlto ) ;


};


#endif //SONIC_ENEMIGO_H
