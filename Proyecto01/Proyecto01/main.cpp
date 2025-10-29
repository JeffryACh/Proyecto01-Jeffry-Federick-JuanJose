/*
* Proyecto 01: Simulador de Flujo de Tráfico y Peaje
* @Authors:
*   - Jeffry Araya Ch.
*   - Federick Fernandez C.
*   - Juan J. Rojas
*
* Created: 28/09/2025 15:40
* Modified: 10/10/2025 16:20
*/

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>        
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "Carro.h"

using namespace std;

const int SCREEN_W = 1200;
const int SCREEN_H = 600;
const float FPS = 60.0f;
/*
TO DO:-------------------------------------------------
hay q hacer:

-Cambiar imagen fondo para que sea unidireccional(paint) (hecho)
-implementar colisiones (hecho)
-Poner peaje
-carro verde (cambiar tamaño) (hecho)
-carro amarillo (cambiar tamaño) (hecho)

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
void dibujarAutos(vector<Carro>& autos) 
{
    for (auto& a : autos) 
        a.dibujar();
}


bool hayCarroAdelante(const Carro& actual, const vector<Carro>& autos, float distanciaMinima, float toleranciaCarril)
{
   
    if (!actual.getEstado()) 
    {
        for (const Carro& otro : autos)
        {
            if (&otro == &actual) continue;
            
            if (fabs(actual.getPosicionY() - otro.getPosicionY()) > toleranciaCarril) continue;

           
            float dx = otro.getPosicionX() - actual.getPosicionX();
            if (dx <= 0.0f) continue;

            
            float frenteActual = actual.getPosicionX() + actual.getAncho();
            float colaOtro = otro.getPosicionX();
            float separacion = colaOtro - frenteActual;

            if (separacion < distanciaMinima) return true;
        }
    }
    else // vertical 
    {
        for (const Carro& otro : autos)
        {
            if (&otro == &actual) continue;
     
            if (fabs(actual.getPosicionX() - otro.getPosicionX()) > toleranciaCarril) continue;

            float dy = otro.getPosicionY() - actual.getPosicionY();
            if (dy <= 0.0f) continue;

            float frenteActual = actual.getPosicionY() + actual.getAlto();
            float colaOtro = otro.getPosicionY();
            float separacion = colaOtro - frenteActual;

            if (separacion < distanciaMinima) return true;
        }
    }

    return false;
}


void resolverSolapamientos(vector<Carro>& autos)
{
    const float toleranciaCarril = 12.0f;

    for (size_t i = 0; i < autos.size(); ++i)
    {
        for (size_t j = i + 1; j < autos.size(); ++j)
        {
            Carro& a = autos[i];
            Carro& b = autos[j];

            
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


int main() {

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
        cerr << "No se pudo cargar la imagen de fondo estres positv" << endl;
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

    vector<Carro> autos;
    srand(time(nullptr));

    float carrilesY[] = { 260, 290, 320 };
    float carrilesX[] = { 560, 590, 620 };

    for (int i = 0; i < 8; i++) {
        Carro a;
        a.setColor (rand() % 4 );
        switch (a.getColor()) 
        {
        case 0: a.setImg( imgAmarillo); break;
        case 1: a.setImg( imgRojo); break;
        case 2: a.setImg( imgAzul); break;
        case 3: a.setImg( imgVerde); break;
        }

        bool horizontal = rand() % 2;
        a.setEstado(!horizontal);
        a.setVelocidad( 2 + rand() % 3);

        
        if (horizontal)
        {
            a.setDimension(60.0f, 30.0f); 
            a.setPosicion(static_cast<float>(rand() % SCREEN_W), carrilesY[rand() % 3]);
        }
        else
        {
            a.setDimension(30.0f, 60.0f); 
            a.setPosicion(carrilesX[rand() % 3], static_cast<float>(rand() % SCREEN_H));
        }

        autos.push_back(a);
    }



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

        
        for (auto& a : autos) {
            
            if (a.getWaitTimer() > 0.0f) {
                float nuevo = a.getWaitTimer() - dt;
                a.setWaitTimer(nuevo > 0.0f ? nuevo : 0.0f);
                if (a.getWaitTimer() > 0.0f) continue;
            }

            
            bool bloqueado = hayCarroAdelante(a, autos, distanciaMinima, toleranciaCarril);
            float moveSpeed = bloqueado ? 0.0f : a.getVelocidad();

            
            if (bloqueado && a.getWaitTimer() <= 0.0f) {
                a.setWaitTimer(1.0f);
                moveSpeed = 0.0f;
            }

            if (!a.getEstado()) {  
                a.setPosicion(a.getPosicionX() + moveSpeed, a.getPosicionY());  
                if (a.getPosicionX() > SCREEN_W) {  
                    a.setEstado(true);  
                    a.setPosicion(carrilesX[rand() % 3], -60);  
                }  
            }  
            else {  
                a.setPosicion(a.getPosicionX(), a.getPosicionY() + moveSpeed);  
                if (a.getPosicionY() > SCREEN_H) {  
                    a.setEstado(false);  
                    a.setPosicion(-100, carrilesY[rand() % 3]);  
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
            al_flip_display();
        }
    }


    al_destroy_bitmap(fondo);
    al_destroy_bitmap(imgAmarillo);
    al_destroy_bitmap(imgRojo);
    al_destroy_bitmap(imgAzul);
    al_destroy_bitmap(imgVerde);
    al_destroy_display(ventana);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
