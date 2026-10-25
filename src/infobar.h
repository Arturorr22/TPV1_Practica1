#ifndef INFOBAR_H
#define INFOBAR_H

#include "texture.h"
#include "rectangle.h"

class InfoBar {
    private:
        // Rectángulo que se emplea como referencia para representar el número de flechas 
        // de las que dispone el jugador
        // Posición => .corner
        // Dimensiones => .width y .height  
        Rectangle rect;

        // Número de flechas restantes
        int arrows;

        // Puntero a la textura de la flecha 
        Texture* texture; 

    public:

        // Constructor de la infoBar
        InfoBar(const Rectangle destRect, const int arrows, Texture* texture);

        // Función que reduce en uno el número de flechas de la infoBar
        void decrementArrows();

        // Renderiza la infoBar en la pantalla
		void render() const;
};

#endif