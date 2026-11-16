#ifndef MOBILEOBJECT_H
#define MOBILEOBJECT_H

#include "sceneobject.h"
#include "vector2D.h"

class MobileObject : public SceneObject {
    private:

        // Velocidad del objeto móvil
        Vector2D<> speed;

    public:

        // Constructor de la clase MobileObject
        MobileObject(Game* game, const Rectangle& rect, Texture* texture, 
            const Vector2D<>& speed);

        // Sobreescritura del método update heredado de la clase GameObject
        bool update(const float delta) override;

        // Sobreescritura del método saveObject heredado de la clase SceneObject
        void saveObject(std::ostream& savedFile) const override;
        
        // Método que carga los datos de un MobileObject ya existente
        void loadObject(std::istream& savedGameFile, const int lineNumber) override;

    protected:

        // Getter de la velocidad
        const Vector2D<>& getSpeed() const;

        // Modificador de la velocidad
        void setSpeed(const Vector2D<>& newSpeed);
};

#endif