#include "arrow.h"
#include "game.h"

// Crea un objeto Arrow con el rectángulo de destino, la velocidad y la textura proporcionados
Arrow::Arrow(Game* game, Rectangle destRect, const Vector2D<>& speed, Texture* texture)
	: game(game), rect(destRect), speed(speed), texture(texture) {
}

// Implementa la lógica de renderizado de la flecha utilizando la textura y el rectángulo
void Arrow::render() const {
	texture->render(rect.to_sdl());
}

// Implementa la lógica de actualización de la flecha
bool Arrow::update(float delta) {
	// Actualiza la posición de la flecha según su velocidad y el tiempo transcurrido
	Vector2D<> displacement = speed * delta;
	rect.move(displacement);

	// Comprueba si la flecha ha salido de la pantalla mirando si la esquina superior 
	// izquierda del rectángulo está fuera de los límites de la ventana
	if (rect.getTopLeftCorner().getX() > Game::WINDOW_WIDTH) {
		return false;
	}
	return true;
}
