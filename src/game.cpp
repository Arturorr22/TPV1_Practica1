#include "game.h"
#include "texture.h"
#include "arrow.h"

#include <SDL3_image/SDL_image.h>

#include <string>
#include <limits>
#include <fstream>

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

	if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer))
		throw "window or renderer: "s + SDL_GetError();

	// Carga las texturas al inicio
	try {
		textures.fill(nullptr);
		string imgRoot = IMAGE_ROOT;
		string levelRoot = LEVEL_ROOT;
		
		for (size_t i = 0; i < textures.size(); i++) {
			auto [name, nrows, ncols] = textureList[i];
			textures[i] = new Texture(renderer, (imgRoot + name).c_str(), nrows, ncols);
		}

		ifstream levelFile((levelRoot + "1.txt").c_str());
		if (!levelFile.is_open())
			throw "Fichero del nivel no encontrado"s;
		else {
			LoadGame(levelFile);
			levelFile.close();
		}
	}
	catch (const string&) {

		// Destruimos las texturas
		for (std::size_t i = 0; i < textures.size(); ++i)
			delete textures[i];

		throw; // relanza la excepción
	}

	// Configura que se pueden utilizar capas translúcidas
	// SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// TODO: crear los objetos del juego
	
	// Creamos el arco
	bow = new Bow(Rectangle({0, (float)(WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 3.15f)}, 
		WINDOW_HEIGHT / 3.15f, WINDOW_WIDTH / 4), {0, 0}, arrows, false, textures[BOW]);

	// Creamos la infoBar
	infoBar = new InfoBar(Rectangle({0, 0}, WINDOW_WIDTH / (5.95f * 2), 
		WINDOW_HEIGHT / (21.95f * 2)), arrows, textures[ARROW]);
	
	// Configuramos la semilla del generador para la generación de globos
	generator.seed(std::random_device()());

	// Calculamos el tiempo que ha de pasar hasta que aparezca el primer globo
	nextBalloon = exponential_distribution<double>(1.0f / baldelay)(generator);
	
	// Almacenamos el tiempo actual al iniciar el programa en la variable LastBalloonTime
	LastBalloonTime = SDL_GetTicks();
}

Game::~Game()
{
	// Destruimos los globos, las flechas y el arco
	for (std::size_t j = 0; j < balloonsVector.size(); ++j)
		delete balloonsVector[j];
	for (std::size_t k = 0; k < arrowsVector.size(); ++k)
		delete arrowsVector[k];
	delete bow;

	delete infoBar;

	// Destruimos las texturas
	for (std::size_t i = 0; i < textures.size(); ++i)
		delete textures[i];

	// Destruimos el renderer y la ventana
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void
Game::render() const
{
	SDL_RenderClear(renderer);

	// Renderizamos las flechas, los globos y el arco
	for (std::size_t i = 0; i < arrowsVector.size(); ++i) {
		arrowsVector[i]->render();
	}
	for (std::size_t i = 0; i < balloonsVector.size(); ++i){
		balloonsVector[i]->render();
	}
	bow->render();
	infoBar->render();

	SDL_RenderPresent(renderer);
}

void
Game::createBalloon() {

	// Calculamos, de manera pseudoaleatoria, la posición del borde inferior 
	// en la que aparecerá el globo, junto con su color y su velocidad en el 
	// eje y
	int color =  uniform_int_distribution<int>(0, 6)(generator);
	float speedY = uniform_real_distribution<float>(-0.05f, -0.02f)(generator);
	float posX =  uniform_real_distribution<float>(WINDOW_WIDTH / 3, 
		WINDOW_WIDTH - WINDOW_WIDTH / 6.25f)(generator);

	// Creamos el rectángulo en base a todas variables calculadas
	Rectangle balloonRect({posX, WINDOW_HEIGHT}, 
		WINDOW_WIDTH / 6.25f,
		WINDOW_HEIGHT / 4.7f);

	// Con todo calculado, creamos un globo y lo incorporamos al vector de 
	// globos
	balloonsVector.push_back(new Balloon(balloonRect, color, Vector2D(0.0f, speedY), textures[BALLOON]));
	
	// Recalculamos el valor hasta el siguiente globo
	nextBalloon = exponential_distribution<double>(1.0f / baldelay)(generator);

	// Actualizamos el valor de LastBalloonTime
	LastBalloonTime = SDL_GetTicks();
}

void
Game::createArrow() {

	// Guardamos el rectángulo del arco para usarlo para crear el rectángulo
	//  de la flecha
	Rectangle bowRect = bow->getRect();
	Rectangle arrowRect({0, bowRect.getTopLeftCorner().getY() + WINDOW_HEIGHT / 6.3f},
		WINDOW_WIDTH / 5.95f,
		WINDOW_HEIGHT / 21.95f);
		
		// Creamos la flecha y la metemos en el vector de flechas
		arrowsVector.push_back(new Arrow(this, arrowRect, Vector2D<>(0.1f, 0.0f), textures[ARROW]));
		
		// Actualizamos el valor de arrows al actual 
		arrows = bow->getArrows();
	}
	
void
Game::update()
{
	// Actualizamos el arco
	bow->update(FRAME_PERIOD);

	// Comprobamos si ya ha pasado suficiente tiempo desde la generación del último
	// globo
	if (SDL_GetTicks() - LastBalloonTime > nextBalloon)
		createBalloon();

	// Una vez hemos generado (o no) un globo, actualizamos todos los globos
	// del vector de globos
	std::size_t i = 0;
	while (i < balloonsVector.size()) {

		// Si el globo se sale de los límites, lo eliminamos del vector
		if(balloonsVector[i]->update(FRAME_PERIOD))
			++i;
		else {
			delete balloonsVector[i];
			balloonsVector.erase(balloonsVector.begin() + i);
		}
	}

	// Si el número de flechas ha disminuido en comparación con el anterior update,
	// disparamos una flecha desde la posición del arco
	if (bow->getArrows() < arrows) {
		infoBar->decrementArrows();
		createArrow();
	}

	// Actualizamos las flechas
	std::size_t j = 0;
	while (j < arrowsVector.size()) {
		if (arrowsVector[j]->update(FRAME_PERIOD))
			++j;
		else {
			delete arrowsVector[j];
			arrowsVector.erase(arrowsVector.begin() + j);
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

		if (arrows == 0 && arrowsVector.empty()) exit = true;
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
			bow->handleEvent(event);
		}
	}
}

bool
Game::checkCollision(const Rectangle& rect) {

	// Comprobamos si alguno de los globos del vector colisiona con el rectángulo
	// recibido de la flecha
	std::size_t i = 0;
	while (i < balloonsVector.size() && !balloonsVector[i]->hit(rect)) ++i;
	return i != balloonsVector.size();
}

// Implementación del método LoadGame
void
Game::LoadGame(std::ifstream& file) {
	string keyword;
	int temp;

	while (file >> keyword) { // Leer palabra de comentarios
		if (keyword == "arrows") {
			if (!(file >> temp) || temp < 1) {
				throw "No se pudo leer un numero valido para arrows"s;
			}
			else arrows = temp;
		}
		else if (keyword == "baldelay") {
			if (!(file >> temp) || temp < 500) {
				throw "No se pudo leer un numero valido para baldelay"s;
			}
			else baldelay = temp;
		}
		else {
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
}

/* else if (keyword == "bgcolor") {
	// Leer el color de fondo (no se utiliza en este ejemplo)
	int r, g, b;
	file >> r >> g >> b;
	}
else if (line == "balpoints") {
}
else if (line == "goal") {
} */
