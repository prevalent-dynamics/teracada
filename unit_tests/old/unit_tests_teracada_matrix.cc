#include <iostream>

#include "teracada_matrix.h"

using namespace std;

void unitTestSimpleMatrix ( void ) {
  TeracadaMatrix objMatrix(TERACADA_DTYPE_INT, 5, 10);

  int row1 [] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int row2 [] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
  int row3 [] = {21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
  int row4 [] = {31, 32, 33, 34, 35, 36, 37, 38, 39, 40};
  int row5 [] = {41, 42, 43, 44, 45, 46, 47, 48, 49, 50};

  objMatrix.insertRow(row1, 10, 1, true);
  objMatrix.insertRow(row2, 10, 2, true);
  objMatrix.insertRow(row3, 10, 3, true);
  objMatrix.insertRow(row4, 10, 4, true);
  objMatrix.insertRow(row5, 10, 5, true);
  objMatrix.insertRow(row5, 10, 6, true);

  objMatrix.print();

  cout << *((int *) objMatrix(2, 4));
}


int main () {
  unitTestSimpleMatrix();
  return 0;
}
