#ifndef CARD_HH
#define CARD_HH

#include <string>
#include <memory>

#include "shader.hh"
#include "texture.hh"
#include "resource_manager.hh"

#include <glad/glad.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Card
{
public:
    struct CardInfo
    {
        std::string suit;
        std::string rank;
        int value;

        bool operator==(const CardInfo& ci) const noexcept;
        CardInfo(std::string suit, std::string rank, int value);
        CardInfo();
    } cardInfo;
    bool isClicked;
    bool isDraggable;
    glm::vec2 size; // texture size
    glm::vec2 position; // position in space
    glm::vec3 tint; // display tint
    float angle; // rotation angle
    glm::mat4 translation; // overall translation

    Texture texture;

    Card(Texture texture, CardInfo cardInfo, glm::vec3 tint = glm::vec3(1.0f, 1.0f, 1.0f));
    Card(CardInfo cardInfo, bool alpha, glm::vec3 tint = glm::vec3(1.0f, 1.0f, 1.0f));
    bool operator==(const Card& c) const noexcept;

    /**
      * @brief update the translation for display on the screen
      */
    void updateTranslation();

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

    /**
      * @brief action to perform when clicked on
      */
    void onClick(glm::vec2 position);

    /**
      * @brief action to preform when clicked and held on
      */
    void onHold(glm::vec2 position);
private:
    static const glm::mat4 identity; // identity matrix... does this stop additional copies from being made?
};

#endif