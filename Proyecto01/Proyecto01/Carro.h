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

	// Getters
	bool getEstado() { return estado; }
	Color getColor() { return color; }
	string getPlaca() { return placa; }
	float getVelocidad() { return velocidad; }
	float getPosicion() { return posicion; }
	float getTiempo() { return tiempo; }

	// Setters
	void setEstado(bool pEstado) { estado = pEstado; }
	void setColor(Color pColor) { color = pColor; }
	void setPlaca(string pPlaca) { placa = pPlaca; }
	void setVelocidad(float pVelocidad) { velocidad = pVelocidad; }
	void setPosicion(float pPosicion) { posicion = pPosicion; }
	void setTiempo(float pTiempo) { tiempo = pTiempo; }

	// Métodos
	void decidirCabina();
	void avanzar(float pVelocidad, float pTiempo, float pPosicion);
}