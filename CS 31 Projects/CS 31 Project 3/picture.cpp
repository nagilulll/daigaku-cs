//
//  main.cpp
//  picture
//
//  Created by Santa Yasumaru on 10/25/23.
//
#include "grid.h"
#include <cctype>
#include <stdbool.h>
#include <cassert>
#include <iostream>
using namespace std;
void plotHorizontalLine(int r, int c, int distance, char ch){
        if (distance >= 0){ //distance is positive
            for (int i = 0; i <= distance; i++){ //i is incremeted until i reaches the distance value
                if(c + i <= getCols() && c + i > 0) //checks for outofbounds
                    setChar(r, c + i, ch); //distance is added to c
            } //Moves right when distance is positive
        }
        else if (distance < 0){ //distance is negative
            for (int i = 0; i >= distance; i--){ //i is decremeted until i reaches the distance value
                if (c + i <= getCols() && c + i > 0) //checks for outofbounds
                    setChar(r, c + i, ch); //c is subtracted by negative distance
            } //Moves left when distance is negative
        }
}
void plotVerticalLine(int r, int c, int distance, char ch){
    if (distance >= 0){
        for (int i = 0; i <= distance; i++){
            if(r + i <= getRows() && r + i > 0)
                setChar(r + i, c, ch);
        }
    }
        else if (distance < 0){
            for (int i = 0; i >= distance; i--){
                if (r + i <= getRows() && r + i > 0)
                    setChar(r + i, c, ch);
            }
        }
    }
void plotRectangle(int r, int c, int height, int width, char ch){
    if (r < 0)
        r = 0;
    else if (c < 0)
        c = 0;
    else if (width >= 0 && height >= 0){ //checks for valid lengths
        plotHorizontalLine(r, c, width, ch); //calls horizontal function
        plotHorizontalLine(r + height, c, width, ch); //calls horizontal function with height added to original r value
        plotVerticalLine(r, c, height, ch); //calls vertical function
        plotVerticalLine(r, c + width, height, ch); //calls vertical function with width added to original c value
    }
}
const int HORIZ = 0;
const int VERT = 1;
const int FG = 0;
const int BG = 1;
bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg){
    if ((dir == 0 || dir == 1) && (fgbg == 0 || fgbg == 1) && r + distance > 0 && c + distance > 0 && r + distance <= getRows() && c + distance <= getCols() && isprint(plotChar)){
        if (dir == 0){
            plotHorizontalLine(r, c, distance, plotChar);
            if (fgbg == BG){
                for (int i = 0; i <= (distance); i++){
                    int row = (dir == 0) ? r : r + i;
                    int col = (dir == 0) ? c + i : c;
                    if (getChar(row, col) == ' ')
                        setChar(row, col, plotChar);
                }
            }
        } else if (dir == 1){
            plotVerticalLine(r, c, distance, plotChar);
            if (fgbg == BG){
                for (int i = 0; i <= (distance); i++){
                    int row = (dir == 0) ? r : r + i;
                    int col = (dir == 0) ? c + i : c;
                    if (getChar(row, col) == ' ')
                        setChar(row, col, plotChar);
                }
            }
        }
        return true; //plot for valid input
    }
    else
        return false; //does not plot if invalid inputs
}
int performCommands(string commandString, char& plotChar, int& mode, int& badPos) {
    int xValue = 1;
    int yValue = 1;
    char currentMode = mode;
    char currentChar = plotChar;
    int commandIndex = 0;
    while (commandIndex < commandString.size()) {
        char command = commandString[commandIndex];
        commandIndex++;
        if (command != 'H' && command != 'h' && command != 'V' && command != 'v' && command != 'F' && command != 'f' && command != 'B' && command != 'b' && command != 'c' && command != 'C' && !isdigit(command)) {
            badPos = commandIndex - 1;
            return 1; // syntax error for not specified letter
        }
        if (!isprint(plotChar) || (mode != BG && mode != FG)){
            return 2; // invalid mode or character
        }
        if (isdigit(command)) {
            badPos = commandIndex - 1;
            return 1; // syntax error for starting with number
        }
        if (command == 'H' || command == 'h') {
            int distance = 0;
            if (commandString[commandIndex] == '-'){
                commandIndex++;
                if (!isdigit(commandString[commandIndex])){
                    badPos = commandIndex - 1;
                    return 1;
                }
                else if (commandIndex < commandString.size() && isdigit(commandString[commandIndex])){
                    distance = (commandString[commandIndex] - '0') * -1;
                    commandIndex++;
                } if (isdigit(commandString[commandIndex])){
                    distance *= 10;
                    distance += (commandString[commandIndex] - '0');
                    commandIndex++;
                    }
                    if (isdigit(commandString[commandIndex])){
                        badPos = commandIndex - 1;
                        return 1;
                    }
            }
            else if (commandIndex < commandString.size() && isdigit(commandString[commandIndex])) {
                distance = commandString[commandIndex] - '0';
                commandIndex++;
                if (isdigit(commandString[commandIndex])){
                    distance *= 10;
                    distance += (commandString[commandIndex] - '0');
                    commandIndex++;
                } if (isdigit(commandString[commandIndex])){
                    badPos = commandIndex - 1;
                    return 1;
                }
            } else {
                badPos = commandIndex;
                return 1;  // Syntax error: Missing or invalid distance
            }
            if (currentChar == '\0' || (currentChar != '\0' && isprint(currentChar))) {
                if (currentMode == FG) {
                    if (!plotLine(xValue, yValue, distance, HORIZ, currentChar, FG)){
                        badPos = commandIndex - 3;
                        return 3;  // Invalid plotChar or out of bounds
                    }
                } else if (currentMode == BG) {
                    if (!plotLine(xValue, yValue, distance, HORIZ, currentChar, BG)){
                        badPos = commandIndex - 3;
                        return 3;  // Invalid plotChar or out of bounds
                    }
                }
                    yValue += distance;  // Update current column position
            } else {
                return 2;  // Non-printable plotChar
            }
        } else if (command == 'V' || command == 'v') {
            // Handle vertical line command
            int distance = 0;
            if (commandString[commandIndex] == '-'){
                commandIndex++;
                if (!isdigit(commandString[commandIndex])){
                    badPos = commandIndex - 1;
                    return 1;
                }
                else if (commandIndex < commandString.size() && isdigit(commandString[commandIndex])){
                    distance = (commandString[commandIndex] - '0') * -1;
                    commandIndex++;
                } if (isdigit(commandString[commandIndex])){
                    distance *= 10;
                    distance += (commandString[commandIndex] - '0');
                    commandIndex++;
                }
                if (isdigit(commandString[commandIndex])){
                    badPos = commandIndex - 1;
                    return 1;
                }
            }
            else if (commandIndex < commandString.size() && isdigit(commandString[commandIndex])) {
                distance = commandString[commandIndex] - '0';
                commandIndex++;
                if (isdigit(commandString[commandIndex])){
                    distance *= 10 + (commandString[commandIndex] - '0');
                    commandIndex++;
                }
                if (isdigit(commandString[commandIndex])){
                    badPos = commandIndex - 1;
                    return 1;
                }
                if (isdigit(commandString[commandIndex])){
                    badPos = commandIndex - 1;
                    return 1;
                }
            } else {
                badPos = commandIndex;
                return 1;  // Syntax error: Missing or invalid distance
            }
            if (currentChar == '\0' || (currentChar != '\0' && isprint(currentChar))) {
                if (currentMode == FG) {
                    if (!plotLine(xValue, yValue, distance, VERT, currentChar, FG)){
                        badPos = commandIndex - 3;
                        return 3;  // Invalid plotChar or out of bounds
                    }
                } else if (currentMode == BG) {
                    if (!plotLine(xValue, yValue, distance, VERT, currentChar, BG)){
                        badPos = commandIndex - 3;
                        return 3;  // Invalid plotChar or out of bounds
                    }
                }
                xValue += distance;  // Update current row position
            } else {
                return 2;  // Non-printable plotChar
            }
        } else if (command == 'F' || command == 'f') {
            if (isprint(commandString[commandIndex])) {
                currentMode = FG;
                currentChar = commandString[commandIndex];
                commandIndex++;
            } else {
                badPos = commandIndex - 1;
                return 1;  // Syntax error: Missing or invalid character
            }
        } else if (command == 'B' || command == 'b') {
            if (isprint(commandString[commandIndex])) {
                if (currentMode == BG) {
                if (!plotLine(xValue, yValue, 1, HORIZ, commandString[commandIndex], BG))
                        return 2;
                }
                currentChar = commandString[commandIndex];
                commandIndex++;
            } else {
                badPos = commandIndex - 1;
                return 1;  // Syntax error: Missing or invalid character
            }
        }
        else if (command == 'c' || command == 'C'){
            clearGrid();
            xValue = 1;
            yValue = 1;
            currentChar = '*';
            currentMode = FG;
            commandIndex++;
        } else {
            badPos = commandIndex - 1;
            return 1;
        }
    }
    // Update mode and plotChar after processing the commands
    mode = currentMode;
    plotChar = currentChar;
    return 0;  // All commands executed successfully
}
int main()
{
    for (;;)
    {
        cout << "Enter the number of grid rows and columns (max 30 each): ";
        int nRows;
        int nCols;
        cin >> nRows >> nCols;
        cin.ignore(10000, '\n');
        if (nRows >= 1  &&  nRows <= MAXROWS  &&  nCols >= 1  &&  nCols <= MAXCOLS)
        {
            setSize(nRows, nCols);
            break;
        }
        cout << "The numbers must be between 1 and 30." << endl;
    }
    char currentChar = '*';
    int currentMode = FG;
    for (;;)
    {
        cout << "Enter a command string (empty line to quit): ";
        string cmd;
        getline(cin, cmd);
        if (cmd == "")
            break;
        int position;
        int status = performCommands(cmd, currentChar, currentMode, position);
        switch (status)
        {
          case 0:
            draw();
            break;
          case 1:
            cout << "Syntax error at position " << position << endl;
            break;
          case 2:
            if (!isprint(currentChar))
                cout << "Current character is not printable" << endl;
            if (currentMode != FG  &&  currentMode != BG)
                cout << "Current mode is " << currentMode << ", not FG or BG" << endl;
            break;
          case 3:
            cout << "Cannot perform command at position " << position << endl;
            break;
          default:
              // It should be impossible to get here.
            cout << "performCommands returned " << status << "!" << endl;
        }
    }
}
