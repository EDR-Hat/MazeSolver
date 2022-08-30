// created by Evan Hatton on September 1st, 2020
// For CS 315 taught by Ali Kooshesh
// Maze Solving Program
// main.cpp

#define tuptype std::tuple<char,int,int>

#include <iostream>
#include "Maze.hpp"

int main(int argc, char *argv[]){

	if ( argc != 2 ){ // check arguments
		std::cout << "Usage: " << argv[0] << " pathToMazeFile." << std::endl;
		exit(1);
	}

	std::ifstream mazeStream;
	mazeStream.open(argv[1], std::ios_base::in);
	if ( ! mazeStream.is_open()) { // check provided filename
		std::cout << "Unable to open the input file >>" << argv[1] << "<< does it exist?" << std::endl;
		exit(2);
	}
	mazeStream.close();

	Maze cornfield(argv[1]);
	if (cornfield.solve() == 2){
		std::cout << "No valid endpoints" << std::endl;
		exit(3);
	}

	cornfield.printPrettyMaze();

	return 0;
}
