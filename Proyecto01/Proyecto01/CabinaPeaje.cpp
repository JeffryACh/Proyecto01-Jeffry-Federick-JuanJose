#include "CabinaPeaje.h"

CabinaPeaje::CabinaPeaje(float pTiempoAtencion) : tiempoAtencion(pTiempoAtencion), cantidadCarrosAtendidos(0) {}

// Getters
float CabinaPeaje::getTiempoAtencion() const
{
    return tiempoAtencion;
}

int CabinaPeaje::getCantidadCarrosAtendidos() const
{
    return cantidadCarrosAtendidos;
}

// Setters
void CabinaPeaje::setTiempoAtencion(float pTiempoAtencion) 
{
    tiempoAtencion = pTiempoAtencion;
}

void CabinaPeaje::setCantidadCarrosAtendidos(int pCantidadCarrosAtendidos) 
{
    cantidadCarrosAtendidos = pCantidadCarrosAtendidos;
}

/*
* Método para procesar un carro en la cabina de peaje
* Observación: Incrementa el contador de carros atendidos y actualiza el tiempo del carro
* @param:
*   - Carro* pCarro: Puntero al carro a procesar
* @return:
*   + Ninguno
*/
void CabinaPeaje::procesarCarro(Carro* pCarro) 
{
    cantidadCarrosAtendidos++;
    pCarro->setTiempo(pCarro->getTiempo() + tiempoAtencion);
}