#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "gameobject.h"
#include "rectangle.h"

class Texture;
class Game;

class SceneObject : public GameObject {
    private:

        // Dimensiones y posición del objeto en escena representados por un rectángulo
        Rectangle rect;

        // Textura del objeto de la escena
        Texture* texture;

    public:

        // Constructor de la clase
        SceneObject(Game* game, const Rectangle& rect, Texture* texture);

        // Método que comprueba si se da la colisión entre dos objetos de la escena
        virtual bool hit(const Rectangle& obj);

        // Método de la clase SceneObject que guarda todos los datos relativos a
        // un objeto de la clase que son vitales para su recreación
        virtual void saveObject(std::ostream& savedFile) const; 

        // Carga los datos de un SceneObject ya existente
        virtual void loadObject(std::istream& savedGameFile, const int lineNumber);
        
    protected:

        // Getter y move del rectángulo
        const Rectangle& getRect() const;
        void moveRect(const Vector2D<>& v);

        // Getter de texture
        Texture* getTexture() const;
};

#endif