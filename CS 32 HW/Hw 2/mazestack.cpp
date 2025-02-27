#include <iostream>
#include <stack>
using namespace std;

class Coord
{
  public:
    Coord(int r, int c) : m_r(r), m_c(c) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
  private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    stack<Coord> coordStack;
    coordStack.push(Coord(sr, sc));
    maze[sr][sc] = '!';
    while (!coordStack.empty())
    {
        Coord old = coordStack.top();
        coordStack.pop();
        
        if (old.r() == er && old.c() == ec)
            return true;
        if (maze[old.r() - 1][old.c()] == '.')
        {
            coordStack.push(Coord(old.r() - 1, old.c()));
            maze[old.r() - 1][old.c()] = '!';
        }
        if (maze[old.r()][old.c() - 1] == '.')
        {
            coordStack.push(Coord(old.r(), old.c() - 1));
            maze[old.r()][old.c() - 1] = '!';
        }
        if (maze[old.r() + 1][old.c()] == '.')
        {
            coordStack.push(Coord(old.r() + 1, old.c()));
            maze[old.r() + 1][old.c()] = '!';
        }
        if (maze[old.r()][old.c() + 1] == '.')
        {
            coordStack.push(Coord(old.r(), old.c() + 1));
            maze[old.r()][old.c() + 1] = '!';
        }
    }
    return false;
}

/*Push the starting coordinate (sr,sc) onto the coordinate stack and
            update maze[sr][sc] to indicate that the algorithm has encountered
            it (i.e., set maze[sr][sc] to have a value other than '.').
        While the stack is not empty,
        {   Pop the top coordinate off the stack. This gives you the current
                (r,c) location that your algorithm is exploring.
            If the current (r,c) coordinate is equal to the ending coordinate,
                then we've solved the maze so return true!
            Check each place you can move from the current cell as follows:
                If you can move NORTH and haven't encountered that cell yet,
                    then push the coordinate (r-1,c) onto the stack and update
                    maze[r-1][c] to indicate the algorithm has encountered it.
                If you can move WEST and haven't encountered that cell yet,
                    then push the coordinate (r,c-1) onto the stack and update
                    maze[r][c-1] to indicate the algorithm has encountered it.
                If you can move SOUTH and haven't encountered that cell yet,
                    then push the coordinate (r+1,c) onto the stack and update
                    maze[r+1][c] to indicate the algorithm has encountered it.
                If you can move EAST and haven't encountered that cell yet,
                    then push the coordinate (r,c+1) onto the stack and update
                    maze[r][c+1] to indicate the algorithm has encountered it.
        }
        There was no solution, so return false*/



int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X........X",
        "XXXXX.X.XX",
        "X...X.X..X",
        "X.X...X..X",
        "X.XXX.XXXX",
        "XXX....X.X",
        "X.XX.XXX.X",
        "X....X...X",
        "XXXXXXXXXX"
    };
    
    if (pathExists(maze, 10,10, 6,5, 1,8))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}


