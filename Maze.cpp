// created by Evan Hatton on Septermber 1st, 2020
// For CS 315 taught by Ali Kooshesh
// Maze Solving Program
// maze.cpp

#define TUPTYPE std::tuple<char,int,int> // could not get typedef to work for this substitution
// had to use this instead
#define TVAL(X,Y) std::get<X>(Y) // cleans up the function to access the tuples values, which
// otherwise makes the code unnecessarially busy

#include "Maze.hpp"

#ifndef MAZE_CPP
#define MAZE_CPP

//this function populates the cells variable from an input file of a maze, which has to be
//rectangular, and have exactly two endpoints with a solution between them in order to solve
//the maze.
Maze::Maze(std::string inputFile){
	std::string line;
	mazeReader.open(inputFile);
	int a = 0; // make a counter for the columns
	std::vector < TUPTYPE > rows; // a container for the rows
	std::vector <bool> truth; // a vector to populate used with correctly
	while (std::getline(mazeReader, line)){
		for (int l = 0; l < line.length(); l++)
			rows.push_back(TUPTYPE ( line[l], a, l) ); // read in the line as tuples
		cells.push_back(rows);
		if ( !maxRow ) // if maxRow has not been set yet, set it
			maxRow = rows.size();
		truth.assign (maxRow, true);
		used.push_back(truth);
		rows.clear();
		a++;
	}
	mazeReader.close();
}

Maze::~Maze(){
	// close streams if still open
	if (mazeWriter.is_open())
		mazeWriter.close();
	if (mazeReader.is_open())
		mazeReader.close();
}

void Maze::printPrettyMaze(){ // prints only
	for(int i = 0; i < cells.size(); i++){
		for( int j = 0; j <cells[i].size(); j++)
			std::cout << TVAL(0,cells[i][j]);
		std::cout << std::endl;
	}
}


void Maze::printMaze(){ //debug function, shows blanks if the cell has been
	// encountered and it is not a wall
	// used it in a loop to find a seg fault
	// way faster than using gdb to identify that particular issue
	for(int i = 0; i < cells.size(); i++){ //loop through each cell
		for ( int j = 0; j < cells[i].size(); j++)
			if (TVAL(0,cells[i][j]) == '1')
				std::cout << '1';
			else
				if (used[i][j]) //if it's not been encountered
					std::cout << '0'; // print the regular cell
				else
					std::cout << ' '; // otherwise show that it's been seen
		std::cout << std::endl;
	}
}
		
// this function returns a tuple that has not been seen before. if non exists
// it returns an error tuple.
TUPTYPE Maze::neighbor( int col, int row){
	std::vector <TUPTYPE > nextDoor;
	std::vector <TUPTYPE > pairs;

	// couldn't think of a way to iterate over these conditions, so
	// wrote them out one at a time. It checks whether the bounds are
	// met for the four surrounding cells of the given cell.
	if (col - 1 >= 0 )
		pairs.push_back( std::make_tuple ('-', col - 1, row) );
	if (col + 1 < cells.size() )
		pairs.push_back( std::make_tuple ('-', col + 1, row) );
	if ( row - 1 >= 0 )
		pairs.push_back( std::make_tuple ('-', col, row - 1) );
	if ( row + 1 < maxRow )
		pairs.push_back( std::make_tuple ('-', col, row + 1) );
	for ( auto pair : pairs)
		if ( TVAL(0, cells[TVAL(1,pair)][TVAL(2,pair)] ) != '1' &&
				used[TVAL(1,pair)][TVAL(2,pair)] )
			// if it's not a wall and hasn't been used
			return cells[TVAL(1,pair)][TVAL(2,pair)];

	return std::make_tuple ('-', -1, -1);
}

void Maze::resetUsed(){
	for( auto row : used )
		row.assign(maxRow, true);
}

// this function looks at the edges of the rectangular maze and returns an endpoint that has not been seen before.
TUPTYPE Maze::findEdge (){
	for( int i = 0; i < cells.size(); i++){
		if ( ('0' == TVAL(0,cells[i][0]) ) && used[i][0]){
			return cells[i][0];
		}

		if ( ('0' == TVAL(0,cells[i][maxRow - 1])) && used[i][maxRow - 1]){
			return cells[i][ maxRow - 1];
		}
	}

	for( int j = 0; j < maxRow; j++){
		if ( ('0' == TVAL(0,cells[0][j])) && used[0][j]){
			return cells[0][ j];
		}
		if ( ('0' == TVAL(0,cells[cells.size() - 1][j])) && used[cells.size() - 1][j]){
			return cells[cells.size() - 1][j];
		}
	}
	return TUPTYPE ('-', -1, -1); // needed for compilation and also can be used as an error code
	// shouldn't reach this point if preconditions are met
}

int Maze::solve(){
	resetUsed();
	TUPTYPE start, end;
	TUPTYPE next; 
	start = findEdge();
	used[TVAL(1,start)][TVAL(2,start)] = false;
	end = findEdge();
	if (TVAL(0,start) == ' ') // if the program calls solve twice
		return 1; // don't do anything, with error code
	if (TVAL(0,start) == '-' || TVAL(0,end) == '-') // if there are no valid endpoints
		return 2; // return different error code
	twine.push(start);
	next = neighbor (TVAL(1,start), TVAL(2,start) );
	do {
		if (TVAL(0,next) == '-') // if there's no neighbors
			twine.pop();
		else{
			twine.push(next);
			used[ TVAL(1,next) ][ TVAL(2,next) ] = false;
		}
		next = neighbor( TVAL(1,twine.top()), TVAL(2,twine.top()) );
	} while ( ! ( twine.empty() || 
				( TVAL(1, twine.top()) == TVAL(1,end) && 
				TVAL(2, twine.top() ) == TVAL(2,end) ) ) );
	// condition is NOT( A OR (B AND C) )
		
	// write stack solution to cell vector
	while (! twine.empty() ){
		cells[TVAL(1,twine.top())][TVAL(2,twine.top())] = 
			std::make_tuple (' ', TVAL(1,twine.top()), TVAL(2,twine.top()) );
		twine.pop();
	}

	return 0;
}

#endif
