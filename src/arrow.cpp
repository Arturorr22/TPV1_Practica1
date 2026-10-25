#include "arrow.h"
#include "game.h"

// Crea un objeto Arrow con el rectángulo de destino, la velocidad y la textura proporcionados
Arrow::Arrow(Game* game, const Rectangle destRect, const Vector2D<>& speed, Texture* texture)
	: game(game), rect(destRect), speed(speed), texture(texture) {
}

// Implementa la lógica de renderizado de la flecha utilizando la textura y el rectángulo
void Arrow::render() const {
	texture->render(rect.to_sdl());
}

// Implementa la lógica de actualización de la flecha
bool Arrow::update(const float delta) {

	// Actualiza la posición de la flecha según su velocidad y el tiempo transcurrido
	rect.move(speed * delta);

	// Creamos el rectángulo de la punta
	Rectangle r = Rectangle({rect.getTopLeftCorner().getX() + (rect.getWidth() * 2 / 3), 
							rect.getTopLeftCorner().getY() + (rect.getHeight() * 1 / 3)}, 
							rect.getWidth() / 4, rect.getHeight() / 2);
	game->checkCollision(r);

	// Comprueba si la flecha ha salido de la pantalla mirando si la esquina superior 
	// izquierda del rectángulo está fuera de los límites de la ventana
	return !(rect.getTopLeftCorner().getX() > Game::WINDOW_WIDTH);
}
