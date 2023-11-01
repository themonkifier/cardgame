#ifndef PILE_HH
#define PILE_HH

#include <string>
#include <vector>
#include <list>

#include <glad/glad.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "deck.hh"

class Pile
{
public:
    /** poker chips
     * variations: {"chip"}
     * values: {{"White", 1}, {"Red", 5}, {"Blue", 10},
     * {"Yellow", 25}, {"Black", 100}, {"Orange", 250}, {"Purple", 500},
     * {"Silver", 1000}, {"Gold", 5000}}
     * 
     * monopoly houses
     * variations: {"house"}
     * values: {{"red", NULL}, {"green", NULL}}
     */

    glm::vec2 position;
    std::size_t size;

    Pile(std::shared_ptr<Deck> d);
    void fillFromDeck();

    GameObject& operator[](std::size_t idx);
    const GameObject& operator[](std::size_t idx) const;
    std::size_t operator()(const Pile& p) const noexcept;

    std::__cxx11::list<GameObject>::iterator begin();
    std::__cxx11::list<GameObject>::iterator end();

    // Pile(std::string pathToTextures, std::string fileExtension, std::vector<std::string> variations,
    //      std::vector<std::pair<std::string, int> > values);
    // Pile(std::string pathToTextures, std::string fileExtension, std::vector<std::string> variations,
    //      std::vector<std::pair<std::string, int> > values,
    //      std::vector<std::pair<std::string, int> > specialTextures);

    /**
     * @brief move one card from `this` to `p`
     * @param p the pile to move to
     * @param pidx the index in pile p to move to
     * @param tidx the index in this pile to move from
     */
    GameObject& move(std::shared_ptr<Pile> p, std::size_t pidx = 0, std::size_t tidx = 0);
    
    /**
     * @brief copy one card from `this` to `p`
     * @param p the pile to move to
     * @param pidx the index in pile p to move to
     * @param tidx the index in this pile to move from
     */
    GameObject& copy(std::shared_ptr<Pile> p, std::size_t pidx = 0, std::size_t tidx = 0);

    /**
     * @brief copy one card from `this`s deck to `this`
     * @param pidx the index in this to move to
     * @param tidx the index in this's deck to move from
     */
    std::shared_ptr<GameObject> copy(std::size_t pidx = 0, std::size_t didx = 0);

    /**
     * @brief sorts the pile by z-index
     */
    void sort();

    /**
     * @brief shuffles the pile
     */
    void shuffle();
private:
    std::list<GameObject> elements;
    std::string pathToTextures, fileExtension;
    std::shared_ptr<Deck> deck; // the cards allowable within this pile
};

template<> struct std::hash<Pile>
{
    std::size_t operator()(const Pile& p) const noexcept
    {
        return 84279839621 * p.size % 11963004479 + 82710400009;
    }
};

#endif