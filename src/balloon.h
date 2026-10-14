#ifndef BALLOON_H
#define BALLOON_H

#include "rectangle.h"
#include "vector2D.h"

class Balloon {
    private:

        // Rectángulo que contiene la posición y las dimensiones
        // del globo.
        // Posición => .corner
        // Dimensiones => .width y .height 
        Rectangle balloonRect;

        // Color del globo que se corresponde con el índice de
        // la fila en la textura 
        int color;

        // Velocidad del globo, de tipo Vector2D
        Vector2D<> speed;
    
    public:

        // Método que devuelve true si detecta una colisión con
        // otro rectángulo
        bool hit(const Rectangle& obj) const;
};

#endif