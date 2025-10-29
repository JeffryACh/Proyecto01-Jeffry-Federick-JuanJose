#include "Carro.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <cmath>
#include <algorithm>

/*
* Implementación de la clase Carro
*/
Carro::Carro() : orientacion(false), estado(true), color(0), placa("AAA-000"),
                 velocidad(0), posicionX(0), posicionY(0),
                 ancho(60), alto(30), tiempo(0), img(nullptr), waitTimer(0.0f) {}

/*
* Constructor parametrizado
*/
Carro::Carro(bool pEstado, int pColor, const std::string& pPlaca)
    : orientacion(false), estado(pEstado), color(pColor), placa(pPlaca),
      velocidad(0), posicionX(0), posicionY(0),
      ancho(60), alto(30), tiempo(0), img(nullptr), waitTimer(0.0f) {}

/*
* Método para dibujar el carro en la pantalla
* Escala uniforme basada en la arista mayor (max ancho/alto) para que todas las
* imágenes tengan la misma «presencia» visual sin deformarse.
*/
void Carro::dibujar()
{
    if (!img) return;

    float origW = static_cast<float>(al_get_bitmap_width(img));
    float origH = static_cast<float>(al_get_bitmap_height(img));
    if (origW <= 0.0f || origH <= 0.0f) return;

    float targetW = getAncho();
    float targetH = getAlto();

    float angle = getEstado() ? ALLEGRO_PI / 2.0f : 0.0f;

    float origLong = std::max(origW, origH);
    float targetLong = std::max(targetW, targetH);
    float scale = (origLong > 0.0f) ? (targetLong / origLong) : 1.0f;

    float destCx = getPosicionX() + getAncho() * 0.5f;
    float destCy = getPosicionY() + getAlto() * 0.5f;

    al_draw_scaled_rotated_bitmap(
        img,
        origW * 0.5f,       
        origH * 0.5f,       
        destCx,             
        destCy,             
        scale,              
        scale,              
        angle,             
        0
    );
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

float Carro::getWaitTimer() const
{
    return waitTimer;
}

void Carro::setWaitTimer(float t)
{
    waitTimer = t;
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
*/
void Carro::avanzar(float pVelocidad, float pTiempo)
{
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