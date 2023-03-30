#include <fstream>
#include <iostream>
#include <string>

using std::string;
using std::ifstream;

class Object {
public:
    Object(int y, int x, int layer);
    ~Object();

    virtual void generateSprite();

    virtual int getHeight();
    virtual int getWidth();
    virtual char getSpriteChar(int i, int j);
    virtual int getY();
    virtual int getX();
    virtual int getLayer();
    virtual void moveXY(int deltaX, int deltaY);
    virtual string getType();

protected:
    static const int MAX_HEIGHT = 20;
    static const int MAX_WIDTH = 20;
    char sprite[MAX_HEIGHT][MAX_WIDTH];
    string spriteName = "default";

    int height = 0;
    int width = 0;


private:
    int y;
    int x;
    int layer;
};

Object::Object(int y, int x, int layer) : y(y), x(x), layer(layer) { }
Object::~Object() { }

void Object::generateSprite() {
    ifstream fsprite("sprites/" + spriteName + ".txt");
    string tmp;

    fsprite >> tmp;
    height = stoi(tmp);
    fsprite >> tmp;
    width = stoi(tmp);

    for (int i = 0; i < height; i++) {
        fsprite.get();
        for (int j = 0; j < width; j++) {
            sprite[i][j] = fsprite.get();
        }
    }

    fsprite.close();
}

int Object::getHeight() { return height; }
int Object::getWidth() { return width; }
char Object::getSpriteChar(int i, int j) { return sprite[i][j]; }
int Object::getY() { return y; }
int Object::getX() { return x; }
int Object::getLayer() { return layer; };
string Object::getType() { return "default"; }

void Object::moveXY(int deltaX, int deltaY) {
    x += deltaX;
    y += deltaY;
}
