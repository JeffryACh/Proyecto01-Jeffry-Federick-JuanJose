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
		void agregarCarro(Carro* pCarro);
		void actualizarPosiciones(float pTiempo);
}