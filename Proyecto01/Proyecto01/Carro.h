#pragma once

#include <string>
#include <iostream>
#include <time.h>

#include "CabinaPeaje.h"

using namespace std;

const int CANTIDAD_CABINAS = 3;


enum Color
{
	Rojo,
	Azul,
	Verde,
	Amarillo
};

class Carro {
private:
	bool estado; // true: encendido, false: apagado
	Color color; // Rojo, Azul, Verde, Amarillo
	string placa; // Formato: 3 letras '-' y 3 números
	float velocidad; // km/h
	float posicion;  // km
	float tiempo;    // horas

public:
	Carro(bool pEstado, Color pColor, string pPlaca)
	{
		estado = pEstado;
		color = pColor;
		placa = pPlaca;
		velocidad = 0.0;
		posicion = 0.0;
		tiempo = 0.0;
	}

	void decidirCabina();
	void avanzar(float pVelocidad, float pTiempo, float pPosicion);
}