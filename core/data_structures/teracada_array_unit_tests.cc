#include <iostream>

#include "teracada.h"

extern "C" {
#include <assert.h>
}

static char acTERACADA_ERRSTR_BUFFER[TC_ERRORSTR_LENGTH];
char* pcTERACADA_ERRSTR = &acTERACADA_ERRSTR_BUFFER[0];


using namespace std;

void UnitTestsTeracadaArrayTypeByte ( void ) {

  cout << ">>> Unit testing TeracadaArray (TC_BYTE): ";

  TeracadaArray objTeracadaArrayByte(TC_BYTE, 5);
  tc_byte aui8Array1[10] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
  objTeracadaArrayByte.insert<tc_byte>(aui8Array1, 10);

  tc_byte aui8Array2[9] = {21, 22, 23, 24, 25, 26, 27, 28, 29};
  objTeracadaArrayByte.insert<tc_byte>(aui8Array2, 9, 3);

  objTeracadaArrayByte.insert<tc_byte>(110);
  objTeracadaArrayByte.insert<tc_byte>(120);
  objTeracadaArrayByte.insert<tc_byte>(130);
  objTeracadaArrayByte.insert<tc_byte>(140);
  objTeracadaArrayByte.insert<tc_byte>(150);

  tc_byte aui8Array3[5] = {1, 2, 3, 4, 5};
  objTeracadaArrayByte.insert<tc_byte>(aui8Array3, 5, 1);

  tc_byte aui8Array4[5] = {151, 152, 153, 154, 155};
  objTeracadaArrayByte.insert<tc_byte>(aui8Array4, 5);

  // Added total 34 elements to the array till now
  assert(objTeracadaArrayByte.getNumElements() == 34);
  assert(*(tc_byte*) objTeracadaArrayByte.get(1) == 1);
  assert(*(tc_byte*) objTeracadaArrayByte.get(-1) == 155);

  objTeracadaArrayByte.remove(2, 2);
  objTeracadaArrayByte.remove(1, 2);
  objTeracadaArrayByte.remove(-3, 2);

  // Try to remove array element out of bound, should be rejected
  objTeracadaArrayByte.remove(-1, 2);

  // Removed 8 elements from the array till now (34 - 6 = 28)
  assert(objTeracadaArrayByte.getNumElements() == 28);
  assert(*(tc_byte*) objTeracadaArrayByte.get(1) == 5);
  assert(*(tc_byte*) objTeracadaArrayByte.get(-1) == 155);

  cout << "(Passed)";

  return;
}


void UnitTestsTeracadaArrayTypeInt ( void ) {

  cout << ">>> Unit testing TeracadaArray [TC_INT]: ";

  TeracadaArray objTeracadaArrayInt(TC_INT, 5);
  tc_int aui8Array1[10] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
  objTeracadaArrayInt.insert<tc_int>(aui8Array1, 10);

  tc_int aui8Array2[9] = {21, 22, 23, 24, 25, 26, 27, 28, 29};
  objTeracadaArrayInt.insert<tc_int>(aui8Array2, 9, 3);

  objTeracadaArrayInt.insert<tc_int>(110);
  objTeracadaArrayInt.insert<tc_int>(120);
  objTeracadaArrayInt.insert<tc_int>(130);
  objTeracadaArrayInt.insert<tc_int>(140);
  objTeracadaArrayInt.insert<tc_int>(150);

  tc_int aui8Array3[5] = {1, 2, 3, 4, 5};
  objTeracadaArrayInt.insert<tc_int>(aui8Array3, 5, 1);

  tc_int aui8Array4[5] = {151, 152, 153, 154, 155};
  objTeracadaArrayInt.insert<tc_int>(aui8Array4, 5);

  // Added total 34 elements to the array till now
  assert(objTeracadaArrayInt.getNumElements() == 34);
  assert(*(tc_int*) objTeracadaArrayInt.get(1) == 1);
  assert(*(tc_int*) objTeracadaArrayInt.get(-1) == 155);

  objTeracadaArrayInt.remove(2, 2);
  objTeracadaArrayInt.remove(1, 2);
  objTeracadaArrayInt.remove(-3, 2);

  // Try to remove array element out of bound, should be rejected
  objTeracadaArrayInt.remove(-1, 2);

  // Removed 8 elements from the array till now (34 - 6 = 28)
  assert(objTeracadaArrayInt.getNumElements() == 28);
  assert(*(tc_int*) objTeracadaArrayInt.get(1) == 5);
  assert(*(tc_int*) objTeracadaArrayInt.get(-1) == 155);

  cout << "(Passed)";

  return;
}


void UnitTestsTeracadaArrayTypeDecimal ( void ) {

  cout << ">>> Unit testing TeracadaArray [TC_DECIMAL]: ";

  TeracadaArray objTeracadaArrayDecimal(TC_DECIMAL, 5);
  tc_decimal aui8Array1[10] = { 10.1, 20.1, 30.1, 40.1, 50.1, 60.1, 70.1, 80.1, 90.1, 100.1 };
  objTeracadaArrayDecimal.insert<tc_decimal>(aui8Array1, 10);

  tc_decimal aui8Array2[9] = {21.2, 22.2, 23.2, 24.2, 25.2, 26.2, 27.2, 28.2, 29};
  objTeracadaArrayDecimal.insert<tc_decimal>(aui8Array2, 9, 3);

  objTeracadaArrayDecimal.insert<tc_decimal>(110.3);
  objTeracadaArrayDecimal.insert<tc_decimal>(120.3);
  objTeracadaArrayDecimal.insert<tc_decimal>(130.3);
  objTeracadaArrayDecimal.insert<tc_decimal>(140.3);
  objTeracadaArrayDecimal.insert<tc_decimal>(150.3);

  tc_decimal aui8Array3[5] = {1.4, 2.4, 3.4, 4.4, 5.4};
  objTeracadaArrayDecimal.insert<tc_decimal>(aui8Array3, 5, 1);

  tc_decimal aui8Array4[5] = {151.5, 152.5, 153.5, 154.5, 155.5};
  objTeracadaArrayDecimal.insert<tc_decimal>(aui8Array4, 5);

  // Added total 34 elements to the array till now
  assert(objTeracadaArrayDecimal.getNumElements() == 34);
  assert(*((tc_decimal*) objTeracadaArrayDecimal.get(1)) - 1.4 < 0.0001);
  assert(*((tc_decimal*) objTeracadaArrayDecimal.get(-1)) - 155.5 < 0.0001);

  objTeracadaArrayDecimal.remove(2, 2);
  objTeracadaArrayDecimal.remove(1, 2);
  objTeracadaArrayDecimal.remove(-3, 2);

  // Try to remove array element out of bound, should be rejected
  objTeracadaArrayDecimal.remove(-1, 2);

  // Removed 8 elements from the array till now (34 - 6 = 28)
  assert(objTeracadaArrayDecimal.getNumElements() == 28);
  assert(*(tc_decimal*) objTeracadaArrayDecimal.get(1) - 5.4 < 0.0001);
  assert(*(tc_decimal*) objTeracadaArrayDecimal.get(-1) - 155.5 < 0.0001);
  cout << "(Passed)";

  return;
}


void UnitTestsTeracadaArrayTypeChar () {

  cout << ">>> Unit testing TeracadaArray [TC_CHAR]: ";

  TeracadaArray objTeracadaArrayChar(TC_CHAR, 5);

  objTeracadaArrayChar.insert<tc_char>("Insert names in this sentence.");

  assert(strcmp((char *) objTeracadaArrayChar.get(), "Insert names in this sentence.") == 0);

  objTeracadaArrayChar.insert<tc_char>("(Johnny) ", 9, 14);
  assert(strcmp((char *) objTeracadaArrayChar.get(), "Insert names (Johnny) in this sentence.") == 0);

  objTeracadaArrayChar.insert<tc_char>(", John", 6, 21);
  assert(strcmp((char *) objTeracadaArrayChar.get(), "Insert names (Johnny, John) in this sentence.") == 0);

  objTeracadaArrayChar.insert<tc_char>(", Adam", 6, 27);
  assert(strcmp((char *) objTeracadaArrayChar.get(), "Insert names (Johnny, John, Adam) in this sentence.") == 0);

  // Added 51 array elements till now
  assert(objTeracadaArrayChar.getNumElements() == 51);

  objTeracadaArrayChar.remove(27, 6);
  assert(strcmp((char *) objTeracadaArrayChar.get(), "Insert names (Johnny, John) in this sentence.") == 0);
  assert(objTeracadaArrayChar.getNumElements() == 45);

  objTeracadaArrayChar.remove(14, 15);
  assert(strcmp((char *) objTeracadaArrayChar.get(), "Insert names in this sentence.") == 0);

  // Removed 21 array elements till now (51 - 21 = 30)
  assert(objTeracadaArrayChar.getNumElements() == 30);

  cout << "(Passed)";

  return;
}


void UnitTestsTeracadaArrayTypeString ( void ) {

  cout << ">>> Unit testing TeracadaArray [TC_STRING]: ";

  TeracadaArray objTeracadaArrayStr(TC_STRING, 2);

  objTeracadaArrayStr.insert("String number 1");
  objTeracadaArrayStr.insert("String number 2");
  objTeracadaArrayStr.insert("String number 3");
  objTeracadaArrayStr.insert("String number 4");
  objTeracadaArrayStr.insert("String number 5");

  // Added 5 array elements till now
  assert(objTeracadaArrayStr.getNumElements() == 5);
  assert(strcmp((char *) objTeracadaArrayStr.get(1), "String number 1") == 0);
  assert(strcmp((char *) objTeracadaArrayStr.get(3), "String number 3") == 0);
  assert(strcmp((char *) objTeracadaArrayStr.get(5), "String number 5") == 0);
  assert(strcmp((char *) objTeracadaArrayStr.get(-1), "String number 5") == 0);
  assert(strcmp((char *) objTeracadaArrayStr.get(-2), "String number 4") == 0);

  objTeracadaArrayStr.remove(3, 3);

  // Removed three array elements till now
  assert(objTeracadaArrayStr.getNumElements() == 2);
  assert(strcmp((char *) objTeracadaArrayStr.get(1), "String number 1") == 0);
  assert(strcmp((char *) objTeracadaArrayStr.get(2), "String number 2") == 0);

  // Try to remove array elements out of bound, should be rejected
  objTeracadaArrayStr.remove(-1, 9);
  assert(objTeracadaArrayStr.getNumElements() == 2);

  cout << "(Passed)";

  return;
}


int main () {
  cout << endl << endl;

  TeracadaArray objTeracadaArrayInt(TC_INT, 10);
  cout << "Size of TeracadaArray object [ TYPE: TC_INT | NUM_ELEMENTS: 10 ]: " << sizeof(objTeracadaArrayInt);

  cout << endl << endl;

  UnitTestsTeracadaArrayTypeByte();
  cout << endl << endl;

  UnitTestsTeracadaArrayTypeInt();
  cout << endl << endl;

  UnitTestsTeracadaArrayTypeDecimal();
  cout << endl << endl;

  UnitTestsTeracadaArrayTypeChar();
  cout << endl << endl;

  UnitTestsTeracadaArrayTypeString();
  cout << endl << endl;

  return 0;
}
