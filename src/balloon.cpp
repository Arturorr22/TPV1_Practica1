#include "balloon.h"

// Número de frames de la animación de explosión del globo
constexpr int EXPLOSION_FRAMES = 6;

// Implementación del constructor de la clase Balloon
// Inicializa todos sus atributos en base a los argumentos recibidos
Balloon::Balloon(const Rectangle destRect, const int color, const Vector2D<>& speed, Texture* texture)
    : rect(destRect)
    , color(color)
    , speed(speed)
    , texture(texture)
    , exploded(false)
    , animationFrame(0)
{
}

// Implementación del método hit de la clase Balloon
// Detecta si hay colisión entre el rectángulo del globo y el rectangulo recibido
// En caso de que haya colisión, se activa la destrucción del globo
bool Balloon::hit(const Rectangle& obj) {
    if (exploded)
        return false;

    if (rect.hasIntersection(obj)) {
        exploded = true;
        return true;
    }
    return false;
}

// Implementación del método render
// Renderiza la textura del globo en función de su color y su animación
void Balloon::render() const {
    if (texture != nullptr) {
        texture->renderFrame(rect.to_sdl(), color, animationFrame);
    }
}

// Implementación del método update
// Comprueba que el globo no se salga de la pantalla por el borde superior o que
// no siga en la escena si ha completado su animación de destrucción
bool Balloon::update(const float delta) {
    if (!exploded) {
        rect.move(speed * delta);
    }
    else {
        ++animationFrame;
    }
    return rect.getTopLeftCorner().getY() + rect.getHeight() > 0 && animationFrame != EXPLOSION_FRAMES;
}
