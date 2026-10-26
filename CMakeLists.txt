#
# Proyecto CMake para las prácticas de TPV1
#

cmake_minimum_required(VERSION 3.15...3.30)

# Proyecto en C++
project(ProyectoSDL VERSION 1.0 LANGUAGES CXX)

# Selecciona la versión C++20 del estándar (sin extensiones)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# El código carga los recursos con rutas relativas al .exe ("../assets/images/..."),
# así que el ejecutable se genera en <raiz>/bin, al lado de la carpeta assets
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Opción para activar el comprobador de memoria
option(ENABLE_ASAN "Activa el comprobador de memoria AddressSanitizer")

# Compila con la biblioteca SDL
if (CMAKE_HOST_WIN32)
	# Utiliza las bibliotecas compiladas que van incluidas
	# en el paquete con el proyecto de Visual Studio

	include_directories(
		libs/SDL3/include
		libs/SDL3_image/include
	)

	link_directories(
		libs/SDL3/lib
		libs/SDL3_image/lib
	)

	link_libraries(
		SDL3.lib
		SDL3_image.lib
	)

	# Si se utiliza el compilador de Visual Studio
	if (MSVC)
		if (ENABLE_ASAN)
			add_compile_options("$<$<CONFIG:Debug>:/fsanitize=address>")
		else()
			# Añade checkML.h automáticamente a todas las unidades de compilación
			add_compile_options("$<$<CONFIG:Debug>:/FI${CMAKE_SOURCE_DIR}/libs/misc/include/checkML.h>")
		endif()
	endif()
else()
	# En otros sistemas operativos utiliza find_package
	# para encontrar las bibliotecas instaladas aparte

	find_package(SDL3 REQUIRED)
	find_package(SDL3_image REQUIRED)

	link_libraries(SDL3::SDL3)
endif()

# Activa AddressSanitizer
if (ENABLE_ASAN AND NOT MSVC)
	add_compile_options("$<$<CONFIG:Debug>:-fsanitize=address>")
	add_link_options("$<$<CONFIG:Debug>:-fsanitize=address>")
endif()

add_executable(ProyectoSDL
	# Archivo de código
	src/main.cpp
	src/game.cpp
	src/rectangle.cpp
	src/texture.cpp
	src/balloon.cpp
	src/bow.cpp
	src/arrow.cpp
	src/infobar.cpp
)

# En Windows copia las DLL de SDL junto al .exe después de compilar
if (CMAKE_HOST_WIN32)
	add_custom_command(TARGET ProyectoSDL POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${CMAKE_SOURCE_DIR}/libs/SDL3/lib/SDL3.dll"
			"${CMAKE_SOURCE_DIR}/libs/SDL3_image/lib/SDL3_image.dll"
			"$<TARGET_FILE_DIR:ProyectoSDL>"
	)
endif()
