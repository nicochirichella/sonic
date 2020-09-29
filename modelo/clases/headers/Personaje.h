//
// Created by nicolas on 26/04/17.


#ifndef SONIC_PERSONAJE_H
#define SONIC_PERSONAJE_H

#include <SDL_ttf.h>
#include <printf.h>
#include "Rectangulo.h"
#include <vector>
using namespace std;
const int RUNING_FORWARD_ANIMATION_FRAMES = 15;
const int RUNING_BACK_ANIMATION_FRAMES = 14;
const int JUMPING_ANIMATION_FRAMES = 9;
const int SQUAT_ANIMATION_FRAMES = 9;

class Personaje : public Rectangulo {
private:
    bool pausa;
    bool arrastrable = true;
    bool saltando = false;
    int indexSprite = 0;
    int velocidadPersonaje = 0;
    int velocidadYPersonaje = 0;
    bool connectado;
    int estado = constantes::eventoEnumCliente::TECLA_DERECHA;
    bool margenIzquierda;
    bool margenDerecha;
    bool miraDerecha = true;
    bool modoAtaque = false;
    SDL_Rect avanzar[RUNING_FORWARD_ANIMATION_FRAMES];
    SDL_Rect retroceder[RUNING_BACK_ANIMATION_FRAMES];
    SDL_Rect saltar[JUMPING_ANIMATION_FRAMES];
    SDL_Rect agacharse[SQUAT_ANIMATION_FRAMES];
    SDL_Rect displayVida[12];
    SDL_Rect* currentClip;
    SDL_Rect* currentClipVida;
    int frameAvanzar = 0;
    int frameRetroceder=0;
    int frameSaltar=0;
    bool llegadoAlFinal = false;
    int vida = 3;
    int puntos = 0;
    bool actualizarPuntosAnillos = false;
    int anillos = 0;
    bool gameOver = false;
    bool escudo = false;
    bool invincible = false;
    bool enMovimiento = false;
    int idEnemigo = -1;
    int idPinche = -1;
    SDL_Texture *escudoTexture;
    SDL_Rect *dstEscudo;
    SDL_Texture *vidaTexture;
    SDL_Rect *dstVida;
    SDL_Texture *invincibilidadTexture0;
    SDL_Texture *invincibilidadTexture1;
    int indiceInvincibilidadTexture = 0;
    int perdiendoVida = 0;
    int perdiendoAnillos = 0;
    SDL_Texture *perdiendoVidaTexture;
    SDL_Texture *perdiendoAnillosTexture;
    SDL_Surface * surfacePuntos;
    SDL_Texture *puntosTexture;
    TTF_Font *gFont;
    SDL_Rect renderQuad = { 20, 50, 150, 40 };
    SDL_Color textColor = { 255, 255, 0 };
    string puntosString;
    int equipo = 0;



public:
    int getEquipo() const;

    void setEquipo(int equipo);

    int getIndiceInvincibilidadTexture() const;

    void setIndiceInvincibilidadTexture(int indiceInvincibilidadTexture);

    bool isGameOver() const;

    void setGameOver(bool gameOver);

    int getAnillos() const;

    void setAnillos(int anillos);

    void setPerdiendoVida(int perdiendoVida);

    int getIdEnemigo() const;

    void setIdEnemigo(int idEnemigo);

    bool isEnMovimiento() const;

    void setEnMovimiento(bool enMovimiento);

    bool isInvincible() const;

    void setInvincible(bool invincible);

    bool isEscudo() const;

    void setEscudo(bool tieneEscudo);

    int getVelocidadYPersonaje() const;

    void setVelocidadYPersonaje(int velocidadYPersonaje);

    int getVida() const;

    void setVida(int vida);

    int getPuntos() const;

    void setPuntos(int puntos);

    Personaje(int id, std::string color, int coordenadaX, int coordenadaY, int cropX, int cropY, std::string rutaImagen,
              int indexZ, int ancho, int alto);

    ~Personaje();
    bool isArrastrable() const;

    void setArrastrable(bool arrastrable);

    int getVelocidadPersonaje() const;

    void setVelocidadPersonaje(int velocidadPersonaje);

    bool isConnectado() const;

    void setConnectado(bool connectado);

    int getIndexSprite() const;

    void setIndexSprite(int indexSprite);

    int getEstado() const;

    void setEstado(int estado);

    void setMargenIzquierda(bool margenIzquierda);

    void setMargenDerecha(bool margenDerecha);

    bool isMargenIzquierda() const;

    bool isMargenDerecha() const;

    bool isMiraDerecha() const;

    void setMiraDerecha(bool miraDerecha);

    int getFrameAvanzar() const;

    void setFrameAvanzar(int indexSprite);

    int getFrameRetroceder() const;

    void setFrameRetroceder(int indexSprite);

    int getFrameSaltar() const;

    void setFrameSaltar(int indexSprite);

    void setSpritesAvanzar();

    void setSpriteRetroceder();

    void setSpriteSaltar();

    void setSpritesAgarcharse();

    void setSpriteVida();

    void dibujar(SDL_Renderer *renderer, int x, int ventanaAncho, int y, int ventanaAlto, bool dibujarVida, int puntos);

    void setPausa(bool);

    SDL_Rect* getVectorAvanazar();

    void setCurrentClipAvanzar(int frame);

    void setCurrentClipRetroceder(int frame);

    void setCurrentClipSaltar(int frame);

    void setCurrentClipVida(int frame);

    bool setCoordenadaXforCamaraMove(int coordenadaX, int ventanaAncho, int margen);

    bool isLlegadoAlFinal() const;

    void setLlegadoAlFinal(bool llegadoAlFinal);

    bool load(SDL_Renderer *);

    bool isModoAtaque() const;

    void setModoAtaque(bool modoAtaque);

    bool isSaltando() const;

    void setSaltando(bool saltando);

    int getIdPinche() const;

    void setIdPinche(int idPinche);

    void setActualizarPuntosAnillos(bool actualizarPuntosAnillos);

    bool isActualizarPuntosAnillos() const;

    int getPerdiendoAnillos() const;

    void setPerdiendoAnillos(int perdiendoAnillos);
};

#endif //SONIC_PERSONAJE_H
