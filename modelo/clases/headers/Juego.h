//
// Hecho por Panasonic
// Es un singleton

#ifndef UNTITLED_JUEGO_H
#define UNTITLED_JUEGO_H

#include "Ventana.h"
#include "Nivel.h"
#include "../headers/Parser.h"
#include "Configuracion.h"
#include <vector>
using namespace std;

class Juego {

private:
    Ventana *ventana;
    Configuracion *configuracion;
    vector<Nivel *> niveles;

public:

    // Levanta el json y carga los niveles.
    Juego();

    ~Juego();

    void InitSDL();

    void Menu(SDL_Window* window, SDL_Renderer* renderer);

    // Por ahora solo permite dibujar el nivel.
    void IniciarJuego(SDL_Window* window, SDL_Renderer* renderer);

    void loop(SDL_Window* window, SDL_Renderer *renderer);

    void cerrarJuego(SDL_Renderer *renderer, SDL_Window *window);

    vector<int> parsearMensaje(const char *mensaje);
};

#endif //UNTITLED_JUEGO_H
