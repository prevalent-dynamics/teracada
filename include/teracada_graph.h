#ifndef _RCORE_TERACADA_GRAPH_H
#define _RCORE_TERACADA_GRAPH_H


#include "teracada_common.h"
#include "teracada_array.h"
#include "teracada_matrix.h"


struct stTeraGraphNode {
  int stmNodeDataType;
  void *stmNodeValue;
};


class TeracadaGraph {
  private:


  public:
    TeracadaGraph ( void );
    ~TeracadaGraph ( void );

};

#endif