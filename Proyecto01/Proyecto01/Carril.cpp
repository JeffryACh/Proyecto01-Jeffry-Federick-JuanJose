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
* Método para agregar un carro al carril
* Observación: Se asume que el puntero pCarro es válido
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
* Método para actualizar las posiciones de los carros en el carril
* Observación: Actualiza la posición de cada carro en función de su velocidad y el tiempo transcurrido
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