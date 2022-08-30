// created by Evan Hatton on September 1st, 2020
// For CS 315 taught by Ali Kooshesh
// Maze Solving Program
// maze.hpp

// this maze class should:
// have a private vector of tuples for cells
// read a file to construct a maze
// find neighbor for each cell
// overload the () operator to return the tuple of that specific value

#define TUPTYPE std::tuple<char,int,int>

#ifndef MAZE_HPP
#define MAZE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <stack>

class Maze {
	public:
		Maze(std::string inputFile);
		TUPTYPE getCell(int col, int row);
		int solve();
		~Maze();
		void printPrettyMaze();


	private:
		std::vector< std::vector< TUPTYPE > > cells;
		std::ifstream mazeReader;
		std::ofstream mazeWriter;
		int maxRow = 0;
		std::vector< std::vector< bool> > used;
		std::stack< TUPTYPE > twine;

		void printMaze();
		TUPTYPE findEdge();
		void resetUsed();
		TUPTYPE neighbor(int col, int row);
};

#endif
