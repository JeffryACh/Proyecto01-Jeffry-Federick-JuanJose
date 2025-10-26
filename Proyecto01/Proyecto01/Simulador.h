#pragma once
#include <vector>
#include "Carril.h"
#include "CabinaPeaje.h"

using namespace std;

class Simulador {
private:
    vector<Carril> carriles;
    vector<CabinaPeaje> cabinas;

public:
    Simulador();

    void iniciar();
    void actualizar(float pTiempo);
    void detener();
};