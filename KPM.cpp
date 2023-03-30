#include <string>

using std::string;

class KPM : public Object {
public:
    KPM(int y, int x, int layer);
    string getType();
};

KPM::KPM(int y, int x, int layer) : Object(y, x, layer) {
    spriteName = "kpm";
    generateSprite();
}

string KPM::getType() {
    return "kpm";
}
