#ifndef BOW_H
#define BOW_H

#include "rectangle.h"
#include <SDL3_image/SDL_image.h>

class Bow {
    private:

        // Rectángulo que contiene la posición y las dimensiones
        // del arco.
        // Posición => .corner
        // Dimensiones => .width y .height 
        Rectangle bowRect;

        // Velocidad del arco, de tipo Vector2D (x, y)
        Vector2D<> speed;

        // Número de flechas disponibles
        int arrows;

        // Variable que indica si el arco está cargado o no
        bool loaded;

    public:

        // Para manejar los eventos
        void handleEvent(const SDL_Event& event);
    
};

#endif