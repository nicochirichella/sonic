//
// Created by nicolas on 26/04/17.
//
// Hecho por Panasonic
// Es un singleton

#ifndef UNTITLED_JUEGOCLIENTE_H
#define UNTITLED_JUEGOCLIENTE_H

#include "Ventana.h"
#include "Nivel.h"
#include "../headers/Parser.h"
#include "Configuracion.h"
#include <vector>
using namespace std;

class JuegoCliente {

private:

    Ventana *ventana;
    Configuracion *configuracion;
    vector<Nivel *> niveles;
    int nivelActual = 0;
    bool noIniciaJuego = true;
    // El ultimo evento de teclado
    int evento = -1;
    /** Atributos de cliente **/
    char* ip;
    int puerto;
    int socketCliente;
    int idCliente;
    bool reconexion;
    bool serverDown;
    bool irMenu;
    bool cupoLleno = false;
    bool cableEstaConectado = true;
    bool threadLatidoEstaCorriendo = false;
    bool imagenCableDesconectado = false;
    bool cambioNivel = false;
    bool cambioNivelListo = false;
    bool gameOver = false;
    bool primeraConexion = false;
    int cantidadJugador = 0;
    bool equiposListos = false;
    int modoDeJuego; //a inicializar en el parser !
    std::vector<int> equipo1 = {};
    std::vector<int> equipo2 {};
    bool quitMenu = false;
    bool juegoGanado = false;
    bool mandeEventoQuieto = false;
public:
    // Levanta el json y carga los niveles.
    JuegoCliente(int puerto, char* ip);

    ~JuegoCliente();

    void InitSDL();

    void LoadMenu(SDL_Window* window, SDL_Renderer* renderer);

    void Menu(SDL_Window* window, SDL_Renderer* renderer);

    // Por ahora solo permite dibujar el nivel.
    void IniciarJuegoCliente(SDL_Window* window, SDL_Renderer* renderer);

    void loop(SDL_Window* window, SDL_Renderer *renderer);

    void cerrarJuegoCliente(SDL_Renderer *renderer, SDL_Window *window);

    void mandarEvento(SDL_Event event, SDL_Window *window, SDL_Renderer *renderer);

    int obtenerEvento(SDL_Event event, SDL_Window* window, SDL_Renderer* renderer);

    vector<int> parsearMensaje(const char *mensaje);

    void actualizarNovedades(char *mensaje);

    void actualizarEnemigos (char *mensaje);

    void actualizarEnemigoFinal(char *mensaje);

    void enviarMensajes();

    int recibir(char *buffer, int bufferSize);

    int enviar(char *buffer, int bufferSize);

    void cerrarConexion();

    void manejarReceive();

    bool iniciarCliente();

    int reconocerTipoMensaje(char *buffer);

    void setearId(char *buffer);

    void manejarMensajePrimeraConexion(char *buffer);

    void procesarMensaje(char *buffer);

    void threadMandarEvento(SDL_Window *window, SDL_Renderer *render);

    void mandarEventoEscucharLatido();

    void threadEscucharLatido();

    void actualizarBonus(const char *mensaje );

    void randomizarEscenario(char *buffer);

    void parsearMensajeRandomizado(char *buffer, vector<int> &nivel1, vector<int> &nivel2, vector<int> &nivel3);

    void eliminarEntidadesEnemigosNivel(vector<int> nivel);

    void eliminarEntidadesEnemigosNivel(vector<int> idsNivel, int indiceNivel);

    void gameOverGrafico(SDL_Renderer *renderer);

    void actualizarGameOver(const char *mensaje);

    void actualizarPrimeraConexion(char *mensaje);

    bool isPrimeraConexion() const;

    void setPrimeraConexion(bool primeraConexion);

    bool isCambioNivel() const;

    void setCambioNivel(bool cambioNivel);

    bool isimagenCableDesconectado() const;

    void setimagenCableDesconectado(bool setImagenCableDesconectado);

    void actualizarEquipos(const char *mensaje);

    void MenuEquipo();

    void loopParaMenuEquipo(SDL_Window *window, SDL_Renderer *renderer);

    string convertirTipoMensajeEnString(int tipoMensaje);

    int obtenerEventoMejorado();

    void mandarEventoMejorado(SDL_Window *window, SDL_Renderer *renderer);

    void threadMandarEventoMejorado(SDL_Window *window, SDL_Renderer *renderer);
};

#endif //UNTITLED_JUEGOCLIENTE_H
