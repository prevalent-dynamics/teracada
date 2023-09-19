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

  TeracadaArray<tc_byte> objTeracadaArrayByte(5);
  objTeracadaArrayByte.disableExceptions();

  tc_byte aui8Array1[10] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
  objTeracadaArrayByte.insertBack(aui8Array1, 10);

  tc_byte aui8Array2[9] = {21, 22, 23, 24, 25, 26, 27, 28, 29};
  objTeracadaArrayByte.insert(3, aui8Array2, 9);

  objTeracadaArrayByte.insertBack(110);
  objTeracadaArrayByte.insertBack(120);
  objTeracadaArrayByte.insertBack(130);
  objTeracadaArrayByte.insertBack(140);
  objTeracadaArrayByte.insertBack(150);

  tc_byte aui8Array3[5] = {1, 2, 3, 4, 5};
  objTeracadaArrayByte.insertFront(aui8Array3, 5);

  tc_byte aui8Array4[5] = {151, 152, 153, 154, 155};
  objTeracadaArrayByte.insertBack(aui8Array4, 5);
  // objTeracadaArrayByte.print();

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

  TeracadaArray<tc_int> objTeracadaArrayInt(5);
  objTeracadaArrayInt.disableExceptions();

  tc_int aui8Array1[10] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
  objTeracadaArrayInt.insertBack(aui8Array1, 10);

  tc_int aui8Array2[9] = {21, 22, 23, 24, 25, 26, 27, 28, 29};
  objTeracadaArrayInt.insert(3, aui8Array2, 9);

  objTeracadaArrayInt.insertBack(110);
  objTeracadaArrayInt.insertBack(120);
  objTeracadaArrayInt.insertBack(130);
  objTeracadaArrayInt.insertBack(140);
  objTeracadaArrayInt.insertBack(150);

  tc_int aui8Array3[5] = {1, 2, 3, 4, 5};
  objTeracadaArrayInt.insertFront(aui8Array3, 5);

  tc_int aui8Array4[5] = {151, 152, 153, 154, 155};
  objTeracadaArrayInt.insertBack(aui8Array4, 5);

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

  TeracadaArray<tc_decimal> objTeracadaArrayDecimal(5);
  objTeracadaArrayDecimal.disableExceptions();

  tc_decimal aui8Array1[10] = { 10.1, 20.1, 30.1, 40.1, 50.1, 60.1, 70.1, 80.1, 90.1, 100.1 };
  objTeracadaArrayDecimal.insertBack(aui8Array1, 10);

  tc_decimal aui8Array2[9] = {21.2, 22.2, 23.2, 24.2, 25.2, 26.2, 27.2, 28.2, 29};
  objTeracadaArrayDecimal.insert(3, aui8Array2, 9);

  objTeracadaArrayDecimal.insertBack(110.3);
  objTeracadaArrayDecimal.insertBack(120.3);
  objTeracadaArrayDecimal.insertBack(130.3);
  objTeracadaArrayDecimal.insertBack(140.3);
  objTeracadaArrayDecimal.insertBack(150.3);

  tc_decimal aui8Array3[5] = {1.4, 2.4, 3.4, 4.4, 5.4};
  objTeracadaArrayDecimal.insertFront(aui8Array3, 5);

  tc_decimal aui8Array4[5] = {151.5, 152.5, 153.5, 154.5, 155.5};
  objTeracadaArrayDecimal.insertBack(aui8Array4, 5);

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

  TeracadaArray<tc_char> objTeracadaArrayChar(5);
  objTeracadaArrayChar.disableExceptions();

  objTeracadaArrayChar.insertBack("Insert names in this sentence.");

  assert(strcmp((char *) objTeracadaArrayChar.get(), "Insert names in this sentence.") == 0);

  objTeracadaArrayChar.insert(14, "(Johnny) ", 9);
  assert(strcmp((char *) objTeracadaArrayChar.get(), "Insert names (Johnny) in this sentence.") == 0);

  objTeracadaArrayChar.insert(21, ", John", 6);
  assert(strcmp((char *) objTeracadaArrayChar.get(), "Insert names (Johnny, John) in this sentence.") == 0);

  objTeracadaArrayChar.insert(27, ", Adam", 6);
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

  TeracadaArray<tc_str> objTeracadaArrayStr(2);
  objTeracadaArrayStr.disableExceptions();

  char acString1[100] = {0};
  char acString2[100] = {0};
  char acString3[100] = {0};
  char acString4[100] = {0};
  char acString5[100] = {0};


  objTeracadaArrayStr.insertBack(strcpy(acString1, "String number 1"));
  objTeracadaArrayStr.insertBack(strcpy(acString2, "String number 2"));
  objTeracadaArrayStr.insertBack(strcpy(acString3, "String number 3"));
  objTeracadaArrayStr.insertBack(strcpy(acString4, "String number 4"));
  objTeracadaArrayStr.insertBack(strcpy(acString5, "String number 5"));

  // Added 5 array elements till now
  assert(objTeracadaArrayStr.getNumElements() == 5);
  assert(strcmp((tc_str) objTeracadaArrayStr.get(1), "String number 1") == 0);
  assert(strcmp((tc_str) objTeracadaArrayStr.get(3), "String number 3") == 0);
  assert(strcmp((tc_str) objTeracadaArrayStr.get(5), "String number 5") == 0);
  assert(strcmp((tc_str) objTeracadaArrayStr.get(-1), "String number 5") == 0);
  assert(strcmp((tc_str) objTeracadaArrayStr.get(-2), "String number 4") == 0);

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

  TeracadaArray<tc_int> objTeracadaArrayInt(10);
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
