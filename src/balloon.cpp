#include "balloon.h"
#include "game.h"
#include "texture.h"
#include <format>

// Implementación del constructor de la clase Balloon
Balloon::Balloon(Game* game, const Rectangle& rect, Texture* texture, 
    const Vector2D<>& speed, const int color, const bool exploded,
    const int animationFrame) 
    : MobileObject(game, rect, texture, speed)
    , color(color)
    , exploded(exploded)
    , animationFrame(animationFrame)
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

// Implementación del método saveObject de la clase Balloon
void Balloon::saveObject(std::ostream& savedFile) const {

    // Escribimos una b minúscula para indicar que se trata de un globo
    savedFile << "b ";

    // Llamamos al método saveObject de MobileObject
    MobileObject::saveObject(savedFile);

    // Añadimos los atributos propios del globo
    savedFile << std::format("{} {} {}", color, exploded ? 1 : 0, animationFrame) << "\n";
}

// Implementación del método loadObject de la clase Balloon
Balloon* Balloon::loadObject(std::istream& savedFile, Game* game) {

    float x, y, w, h, sX, sY;
    savedFile >> x >> y >> w >> h >> sX >> sY;
    int c, e, aF;
    savedFile >> c >> e >> aF;

    return new Balloon(game, Rectangle({x, y}, w, h), game->getTexture(Game::BALLOON), 
        Vector2D(sX, sY), c, e == 1 ? true : false, aF);
} 