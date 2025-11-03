#include "Carro.h"

/*
* Implementación de la clase Carro
*/

Carro::Carro() : orientacion(false), estado(true), color(0), placa(generarPlacaAleatoria()),
                 velocidad(0), posicionX(0), posicionY(0),
                 ancho(60), alto(30), tiempo(0), img(nullptr), waitTimer(0.0f),
                 peajeCooldown(0.0f),
                 id(0), tiempoGenerado(0.0f), tiempoLlegadaCola(0.0f),
                 tiempoSalida(0.0f), tiempoServicioAsignado(0.0f),
                 cabinaAsignada(nullptr) {}

/*
* Constructor parametrizado
*/
Carro::Carro(bool pEstado, int pColor, const string& pPlaca)
    : orientacion(false), estado(pEstado), color(pColor), placa(pPlaca),
      velocidad(0), posicionX(0), posicionY(0),
      ancho(60), alto(30), tiempo(0), img(nullptr), waitTimer(0.0f),
      peajeCooldown(0.0f),
      id(0), tiempoGenerado(0.0f), tiempoLlegadaCola(0.0f),
      tiempoSalida(0.0f), tiempoServicioAsignado(0.0f),
      cabinaAsignada(nullptr) {}

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

    float origLong = max(origW, origH);
    float targetLong = max(targetW, targetH);
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
* Observación: Actualiza la posición X o Y según la orientación del carro
* @param:
*   - float pVelocidad: Velocidad del carro
*   - float pTiempo: Tiempo transcurrido
* @return:
*   + Ninguno
*/
void Carro::avanzar(float pVelocidad, float pTiempo)
{
    if (!orientacion) { // Movimiento horizontal
        posicionX += pVelocidad * pTiempo;
    } else { // Movimiento vertical
        posicionY += pVelocidad * pTiempo;
    }
}


// getters/setters de cooldown y cabina
void Carro::setCabinaAsignada(CabinaPeaje* c) { cabinaAsignada = c; }
CabinaPeaje* Carro::getCabinaAsignada() const { return cabinaAsignada; }

void Carro::setPeajeCooldown(float t) { peajeCooldown = t; }
float Carro::getPeajeCooldown() const { return peajeCooldown; }

// setters/getters nuevos 
void Carro::setId(int pId) { id = pId; }
int Carro::getId() const { return id; }

void Carro::setTiempoGenerado(float t) { tiempoGenerado = t; }
float Carro::getTiempoGenerado() const { return tiempoGenerado; }

void Carro::setTiempoLlegadaCola(float t) { tiempoLlegadaCola = t; }
float Carro::getTiempoLlegadaCola() const { return tiempoLlegadaCola; }

void Carro::setTiempoSalida(float t) { tiempoSalida = t; }
float Carro::getTiempoSalida() const { return tiempoSalida; }

void Carro::setTiempoServicioAsignado(float t) { tiempoServicioAsignado = t; }
float Carro::getTiempoServicioAsignado() const { return tiempoServicioAsignado; }

/*
* Método para decidir a qué cabina de peaje ir
* Observación: Selecciona la cabina con la cola más corta
* @param:
*   - const vector<CabinaPeaje>& cabinas: Vector de cabinas disponibles
* @return:
*   + int: Índice de la cabina seleccionada
*/
int Carro::decidirCabina(const vector<CabinaPeaje>& cabinas) const {
    int mejor = 0;
    int tamMejor = cabinas.empty() ? 0 : cabinas[0].colaSize();
    for (size_t i = 1; i < cabinas.size(); ++i) {
        int tam = cabinas[i].colaSize();
        if (tam < tamMejor) {
            mejor = static_cast<int>(i);
            tamMejor = tam;
        }
    }
    return mejor;
}

/*
* Método para generar una placa aleatoria
* Observación: La placa tiene el formato 3 letras '-' 3 números
* @param:
*   - Ninguno
* @return:
*   + string: Placa generada aleatoriamente
*/
string Carro::generarPlacaAleatoria() const
{
    const string letras = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const string numeros = "0123456789";
    string placaGenerada;
    for (int i = 0; i < 3; ++i)
        placaGenerada += letras[rand() % letras.size()];
    placaGenerada += '-';
    for (int i = 0; i < 3; ++i)
        placaGenerada += numeros[rand() % numeros.size()];
    return placaGenerada;
}
