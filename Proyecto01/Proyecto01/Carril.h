#pragma once
#include <vector>
#include "Carro.h"

using namespace std;

class Carril
{
private:
    vector<Carro*> carrosEnCarril;

public:
    Carril();

    // Getters
    vector<Carro*> getCarrosEnCarril() const;

    // Setters
    void setCarrosEnCarril(const vector<Carro*>& pCarrosEnCarril);

    // Métodos
    void agregarCarro(Carro* pCarro);
    void actualizarPosiciones(float pTiempo);
};