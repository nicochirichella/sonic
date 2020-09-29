//
// Created by nicolas on 19/03/17.
//

#ifndef UNTITLED_NIVEL_H
#define UNTITLED_NIVEL_H


#include "Escenario.h"

class Nivel {
private:
    Escenario* escenario;
    int id;
    int CantMaxPeces;
    int CantMaxCangrejos;
    int CantMaxPiedras;
    int CantMaxBonus;
    int CantMaxMoscas;
    int CantMaxAnillos;
    int CantMaxPinches;
    int CantMaxInvincibilidad;
    int CantMaxBonusAnillo;
    int CantMaxEscudo;
    int CantMinPeces;
    int CantMinCangrejos;
    int CantMinPiedras;
    int CantMinBonus;
    int CantMinMoscas;
    int CantMinAnillos;
    int CantMinPinches;
    int CantMinInvincibilidad;
    int CantMinBonusAnillo;
    int CantMinEscudo;
    int CantTotalPeces;
    int CantTotalCangrejos;
    int CantTotalPiedras;
    int CantTotalBonus;
    int CantTotalMoscas;
    int CantTotalAnillos;
    int CantTotalPinches;
    int CantTotalInvincibilidad;
    int CantTotalBonusAnillo;
    int CantTotalEscudo;

public:
    int getCantMinInvincibilidad() const;

    void setCantMinInvincibilidad(int CantMinInvincibilidad);

    int getCantMinBonusAnillo() const;

    void setCantMinBonusAnillo(int CantMinBonusAnillo);

    int getCantMinEscudo() const;

    void setCantMinEscudo(int CantMinEscudo);

    int getCantTotalInvincibilidad() const;

    void setCantTotalInvincibilidad(int CantTotalInvincibilidad);

    int getCantTotalBonusAnillo() const;

    void setCantTotalBonusAnillo(int CantTotalBonusAnillo);

    int getCantTotalEscudo() const;

    void setCantTotalEscudo(int CantTotalEscudo);



public:
    int getCantMaxInvincibilidad() const;

    void setCantMaxInvincibilidad(int CantMaxInvincibilidad);

    int getCantMaxBonusAnillo() const;

    void setCantMaxBonusAnillo(int CantMaxBonusAnillo);

    int getCantMaxEscudo() const;

    void setCantMaxEscudo(int CantMaxEscudo);

    int getCantTotalPeces() const;

    void setCantTotalPeces(int CantTotalPeces);

    int getCantTotalCangrejos() const;

    void setCantTotalCangrejos(int CantTotalCangrejos);

    int getCantTotalPiedras() const;

    void setCantTotalPiedras(int CantTotalPiedras);

    int getCantTotalBonus() const;

    void setCantTotalBonus(int CantTotalBonus);

    int getCantTotalMoscas() const;

    void setCantTotalMoscas(int CantTotalMoscas);

    int getCantTotalAnillos() const;

    void setCantTotalAnillos(int CantTotalAnillos);

    int getCantTotalPinches() const;

    void setCantTotalPinches(int CantTotalPinches);

public:
    Nivel(Escenario* escenario,int id);
    ~Nivel();
    Escenario* getEscenario();
    void setCantMaxPeces(int cantidad);
    int getCantMaxPeces();
    void setCantMaxCangrejos(int cantidad);
    int getCantMaxCangrejos();
    void setCantMaxPiedras(int cantidad);
    int getCantMaxPiedras();
    void setCantMaxBonus(int cantidad);
    int getCantMaxBonus();
    void setCantMaxMoscas(int cantidad);
    int getCantMaxMoscas();
    void setCantMaxAnillos(int cantidad);
    int getCantMaxAnillos();
    void setCantMaxPinches(int cantidad);
    int getCantMaxPinches();


    void setCantMinPeces(int cantidad);
    int getCantMinPeces();
    void setCantMinCangrejos(int cantidad);
    int getCantMinCangrejos();
    void setCantMinPiedras(int cantidad);
    int getCantMinPiedras();
    void setCantMinBonus(int cantidad);
    int getCantMinBonus();
    void setCantMinMoscas(int cantidad);
    int getCantMinMoscas();
    void setCantMinAnillos(int cantidad);
    int getCantMinAnillos();
    void setCantMinPinches(int cantidad);
    int getCantMinPinches();
};


#endif //UNTITLED_NIVEL_H
