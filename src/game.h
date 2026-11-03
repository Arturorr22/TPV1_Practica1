#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <array>
#include <istream>
#include <vector>
#include <random>
#include <fstream>


// Declaraciones anticipadas
#include "rectangle.h"
#include "vector2D.h"

#include "balloon.h"
#include "bow.h"
#include "infobar.h"

// Declaraciones anticipadas
class Texture;
class Arrow;

// Clase principal del juego.

class Game
{
public:
	// Número de actualizaciones por segundo
	static constexpr int FRAME_RATE = 30;
	// Periodo entre actualizaciones en milisegundos
	static constexpr int FRAME_PERIOD = 1000 / FRAME_RATE;
	// Tamaño real de la ventana
	static constexpr int WINDOW_WIDTH = 800;
	static constexpr int WINDOW_HEIGHT = 600;

	enum TextureName
	{
		BALLOON = 0,
		ARROW,
		BOW,
		NUM_TEXTURES
	};

private:
	// Rutas y título
	static constexpr const char* WINDOW_TITLE = "Bow & Arrow 1.0";
	static constexpr const char* IMAGE_ROOT = "../assets/images/";
	static constexpr const char* LEVEL_ROOT = "../assets/levels/";

	// Carga de niveles
	static constexpr int DEFAULT_GOAL = 1000;
	static constexpr int MIN_ARROWS = 1;
	static constexpr int MIN_BALDELAY = 500;

	// Render
	static constexpr Uint8 OPAQUE_ALPHA = 255;

	// Globos
	static constexpr int NUM_BALLOON_COLORS = 7;           // filas de balloons.png
	static constexpr float MIN_BALLOON_SPEED = -0.05f;
	static constexpr float MAX_BALLOON_SPEED = -0.02f;
	static constexpr float BALLOON_MIN_X_DIV = 3.0f;       // posX mínima = WINDOW_WIDTH / 3
	static constexpr float BALLOON_WIDTH_DIV = 6.25f;
	static constexpr float BALLOON_HEIGHT_DIV = 4.7f;
	static constexpr float BALLOONS_PER_DELAY = 1.0f;      // tasa = BALLOONS_PER_DELAY / baldelay

	// Arco
	static constexpr float BOW_SIZE_DIV = 3.15f;
	static constexpr float BOW_WIDTH_DIV = 4.0f;

	// Flecha
	static constexpr float ARROW_WIDTH_DIV = 5.95f;
	static constexpr float ARROW_HEIGHT_DIV = 21.95f;
	static constexpr float ARROW_OFFSET_DIV = 6.3f;
	static constexpr float ARROW_SPEED = 0.1f;

	// InfoBar (mitad del tamaño de la flecha)
	static constexpr float INFOBAR_SCALE = 2.0f;

	SDL_Window* window;
	SDL_Renderer* renderer;
	std::array<Texture*, NUM_TEXTURES> textures;

	void render() const;
	void update();
	void handleEvents();

	bool exit;

	int levelToLoad = 1;

	// Número de flechas
	int arrows = 1;

	// tiempo de espera en milisegundos entre la generación de un globo y su 
	// siguiente
	int baldelay = 500;

	// Meta de puntos, puntos por globo y color de fondo
	int goal = 100;
	int balPoints = 10;
	int playerPoints = 0;

	struct Color { int r, g, b; };
	Color backgroundColor;

	// Arco
	Bow* bow;

	// InfoBar
	InfoBar* infoBar;

	// Generador de números pseudoaleatorios
	std::mt19937_64 generator;

	// Variables para gestionar el tiempo
	uint64_t LastBalloonTime;
	uint64_t nextBalloon;

	// Vector que almacena los globos generados
	std::vector<Balloon*> balloonsVector;

	// Elementos del juego
	std::vector<Arrow*> arrowsVector;

	// Método que lee de un archivo de entrada los datos de baldelay, arrows, etc...
	void LoadGame(std::ifstream& file);
	void loadNextLevel();

	void createBalloon();
	void createArrow();

	// Actualizan los globos y las flechas, eliminando los que ya no siguen en escena
	void updateBalloons();
	void updateArrows();
public:

	Game();
	~Game();

	// Obtiene una textura por su nombre
	Texture* getTexture(TextureName name) const;

	// Ejecuta el bucle principal del juego
	void run();

	// Comprueba si hay algún objeto colocado en ese rectángulo
	bool checkCollision(const Rectangle& rect);

	// Método que aumenta el total de puntos del jugador respecto a los puntos que suelta 
	// cada globo
	void increasePoints();
};

inline Texture*
Game::getTexture(TextureName name) const
{
	return textures[name];
}

#endif // GAME_H
