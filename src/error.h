#pragma once

#include <SDL3/SDL.h>

#include <stdexcept>
#include <string>

// Clase base de todos los errores del juego. No añade funcionalidad: reutiliza
// los constructores y el método what() de logic_error. Su tipo permite filtrar
// los errores propios del juego en los bloques catch
class GameError : public std::logic_error {
public:
	using std::logic_error::logic_error;
};

// Error de inicialización o uso de SDL. El mensaje incluye el texto que da
// SDL_GetError(), así que hay que lanzarla justo después de que falle la llamada
class SDLError : public GameError {
public:

	// context indica qué se estaba haciendo cuando falló SDL (por ejemplo, "SDL_Init")
	explicit SDLError(const std::string& context)
		: GameError("Error de SDL en " + context + ": " + SDL_GetError()) {
	}
};

// Error al no encontrar un fichero que se intenta abrir
class FileNotFoundError : public GameError {
public:

	// El mensaje incluye el nombre del fichero que no se ha encontrado
	explicit FileNotFoundError(const std::string& fileName)
		: GameError("No se ha encontrado el fichero: " + fileName) {
	}
};

// Error de formato al leer un archivo de datos (niveles o partidas guardadas).
// Almacena el nombre del archivo y el número de línea del error
class FileFormatError : public GameError {
private:

	// Archivo y línea (empezando en 1) donde se produjo el error
	std::string fileName;
	int lineNumber;

	// Compone el mensaje completo. Es estática porque se usa para construir la clase
	// base, cuando los atributos de esta clase todavía no existen
	static std::string buildMessage(const std::string& fileName, int lineNumber,
		const std::string& message) {
		return "Error de formato en " + fileName + " (linea " + std::to_string(lineNumber)
			+ "): " + message;
	}

public:

	FileFormatError(const std::string& fileName, int lineNumber, const std::string& message)
		: GameError(buildMessage(fileName, lineNumber, message))
		, fileName(fileName)
		, lineNumber(lineNumber) {
	}

	// Nombre del archivo donde se produjo el error
	const std::string& getFileName() const { return fileName; }

	// Número de línea donde se produjo el error
	int getLineNumber() const { return lineNumber; }
};
