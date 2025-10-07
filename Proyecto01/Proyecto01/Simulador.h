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

	// Getters
	vector<Carril> getCarriles() { return carriles; }
	CabinaPeaje* getCabinas() { return cabinas; }

	// Setters
	void setCarriles(vector<Carril> pCarriles) { carriles = pCarriles; }
	void setCabinas(CabinaPeaje pCabinas[CANTIDAD_CABINAS]) { for (int i = 0; i < CANTIDAD_CABINAS; i++) cabinas[i] = pCabinas[i]; }

	void iniciar();
	void actualizar(float pTiempo);
	void detener();
}