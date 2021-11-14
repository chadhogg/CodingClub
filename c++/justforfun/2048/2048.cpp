/// \file 2048.cpp
/// \author Chad Hogg
/// \brief A solution to https://open.kattis.com/problems/2048

#include <iostream>

/// The length and width of the grid.
const unsigned int GRID_SIZE {4U};
/// The value that represents an empty square on the grid.
const unsigned int EMPTY {0U};
/// The number of directions that exist.
const unsigned int NUM_DIRECTIONS {4U};

/// \brief Reads the grid values from an input stream.
/// \param[inout] is The input stream to read from.
/// \param[out] grid The grid that should be filled with values.
/// \post GRID_SIZE * GRID_SIZE unsigned integers have been extracted from the
///   stream and placed into the grid.
static void initGridFromInput (std::istream& is, unsigned int (&grid)[GRID_SIZE][GRID_SIZE]) {
  for (unsigned int i = 0U; i < GRID_SIZE; ++i) {
    for (unsigned int j = 0U; j < GRID_SIZE; ++j) {
      is >> grid [i][j];
    }
  }
}

/// \brief Writes the grid values to an output stream.
/// \param[inout] os The output stream to write to.
/// \param[in] grid The grid that should be printed.
/// \post Each value from the grid has been inserted into the stream, using
///   spaces and newlines as separators.
static void printGrid (std::ostream& os, const unsigned int (&grid)[GRID_SIZE][GRID_SIZE]) {
  for (unsigned int i = 0U; i < GRID_SIZE; ++i) {
    for (unsigned int j = 0U; j < GRID_SIZE; ++j) {
      os << grid[i][j];
      if (j < GRID_SIZE - 1) {
	os << ' ';
      }
      else {
	os << '\n';
      }
    }
  }
  os.flush ();
}

/// \brief Rotates a 4x4 matrix counter-clockwise multiple times.
/// \param[inout] grid The matrix to rotate.
/// \param[in] times How many times to rotate.
/// \post The matrix has been rotated that many times.
/// \note This could certainly be optimized, but it seems unnecessary.
static void rotateCCW (unsigned int (&grid)[GRID_SIZE][GRID_SIZE], unsigned int times) {
  for (unsigned int x = 0U; x < times % NUM_DIRECTIONS; ++x) {
    unsigned int copy[GRID_SIZE][GRID_SIZE];
    std::copy (&grid[0][0], &grid[0][0] + GRID_SIZE * GRID_SIZE, &copy[0][0]);
    for (unsigned int i = 0U; i < GRID_SIZE; ++i) {
      for (unsigned int j = 0U; j < GRID_SIZE; ++j) {
	grid[i][j] = copy[j][GRID_SIZE - 1 - i];
      }
    }
  }
}

/// \brief Does a leftward update step of the 2048 game.
/// \param[inout] grid The matrix that should be modified.
/// \post Each number has been moved leftward through any empty locations.  If
///   it ended up next to a tile with the same number on it, they were combined
///   into a single tile.  The result of a combination is not further combined.
/// \note If you rotate the matrix first, and reverse the rotation afterward
///   you can simulate moving in any other cardinal direction with this.
static void moveLeft (unsigned int (&grid)[GRID_SIZE][GRID_SIZE]) {
  bool merged[GRID_SIZE][GRID_SIZE] {};
  
  for (unsigned int i = 0U; i < GRID_SIZE; ++i) {
    for (unsigned int j = 1U; j < GRID_SIZE; ++j) {
      unsigned int toSlide {j};
      while (toSlide > 0U && grid[i][toSlide] != EMPTY && grid[i][toSlide - 1] == EMPTY) {
	grid[i][toSlide - 1] = grid[i][toSlide];
	grid[i][toSlide] = EMPTY;
	--toSlide;
      }
      if (toSlide > 0U && grid[i][toSlide] == grid[i][toSlide - 1] && !merged[i][toSlide - 1]) {
	grid[i][toSlide - 1] <<= 1;
	grid[i][toSlide] = EMPTY;
	merged[i][toSlide - 1] = true;
      }
    }
  }
}

/// \brief Runs the program.
/// \return Always 0, no matter what.
int main () {
  unsigned int grid[GRID_SIZE][GRID_SIZE];
  initGridFromInput (std::cin, grid);
  unsigned int direction;
  std::cin >> direction;
  
  rotateCCW (grid, direction);
  moveLeft (grid);
  rotateCCW (grid, (NUM_DIRECTIONS - direction));

  printGrid (std::cout, grid);
  return 0;
}
