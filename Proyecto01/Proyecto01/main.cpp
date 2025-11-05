/*
* Proyecto 01: Simulador de Flujo de Tráfico y Peaje
* @Authors:
*   - Jeffry Araya Ch.
*   - Federick Fernandez C.
*   - Juan J. Rojas
*
* Created: 28/09/2025 15:40
* Modified: 04/11/2025 18:15
*/

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <memory>        
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "Simulador.h"

Simulador sim(5 , 0.6f );

#include "Carro.h"

//NOTA IMPORTANTE:  Cambien la imagen del peaje cuando ya este subido este archivo en el repo :)
using namespace std;

const int SCREEN_W = 1200;
const int SCREEN_H = 600;
const float FPS = 60.0f;
/*
TO DO:-------------------------------------------------
hay q hacer:

-Cambiar imagen fondo para que sea unidireccional(paint) (hecho)
-implementar colisiones (hecho)
-Poner peaje con logica (imagen) (hecho)
-carro verde (cambiar tamaño) (hecho)
-carro amarillo (cambiar tamaño) (hecho)
-Estadisticas visuales 
*/
void dibujarFondo(ALLEGRO_BITMAP* fondo) 
{
    al_draw_scaled_bitmap(
        fondo,
        0, 0, al_get_bitmap_width(fondo), al_get_bitmap_height(fondo),
        0, 0, SCREEN_W, SCREEN_H,
        0
    );
}
void dibujarAutos(const vector<shared_ptr<Carro>>& autos) 
{
    for (const auto& a : autos)
        a->dibujar();
}

bool hayCarroAdelante(const Carro& actual, const vector<shared_ptr<Carro>>& autos, float distanciaMinima, float toleranciaCarril)
{
    if (!actual.getEstado())
    {
        for (const auto& otroPtr : autos)
        {
            const Carro* otro = otroPtr.get();
            if (otro == &actual) continue;

           
            if (fabs(actual.getPosicionY() - otro->getPosicionY()) > toleranciaCarril) continue;

            float dx = otro->getPosicionX() - actual.getPosicionX();
            if (dx <= 0.0f) continue;

            float frenteActual = actual.getPosicionX() + actual.getAncho();
            float colaOtro = otro->getPosicionX();
            float separacion = colaOtro - frenteActual;

            if (separacion < distanciaMinima) return true;
        }
    }
    else // vertical 
    {
        for (const auto& otroPtr : autos)
        {
            const Carro* otro = otroPtr.get();
            if (otro == &actual) continue;

           
            if (fabs(actual.getPosicionX() - otro->getPosicionX()) > toleranciaCarril) continue;

            float dy = otro->getPosicionY() - actual.getPosicionY();
            if (dy <= 0.0f) continue;

            float frenteActual = actual.getPosicionY() + actual.getAlto();
            float colaOtro = otro->getPosicionY();
            float separacion = colaOtro - frenteActual;

            if (separacion < distanciaMinima) return true;
        }
    }

    return false;
}


void resolverSolapamientos(vector<shared_ptr<Carro>>& autos)
{
    const float toleranciaCarril = 12.0f;

    for (size_t i = 0; i < autos.size(); ++i)
    {
        for (size_t j = i + 1; j < autos.size(); ++j)
        {
            Carro& a = *autos[i];
            Carro& b = *autos[j];


            if (a.getCabinaAsignada() != nullptr || b.getCabinaAsignada() != nullptr) continue;

            if (a.getEstado() != b.getEstado()) continue;

            if (!a.getEstado()) 
            {
                if (fabs(a.getPosicionY() - b.getPosicionY()) > toleranciaCarril) continue;

                float ax1 = a.getPosicionX();
                float ax2 = a.getPosicionX() + a.getAncho();
                float bx1 = b.getPosicionX();
                float bx2 = b.getPosicionX() + b.getAncho();

                if (!(ax2 < bx1 || ax1 > bx2))
                {
                    if (ax1 < bx1)
                    {
                        float nuevaX = bx1 - a.getAncho() - 1.0f;
                        a.setPosicion(nuevaX, a.getPosicionY());
                        a.setWaitTimer(1.0f);
                    }
                    else
                    {
                        float nuevaX = ax1 - (ax2 - bx1) - 1.0f;
                        b.setPosicion(nuevaX, b.getPosicionY());
                        b.setWaitTimer(1.0f);
                    }
                }
            }
            else // vertical
            {
                if (fabs(a.getPosicionX() - b.getPosicionX()) > toleranciaCarril) continue;

                float ay1 = a.getPosicionY();
                float ay2 = a.getPosicionY() + a.getAlto();
                float by1 = b.getPosicionY();
                float by2 = b.getPosicionY() + b.getAlto();

                if (!(ay2 < by1 || ay1 > by2))
                {
                    if (ay1 < by1)
                    {
                        float nuevaY = by1 - a.getAlto() - 1.0f;
                        a.setPosicion(a.getPosicionX(), nuevaY);
                        a.setWaitTimer(1.0f);
                    }
                    else
                    {
                        float nuevaY = ay1 - (ay2 - by1) - 1.0f;
                        b.setPosicion(b.getPosicionX(), nuevaY);
                        b.setWaitTimer(1.0f);
                    }
                }
            }
        }
    }
}

/*
* Método para generar una placa aleatoria
* Observación: La placa tiene el formato 3 letras '-' 3 números
* @param:
*   - Ninguno
* @return:
*   + string: Placa generada aleatoriamente
*/
string generarPlacaAleatoria() 
{
    string letras = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string numeros = "0123456789";
    string placaGenerada;
    for (int i = 0; i < 3; ++i)
        placaGenerada += letras[rand() % letras.size()];
    placaGenerada += '-';
    for (int i = 0; i < 3; ++i)
        placaGenerada += numeros[rand() % numeros.size()];
    return placaGenerada;
}

/*
* Función para verificar si una placa ya existe en un vector de carros
* Observación: Recorre el vector y compara las placas
* @param:
*   - const string& placa: Placa a verificar
*   - const vector<shared_ptr<Carro>>& autos: Vector de carros
* @return:
*   + bool: true si la placa existe, false en caso contrario
*/
bool exisePlaca(const string& placa, const vector<shared_ptr<Carro>>& autos)
{
    for (const auto& a : autos)
    {
        if (a->getPlaca() == placa)
        {
            return true;
        }
    }
    return false;
}

int main() 
{

    if (!al_init()) 
    {
        cerr << "No se pudo inicializar Allegro culpa de Rodirgo chavez" << endl;
        return 1;
    }

    if (!al_init_image_addon()) 
    {
        cerr << " No se pudo inicializar el addon de imágenes" << endl;
        return 1;
    }

    if (!al_install_keyboard()) 
    {
        cerr << "No se pudo inicializar el teclado." << endl;
        return 1;
    }

    ALLEGRO_DISPLAY* ventana = al_create_display(SCREEN_W, SCREEN_H);
    if (!ventana) 
    {
        cerr << "No se pudo crear la ventana." << endl;
        return 1;
    }

    al_set_window_title(ventana, "Simulador de Flujo de Tráfico y Peaje");

    ALLEGRO_BITMAP* fondo = al_load_bitmap("Fondo.jpg");
    if (!fondo) 
    {
        cerr << "No se pudo cargar la imagen de fondo estres positivo" << endl;
        al_destroy_display(ventana);
        return 1;
    }

    ALLEGRO_BITMAP* imgAmarillo = al_load_bitmap("carroAmarillo.png");
    ALLEGRO_BITMAP* imgRojo = al_load_bitmap("carroRojo.png");
    ALLEGRO_BITMAP* imgAzul = al_load_bitmap("carroAzul.png");
    ALLEGRO_BITMAP* imgVerde = al_load_bitmap("carroVerde.png");

    if (!imgAmarillo || !imgRojo || !imgAzul || !imgVerde) 
    {
        cerr << "No se pudieron cargar las imágenes de los autos." << endl;
        al_destroy_bitmap(fondo);
        al_destroy_display(ventana);
        return 1;
    }

    ALLEGRO_BITMAP* imgPeaje = al_load_bitmap("casetaDePeaje.png");
    if (!imgPeaje) 
    {
        cerr << "No se pudo cargar la imagen del peaje (Image 2 nov 2025, 12_15_24.png)." << endl;
        imgPeaje = nullptr;
    }

    vector<shared_ptr<Carro>> autos;
    srand(time(nullptr));

    float carrilesY[] = { 260, 290, 320 };
    float carrilesX[] = { 560, 590, 620 };

    for (int i = 0; i < 8; i++) 
    {
        auto a = make_shared<Carro>();
        a->setColor(rand() % 4);
        switch (a->getColor()) 
        {
        case 0: a->setImg(imgAmarillo); break;
        case 1: a->setImg(imgRojo); break;
        case 2: a->setImg(imgAzul); break;
        case 3: a->setImg(imgVerde); break;
        }

        bool horizontal = rand() % 2;
        a->setEstado(!horizontal);
        a->setVelocidad(2 + rand() % 3);
        a->setTiempoGenerado(sim.getTiempoSim());
        a->setId(sim.getNextId());

        if (horizontal) 
        {
            a->setDimension(60.0f, 30.0f);
            int maxX = max(50, SCREEN_W - 400); 
            a->setPosicion(static_cast<float>(rand() % maxX), carrilesY[rand() % 3]);
        }
        else 
        {
            a->setDimension(30.0f, 60.0f);
            int maxY = max(50, SCREEN_H - 200);
            a->setPosicion(carrilesX[rand() % 3], static_cast<float>(rand() % maxY));
        }

		string placa = generarPlacaAleatoria();
        while (exisePlaca(placa, autos))
			placa = generarPlacaAleatoria();
		

		a->setPlaca(placa);

        autos.push_back(a);
        sim.agregarVehiculo(a);
    }

    resolverSolapamientos(autos);

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();

    al_register_event_source(queue, al_get_display_event_source(ventana));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());

    bool salir = false;
    bool dibujar = true;
    al_start_timer(timer);

   
    const float distanciaMinima = 50.0f;
    const float toleranciaCarril = 12.0f;
    const float dt = 1.0f / FPS; 

    while (!salir) 
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            salir = true;
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            salir = true;
        else if (ev.type == ALLEGRO_EVENT_TIMER)
            dibujar = true;

        sim.actualizar(dt);

        for (auto& a : autos) 
        {
            Carro* ptr = a.get();

            if (ptr->getCabinaAsignada() == nullptr && ptr->getWaitTimer() <= 0.0f)
            {
                bool bloqueado = hayCarroAdelante(*ptr, autos, distanciaMinima, toleranciaCarril);
                float moveSpeed = bloqueado ? 0.0f : ptr->getVelocidad();

                if (!ptr->getEstado())
                {
                    ptr->setPosicion(ptr->getPosicionX() + moveSpeed, ptr->getPosicionY());
                    if (ptr->getPosicionX() > SCREEN_W) 
                    {
                        ptr->setEstado(true);
                        ptr->setPosicion(carrilesX[rand() % 3], -60);
                    }
                }
                else 
                {
                    ptr->setPosicion(ptr->getPosicionX(), ptr->getPosicionY() + moveSpeed);
                    if (ptr->getPosicionY() > SCREEN_H)
                    {
                        ptr->setEstado(false);
                        ptr->setPosicion(-100, carrilesY[rand() % 3]);
                    }
                }
            }
        }

        resolverSolapamientos(autos);

        if (dibujar && al_is_event_queue_empty(queue)) 
        {
            dibujar = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            dibujarFondo(fondo);

            dibujarAutos(autos);

            if (imgPeaje) 
            {
                int pw = al_get_bitmap_width(imgPeaje);
                int ph = al_get_bitmap_height(imgPeaje);
                const float targetW = 180.0f;                 
                const float scale = targetW / static_cast<float>(pw);
                const float heightScale = 0.6f;                
                const float targetH = static_cast<float>(ph) * scale * heightScale;
                const auto& cabinas = sim.getCabinas();
                if (!cabinas.empty()) 
                {
                    const auto& cab = cabinas.front();              
                    const float offsetY = 146.0f;                      // bajar la imagen de peaje
                    const float offsetX = 80.0f;                       // mover a la derecha de peaje
                    float dx = cab.getPosX() - targetW * 0.5f + offsetX;
                    float dy = cab.getPosY() - targetH * 0.5f + offsetY;
                    al_draw_scaled_bitmap(
                        imgPeaje,
                        0, 0, pw, ph,
                        dx, dy,
                        targetW, targetH,
                        0
                    );
                }
            }

            al_flip_display();
        }
    }
	sim.getEstadisticas().generarDatosVehiculos("DatosVehiculos.csv");
    sim.getEstadisticas().generarCSV("DatosGenerales.csv");


    al_destroy_bitmap(fondo);
    al_destroy_bitmap(imgAmarillo);
    al_destroy_bitmap(imgRojo);
    al_destroy_bitmap(imgAzul);
    al_destroy_bitmap(imgVerde);
    if (imgPeaje) al_destroy_bitmap(imgPeaje);
    al_destroy_display(ventana);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
