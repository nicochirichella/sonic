//
// Hecho por Panasonic
// Es un singleton

#ifndef UNTITLED_JUEGOSERVIDOR_H
#define UNTITLED_JUEGOSERVIDOR_H

#include "Ventana.h"
#include "Nivel.h"
#include "../headers/Parser.h"
#include "Configuracion.h"
#include <vector>
#include <mutex>
using namespace std;

class JuegoServidor {

private:

    Ventana *ventana;
    Configuracion *configuracion;
    vector<Nivel *> niveles;
    //Atributos Servidor
    int cantidadMaximaConexiones;
    int puerto;
    int socketServidor;
    int nivelActual = 0;
    //Vector de Vector de int. Primera pos: idCliente, Segunda pos: socketCliente
    vector<vector<int>> vectorClientes;
    vector<vector<int>> vectorColision = {{-1,-1},{-1,-1},{-1,-1},{-1,-1}};
    vector<vector<int>> vectorColisionEnemigos = {{-1,-1},{-1,-1},{-1,-1},{-1,-1}};
    vector<vector<int>> vectorColisionEnemigoFinal ={{-1,-1},{-1,-1},{-1,-1},{-1,-1}};
    vector<vector<int>> vectorColisionFuegos ={{-1,-1},{-1,-1},{-1,-1},{-1,-1}};
    vector <int> colisionEnemigosForEnemigos = {-1};
    vector <int> colisionFuegosForFuegos = {-1};
    bool cambiandoNivel = false;
    bool mensajeNuevoNivel = false;
    bool imortal = false;
    bool lockImortal = false;
    mutex enviarMutex;
    int contadorParaEquipos = 0;
    int contadorParaCambioNivel = 0;
    int contadorDesconectado = 0;
    vector <bool> persoDesconectado ={false,false,false,false};
    int modoDeJuego; //a inicializar en el parser !
    std::vector<int> equipo1 = {};
    std::vector<int> equipo2 {};
    int puntosEquipo1 = 0;
    int puntosEquipo2 = 0;
    int puntosColaborativos = 0;
    std::vector<bool> jugadorGameOver ={false,false,false,false};
    int contadorGameOver = 0;
    bool finDeJuego = false;

public:
    bool isCambiandoNivel() const;

    void setCambiandoNivel(bool cambiandoNivel);

    // Levanta el json y carga los niveles.
    JuegoServidor(int puerto, int maximasConexiones, int modo);

    ~JuegoServidor();

    void cerrarJuegoServidor();

    void loopNuevosClientes();

    void iniciarServidor();

    int enviar(int socketCliente, char *buffer, int bufferSize);

    int recibir(int socketCliente, char *buffer, int bufferSize);

    void cerrarServidor();

    void cerrarConexionCliente(int socketCliente);

    void aceptarConexiones();

    void manejarReceive(int idCliente, bool reconexion);

    int getSocketCliente(int idCliente);

    int getIdClienteLiberado();

    void liberarIdCliente(int idCliente);

    void bindSocketIdCliente(int idCliente, int socketCliente);

    void actualizarEstado(char *bufferLectura, char *bufferSend, int idCliente);

    void crearMensajeEstado(char *buffer);

    void threadEstoySaltando(int idCliente);

    void crearMensajeEstadoSaltar(char *buffer, int idCliente);

    void crearMensajeLatido(char *buffer);

    void manejarEnemigos();

    void crearMensajeEnemigos(char *buffer);

    void crearMensajeEnemigoFinal(char *buffer);

    void threadInvincibilidad(int idCliente);

    void threadInvincibilidadPerdidaVida(int idCliente);

    void threadInvincibilidadPerdidaVidaEnemigoFinal();

    void threadImortalidad();

    void randomizarEntidadesEnemigos();

    int random(int i, int j);

    void crearMensajeRandomizar(char *buffer);

    string convertirTipoMensajeEnString(int tipoMensaje);

    string convertirIdString(int tipoMensaje);

    void threadSpinAtack(int idCliente);

    void juegoGanado(int socketServidor);

    void iniciarClienteParaGameOver();

    const vector<vector<int>> &getVectorColision() const;

    void setVectorColision(const vector<vector<int>> &vectorColision);

    void setColision(int idCliente,const vector<int> &colision);

    const vector<vector<int>> &getVectorColisionEnemigos() const;

    void setVectorColisionEnemigos(const vector<vector<int>> &vectorColisionEnemigos);

    void setColisionEnemigos(int idCliente, const vector<int> &colisionEnemigos);

    void setTodasColisiones(int id, int tecla, bool imortal, int ventanaAncho);

    void manejarColisiones(int idCliente);

    void setColisionEnemigoFinal(int idCliente,const vector<int> &colision);

    const vector<vector<int>> &getVectorColisionEnemigoFinal() const;

    void setVectorColisionEnemigoFinal(const vector<vector<int>> &vectorColisionEnemigoFinal);

    const vector<vector<int>> &getVectorColisionFuegos() const;

    void setVectorColisionFuegos(const vector<vector<int>> &vectorColisionFuegos);

    void setColisionFuegos(int idCliente,const vector<int> &colision);


};

#endif //UNTITLED_JUEGOSERVIDOR_H
