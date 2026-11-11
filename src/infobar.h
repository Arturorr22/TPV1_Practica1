#ifndef INFOBAR_H
#define INFOBAR_H

#include "gameobject.h"
#include "rectangle.h"

class InfoBar : public GameObject {
    private:

        // Constantes para la representación de los datos
        static constexpr int SIZE_DIVISOR = 20;
        static constexpr int DIGIT_ROW = 0;

        // Dimensiones y posición de la barra de flechas representada por un rectángulo
        Rectangle arrowsBarRect;

        // Punteros a la textura de la flecha y de los dígitos
        Texture* arrowTexture;
        Texture* digitsTexture;

        // Variables que almacenan el número de flechas y puntos del jugador
        int arrows;
        int playerPoints;

    public:

        // Método constructor de la clase InfoBar
        InfoBar(Game* game, Texture* arrowTexture, Texture* digitsTexture, const int arrows, 
            const int playerPoints);

        // Método render de la clase InfoBar
        void render() const override;

        // Método update de la clase InfoBar
        bool update(const float delta) override;

        // Métodos que actualizan el valor de arrows y playerPoints
        void updateArrows(const int newArrowsValue);
        void updatePlayerPoints(const int newPlayerPointsValue);
};


#endif