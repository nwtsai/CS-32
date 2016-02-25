#include <string>
#include <queue>
#include <iostream>

using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	queue<Coord> coordQueue;
	coordQueue.push(Coord(sr, sc));
	maze[sr][sc] = '*';

	while (!coordQueue.empty())
	{
		Coord current = coordQueue.front();
		coordQueue.pop();
		int r = current.r();
		int c = current.c();

		cerr << r << " " << c << endl;

		if (r == er && c == ec)
			return true;
		if (maze[r - 1][c] == '.') // if can move North and has not encountered
		{
			coordQueue.push(Coord(r - 1, c));
			maze[r - 1][c] = '*';
		}
		if (maze[r][c + 1] == '.') // if can move East and has not encountered
		{
			coordQueue.push(Coord(r, c + 1));
			maze[r][c + 1] = '*';
		}
		if (maze[r + 1][c] == '.') // if can move South and has not encountered
		{
			coordQueue.push(Coord(r + 1, c));
			maze[r + 1][c] = '*';
		}
		if (maze[r][c - 1] == '.') // if can move West and has not encountered
		{
			coordQueue.push(Coord(r, c - 1));
			maze[r][c - 1] = '*';
		}
	}
	return false;
}

int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X........X",
		"XX.X.XXXXX",
		"X..X.X...X",
		"X..X...X.X",
		"XXXX.XXX.X",
		"X.X....XXX",
		"X..XX.XX.X",
		"X...X....X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 6, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}