#ifndef CARD_HH
#define CARD_HH

#include <memory>

#include "shader.hh"

#include <glad/glad.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Card
{
public:
    unsigned int x, y; // position in spritesheet
    unsigned int width, height; // texture size
    glm::vec2 position; // position in space
    float angle; // rotation angle
    glm::mat4 translation; // overall translation

    Shader* shader; // card shader being used

    Card(unsigned const int x, unsigned const int y, unsigned const int width, unsigned const int height, Shader* shader);
    ~Card();

    /**
      * @brief display the card on the screen
      */
    void show();

    /**
      * @brief rotate by the given angle (in radians)
      * @note 0.05f - 0.1f seems to be a pretty good range of values
      * @param angle angle to rotate by (in radians)
      */
    void rotate(float angle);

    /**
      * @brief set the current rotation to the given angle (in radians)
      * @param angle angle to rotate to (in radians)
      */
    void setRotation(float angle);

    /**
      * @brief translate relative to the current position
      */
    void translate(glm::vec2 positionDifference);

    /**
      * @brief move to an absolute location
      */
    void move(glm::vec2 newPosition);
private:
    static const glm::mat4 identity; // identity matrix... does this stop additional copies from being made?
};

#endif