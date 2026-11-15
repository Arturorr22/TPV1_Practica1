#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class Game;

class GameObject {
    protected:
            
        // Puntero a Game
        Game* game;

    public:

        // Constructor de la clase
        GameObject(Game* game);

        // Métodos abstractos de la clase que más tarde se sobrecargarán
        virtual ~GameObject() = default;
        virtual void render() const = 0;
        virtual bool update(const float delta) = 0;

};

#endif 