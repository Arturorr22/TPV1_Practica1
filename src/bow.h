#ifndef BOW_H
#define BOW_H

#include "sceneobject.h"
#include <SDL3_image/SDL_image.h>

class Bow : public SceneObject {
    private: 

        // Número de flechas de las que dispone el arco
        int numArrows;

        // Variable que indica si el arco está cargado o no
        bool loaded;

        // Velocidad de movimiento vertical del arco
        static constexpr float BOW_SPEED = 0.1f;

        // Frames de la textura del arco
        static constexpr int BOW_ROW = 0;
        static constexpr int LOADED_FRAME = 0;
        static constexpr int UNLOADED_FRAME = 1;

    public:

        // Constructor de la clase Bow
        Bow(Game* game, const Rectangle& rect, Texture* texture, const int numArrows, 
            const bool loaded);

        // Método para manejar los eventos
        void handleEvent(const SDL_Event& event);

        // Declaramos la sobreescritura de los métodos render y update
        void render() const override;
        void update() override;

};

#endif
