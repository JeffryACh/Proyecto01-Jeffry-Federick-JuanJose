#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "Carro.h"

using namespace std;

class Carril
{
	private:
		vector<Carro*> carrosEnCarril; // Vector para almacenar los carros en el carril
	public:
		Carril() 
		{
			carrosEnCarril = vector<Carro*>();
		}

		// Getters
		vector<Carro*> getCarrosEnCarril() { return carrosEnCarril; }

		// Setters
		void setCarrosEnCarril(vector<Carro*> pCarrosEnCarril) { carrosEnCarril = pCarrosEnCarril; }

		// Métodos
		void agregarCarro(Carro* pCarro);
		void actualizarPosiciones(float pTiempo);
}