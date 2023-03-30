#include <iostream>
#include <chrono>
#include <random>
using namespace std;

class Maze {
public:
    Maze() {
        maze[0] = 1;
        for (int i = 1; i < treesize; i++) {
            int parentIndex = (i - 1) / 2;
            if (maze[parentIndex]) {
                maze[i] = getRandomWithChance(110 - i * 100 / treesize) * rand_uns(40, 90);

                if (getLeftChild(i) == -1 && isWinningSet == 0) {
                    if (i == (treesize - 1) || getRandomWithChance(100 / (1 << (DEPTH - 1)))) {
                        maze[i] = 100;
                        isWinningSet = 1;
                    }
                }
            }
        }
    }

    void printMaze() {
        int two = 1, k = 1;
        for (int i = 0; i < treesize; i++) {
            cout << maze[i] << ' ';
            if (i == k - 1) {
                cout << endl;
                two *= 2;
                k += two;
            }
        }
    }

    int getValue(int index) {
        if (index == -1) return 0;
        return maze[index];
    }

    int getLeftChild(int index) {
        return index * 2 + 1 < treesize ? index * 2 + 1 : -1;
    }

    int getRightChild(int index) {
        return index * 2 + 2 < treesize ? index * 2 + 2 : -1;
    }

    int getParent(int index) {
        return (index - 1) / 2;
    }



protected:
    static const int DEPTH = 5;
    int treesize = (1 << DEPTH) - 1;
    int maze[(1 << DEPTH) - 1] = { 0 };

private:
    int isWinningSet = 0;

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
