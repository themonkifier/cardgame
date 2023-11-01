#include "../include/pile.hh"

Pile::Pile(std::shared_ptr<Deck> d)
    : deck(std::move(d))
{
    size = 0;
}

void Pile::fillFromDeck()
{
    for (GameObject& a : deck->elements)
    {
        elements.push_back(a);
    }
    size += deck->size;
}

GameObject& Pile::operator[](std::size_t idx)
{
    auto it = elements.begin();
    
    while (idx > 0)
    {
        it++;
        idx--;
    }

    return *it;
}
const GameObject& Pile::operator[](std::size_t idx) const
{
    auto it = elements.begin();

    while (idx > 0)
    {
        it++;
        idx--;
    }

    return *it;
}

std::__cxx11::list<GameObject>::iterator Pile::begin()
{
    return elements.begin();
}
std::__cxx11::list<GameObject>::iterator Pile::end()
{
    return elements.end();
}

void Pile::shuffle()
{
    srandom(time(nullptr));

    for (int k = size - 1; k > 0; k--)
    {
        int randPos = random() % k;
        GameObject temp = std::move((*this)[k]);
        (*this)[k] = std::move((*this)[randPos]);
        (*this)[randPos] = std::move(temp);
    }
}

GameObject& Pile::move(std::shared_ptr<Pile> p, std::size_t pidx, std::size_t tidx)
{
    auto it = elements.begin();
    for (; it != elements.end() && pidx > 0; it++, tidx--);
    GameObject& gameobject = *it;
    elements.erase(it);

    it = p->elements.begin();
    for (; it != p->elements.end() && pidx > 0; it++, pidx--);
    p->elements.insert(it, gameobject);

    size--;
    p->size++;
    return gameobject;
}

GameObject& Pile::copy(std::shared_ptr<Pile> p, std::size_t pidx, std::size_t tidx)
{
    auto it = elements.begin();
    for (; it != elements.end() && pidx > 0; it++, tidx--);
    GameObject& gameobject = *it;

    it = p->elements.begin();
    for (; it != p->elements.end() && pidx > 0; it++, pidx--);
    p->elements.insert(it, gameobject);

    size--;
    p->size++;
    return gameobject;
}

std::shared_ptr<GameObject> Pile::copy(std::size_t pidx, std::size_t didx)
{
    auto dit = deck->elements.begin();
    for (; dit != deck->elements.end() && pidx > 0; dit++, didx--);
    std::shared_ptr<GameObject> gameobject = std::make_shared<GameObject>(*dit);

    auto pit = elements.begin();
    for (; pit != elements.end() && pidx > 0; pit++, pidx--);
    elements.insert(pit, *gameobject);

    size++;
    
    return gameobject;
}

void Pile::sort()
{
    elements.sort();
}

std::size_t Pile::operator()(const Pile& p) const noexcept
{
    return 84279839621 * p.size % 11963004479 + 82710400009;
}