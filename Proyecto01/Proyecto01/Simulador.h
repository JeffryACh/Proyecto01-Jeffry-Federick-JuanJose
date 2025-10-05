#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <time.h>

#include "Carril.h"
#include "CabinaPeaje.h"

using namespace std;

class Simulador
{
private:
	vector<Carril> carriles; // Vector de carriles
	CabinaPeaje cabinas[CANTIDAD_CABINAS]; // Array de cabinas de peaje
public:
	Simulador();
	void iniciar();
	void actualizar(float pTiempo);
	void detener();
}