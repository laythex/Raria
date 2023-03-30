#include <string>

using std::string;

class KPMSmall : public Object {
public:
    KPMSmall(int y, int x, int layer);
    string getType();
};

KPMSmall::KPMSmall(int y, int x, int layer) : Object(y, x, layer) {
    spriteName = "kpm small";
    generateSprite();
}

string KPMSmall::getType() {
    return "kpm";
}
