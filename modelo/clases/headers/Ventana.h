//
// Created by nicolas on 19/03/17.
//

#ifndef UNTITLED_VENTANA_H
#define UNTITLED_VENTANA_H


class Ventana {

private:
    int ancho;
    int alto;

public:

    Ventana();

    Ventana(int ancho, int alto);

    ~Ventana();

    void setDimension(int ancho, int alto);

    int getAncho();

    int getAlto();

};


#endif //UNTITLED_VENTANA_H
