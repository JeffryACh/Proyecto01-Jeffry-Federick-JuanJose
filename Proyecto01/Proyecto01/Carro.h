#pragma once

#include <string>
#include <iostream>
#include <time.h>

#include "CabinaPeaje.h"

using namespace std;

const int CANTIDAD_CABINAS = 3;




class Carro {
private:
	bool orientacion; // false: horizontal, true: vertical
	bool estado; // true: en pantalla, false: fuera de pantalla
	int color; // 1: rojo, 2: azul, 3: verde, 4: amarillo
	string placa; // Formato: 3 letras '-' y 3 números
	float velocidad; // km/h
	float posicionX, posicioY;
	float ancho, alto; // dimensiones del carro
	float tiempo;    // horas

public:
	Carro(bool pEstado, int pColor, string pPlaca)
	{
		estado = pEstado;
		color = pColor;
		placa = pPlaca;
		velocidad = 0.0;
		posicionX, posicioY, ancho, alto = 0.0;
		tiempo = 0.0;
	}

	// Getters
	bool getEstado() { return estado; }
	int getColor() { return color; }
	string getPlaca() { return placa; }
	float getVelocidad() { return velocidad; }
	float getPosicionX() { return posicionX; }
	float getPosicionY() { return posicioY; }
	float getTiempo() { return tiempo; }

	// Setters
	void setEstado(bool pEstado) { estado = pEstado; }
	void setColor(int pColor) { color = pColor; }
	void setPlaca(string pPlaca) { placa = pPlaca; }
	void setVelocidad(float pVelocidad) { velocidad = pVelocidad; }
	void setPosicionX(float pPosicionX) { posicionX = pPosicionX; }
	void setPosicionY(float pPosicionY) { posicioY = pPosicionY; }
	void setTiempo(float pTiempo) { tiempo = pTiempo; }

	// Métodos
	void decidirCabina();
	void avanzar(float pVelocidad, float pTiempo, float pPosicion);
};