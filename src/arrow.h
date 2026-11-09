#ifndef ARROW_H
#define ARROW_H

#include "mobileobject.h"

// Clase que representa una flecha en el juego. Es un objeto móvil de la escena
// (game, rect, texture y speed los guardan sus clases base)
class Arrow : public MobileObject {
private:

	// Proporciones del rectángulo de la punta respecto al rectángulo de la flecha
	static constexpr float TIP_X_FACTOR = 2.0f / 3.0f;
	static constexpr float TIP_Y_FACTOR = 1.0f / 3.0f;
	static constexpr float TIP_WIDTH_FACTOR = 1.0f / 4.0f;
	static constexpr float TIP_HEIGHT_FACTOR = 1.0f / 2.0f;

	// Calcula el rectángulo de la punta a partir de la posición actual de la flecha
	Rectangle getTipRect() const;

public:

	// Constructor de la clase Arrow
	Arrow(Game* game, const Rectangle& rect, Texture* texture, const Vector2D<>& speed);

	// Renderiza la flecha en la pantalla
	void render() const override;

	// Mueve la flecha, comprueba si su punta alcanza algo y devuelve true si sigue
	// activa, false si ha salido de la pantalla. La velocidad es en píxeles/milisegundo
	bool update(const float delta) override;

	// Una flecha no es un objetivo: nunca recibe impactos (ni de sí misma)
	bool hit(const Rectangle& obj) override;
};

#endif // ARROW_H
