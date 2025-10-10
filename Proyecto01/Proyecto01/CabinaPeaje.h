#pragma once
#include <string>
#include <iostream>
#include <time.h>

#include "Carro.h"

using namespace std;

typedef struct ColaCarros
{
	Carro* carro;
	ColaCarros* siguiente;
} NodoColaCarros;

class CabinaPeaje
{
private:
	float tiempoAtencion; // tiempo en horas que tarda en atender un carro
	NodoColaCarros* frente; // puntero al primer carro en la cola
	NodoColaCarros* fin; // puntero al último carro en la cola
	int cantidadCarrosAtendidos; // contador de carros atendidos

public:
	CabinaPeaje(float pTiempoAtencion)
	{
		tiempoAtencion = pTiempoAtencion;
		frente = nullptr;
		fin = nullptr;
		cantidadCarrosAtendidos = 0;
	}

	// Getters
	float getTiempoAtencion() { return tiempoAtencion; }
	NodoColaCarros* getFrente() { return frente; }
	NodoColaCarros* getFin() { return fin; }
	int getCantidadCarrosAtendidos() { return cantidadCarrosAtendidos; }

	// Setters
	void setTiempoAtencion(float pTiempoAtencion) { tiempoAtencion = pTiempoAtencion; }
	void setFrente(NodoColaCarros* pFrente) { frente = pFrente; }
	void setFin(NodoColaCarros* pFin) { fin = pFin; }
	void setCantidadCarrosAtendidos(int pCantidadCarrosAtendidos) { cantidadCarrosAtendidos = pCantidadCarrosAtendidos; }

	// Métodos
	void procesarCarro(Carro* pCarro);
};