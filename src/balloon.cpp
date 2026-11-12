#include "balloon.h"

// Implementación del constructor de la clase Balloon
Balloon::Balloon(Game* game, const Rectangle& rect, Texture* texture, 
    const Vector2D<>& speed, const int color) 
    : MobileObject(game, rect, texture, speed)
    , color(color)
    , exploded(false)
    , animationFrame(0)
{}

// Implementación del método hit de la clase Balloon
bool Balloon::hit(const Rectangle& obj) {
    if (exploded || !SceneObject::hit(obj))
        return false;

    game->increasePoints();
    exploded = true;
    return true;
}

// Implementación del método render de la clase Balloon
void Balloon::render() const {
    if (texture != nullptr) {
        texture->renderFrame(rect.to_sdl(), color, animationFrame);
    }
}

// Implementación del método update de la clase Balloon
bool Balloon::update(const float delta) {
    if (!exploded) {
        return MobileObject::update(delta);
    }
    else {
        ++animationFrame;
        return animationFrame != EXPLOSION_FRAMES;
    }
}