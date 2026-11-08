#ifndef MOBILEOBJECT_H
#define MOBILEOBJECT_H

#include "sceneobject.h"

class MobileObject : public SceneObject {
    protected:

        // Velocidad del objeto móvil
        Vector2D<> speed;

    public:

        // Constructor de la clase MobileObject
        MobileObject(Game* game, const Rectangle& rect, Texture* texture, 
            const Vector2D<>& speed);
};

#endif