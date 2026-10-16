#include "game.h"

#include <string>

#include <SDL3_image/SDL_image.h>

#include "texture.h"
#include "arrow.h"

// GENERACIÓN ALEATORIA DE GLOBOS
#include "rectangle.h"
#include "vector2D.h"
#include "balloon.h"
// GENERACIÓN ALEATORIA DE GLOBOS

using namespace std;

// Constantes
constexpr const char* const WINDOW_TITLE = "Bow & Arrow 1.0";
constexpr const char* IMAGE_ROOT = "../assets/images/";
constexpr const char* LEVEL_ROOT = "../assets/levels/";
const float FRAME_PERIOD_SEG = (float)FRAME_PERIOD / 1000;

// Estructura para especificar las texturas que hay que
// cargar y el tamaño de su matriz de frames
struct TextureSpec
{
	const char* name;
	int nrows = 1;
	int ncols = 1;
};

constexpr array<TextureSpec, Game::NUM_TEXTURES> textureList {
	TextureSpec{"balloons.png", 7, 6},
	{"arrow.png"},  
	{"bow.png", 1, 2},
};

Game::Game()
  : exit(false)
{
	// Carga SDL y crea la ventana con su renderizador
	if (!SDL_Init(SDL_INIT_VIDEO))
		throw "SDL_Init: "s + SDL_GetError();

	if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE,
	                                 WINDOW_WIDTH, WINDOW_HEIGHT,
	                                 0, &window, &renderer))
		throw "window or renderer: "s + SDL_GetError();

	// Carga las texturas al inicio
	try {
		textures.fill(nullptr);
		string imgRoot = IMAGE_ROOT;

		for (size_t i = 0; i < textures.size(); i++) {
			auto [name, nrows, ncols] = textureList[i];
			textures[i] = new Texture(renderer, (imgRoot + name).c_str(), nrows, ncols);
		}
	}
	catch (...) {
		// TODO: liberar la memoria reservada
		throw; // relanza la excepción
	}

	// Configura que se pueden utilizar capas translúcidas
	// SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// TODO: crear los objetos del juego
	Arrow* arrow = new Arrow(this, textures[ARROW], Vector2D<>(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), Vector2D<>(0.5f, 0.0f));
	arrows.push_back(arrow);

	// Configuramos la semilla del generador para la generación de globos
	generator.seed(std::random_device()());

	// Almacenamos el tiempo actual al iniciar el programa en las variable LastBalloonTime y LastUpdateTime
	LastBalloonTime = LastUpdateTime = SDL_GetTicks();

}

Game::~Game()
{
	// TODO: liberar la memoria reservada por la clase
}

void
Game::render() const
{
	SDL_RenderClear(renderer);

	// Generación de flechas
	for (std::size_t i = 0; i < arrows.size(); ++i) {
		arrows[i]->render();
	}

	// GENERACIÓN ALEATORIA DE GLOBOS {

	for (std::size_t i = 0; i < balloons.size(); ++i){
		balloons[i].render();
	}
	
	// { GENERACIÓN ALEATORIA DE GLOBOS

	SDL_RenderPresent(renderer);
}

void
Game::update()
{
	// Actualización de flechas
	for (std::size_t i = 0; i < arrows.size(); ++i) {
		if (!(arrows[i]->update(FRAME_PERIOD_SEG))) {
			delete arrows[i];
			arrows.erase(arrows.begin() + i);
			--i;
		}
	}

	// GENERACIÓN ALEATORIA DE GLOBOS {

	frameTime = SDL_GetTicks();

	if (frameTime - LastBalloonTime > 2000) {

		// Calculamos, de manera pseudoaleatoria, la posición en el borde inferior en la que aparecerá
		// el globo, su color y su velocidad en y
		int c =  uniform_int_distribution<int>(0, 6)(generator);
		float sY = uniform_real_distribution<float>(-0.10f, -0.02f)(generator);
		float x =  uniform_real_distribution<float>(WINDOW_WIDTH / 3, WINDOW_WIDTH - textures[BALLOON]->getFrameWidth())(generator);
	
		// Creamos el rectángulo y el vector velocidad en base a esto último 
		// El rectángulo se encontrará en el borde inferior de la pantalla, 
		// con un tamaño equivalente a la mitad del tamaño de un solo frame de la textura
		Rectangle r = Rectangle({x, WINDOW_HEIGHT}, 
			textures[BALLOON]->getFrameWidth() / 2,
			textures[BALLOON]->getFrameHeight() / 2
		);
		Vector2D<> s = Vector2D(0.0f, sY);
	
		// Con todo calculado, creamos un globo y lo incorporamos al vector de globos
		Balloon b = Balloon(r, c, s, textures[BALLOON]);
		balloons.push_back(b);

		// Actualizamos el valor de LastBalloonTime
		LastBalloonTime = SDL_GetTicks();
	}

	if (frameTime - LastUpdateTime > FRAME_PERIOD) {

		// Una vez hemos generado (o no) un globo, actualizamos todos los globos del vector de globos
		for (std::size_t i = 0; i < balloons.size(); ++i) {

			// Si el globo se sale de los límites, lo eliminamos
			if(!(balloons[i].update(FRAME_PERIOD))) {
				balloons.erase(balloons.begin() + i);
			}
		}

		// Actualizamos el valor de LastUpdateTime
		LastUpdateTime = SDL_GetTicks();
	}

	// } GENERACIÓN ALEATORIA DE GLOBOS
}

void
Game::run()
{
	while (!exit) {
		int frameTime = SDL_GetTicks();
		handleEvents();
		update();
		render();
		int frameTime2 = SDL_GetTicks();
		if (frameTime - frameTime2 < FRAME_PERIOD) {
			SDL_Delay(FRAME_PERIOD - (frameTime - frameTime2));
		}
	}
}

void
Game::handleEvents()
{
	SDL_Event event;

	// Only quit is handled directly, everything else is delegated
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT)
			exit = true;

		// TODO
	}
}

// bool
// Game::checkCollision(const Rectangle& r)
// {
//	// TODO
//	return false;
// }
