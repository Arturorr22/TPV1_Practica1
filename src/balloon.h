#ifndef BALLOON_H
#define BALLOON_H

#include "mobileobject.h"

class Balloon : public MobileObject {
    private:

        // Color del globo que se corresponde con el índice de
        // la fila en la textura 
        int color;

        // Variables para controlar la animación de explosión del globo
        bool exploded;
        int animationFrame;

        // Número de frames de la animación de explosión del globo
        static constexpr int EXPLOSION_FRAMES = 6;

    public:

        // Constructor de la clase Balloon
        Balloon(Game* game, const Rectangle& rect, Texture* texture, 
            const Vector2D<>& speed, const int color);

        // Método hit de la clase Balloon
        bool hit(const Rectangle& obj) override;

        // Método render de la clase Balloon
        void render() const override;

        // Método update de la clase Balloon
        bool update(const float delta) override;

};

#endif