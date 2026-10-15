#ifndef BALLOON_H
#define BALLOON_H

#include "texture.h"
#include "rectangle.h"
#include "vector2D.h"

class Balloon {
    private:

        // Rectángulo que contiene la posición y las dimensiones
        // del globo.
        // Posición => .corner
        // Dimensiones => .width y .height 
        Rectangle rect;

        // Color del globo que se corresponde con el índice de
        // la fila en la textura 
        int color;

        // Velocidad del globo, de tipo Vector2D
        Vector2D<> speed;

        // Puntero que apunta a la textura del globo
        Texture* texture;
    
    public:

        // Constructora de la clase
        Balloon(Rectangle destRect, int color, Vector2D<> speed, Texture* texture);

        // Método que devuelve true si detecta una colisión con
        // otro rectángulo
        bool hit(const Rectangle& obj) const;

        // Método que renderiza el objeto en la ventana de acuerdo a su forma y textura
        void render() const;

        // Actualiza el globo en cada ciclo de juego
        bool update(float delta);

};

#endif