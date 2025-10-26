#include "Carro.h"
#include <allegro5/allegro_primitives.h>

/*
* Implementación de la clase Carro
*/
Carro::Carro() : orientacion(false), estado(true), color(0), placa("AAA-000"),
                 velocidad(0), posicionX(0), posicionY(0), ancho(0), alto(0), tiempo(0), img(nullptr) {}

/*
* Constructor parametrizado
*/
Carro::Carro(bool pEstado, int pColor, const std::string& pPlaca)
    : orientacion(false), estado(pEstado), color(pColor), placa(pPlaca),
      velocidad(0), posicionX(0), posicionY(0), ancho(0), alto(0), tiempo(0), img(nullptr) {}


/*
* Método para dibujar el carro en la pantalla
* Observación: Se asume que la imagen del carro ya ha sido cargada en 'img'
* @param:
*   - Ninguno
* @return:
*   + Ninguno
*/
void Carro::dibujar()
{
    if (img) 
    {
        al_draw_scaled_bitmap(img, 0, 0, al_get_bitmap_width(img), al_get_bitmap_height(img),
                              posicionX, posicionY, ancho, alto, 0);
    }
}

// Getters
bool Carro::getEstado() const
{
    return estado;
}

int Carro::getColor() const
{
    return color;
}

string Carro::getPlaca() const
{
    return placa;
}

float Carro::getVelocidad() const
{
    return velocidad;
}

float Carro::getPosicionX() const
{
    return posicionX;
}

float Carro::getPosicionY() const
{
    return posicionY;
}

float Carro::getTiempo() const
{
    return tiempo;
}

ALLEGRO_BITMAP* Carro::getImg() const
{
    return img;
}

float Carro::getAncho() const
{
    return ancho;
}

float Carro::getAlto() const
{
    return alto;
}

//  Setters

void Carro::setEstado(bool pEstado)
{
    estado = pEstado;
}

void Carro::setColor(int pColor)
{
    color = pColor;
}

void Carro::setPlaca(const string &pPlaca)
{
    placa = pPlaca;
}

void Carro::setVelocidad(float pVelocidad)
{
    velocidad = pVelocidad;
}

void Carro::setPosicion(float pPosicionX, float pPosicionY)
{
    posicionX = pPosicionX;
    posicionY = pPosicionY;
}

void Carro::setTiempo(float pTiempo)
{
    tiempo = pTiempo;
}

void Carro::setDimension(float pAncho, float pAlto)
{
    ancho = pAncho;
    alto = pAlto;
}

void Carro::setImg(ALLEGRO_BITMAP* pImg)
{
    img = pImg;
}

/*
* Método para avanzar el carro en función de su velocidad y el tiempo transcurrido
* Observación: Actualiza la posición del carro
* @param:
*   - float pVelocidad: Velocidad del carro
*   - float pTiempo: Tiempo transcurrido
* @return:
*   + Ninguno
*/
void Carro::avanzar(float pVelocidad, float pTiempo)
{
    // Actualiza la posición en función de la velocidad y el tiempo
    if (!orientacion) { // Movimiento horizontal
        posicionX += pVelocidad * pTiempo;
    } else { // Movimiento vertical
        posicionY += pVelocidad * pTiempo;
    }
}

void Carro::decidirCabina()
{
    // Implementación pendiente
}