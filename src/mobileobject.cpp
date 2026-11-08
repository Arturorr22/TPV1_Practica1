#include "mobileobject.h"

// Implementación del constructor de la clase MobileObject
MobileObject::MobileObject(Game* game, const Rectangle& rect, Texture* texture, 
    const Vector2D<>& speed) 
    : SceneObject(game, rect, texture)
    , speed(speed)
{}