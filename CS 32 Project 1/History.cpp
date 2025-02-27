#include <iostream>
#include "Game.h"
#include "City.h"
#include "Tooter.h"
#include "Player.h"
#include "globals.h"
#include "History.h"
using namespace std;

History::History(int nRows, int nCols)
{
    m_rows = min(nRows, MAXROWS);
    m_cols = min(nCols, MAXCOLS);

    for (int r = 0; r < m_rows; r++)
        for (int c = 0; c < m_cols; c++)
            m_history[r][c] = 0;
}

bool History::record(int r, int c)
{
    if (r < 1 || r > m_rows || c < 1 || c > m_cols)
        return false;
    m_history[r - 1][c - 1]++;
    return true;
}

void History::display() const
{
    clearScreen();
    for (int r = 0; r < m_rows; r++){
        for (int c = 0; c < m_cols; c++){
            int fails = m_history[r][c];
            switch (fails)
            {
            case 0:
                cout << '.';
                break;
            case 26:
                cout << 'Z';
                break;
            default:
                cout << char('A' + min(fails - 1, 24));
                break;
            }
        }
        cout << endl;
    }
    cout << endl;
}
