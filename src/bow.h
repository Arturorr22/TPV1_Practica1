#ifndef BOW_H
#define BOW_H

#include "sceneobject.h"
#include <SDL3/SDL_events.h>

enum dir {
    none,
    up,
    down
};

class Bow : public SceneObject {
    private: 

        // Número de flechas de las que dispone el arco
        int numArrows;

        // Variable que indica si el arco está cargado o no
        bool loaded;

        // Dirección del movimiento del arco
        dir bowDir;

        // Velocidad de movimiento vertical del arco
        static constexpr float BOW_SPEED = 0.4f;

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
        bool update(const float delta) override;

        // Método hit de la clase Bow
        bool hit(const Rectangle& obj) override;

        // Método del arco que devuelve el número de flechas que este tiene
        int getArrows() const;

        // Sobreescritura del método saveObject heredado de SceneObject
        void saveObject(std::ostream& savedFile) const override;

        // Método loadObject para cargar un arco en base a los datos guardados 
        void loadObject(std::istream& savedGameFile, const int lineNumber) override;

};

#endif