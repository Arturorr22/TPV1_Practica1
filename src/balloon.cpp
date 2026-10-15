#include "balloon.h"

// Implementación del constructor de la clase Balloon
// Inicializa todos sus atributos en base a los argumentos recibidos
Balloon::Balloon(Rectangle destRect, int color, Vector2D<> speed, Texture* texture) 
    : rect(destRect)
    , color(color)
    , speed(speed)
    , texture(texture) 
{    
}

// Implementación del método hit de la clase Balloon
// Detecta si hay colisión entre el rectángulo del globo
// y el rectangulo recibido
// En caso de que haya colisión, el globo de destruye
bool Balloon::hit(const Rectangle& obj) const {
    return rect.hasIntersection(obj);
}

// Implementación del método render
// Renderiza la textura del globo en función de su color
void Balloon::render() const {
    if (texture != nullptr) {
        texture->renderFrame(rect.to_sdl(), color, 0); // 0 se cambiará
    }
}

// Implementación del método update
// Comprueba que el globo no se salga de la pantalla por el borde superior
bool Balloon::update(float delta) {
    rect.move(speed * delta); 
    return rect.getTopLeftCorner().getY() + rect.getHeight() > 0;
}