#ifndef CLASSHEADER_H_INCLUDED
#define CLASSHEADER_H_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <bitset>
#include <algorithm>
#include <cstring>

using namespace std;

class DisplayFunctions {
public:
    void initializeBattlefield(char** battlefield, int rows, int cols, int cellWidth);
    void displayBattlefield(char** battlefield, int rows, int cols, int cellWidth, int cellHeight);
    void displayBattlefield(char** battlefield, int rows, int cols, int cellWidth, int cellHeight, ofstream& outfile);
    void location(char** battlefield, int M, int N, int CELL_WIDTH, int x, int y, const string& id);

private:
    void displayBattlefieldHelper(char** battlefield, int rows, int cols, int cellWidth, int cellHeight, ostream& os);
};

void DisplayFunctions::initializeBattlefield(char** battlefield, int rows, int cols, int cellWidth) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            memset(battlefield[i] + j * (cellWidth + 1), ' ', cellWidth);
            battlefield[i][j * (cellWidth + 1) + cellWidth] = '\0';  // Null-terminate the C-string
        }
    }
}

void DisplayFunctions::displayBattlefieldHelper(char** battlefield, int rows, int cols, int cellWidth, int cellHeight, ostream& os) {
    os << "    "; // Leading spaces for column headers
    for (int j = 0; j < cols; ++j) {
        os << setw(cellWidth) << setfill(' ') << j << ' ';
    }
    os << endl;

    for (int i = 0; i < rows; ++i) {
        os << setw(2) << setfill('0') << i << " +";
        for (int j = 0; j < cols; ++j) {
            for (int w = 0; w < cellWidth; ++w) {
                os << '-';
            }
            os << '+';
        }
        os << endl;

        for (int h = 0; h < cellHeight; ++h) {
            os << "   |"; // Left boundary
            for (int j = 0; j < cols; ++j) {
                os << setw(cellWidth) << setfill(' ') << (battlefield[i] + j * (cellWidth + 1)) << '|';
            }
            os << endl;
        }
    }

    os << "   +";
    for (int j = 0; j < cols; ++j) {
        for (int w = 0; w < cellWidth; ++w) {
            os << '-';
        }
        os << '+';
    }
    os << endl;
}

void DisplayFunctions::displayBattlefield(char** battlefield, int rows, int cols, int cellWidth, int cellHeight) {
    displayBattlefieldHelper(battlefield, rows, cols, cellWidth, cellHeight, cout);
}

void DisplayFunctions::displayBattlefield(char** battlefield, int rows, int cols, int cellWidth, int cellHeight, ofstream& outfile) {
    displayBattlefieldHelper(battlefield, rows, cols, cellWidth, cellHeight, outfile);
}

void DisplayFunctions::location(char** battlefield, int N, int M, int CELL_WIDTH, int x, int y, const string& id) {
    if (x >= 0 && x < M && y >= 0 && y < N) { // Check bounds to prevent out-of-bounds access
        strncpy(battlefield[y] + x * (CELL_WIDTH + 1), id.c_str(), CELL_WIDTH); // Copy id into the battlefield cell
        battlefield[y][x * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
    } else {
        cout << "Error: Coordinates (" << x << ", " << y << ") are out of bounds." << endl;
    }
}

#endif // CLASSHEADER_H_INCLUDED

