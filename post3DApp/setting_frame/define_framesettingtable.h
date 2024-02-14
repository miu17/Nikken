#ifndef FRAMESETTINGDATATYPE_H
#define FRAMESETTINGDATATYPE_H

#include "define_customtable.h"

/* 通りグループ定義　*/
namespace post3dapp{
class FrameDataType : public CustomTableDataType
{

public:

    FrameDataType();

};

/* 通り定義　*/

class FrameSubDataType : public CustomTableDataType
{

public:

    FrameSubDataType();
};


/* XY座標　*/

class FrameXYDataType : public CustomTableDataType
{

public:

    FrameXYDataType();
};
} // namespace post3dapp
#endif
