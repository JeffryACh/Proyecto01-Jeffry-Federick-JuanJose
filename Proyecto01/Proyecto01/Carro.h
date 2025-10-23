#pragma once

#include <string>
#include <iostream>
#include <time.h>

// #include "CabinaPeaje.h"

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
	ALLEGRO_BITMAP* img;

public:
	Carro(){}

	Carro(bool pEstado, int pColor, string pPlaca)
	{
		estado = pEstado;
		color = pColor;
		placa = pPlaca;
		velocidad = 0.0;
		posicionX, posicioY, ancho, alto = 0.0;
		tiempo = 0.0;
	}

	void dibujar()
	{
		if (img != nullptr)
		{	
			if (estado) {
				// Aqui es si va bajando
				float bw = al_get_bitmap_width(img);
				float bh = al_get_bitmap_height(img);
				float cx = bw / 2.0f;
				float cy = bh / 2.0f;
				float xscale = ancho / bw;
				float yscale = alto / bh;
				al_draw_scaled_rotated_bitmap(img, cx, cy,
					posicionX + ancho / 2.0f, posicioY + alto / 2.0f,
					xscale, yscale, ALLEGRO_PI / 2.0f, 0);
			}
			else {
				al_draw_scaled_bitmap(
					img, 0, 0, al_get_bitmap_width(img), al_get_bitmap_height(img),
					posicionX, posicioY, ancho, alto,
					0
				);
			}
		}
		else
		{
			cerr << "Error: Imagen del carro no cargada." << endl;
		}
	}
	



	// Getters
	bool getEstado() { return estado; }
	int getColor() { return color; }
	string getPlaca() { return placa; }
	float getVelocidad() { return velocidad; }
	float getPosicionX() { return posicionX; }
	float getPosicionY() { return posicioY; }
	float getTiempo() { return tiempo; }
	ALLEGRO_BITMAP* getImg() { return img; }
	float getAncho() { return ancho; }
	float getAlto() { return alto; }

	// Setters
	void setEstado(bool pEstado) { estado = pEstado; }
	void setColor(int pColor) { color = pColor; }
	void setPlaca(string pPlaca) { placa = pPlaca; }
	void setVelocidad(float pVelocidad) { velocidad = pVelocidad; }
	void setPosicion(float pPosicionX, float pPosicionY) { posicionX = pPosicionX; posicioY = pPosicionY;}
	void setPosicionX(float pPosicionX) { posicionX = pPosicionX; }
	void setPosicionY(float pPosicionY) { posicioY = pPosicionY; }
	void setTiempo(float pTiempo) { tiempo = pTiempo; }
	void setDimension(float pAncho, float pAlto) { ancho = pAncho; alto = pAlto;}
	void setImg(ALLEGRO_BITMAP* pImg) { img = pImg; }

	// Métodos
	void decidirCabina();
	void avanzar(float pVelocidad, float pTiempo, float pPosicion);
};