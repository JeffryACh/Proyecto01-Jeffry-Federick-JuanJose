#include "CabinaPeaje.h"


CabinaPeaje::CabinaPeaje(float pMin, float pMax, float px, float py, float pSpacing)
    : ocupada(false), tiempoRestanteServicio(0.0f),
    tiempoTotalOcupada(0.0f), cantidadCarrosAtendidos(0),
    tiempoAtencionMin(pMin), tiempoAtencionMax(pMax),
    enServicio(nullptr),
    posX(px), posY(py), spacing(pSpacing)
{
}

/*
* Metodo para reposicionar los carros en la cola de la cabina de peaje
* Observacion: Recalcula las posiciones de los carros en la cola basandose en su orden
*/
void CabinaPeaje::reposicionarCola() {
    vector<shared_ptr<Carro>> temp;
    while (!cola.empty()) {
        temp.push_back(cola.front());
        cola.pop();
    }
    for (size_t i = 0; i < temp.size(); ++i) {
        auto& carro = temp[i];
        if (!carro) continue;
        if (!carro->getEstado()) {
            float x = posX - static_cast<float>(i + 1) * spacing;
            float y = carro->getPosicionY();
            carro->setPosicion(x, y);
        } else {
            float x = carro->getPosicionX();
            float y = posY - static_cast<float>(i + 1) * spacing;
            carro->setPosicion(x, y);
        }
        cola.push(carro);
    }
}

/*
* Metodo para encolar un carro en la cabina de peaje
* Observacion: Asigna la cabina al carro y lo anade a la cola
*/
void CabinaPeaje::encolar(shared_ptr<Carro> carro)
{
    if (!carro) return;
    carro->setCabinaAsignada(this);
    cola.push(carro);
    reposicionarCola();
    carro->setTiempoLlegadaCola(carro->getTiempo());
}

/*
* Metodo para obtener el tamano de la cola de la cabina de peaje
* Observacion: Retorna el numero de carros en la cola
*/
int CabinaPeaje::colaSize() const
{
    return static_cast<int>(cola.size());
}

/*
* Metodo para verificar si la cabina de peaje esta ocupada
* Observacion: Retorna el estado de ocupacion de la cabina
*/
bool CabinaPeaje::estaOcupada() const 
{
    return ocupada;
}

/*
* Metodo para iniciar el servicio de un carro en la cabina de peaje
* Observacion: Configura el servicio del carro y actualiza las estadisticas
*/
void CabinaPeaje::iniciarServicio(shared_ptr<Carro> carro, float servicioAsignado, float tiempoSim, Estadisticas& estad) 
{
    if (!carro) return;
    ocupada = true;
    tiempoRestanteServicio = servicioAsignado;
    cantidadCarrosAtendidos++;
    estad.registrarInicioServicio(carro->getId(), tiempoSim, carro->getPlaca(), carro->getEstado(), obtenerColorCarro(carro));
    enServicio = carro;
    carro->setTiempoServicioAsignado(servicioAsignado);
    carro->setWaitTimer(servicioAsignado);
    carro->setCabinaAsignada(this);
    if (!carro->getEstado())
        carro->setPosicion(posX, carro->getPosicionY());
    else 
        carro->setPosicion(carro->getPosicionX(), posY);
}

/*
* Metodo para actualizar el estado de la cabina de peaje
* Observacion: Actualiza el tiempo de servicio y maneja la cola de carros
* Ahora: cuando termina el servicio se registra la salida inmediatamente en Estadisticas
*/
void CabinaPeaje::actualizar(float dt, float tiempoSim, Estadisticas& estad) 
{
    if (ocupada) 
    {
        float reduccion = dt;
        tiempoRestanteServicio -= reduccion;
        tiempoTotalOcupada += reduccion;
        if (tiempoRestanteServicio <= 0.0f) 
        {
            if (enServicio) {
                enServicio->setTiempoSalida(tiempoSim);
                estad.registrarSalida(enServicio->getId(), tiempoSim, enServicio->getTiempoServicioAsignado(), enServicio->getPlaca(), enServicio->getEstado(), obtenerColorCarro(enServicio));
                enServicio->setCabinaAsignada(nullptr);
                enServicio->setTiempoServicioAsignado(0.0f);
                enServicio->setPeajeCooldown(3.0f);
                const float escapeDist = 48.0f;
                if (!enServicio->getEstado())
                    enServicio->setPosicion(posX - escapeDist, enServicio->getPosicionY());
                else
                    enServicio->setPosicion(enServicio->getPosicionX(), posY + escapeDist);
            }
            ocupada = false;
            tiempoRestanteServicio = 0.0f;
            enServicio = nullptr;
        }
    }
    if (!ocupada && !cola.empty())
    {
        shared_ptr<Carro> siguiente = cola.front();
        cola.pop();
        static mt19937 rng((random_device())());
        uniform_real_distribution<float> dist(tiempoAtencionMin, tiempoAtencionMax);
        float servicio = dist(rng);
        iniciarServicio(siguiente, servicio, tiempoSim, estad);

        reposicionarCola();
    }
}

// Getters
float CabinaPeaje::getTiempoTotalOcupada() const { return tiempoTotalOcupada; }
int CabinaPeaje::getCantidadCarrosAtendidos() const { return cantidadCarrosAtendidos; }

// Posicion
float CabinaPeaje::getPosX() const { return posX; }
float CabinaPeaje::getPosY() const { return posY; }
