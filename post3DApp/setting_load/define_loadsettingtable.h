#ifndef LOADSETTINGDATATYPE_H
#define LOADSETTINGDATATYPE_H

#include "define_customtable.h"

/* 節点荷重 */
namespace post3dapp{
class JointLoadType : public CustomTableDataType
{

public:

    JointLoadType();
};


/* 部材荷重 */

class MemberLoadType : public CustomTableDataType
{

public:

    MemberLoadType();

};

/* 床荷重 */

class SlabLoadType : public CustomTableDataType
{

public:

    SlabLoadType();

};
} // namespace post3dapp

#endif
