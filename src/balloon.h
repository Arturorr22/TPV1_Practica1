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
        static constexpr int COLORS = 6;

    public:

        // Constructor de la clase Balloon
        Balloon(Game* game, const Rectangle& rect, Texture* texture, 
            const Vector2D<>& speed, const int color, const bool exploded, 
            const int animationFrame);

        // Método hit de la clase Balloon
        bool hit(const Rectangle& obj) override;

        // Método render de la clase Balloon
        void render() const override;

        // Método update de la clase Balloon
        bool update(const float delta) override;

        // Sobreescritura del método saveObject heredado de la clase SceneObject
        void saveObject(std::ostream& savedFile) const override;

        // Método loadObject para cargar un globo en base a los datos guardados
        void loadObject(std::istream& savedGameFile, const int lineNumber) override;

};

#endif