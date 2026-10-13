#include "balloon.h"

/*
Implementación del método hit de la clase Balloon
Detecta si hay colisión entre el rectángulo del globo
y el rectangulo recibido
En caso de que haya colisión, el globo de destruye
*/
bool Balloon::hit(const Rectangle& obj) const {
    bool collision = balloonRect.hasIntersection(obj);
    if (collision) {
        // Destroy(globo);
    }
    return collision;
}