#ifndef BOW_H
#define BOW_H

#include "texture.h"
#include "rectangle.h"
#include <SDL3_image/SDL_image.h>

class Bow {
    private:

        // Rectángulo que contiene la posición y las dimensiones
        // del arco.
        // Posición => .corner
        // Dimensiones => .width y .height 
        Rectangle rect;

        // Velocidad del arco, de tipo Vector2D (x, y)
        Vector2D<> speed;

        // Número de flechas disponibles
        int arrows;

        // Variable que indica si el arco está cargado o no
        bool loaded;

        // Puntero que apunta a la textura del arco
        Texture* texture;

    public:

        // Constructora del arco con argumentos y vacía
        Bow(const Rectangle destRect, const Vector2D<>& speed, const int arrows, 
            const bool loaded, Texture* texture);

        // Para manejar los eventos
        void handleEvent(const SDL_Event& event);

        // Método que renderiza el objeto en la ventana de acuerdo a su forma y textura
        void render() const;

        // Actualiza el arco en cada ciclo de juego
        bool update(const float delta);

        // Método que devuelve el valor de arrows
        int getArrows() const;

        // Método que devuelve el rectángulo del arco
        Rectangle getRect() const;
    
};

#endif