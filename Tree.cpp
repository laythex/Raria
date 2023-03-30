#include <string>
#include <chrono>
#include <random>

using std::string;

class Tree : public Object {
public:
    Tree(int y, int x, int layer);
    string getType();
private:
    int rand_uns(int min, int max) {
        unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
        static std::default_random_engine e(seed);
        std::uniform_int_distribution<int> d(min, max);
        return d(e);
    }

};

Tree::Tree(int y, int x, int layer) : Object(y, x, layer) {
    spriteName = "tree" + std::to_string(rand_uns(5, 8));
    generateSprite();
}

string Tree::getType() {
    return "tree";
}
