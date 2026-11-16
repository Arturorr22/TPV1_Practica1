#include "balloon.h"
#include "game.h"
#include "texture.h"
#include "error.h"
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

    exploded = true;
    return true;
}

// Implementación del método render de la clase Balloon
void Balloon::render() const {
    if (SceneObject::getTexture() != nullptr) {
        SceneObject::getTexture()->renderFrame(SceneObject::getRect().to_sdl(), color, animationFrame);
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
void Balloon::loadObject(std::istream& savedGameFile, const int lineNumber) {

    // Leemos los datos del MobileObject
    MobileObject::loadObject(savedGameFile, lineNumber);

    // Leemos los datos propios del globo
    int colorIndex, explodedFlag, animationFrame;

    if (!(savedGameFile >> colorIndex >> explodedFlag >> animationFrame) || MobileObject::getSpeed().getX() != 0
        || MobileObject::getSpeed().getY() >= 0 || colorIndex < 0 || colorIndex > COLORS || (explodedFlag != 0 && explodedFlag != 1) 
        || animationFrame < 0 || animationFrame >= EXPLOSION_FRAMES) {
        throw FileFormatError("saved.txt", lineNumber, "Formato inválido del globo.");
    }

    // Devolvemos el globo
    color = colorIndex;
    exploded = (explodedFlag == 1 ? true : false);
    this->animationFrame = animationFrame;
} 