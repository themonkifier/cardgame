#ifndef PILE_HH
#define PILE_HH

#include <string>
#include <vector>
#include <list>
#include <optional>

#include <glad/glad.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "deck.hh"

template <typename T>
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

    Pile(std::shared_ptr<Deck<T>> d, bool isDraggable = true)
        : deck(std::move(d)), isDraggable(isDraggable)
    {
        size = 0;
    }
    
    void fillFromDeck()
    {
        for (T& a : deck->elements)
        {
            elements.push_back(a);
            elements.back().texture.isDraggable = isDraggable;
        }
        size += deck->size;
    }

    T& operator[](std::size_t idx)
    {
        auto it = elements.begin();
        
        while (idx > 0)
        {
            it++;
            idx--;
        }

        return *it;
    }

    const T& operator[](std::size_t idx) const
    {
        auto it = elements.begin();

        while (idx > 0)
        {
            it++;
            idx--;
        }

        return *it;
    }

    inline std::size_t operator()(const Pile<T>& p) const noexcept {
        return 84279839621 * p.size % 11963004479 + 82710400009;
    }

    inline T& front() {
        return elements.front();
    }

    inline T& back() {
        return elements.back();
    }

    typename std::__cxx11::list<T>::iterator begin()
    {
        return elements.begin();
    }

    typename std::__cxx11::list<T>::iterator end()
    {
        return elements.end();
    }

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
    std::optional<std::shared_ptr<T> > deal(std::shared_ptr<Pile<T>> p, std::size_t pidx = 0, std::size_t tidx = 0)
    {
        if (size <= 0) return {};

        auto it = elements.begin();
        for (; it != elements.end() && tidx > 0; it++, tidx--);
        T& gameobject = *it;
        elements.erase(it);

        it = p->elements.begin();
        for (; it != p->elements.end() && pidx > 0; it++, pidx--);
        p->elements.insert(it, gameobject);

        size--;
        p->size++;
        return std::make_shared<T>(gameobject);
    }
    
    /**
     * @brief copy one card from `this` to `p`
     * @param p the pile to move to
     * @param pidx the index in pile p to move to
     * @param tidx the index in this pile to move from
     */
    std::optional<std::shared_ptr<T> > copy(std::shared_ptr<Pile<T>> p, std::size_t pidx = 0, std::size_t tidx = 0)
    {
        if (size <= 0) return {};

        auto it = elements.begin();
        for (; it != elements.end() && tidx > 0; it++, tidx--);
        T& gameobject = *it;

        it = p->elements.begin();
        for (; it != p->elements.end() && pidx > 0; it++, pidx--);
        p->elements.insert(it, gameobject);

        p->size++;
        return gameobject;
    }

    /**
     * @brief copy one card from `this`s deck to `this`
     * @param pidx the index in this to move to
     * @param tidx the index in this's deck to move from
     */
    std::optional<std::shared_ptr<T> > copy(std::size_t pidx = 0, std::size_t didx = 0)
    {
        if (deck->size <= 0) return {};

        auto dit = deck->elements.begin();
        for (; dit != deck->elements.end() && didx > 0; dit++, didx--);
        std::shared_ptr<T> gameobject = std::make_shared<T>(*dit);

        auto pit = elements.begin();
        for (; pit != elements.end() && pidx > 0; pit++, pidx--);
        elements.insert(pit, *gameobject);

        size++;
        
        return gameobject;
    }

    /**
     * @brief sorts the pile by z-index
     */
    void sort() {
        elements.sort();
    }

    std::size_t length() {
        return elements.size();
    }

    /**
     * @brief shuffles the pile
     */
    void shuffle()
    {
        srandom(time(nullptr));

        for (int k = size - 1; k > 0; k--)
        {
            int randPos = random() % k;
            T temp = std::move((*this)[k]);
            (*this)[k] = std::move((*this)[randPos]);
            (*this)[randPos] = std::move(temp);
        }
    }

    void move(glm::vec2 newPosition) {
        position = newPosition;
        for (T& t : elements) {
            t.texture.move(newPosition);
        }
    }
private:
    std::list<T> elements;
    std::string pathToTextures, fileExtension;
    std::shared_ptr<Deck<T>> deck; // the cards allowable within this pile
    bool isDraggable;
};

template <typename T> struct std::hash<Pile<T> >
{
    std::size_t operator()(const Pile<T>& p) const noexcept
    {
        return 84279839621 * p.size % 11963004479 + 82710400009;
    }
};

#endif