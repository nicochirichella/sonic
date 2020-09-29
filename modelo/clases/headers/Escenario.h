//
// Created by nicolas on 19/03/17.
//

#ifndef UNTITLED_ESCENARIO_H
#define UNTITLED_ESCENARIO_H

#include "../headers/Capa.h"
#include "../headers/Entidad.h"
#include "Dibujable.h"
#include "Personaje.h"
#include "Enemigo.h"
#include "EnemigoFinal.h"
#include "Fuego.h"
#include <vector>

class Escenario {

private:
    int ancho;
    int alto;
    int xCamara;
    int yCamara;
    bool margenDerecha;
    bool margenIzquierda;
    int margenParaVista = 120;
    std::vector<Capa *> capas;
    bool nivelCambiado = false;
    std::vector <Entidad *> entidades;
    std::vector <Entidad *> aguas;
    EnemigoFinal * enemigoFinal;
    std::vector<Fuego *> fuegos = {};
    std::vector<Enemigo *> enemigos;
    std::vector<Personaje *> personajes;


public:
    Escenario();

    Escenario(int ancho, int alto);

    ~Escenario();

    const vector<Fuego *> &getFuegos() const;

    void setFuegos(const vector<Fuego *> &fuegos);


    EnemigoFinal *getEnemigoFinal() const;

    void setCapas(vector<Capa *> capas);

    void iniciarEscenario(SDL_Renderer *renderer, int idCliente);

    int getAlto();

    const vector<Entidad *> &getEntidades() const;

    void setEnemigos(const vector<Enemigo *> &enemigos);

    const vector<Enemigo *> &getEnemigos() const;

    Enemigo* getEnemigo(int id);

    Enemigo* getEnemigoForJuegoCliente(int id);

    Entidad* getEntitad(int id);

    Dibujable* getDibujable(int id);

    void setPersonajes(std::vector<Personaje *> personajes);

    int getAncho();

    void setAlto(int alto);

    void setAncho(int ancho);

    int getXCamara();
    int getYCamara();

    void setXCamara(int xCamara);
    void setYCamara(int YCamara);

    void setMargenIzquierda(bool margen);
    void setMargenDerecha(bool margen);

    void setDibujables(std::vector<Dibujable *> capas, std::vector<Dibujable *> entidades);

    void setEntidades(std::vector<Entidad *> entidades);

    void dibujar(SDL_Renderer *renderer, int ventanaAlto, int ventanaAncho, int velScroll, int idCliente);

    void cambiarPosicionPersonaje(int id, int x, int y, int vida, int puntos, int anillos, int idCliente);

    void cambiarPosicionEnemigos(int id, int x, int y);

    void cambiarPosicionEnemigoFinal(int x, int);

    void cambiarPosicionFuegos(int id, int x, int y);

    void teclaArriba (int id, int velScroll, int ventanaAncho, int frame, bool &estaSaltando);

    void teclaArribaIzquierda (int id, int velScroll, int ventanaAncho, int frame, bool &estaSaltandoIzquierda);

    void quieto (int id, int frame);

    void teclaF1(int id);

    void mooveCamara(int velScroll, int ventanaAncho, int id);

    Personaje *getPersonaje(int id);

    void teclaIzquierda(int id, int velScroll, int ventanaAncho);

    void teclaDerecha(int id, int velScroll, int ventanaAncho);

    vector<int> mirarColisiones(int id, int tecla, bool imortal, int ventanaAncho);

    vector<int> mirarColisionesEnemigos(int id, int tecla,bool imortal);

    Enemigo *getEnemigoById(int id);

    vector<int> mirarColisionesEnemigosForEnemigos(int nbCliente, bool imortal);

    vector<int> mirarColisionesEnemigoFinal(int id, int tecla, bool imortal);

    vector<int> mirarColisionesFuegos(int id, int tecla,bool imortal);

    vector<int> mirarColisionesFuegosForFuegos(int nbCliente, bool imortal);

};


#endif //UNTITLED_ESCENARIO_H
