#include <string>

using std::string;

class Player : public Object {
public:
    Player(int y, int x, int layer);
    string getType();
};

Player::Player(int y, int x, int layer) : Object(y, x, layer) {
    spriteName = "player";
    generateSprite();
}

string Player::getType() {
    return "player";
}
