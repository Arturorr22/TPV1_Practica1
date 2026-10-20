#include "game.h"

#include <string>

#include <SDL3_image/SDL_image.h>

#include "texture.h"
#include "arrow.h"

// GENERACIÓN ALEATORIA DE GLOBOS
// GENERACIÓN ALEATORIA DE GLOBOS

using namespace std;

// Constantes
constexpr const char* const WINDOW_TITLE = "Bow & Arrow 1.0";
constexpr const char* IMAGE_ROOT = "../assets/images/";
constexpr const char* LEVEL_ROOT = "../assets/levels/";

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

	// Cargamos el nivel deseado
	ifstream nivel("1.txt");
	LoadGame(nivel);
	nivel.close();
	
	// Creamos el arco
	Rectangle bowRect({0, (float)(WINDOW_HEIGHT / 2 - textures[BOW]->getFrameHeight() / 2)}, 
		textures[BOW]->getFrameHeight() / 2, textures[BOW]->getFrameWidth() / 2);
	bow = Bow(bowRect, {0, 0}, arrows, false, textures[BOW]);
	
	// Configuramos la semilla del generador para la generación de globos
	generator.seed(std::random_device()());
	
	// Almacenamos el tiempo actual al iniciar el programa en la variable LastBalloonTime
	LastBalloonTime = SDL_GetTicks();
}

Game::~Game()
{
	// TODO: liberar la memoria reservada por la clase
}

void
Game::render() const
{
	SDL_RenderClear(renderer);

	// Renderizamos las flechas, los globos y el arco
	for (std::size_t i = 0; i < arrowsVector.size(); ++i) {
		arrowsVector[i].render();
	}
	for (std::size_t i = 0; i < balloonsVector.size(); ++i){
		balloonsVector[i].render();
	}
	bow.render();

	SDL_RenderPresent(renderer);
}

void
Game::update()
{
	// Actualizamos el arco
	bow.update(FRAME_PERIOD);

	// Comprobamos si ya ha pasado suficiente tiempo desde la generación del último
	// globo
	if (SDL_GetTicks() - LastBalloonTime > baldelay) {

		// Calculamos, de manera pseudoaleatoria, la posición del borde inferior 
		// en la que aparecerá el globo, junto con su color y su velocidad en el 
		// eje y
		int color =  uniform_int_distribution<int>(0, 6)(generator);
		float speedY = uniform_real_distribution<float>(-0.05f, -0.02f)(generator);
		float posX =  uniform_real_distribution<float>(WINDOW_WIDTH / 3, 
			WINDOW_WIDTH - textures[BALLOON]->getFrameWidth() / 2)(generator);
	
		// Creamos el rectángulo en base a todas variables calculadas
		Rectangle balloonRect({posX, WINDOW_HEIGHT}, 
			textures[BALLOON]->getFrameWidth() / 2,
			textures[BALLOON]->getFrameHeight() / 2);

		// Con todo calculado, creamos un globo y lo incorporamos al vector de 
		// globos
		Balloon b(balloonRect, color, Vector2D(0.0f, speedY), textures[BALLOON]);
		balloonsVector.push_back(b);

		// Actualizamos el valor de LastBalloonTime
		LastBalloonTime = SDL_GetTicks();
	}

	// Una vez hemos generado (o no) un globo, actualizamos todos los globos
	// del vector de globos
	for (std::size_t i = 0; i < balloonsVector.size(); ++i) {

		// Si el globo se sale de los límites, lo eliminamos del vector
		if(!(balloonsVector[i].update(FRAME_PERIOD))) {
			balloonsVector.erase(balloonsVector.begin() + i);
		}
	}

	// Si el número de flechas ha disminuido en comparación con el anterior update,
	// disparamos una flecha desde la posición del arco
	if (bow.getArrows() < arrows) {
		
		// Guardamos el rectángulo del arco para usarlo para crear el rectángulo
		//  de la flecha
		Rectangle bowRect = bow.getRect();
		Rectangle arrowRect({0, 
			bowRect.getTopLeftCorner().getY() + bowRect.getHeight() / 2},
			textures[ARROW]->getFrameWidth() / 3,
			textures[ARROW]->getFrameHeight() / 3);

		// Creamos la flecha y la metemos en el vector de flechas
		Arrow arrow(this, arrowRect, Vector2D<>(0.1f, 0.0f), textures[ARROW]);
		arrowsVector.push_back(arrow);

		// Actualizamos el valor de arrows al actual 
		arrows = bow.getArrows();
	}

	// Actualizamos las flechas
	for (std::size_t i = 0; i < arrowsVector.size(); ++i) {
		if (!(arrowsVector[i].update(FRAME_PERIOD))) {
			arrowsVector.erase(arrowsVector.begin() + i);
		}
	}
}

void
Game::run()
{
	while (!exit) {

		// Calculamos frameStart y despues frameEnd 
		int frameStart = SDL_GetTicks();

		// Manejamos los posibles eventos 
		handleEvents();

		// Actualizamos el estado de todos los objetos del juego
		update();

		// Renderizamos todos los objetos del juego
		render();

		// Usamos frameEnd y frameStart para lograr que el juego se ciña a
		// FRAME_PERIOD, que se corresponde con el tiempo entre frames
		int frameEnd = SDL_GetTicks();
		if (frameEnd - frameStart < FRAME_PERIOD) {
			SDL_Delay(FRAME_PERIOD - (frameEnd - frameStart));
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
		else if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
			bow.handleEvent(event);
		}
	}
}

bool
Game::checkCollision(const Rectangle& rect) {

	// Comprobamos si alguno de los globos del vector colisiona con el rectángulo
	// recibido de la flecha
	std::size_t i = 0;
	while (i < balloonsVector.size() && !balloonsVector[i].hit(rect)) ++i;
	return i != balloonsVector.size();
}

// Implementación del método LoadGame
void
Game::LoadGame(std::ifstream& archivo) {
	
}