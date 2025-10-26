#pragma once
#include <string>
#include <allegro5/allegro.h>

using namespace std;

class Carro {
private:
    bool orientacion; // false: horizontal, true: vertical
    bool estado;      // true: en pantalla, false: fuera de pantalla
    int color;        // 0: amarillo, 1: rojo, 2: azul, 3: verde
    string placa; // Formato: 3 letras '-' y 3 números
    float velocidad;  // km/h
    float posicionX, posicionY; // Posición en la pantalla
    float ancho, alto; // Dimensiones del carro
    float tiempo;      // Tiempo en el sistema
    ALLEGRO_BITMAP* img; // Imagen del carro

public:
    Carro();
    Carro(bool pEstado, int pColor, const std::string& pPlaca);

    void dibujar();

    // Getters
    bool getEstado() const;
    int getColor() const;
    string getPlaca() const;
    float getVelocidad() const;
    float getPosicionX() const;
    float getPosicionY() const;
    float getTiempo() const;
    ALLEGRO_BITMAP* getImg() const;
    float getAncho() const;
    float getAlto() const;

    // Setters
    void setEstado(bool pEstado);
    void setColor(int pColor);
    void setPlaca(const string& pPlaca);
    void setVelocidad(float pVelocidad);
    void setPosicion(float pPosicionX, float pPosicionY);
    void setTiempo(float pTiempo);
    void setDimension(float pAncho, float pAlto);
    void setImg(ALLEGRO_BITMAP* pImg);

    // Métodos
    void decidirCabina();
    void avanzar(float pVelocidad, float pTiempo);
};