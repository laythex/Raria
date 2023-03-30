#include <iostream>
#include <unistd.h>

#include "Maze.cpp"

#include "Object.cpp"
#include "Player.cpp"
#include "Tree.cpp"
#include "KPM.cpp"
#include "KPMSmall.cpp"

using std::cout;
using std::endl;

class GameField {
public:
    GameField() {}
    GameField(int h, int w, int ground);
    ~GameField();

    void printField();
    void drawEmpty();
    void drawGround();
    void generateForest();
    void render(bool print);
    void addObject(Object* o);
    void popObject(int index);
    void drawObject(Object o);
    void sortByLayer();
    void turnLeft();
    void turnRight();
    void goBack();
    int isLeftPossible();
    int isRightPossible();
    int isReturnPossible();

    Object* getPlayer();

    void setPixel(int y, int x, char c);
    char getPixel(int y, int x);


private:
    int height, width;
    int groundLevel;
    char** field;
    int nObjs = 0;
    Object** objs;

    int location, curLoc;
    Maze maze;

    int rand_uns(int min, int max) {
        unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
        static std::default_random_engine e(seed);
        std::uniform_int_distribution<int> d(min, max);
        return d(e);
    }

    int getRandomWithChance(int chance) {
        return chance > rand_uns(0, 99);
    }
};

GameField::GameField(int h, int w, int ground) {
    height = h;
    width = w;
    groundLevel = ground;

    maze = Maze();
    location = 0;
    curLoc = -1;

    objs = new Object*[500];

    field = new char*[height];
    for (int i = 0; i < height; i++) {
        field[i] = new char[width];
    }

}

GameField::~GameField() {
    for (int i = 0; i < height; i++) {
        delete[] field[i];
    }
    delete[] field;

    for (int i = 0; i < nObjs; i++) {
        delete objs[i];
    }
    delete[] objs;
}

void GameField::printField() {
    for (int i = -1; i <= height; i++) {
        for (int j = -1; j <= width; j++) {
            if (i == -1 || i == height) cout << '-';
            else if (j == -1 || j == width) cout << '|';
            else cout << field[i][j];
        }
        cout << endl;
    }
}

void GameField::drawEmpty() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            field[i][j] = ' ';
        }
    }
}

void GameField::drawGround() {
    int leftTurnPos = maze.getValue(maze.getLeftChild(location)) * width / 100;
    int rightTurnPos = maze.getValue(maze.getRightChild(location)) * width / 100;
    int minPos = 30 * width / 100;

    int tmp = 0;
    if (leftTurnPos > rightTurnPos) tmp = 1;
    else if (rightTurnPos > leftTurnPos) tmp = -1;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < max(max(leftTurnPos, rightTurnPos), minPos) + tmp * (groundLevel - i) - 1; j++) {
            if ((abs(i - groundLevel)) <= 1) {
                field[i][j] = '=';
            }
        }
    }

    if (leftTurnPos) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (i - groundLevel < -3 && abs(j - leftTurnPos) <= 2) field[i][j] = '=';
                else if (i - groundLevel == -3 && abs(j - leftTurnPos + 1) <= 2) field[i][j] = '=';
                else if (i - groundLevel == -2 && abs(j - leftTurnPos + 2) <= 2) field[i][j] = '=';
            }
        }
    }

    if (rightTurnPos) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (i - groundLevel > 3 && abs(j - rightTurnPos) <= 2) field[i][j] = '=';
                else if (i - groundLevel == 3 && abs(j - rightTurnPos + 1) <= 2) field[i][j] = '=';
                else if (i - groundLevel == 2 && abs(j - rightTurnPos + 2) <= 2) field[i][j] = '=';
            }
        }
    }
}

void GameField::generateForest() {
    if (location == curLoc) return;

    curLoc = location;

    for (int i = 0; i < nObjs; i++) {
        if (objs[i]->getType() == "tree") popObject(i);
        if (objs[i]->getType() == "kpm") popObject(i);
    }

    int leftTurnPos = maze.getValue(maze.getLeftChild(location)) * width / 100;
    int rightTurnPos = maze.getValue(maze.getRightChild(location)) * width / 100;
    int minPos = 30 * width / 100;

    if (minPos == max(max(leftTurnPos, rightTurnPos), minPos)) {



        return;
    }

    for (int i = 0; i < width * 85 / 100; i += rand_uns(18, 20)) {
        if (abs(i - leftTurnPos + 3) < 7) continue;
        Object* tree = new Tree(0 + rand_uns(0, 3), i, rand_uns(-100, 0));
        addObject(tree);
    }

    for (int i = 0; i < width * 85 / 100; i += rand_uns(18, 20)) {
        if (abs(i - rightTurnPos + 3) < 7) continue;
        Object* tree = new Tree(16 + rand_uns(0, 3), i, rand_uns(-100, 0));
        addObject(tree);
    }

    for (int i = max(max(leftTurnPos, rightTurnPos), minPos); i < width; i += rand_uns(18, 20)) {
        //Object* tree = new Tree(8 + rand_uns(0, 3), i, rand_uns(-100, 0));
        //addObject(tree);
    }


    Object* kpm = new KPM(0, 90 * width / 100 - 5, -100);
    addObject(kpm);



}

void GameField::addObject(Object* o) {
    for (int i = 0; i < nObjs; i++) {
        if (objs[i]->getType() == "default") {
            objs[i] = o;
            return;
        }
    }

    objs[nObjs] = o;
    nObjs++;
}

void GameField::popObject(int index) {
    delete objs[index];
    objs[index] = new Object(0, 0, 0);
}

void GameField::drawObject(Object o) {
    for (int i = 0; i < o.getHeight(); i++) {
        for (int j = 0; j < o.getWidth(); j++) {
            if (o.getSpriteChar(i, j) == 'n') continue;
            field[i + o.getY()][j + o.getX()] = o.getSpriteChar(i, j);
        }
    }
}

void GameField::render(bool print) {
    system("cls");
    maze.printMaze();
    cout << "Current: " << location << endl;
    drawEmpty();
    drawGround();
    generateForest();
    sortByLayer();
    for (int i = 0; i < nObjs; i++) {
        drawObject(*objs[i]);
    }
    if (print) printField();
    cout << nObjs << endl;
}

Object* GameField::getPlayer() {
    return objs[0];
}

void GameField::setPixel(int y, int x, char c) {
    field[y][x] = c;
}

char GameField::getPixel(int y, int x) {
    return field[y][x];
}

void GameField::sortByLayer() {
    for (int i = 0; i < nObjs - 1; i++) {
        for (int j = 0; j < nObjs - i - 1; j++) {
            if (objs[j]->getLayer() > objs[j + 1]->getLayer()) {
                Object* tmp = objs[j];
                objs[j] = objs[j + 1];
                objs[j + 1] = tmp;
            }
        }
    }
}

void GameField::turnLeft() { location = maze.getLeftChild(location); }
void GameField::turnRight() { location = maze.getRightChild(location); }
void GameField::goBack() { location = maze.getParent(location); }

int GameField::isLeftPossible() { return maze.getValue(maze.getLeftChild(location)) > 0; }
int GameField::isRightPossible() { return maze.getValue(maze.getRightChild(location)) > 0; }
int GameField::isReturnPossible() {
    if (location == 0) return 0;
    return maze.getValue(maze.getParent(location));
}
