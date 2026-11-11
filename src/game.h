// Dependencias circulares
// Meter toda la gestion de exit en update
// Crear LISTA de SceneObjects
// Ordenar las propiedades de Game en base al momento en el que se usan
// Comprobar si hay variables que podría quitar
// Correo Rubén
// Preguntar por prácticas de #include's
// Revisar soltar teclas en handleEvents

#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>

#include <array>
#include <fstream>
#include <istream>
#include <list>
#include <random>

#include "rectangle.h"

class Texture;
class Bow;
class SceneObject;
class InfoBar;

class Game {
public:

	// Número de actualizaciones por segundo
	static constexpr int FRAME_RATE = 30;
	// Periodo entre actualizaciones en milisegundos
	static constexpr int FRAME_PERIOD = 1000 / FRAME_RATE;

	// Tamaño real de la ventana
	static constexpr int WINDOW_WIDTH = 800;
	static constexpr int WINDOW_HEIGHT = 600;

	enum TextureName {
		BALLOON = 0,
		ARROW,
		BOW,
		DIGITS,
		NUM_TEXTURES
	};

	Game();
	~Game();

	// Obtiene una textura por su nombre
	Texture* getTexture(TextureName name) const;

	// Ejecuta el bucle principal del juego
	void run();

	// Crea una flecha. Es público para que Bow pueda llamarlo
	void createArrow(const Rectangle& bowRect);

	// Comprueba si hay algún objeto colocado en ese rectángulo
	bool checkCollision(const Rectangle& rect);

	// Aumenta los puntos del jugador según los puntos por globo explotado
	void increasePoints();

private:

	struct Color {
		int r;
		int g;
		int b;
	};

	// Rutas y título
	static constexpr const char* WINDOW_TITLE = "Bow & Arrow 1.0";
	static constexpr const char* IMAGE_ROOT = "../assets/images/";
	static constexpr const char* LEVEL_ROOT = "../assets/levels/";

	// Valores por defecto y mínimos usados al cargar niveles
	static constexpr Color DEFAULT_BACKGROUND = {0, 0, 0};
	static constexpr int DEFAULT_ARROWS = 10;
	static constexpr int DEFAULT_BALDELAY = 750;
	static constexpr int DEFAULT_BALPOINTS = 10;
	static constexpr int DEFAULT_GOAL = 0;
	static constexpr int MIN_BACKGROUND = 0;
	static constexpr int MIN_ARROWS = 1;
	static constexpr int MIN_BALDELAY = 1;
	static constexpr int MIN_BALPOINTS = 1;
	static constexpr int MIN_GOAL = 0;

	// Constantes de renderizado
	static constexpr Uint8 OPAQUE_ALPHA = 255;

	// Proporciones del arco
	static constexpr float BOW_HEIGHT_DIV = 6.3f;
	static constexpr float BOW_WIDTH_DIV = 8.0f;

	// Globos
	static constexpr int NUM_BALLOON_COLORS = 7;
	static constexpr float MIN_BALLOON_SPEED = -0.05f;
	static constexpr float MAX_BALLOON_SPEED = -0.02f;
	static constexpr float BALLOON_MIN_X_DIV = 3.0f;
	static constexpr float BALLOON_WIDTH_DIV = 6.25f;
	static constexpr float BALLOON_HEIGHT_DIV = 4.7f;
	static constexpr float BALLOONS_PER_DELAY = 1.0f;

	// Flechas
	static constexpr float ARROW_WIDTH_DIV = 8.0f;
	static constexpr float ARROW_HEIGHT_DIV = 40.0f;
	static constexpr float ARROW_OFFSET_DIV = 13.0f;
	static constexpr float ARROW_SPEED = 0.1f;

	// Estado de la partida
	bool exit;
	Color backgroundColor;
	int arrows;
	int baldelay;
	int balPoints;
	int goal;
	int levelToLoad;
	int playerPoints;

	// Recursos y objetos de la escena
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::array<Texture*, NUM_TEXTURES> textures;
	Bow* bow;
	std::list<SceneObject*> sceneObjects;
	SceneObject* lastArrow;
	InfoBar* infoBar;

	// Generador y temporizadores
	std::mt19937_64 generator;
	uint64_t LastBalloonTime;
	uint64_t nextBalloon;

	// Carga los valores de un nivel
	void LoadGame(std::ifstream& file);
	void addObject(SceneObject* obj);
	void createBalloon();
	void render() const;
	void update();
	void handleEvents();
	void loadNextLevel();
};

inline Texture*
Game::getTexture(TextureName name) const
{
	return textures[name];
}

#endif // GAME_H
