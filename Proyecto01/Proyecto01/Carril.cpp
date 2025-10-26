#include "Carril.h"

Carril::Carril() {}

// Getters
vector<Carro*> Carril::getCarrosEnCarril() const 
{
    return carrosEnCarril;
}

// Setters
void Carril::setCarrosEnCarril(const vector<Carro*>& pCarrosEnCarril) 
{
    carrosEnCarril = pCarrosEnCarril;
}

/*
* M�todo para agregar un carro al carril
* Observaci�n: Se asume que el puntero pCarro es v�lido
* @param:
*   - Carro* pCarro: Puntero al carro a agregar
* @return:
*   + Ninguno
*/
void Carril::agregarCarro(Carro* pCarro) 
{
    carrosEnCarril.push_back(pCarro);
}

/*
* M�todo para actualizar las posiciones de los carros en el carril
* Observaci�n: Actualiza la posici�n de cada carro en funci�n de su velocidad y el tiempo transcurrido
* @param:
*   - float pTiempo: Tiempo transcurrido
* @return:
*   + Ninguno
*/
void Carril::actualizarPosiciones(float pTiempo) 
{
    for (auto& carro : carrosEnCarril) 
    {
        carro->avanzar(carro->getVelocidad(), pTiempo);
    }
}