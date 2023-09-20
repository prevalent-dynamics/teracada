#include <teracada_dict.h>


void unitTestsTeracadaDict ( void ) {
  tc_char acString[100] = {0};
  TeracadaDict objDict;

  objDict.update<tc_str>(strcpy(acString, "key1"), strcpy(acString, "value1"));
}


int main () {
  unitTestsTeracadaDict();
}