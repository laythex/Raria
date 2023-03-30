#include <iostream>
#include <string>
#include <windows.h>
#include "GameField.cpp"

using std::string;
using std::cout;
using std::cin;
using std::endl;


class GameController {
public:
    GameController() { }

    void createWorld() {
        int groundLevel = 15;
        gf = new GameField(30, 150, groundLevel);
        Object* player = new Player(groundLevel - 2, 10, 0);
        gf->addObject(player);
        location = 0;
    }

    void start() {
        ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
        createWorld();
        gf->render(true);
        while (true) {
            cout << " Available actions:" << endl;
            if (gf->isLeftPossible()) cout << "  * turn left" << endl;
            if (gf->isRightPossible()) cout << "  * turn right" << endl;
            if (gf->isReturnPossible()) cout << "  * go back" << endl;
            cout << " >";
            getline(cin, commandLine);
            execute();
            gf->render(true);
        }
    }

    void execute() {
        getNextCommand();
        if (command == "turn") {
            getNextCommand();
            if (command == "right") {
                if (gf->isRightPossible()) gf->turnRight();
                return;
            }
            if (command == "left") {
                if (gf->isLeftPossible()) gf->turnLeft();
                return;
            }
        }
        if (command == "go") {
            getNextCommand();
            if (command == "back") {
                if (gf->isReturnPossible()) gf->goBack();
                return;
            }
        }
    }

    void getNextCommand() {
        int k = 0;
        int length = commandLine.size();
        while (k < length) {
            if (commandLine[k] == ' ' || k == length) {
                command = commandLine.substr(0, k);
                commandLine = commandLine.substr(k + 1, length);
                k++;
                return;
            }
            k++;
        }
        command = commandLine;
        commandLine = "";
    }

private:
    string commandLine;
    string command;
    GameField* gf;
    Maze maze;
    int location;
};
