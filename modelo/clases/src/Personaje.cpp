//
// Created by nicolas on 26/04/17.
//

#include "../headers/Personaje.h"
#include <SDL_ttf.h>

Personaje::Personaje(int id, std::string color, int coordenadaX, int coordenadaY, int cropX, int cropY,
                     std::string rutaImagen, int indexZ,
                     int ancho, int alto) : Rectangulo(id, color, coordenadaX, coordenadaY, cropX, cropY,
                                                       rutaImagen, indexZ, ancho, alto) {
    this->pausa = false;
    this->connectado = false;

    setSpritesAvanzar();
    setSpritesAgarcharse();
    setSpriteRetroceder();
    setSpriteSaltar();
    setSpriteVida();
    this->currentClip = &this->avanzar[0];
    this->currentClipVida = &this->displayVida[0];
    this->setMargenDerecha(false);
    this->setMargenIzquierda(false);
    this->dstEscudo = new SDL_Rect;
    this->dstEscudo->x = this->coordenadaX;
    this->dstEscudo->y = this->coordenadaY;
    this->dstEscudo->h = 140;
    this->dstEscudo->w = 140;
    this->dstVida = new SDL_Rect;
    this->dstVida->x = 0;
    this->dstVida->y = 0;
    this->dstVida->h = 30;
    this->dstVida->w = 100;

    //this->gFont = NULL;


}

Personaje::~Personaje() {
    //SDL_FreeSurface(surfacePuntos);
    delete this->dst;
    delete this->dstEscudo;
    delete this->dstVida;
}

bool Personaje::setCoordenadaXforCamaraMove(int coordenadaX, int ventanaAncho, int margen) {
    if (coordenadaX <= margen) {
//        this->coordenadaX = margen;
        if (this->isConnectado()) {
            this->setMargenIzquierda(true);
            return true;
        }
    } else if (coordenadaX + this->getAncho() + margen > ventanaAncho) {
//        this->coordenadaX = ventanaAncho - this->getAncho() - margen;
        if (this->isConnectado()) {
            this->setMargenDerecha(true);
            return true;
        }
    } else {
        this->coordenadaX = coordenadaX;
        this->setMargenDerecha(false);
        this->setMargenIzquierda(false);
    }
    return false;
}


void Personaje::dibujar(SDL_Renderer *renderer, int x, int ventanaAncho, int y, int ventanaAlto, bool dibujarVidaPuntos,
                        int puntos) {

    if (!pausa) {
        x = 0;
        y = 0;
    }
    if (0 < coordenadaX + x + this->getAncho() && coordenadaX + x < ventanaAncho) {
        dst->x = coordenadaX + x;
        dst->y = coordenadaY + y;
        SDL_Rect *newDst = dst;

        if (this->texture != NULL) {

            /**
             * LO QUE HICE ACA FUE:
             * CURRENT CLIP ARRANCA EN NULL, SI ENTRA AL IF MANDO TOOOODA LA IMAGEN SIN CROPEARLA, SINO LA CROPEO.
             * NEW DST EN PRINCIPIO ES EL DST ORIGINAL, SI ENTRA AL IF, LO CAMBIO PARA LAS COORDENADAS QUE VINO EL JSON Y EL TAMAÑO DE LA IMAGEN.
             * PUEEEEDE QUE SEA ESO QUE HAGA QUE SE MUEVA LA IMAGEN CHIQUITA...
             */
            SDL_RenderCopyEx(renderer, this->texture, this->currentClip, newDst, 0.0, NULL, SDL_FLIP_NONE);
            if (this->isEscudo()) {
                dstEscudo->x = coordenadaX + x - 25;
                dstEscudo->y = coordenadaY + y - 5;
                SDL_RenderCopy(renderer, escudoTexture, NULL, dstEscudo);
            }
            if (this->isInvincible()) {
                if (this->indiceInvincibilidadTexture == 0) {
                    SDL_RenderCopy(renderer, invincibilidadTexture0, NULL, dst);
                } else {
                    SDL_RenderCopy(renderer, invincibilidadTexture1, NULL, dst);
                }
            }
            if (this->perdiendoVida > 0) {
                SDL_RenderCopy(renderer, perdiendoVidaTexture, NULL, dst);
                this->perdiendoVida--;
            }
            if (this->perdiendoAnillos > 0) {
                SDL_RenderCopy(renderer, perdiendoAnillosTexture, NULL, dst);
                this->perdiendoAnillos--;
            }
            if (dibujarVidaPuntos) {
//                SDL_Rect *newDstVida = dstVida;
                SDL_RenderCopyEx(renderer, this->vidaTexture, this->currentClipVida, dstVida, 0.0, NULL, SDL_FLIP_NONE);

                if (this->isActualizarPuntosAnillos()) {
                    this->puntosString = "Puntos :" + std::to_string(this->getPuntos()) ;
                    this->puntosString += " Anillos :" + std::to_string(this->getAnillos());
                    this->surfacePuntos = TTF_RenderText_Blended_Wrapped(this->gFont, this->puntosString.c_str(), textColor, 400);
                    this->puntosTexture = SDL_CreateTextureFromSurface(renderer, surfacePuntos);
                    this->setActualizarPuntosAnillos(false);
                }
                SDL_RenderCopyEx(renderer, this->puntosTexture, NULL, &this->renderQuad, 0.0, NULL, SDL_FLIP_NONE);
            }

        } else {
            setFill(renderer);
        }
    }
}


void Personaje::setPausa(bool boolean) {
    this->pausa = boolean;
}

void Personaje::setSpritesAvanzar() {
    avanzar[0].x = 0;
    avanzar[0].y = 0;
    avanzar[0].w = 95 - avanzar[0].x;
    avanzar[0].h = 110 - avanzar[0].y;

    avanzar[1].x = 110;
    avanzar[1].y = 0;
    avanzar[1].w = 195 - avanzar[1].x;
    avanzar[1].h = 110 - avanzar[1].y;

    avanzar[2].x = 220;
    avanzar[2].y = 0;
    avanzar[2].w = 300 - avanzar[2].x;
    avanzar[2].h = 110 - avanzar[2].y;

    avanzar[3].x = 320;
    avanzar[3].y = 0;
    avanzar[3].w = 410 - avanzar[3].x;
    avanzar[3].h = 110 - avanzar[3].y;

    avanzar[4].x = 420;
    avanzar[4].y = 0;
    avanzar[4].w = 525 - avanzar[4].x;
    avanzar[4].h = 110 - avanzar[4].y;

    avanzar[5].x = 525;
    avanzar[5].y = 0;
    avanzar[5].w = 610 - avanzar[5].x;
    avanzar[5].h = 110 - avanzar[5].y;

    avanzar[6].x = 630;
    avanzar[6].y = 0;
    avanzar[6].w = 710 - avanzar[6].x;
    avanzar[6].h = 110 - avanzar[6].y;

    avanzar[7].x = 730;
    avanzar[7].y = 0;
    avanzar[7].w = 810 - avanzar[7].x;
    avanzar[7].h = 110 - avanzar[7].y;

    avanzar[8].x = 830;
    avanzar[8].y = 0;
    avanzar[8].w = 930 - avanzar[8].x;
    avanzar[8].h = 110 - avanzar[8].y;

    avanzar[9].x = 935;
    avanzar[9].y = 0;
    avanzar[9].w = 1024 - avanzar[9].x;
    avanzar[9].h = 110 - avanzar[9].y;

    avanzar[10].x = 0;
    avanzar[10].y = 120;
    avanzar[10].w = 110 - avanzar[10].x;
    avanzar[10].h = 230 - avanzar[10].y;

    avanzar[11].x = 120;
    avanzar[11].y = 120;
    avanzar[11].w = 210 - avanzar[11].x;
    avanzar[11].h = 230 - avanzar[11].y;

    avanzar[12].x = 220;
    avanzar[12].y = 120;
    avanzar[12].w = 310 - avanzar[12].x;
    avanzar[12].h = 230 - avanzar[12].y;

    avanzar[13].x = 324;
    avanzar[13].y = 120;
    avanzar[13].w = 410 - avanzar[13].x;
    avanzar[13].h = 230 - avanzar[13].y;

    avanzar[14].x = 434;
    avanzar[14].y = 120;
    avanzar[14].w = 515 - avanzar[14].x;
    avanzar[14].h = 230 - avanzar[14].y;


}

void Personaje::setSpriteRetroceder() {
    retroceder[0].x = 930;
    retroceder[0].y = 360;
    retroceder[0].w = 1024 - retroceder[0].x;
    retroceder[0].h = 468 - retroceder[0].y;

    retroceder[1].x = 830;
    retroceder[1].y = 360;
    retroceder[1].w = 920 - retroceder[1].x;
    retroceder[1].h = 468 - retroceder[1].y;

    retroceder[2].x = 730;
    retroceder[2].y = 360;
    retroceder[2].w = 810 - retroceder[2].x;
    retroceder[2].h = 468 - retroceder[2].y;

    retroceder[3].x = 620;
    retroceder[3].y = 360;
    retroceder[3].w = 710 - retroceder[3].x;
    retroceder[3].h = 468 - retroceder[3].y;

    retroceder[4].x = 510;
    retroceder[4].y = 360;
    retroceder[4].w = 610 - retroceder[4].x;
    retroceder[4].h = 468 - retroceder[4].y;

    retroceder[5].x = 420;
    retroceder[5].y = 360;
    retroceder[5].w = 500 - retroceder[5].x;
    retroceder[5].h = 468 - retroceder[5].y;

    retroceder[6].x = 315;
    retroceder[6].y = 360;
    retroceder[6].w = 410 - retroceder[6].x;
    retroceder[6].h = 468 - retroceder[6].y;

    retroceder[7].x = 210;
    retroceder[7].y = 360;
    retroceder[7].w = 300 - retroceder[7].x;
    retroceder[7].h = 468 - retroceder[7].y;

    retroceder[8].x = 100;
    retroceder[8].y = 360;
    retroceder[8].w = 200 - retroceder[8].x;
    retroceder[8].h = 468 - retroceder[8].y;

    retroceder[9].x = 0;
    retroceder[9].y = 360;
    retroceder[9].w = 95 - retroceder[9].x;
    retroceder[9].h = 468 - retroceder[9].y;

    retroceder[10].x = 915;
    retroceder[10].y = 480;
    retroceder[10].w = 1024 - retroceder[10].x;
    retroceder[10].h = 587 - retroceder[10].y;

    retroceder[11].x = 820;
    retroceder[11].y = 480;
    retroceder[11].w = 910 - retroceder[11].x;
    retroceder[11].h = 587 - retroceder[11].y;

    retroceder[12].x = 720;
    retroceder[12].y = 480;
    retroceder[12].w = 800 - retroceder[12].x;
    retroceder[12].h = 587 - retroceder[12].y;

    retroceder[13].x = 610;
    retroceder[13].y = 480;
    retroceder[13].w = 700 - retroceder[13].x;
    retroceder[13].h = 587 - retroceder[13].y;

}

void Personaje::setSpriteSaltar() {
    saltar[0].x = 0;
    saltar[0].y = 240;
    saltar[0].w = 95 - saltar[0].x;
    saltar[0].h = 320 - saltar[0].y;

    saltar[1].x = 110;
    saltar[1].y = 240;
    saltar[1].w = 195 - saltar[1].x;
    saltar[1].h = 320 - saltar[1].y;

    saltar[2].x = 210;
    saltar[2].y = 240;
    saltar[2].w = 300 - saltar[2].x;
    saltar[2].h = 320 - saltar[2].y;

    saltar[3].x = 310;
    saltar[3].y = 240;
    saltar[3].w = 410 - saltar[3].x;
    saltar[3].h = 320 - saltar[3].y;

    saltar[4].x = 420;
    saltar[4].y = 240;
    saltar[4].w = 505 - saltar[4].x;
    saltar[4].h = 320 - saltar[4].y;

    saltar[5].x = 520;
    saltar[5].y = 240;
    saltar[5].w = 610 - saltar[5].x;
    saltar[5].h = 320 - saltar[5].y;

    saltar[6].x = 620;
    saltar[6].y = 240;
    saltar[6].w = 715 - saltar[6].x;
    saltar[6].h = 320 - saltar[6].y;

    saltar[7].x = 720;
    saltar[7].y = 240;
    saltar[7].w = 810 - saltar[7].x;
    saltar[7].h = 320 - saltar[7].y;

    saltar[8].x = 820;
    saltar[8].y = 240;
    saltar[8].w = 920 - saltar[8].x;
    saltar[8].h = 320 - saltar[8].y;

}

void Personaje::setSpritesAgarcharse() {
    agacharse[0].x = 935;
    agacharse[0].y = 600;
    agacharse[0].w = 1024 - agacharse[0].x;
    agacharse[0].h = 685 - agacharse[0].y;

    agacharse[1].x = 830;
    agacharse[1].y = 600;
    agacharse[1].w = 195 - agacharse[1].x;
    agacharse[1].h = 910 - agacharse[1].y;

    agacharse[2].x = 725;
    agacharse[2].y = 600;
    agacharse[2].w = 815 - agacharse[2].x;
    agacharse[2].h = 685 - agacharse[2].y;

    agacharse[3].x = 625;
    agacharse[3].y = 600;
    agacharse[3].w = 715 - agacharse[3].x;
    agacharse[3].h = 685 - agacharse[3].y;

    agacharse[4].x = 515;
    agacharse[4].y = 600;
    agacharse[4].w = 610 - agacharse[4].x;
    agacharse[4].h = 685 - agacharse[4].y;

    agacharse[5].x = 415;
    agacharse[5].y = 600;
    agacharse[5].w = 510 - agacharse[5].x;
    agacharse[5].h = 685 - agacharse[5].y;

    agacharse[6].x = 315;
    agacharse[6].y = 600;
    agacharse[6].w = 410 - agacharse[6].x;
    agacharse[6].h = 685 - agacharse[6].y;

    agacharse[7].x = 210;
    agacharse[7].y = 600;
    agacharse[7].w = 310 - agacharse[7].x;
    agacharse[7].h = 685 - agacharse[7].y;

    agacharse[8].x = 110;
    agacharse[8].y = 600;
    agacharse[8].w = 200 - agacharse[8].x;
    agacharse[8].h = 685 - agacharse[8].y;
}

void Personaje::setSpriteVida() {
    displayVida[0].x = 0;
    displayVida[0].y = 0;
    displayVida[0].w = 100;
    displayVida[0].h = 30;

    displayVida[1].x = 0;
    displayVida[1].y = 35;
    displayVida[1].w = 100;
    displayVida[1].h = 30;

    displayVida[2].x = 0;
    displayVida[2].y = 70;
    displayVida[2].w = 100;
    displayVida[2].h = 30;

    displayVida[3].x = 0;
    displayVida[3].y = 105;
    displayVida[3].w = 100;
    displayVida[3].h = 30;

    displayVida[4].x = 0;
    displayVida[4].y = 140;
    displayVida[4].w = 100;
    displayVida[4].h = 30;

    displayVida[5].x = 0;
    displayVida[5].y = 175;
    displayVida[5].w = 100;
    displayVida[5].h = 30;

    displayVida[6].x = 0;
    displayVida[6].y = 207;
    displayVida[6].w = 100;
    displayVida[6].h = 30;

    displayVida[7].x = 0;
    displayVida[7].y = 240;
    displayVida[7].w = 100;
    displayVida[7].h = 30;

    displayVida[8].x = 0;
    displayVida[8].y = 272;
    displayVida[8].w = 100;
    displayVida[8].h = 30;

    displayVida[9].x = 0;
    displayVida[9].y = 302;
    displayVida[9].w = 100;
    displayVida[9].h = 30;

    displayVida[10].x = 0;
    displayVida[10].y = 338;
    displayVida[10].w = 100;
    displayVida[10].h = 30;

    displayVida[11].x = 0;
    displayVida[11].y = 372;
    displayVida[11].w = 100;
    displayVida[11].h = 30;

}

SDL_Rect *Personaje::getVectorAvanazar() {
    return this->avanzar;
}

void Personaje::setCurrentClipAvanzar(int indexSprite) {

    this->currentClip = &this->avanzar[indexSprite];
}

void Personaje::setCurrentClipRetroceder(int indexSprite) {
    this->currentClip = &this->retroceder[indexSprite];
}

void Personaje::setCurrentClipSaltar(int indexSprite) {

    this->currentClip = &this->saltar[indexSprite];
}

void Personaje::setCurrentClipVida(int frame) {
    this->currentClipVida = &this->displayVida[frame];
}

int Personaje::getFrameAvanzar() const {
    return frameAvanzar;
}

void Personaje::setFrameAvanzar(int frameAvanzar) {
    this->frameAvanzar = frameAvanzar;
}

int Personaje::getFrameRetroceder() const {
    return frameRetroceder;
}

void Personaje::setFrameRetroceder(int frameRetroceder) {
    this->frameRetroceder = frameRetroceder;
}

int Personaje::getFrameSaltar() const {
    return frameSaltar;
}

void Personaje::setFrameSaltar(int frameSaltar) {
    this->frameSaltar = frameSaltar;
}

bool Personaje::isMiraDerecha() const {
    return miraDerecha;
}

void Personaje::setMiraDerecha(bool miraDerecha) {
    this->miraDerecha = miraDerecha;
}

void Personaje::setMargenIzquierda(bool margenIzquierda) {
    Personaje::margenIzquierda = margenIzquierda;
}

void Personaje::setMargenDerecha(bool margenDerecha) {
    Personaje::margenDerecha = margenDerecha;
}

bool Personaje::isMargenIzquierda() const {
    return margenIzquierda;
}

bool Personaje::isMargenDerecha() const {
    return margenDerecha;
}

int Personaje::getIndexSprite() const {
    return indexSprite;
}

void Personaje::setIndexSprite(int indexSprite) {
    Personaje::indexSprite = indexSprite;
}

int Personaje::getEstado() const {
    return estado;
}

void Personaje::setEstado(int estado) {
    Personaje::estado = estado;
}

bool Personaje::isConnectado() const {
    return connectado;
}

void Personaje::setConnectado(bool connectado) {
    Personaje::connectado = connectado;
}

int Personaje::getVelocidadPersonaje() const {
    return velocidadPersonaje;
}

void Personaje::setVelocidadPersonaje(int velocidadPersonaje) {
    Personaje::velocidadPersonaje = velocidadPersonaje;
}

bool Personaje::isArrastrable() const {
    return arrastrable;
}

void Personaje::setArrastrable(bool arrastrable) {
    Personaje::arrastrable = arrastrable;
}

bool Personaje::isLlegadoAlFinal() const {
    return llegadoAlFinal;
}

void Personaje::setLlegadoAlFinal(bool llegadoAlFinal) {
    Personaje::llegadoAlFinal = llegadoAlFinal;
}

int Personaje::getVelocidadYPersonaje() const {
    return velocidadYPersonaje;
}

void Personaje::setVelocidadYPersonaje(int velocidadYPersonaje) {
    this->velocidadYPersonaje = velocidadYPersonaje;
}

int Personaje::getVida() const {
    return vida;
}

void Personaje::setVida(int vida) {
    Personaje::vida = vida;
}

int Personaje::getPuntos() const {
    return puntos;
}

void Personaje::setPuntos(int puntos) {
    Personaje::puntos = puntos;
}

bool Personaje::isInvincible() const {
    return invincible;
}

void Personaje::setInvincible(bool invincible) {
    Personaje::invincible = invincible;
}

bool Personaje::isEscudo() const {
    return escudo;
}

void Personaje::setEscudo(bool tieneEscudo) {
    Personaje::escudo = tieneEscudo;
}


bool Personaje::load(SDL_Renderer *renderer) {
    const std::string CONTEXTO = "Personaje::load";
    std::string textureText = "0";
    SDL_Texture *texture = NULL;

    gFont = TTF_OpenFont("fonts/sonic_comic.ttf", 70);
    if (gFont == NULL) {
        printf("Failed to load sonic font! SDL_ttf Error: %s\n", TTF_GetError());
    }else{
        this->puntosString = "Puntos :" + std::to_string(this->getPuntos()) ;
        this->puntosString += " Anillos :" + std::to_string(this->getAnillos());
        this->surfacePuntos = TTF_RenderText_Blended_Wrapped(this->gFont, this->puntosString.c_str(), textColor, 400);
        this->puntosTexture = SDL_CreateTextureFromSurface(renderer, surfacePuntos);
    }

    SDL_Surface *imagenEscudo = IMG_Load("images/escudoNuevohd.png");
    SDL_Surface *imagenInvin = IMG_Load("images/invicibilidadPerso.png");
    SDL_Surface *imagenInvin2 = IMG_Load("images/invicibilidadPerso2.png");
    SDL_Surface *imagenPerdVida = IMG_Load("images/vidaPerdidabig.png");
    SDL_Surface *imagenPerdAnillos = IMG_Load("images/anillosPerdidosbig.png");
    SDL_Surface *imagenVida = IMG_Load("images/vida.png");


    this->invincibilidadTexture0 = SDL_CreateTextureFromSurface(renderer, imagenInvin);
    this->invincibilidadTexture1 = SDL_CreateTextureFromSurface(renderer, imagenInvin2);
    this->escudoTexture = SDL_CreateTextureFromSurface(renderer, imagenEscudo);
    this->perdiendoVidaTexture = SDL_CreateTextureFromSurface(renderer, imagenPerdVida);
    this->perdiendoAnillosTexture = SDL_CreateTextureFromSurface(renderer, imagenPerdAnillos);
    this->vidaTexture = SDL_CreateTextureFromSurface(renderer, imagenVida);


    SDL_FreeSurface(imagenEscudo);
    SDL_FreeSurface(imagenInvin);
    SDL_FreeSurface(imagenInvin2);
    SDL_FreeSurface(imagenPerdVida);
    SDL_FreeSurface(imagenPerdAnillos);
    SDL_FreeSurface(imagenVida);


    if (this->rutaImagen != "") {
        SDL_Surface *image = IMG_Load(this->rutaImagen.c_str());
        if (image) {
            Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Exito al cargar la imagen");

            texture = SDL_CreateTextureFromSurface(renderer, image);

            if (!texture) {
                Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO,
                             "Fallo al crear la textura. SDL Error: " + std::string(SDL_GetError()));
                return false;
            }

            Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Exito al crear la textura");
            SDL_FreeSurface(image);
            this->texture = texture;
        } else {
            Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO,
                         "Fallo al cargar la imagen. SDL Error: " + std::string(SDL_GetError()));
            std::string pathNotFound = IMAGES_NOTFOUND_PATH;
            SDL_Surface *imageNotFound = IMG_Load(pathNotFound.c_str());
            if (!imageNotFound) {
                Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO,
                             "Fallo al cargar la imagenNotFound. SDL Error: " + std::string(SDL_GetError()));
                return false;
            }
            SDL_Texture *textureNotFound = NULL;
            SDL_SetColorKey(imageNotFound, SDL_TRUE, SDL_MapRGB(imageNotFound->format, 0, 0xFF, 0xFF));

            textureNotFound = SDL_CreateTextureFromSurface(renderer, imageNotFound);

            if (!textureNotFound) {
                Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO,
                             "Fallo al crear la textureNotFound. SDL Error: " + std::string(SDL_GetError()));
                return false;
            }

            this->texture = textureNotFound;
        }
        return true;

    } else {
        return true;
    }

}

void Personaje::setPerdiendoVida(int perdiendoVida) {
    Personaje::perdiendoVida = perdiendoVida;
}

void Personaje::setAnillos(int anillos) {
    Personaje::anillos = anillos;
}

int Personaje::getAnillos() const {
    return anillos;
}

bool Personaje::isEnMovimiento() const {
    return enMovimiento;
}

void Personaje::setEnMovimiento(bool enMovimiento) {
    Personaje::enMovimiento = enMovimiento;
}

int Personaje::getIdEnemigo() const {
    return idEnemigo;
}

void Personaje::setIdEnemigo(int idEnemigo) {
    Personaje::idEnemigo = idEnemigo;
}

bool Personaje::isModoAtaque() const {
    return modoAtaque;
}

void Personaje::setModoAtaque(bool modoAtaque) {
    Personaje::modoAtaque = modoAtaque;
}

bool Personaje::isGameOver() const {
    return gameOver;
}

void Personaje::setGameOver(bool gameOver) {
    Personaje::gameOver = gameOver;
}

bool Personaje::isSaltando() const {
    return saltando;
}

void Personaje::setSaltando(bool saltando) {
    Personaje::saltando = saltando;
}

bool Personaje::isActualizarPuntosAnillos()  const {
    return actualizarPuntosAnillos;
}



int Personaje::getIdPinche() const {
    return idPinche;
}

void Personaje::setIdPinche(int idPinche) {
    Personaje::idPinche = idPinche;
}

void Personaje::setActualizarPuntosAnillos(bool actualizarPuntosAnillos) {
    Personaje::actualizarPuntosAnillos = actualizarPuntosAnillos;
}

int Personaje::getEquipo() const {
    return equipo;
}

void Personaje::setEquipo(int equipo) {
    Personaje::equipo = equipo;
}

int Personaje::getIndiceInvincibilidadTexture() const {
    return indiceInvincibilidadTexture;
}

void Personaje::setIndiceInvincibilidadTexture(int indiceInvincibilidadTexture) {
    Personaje::indiceInvincibilidadTexture = indiceInvincibilidadTexture;
}

int Personaje::getPerdiendoAnillos() const {
    return perdiendoAnillos;
}

void Personaje::setPerdiendoAnillos(int perdiendoAnillos) {
    Personaje::perdiendoAnillos = perdiendoAnillos;
}
