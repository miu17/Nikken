#include "factory_unifieddata.h"

#include "define_unifieddata.h"
#include "fixed_data.h"
#include "unified_analysisdata.h"
#include "unified_inputdata.h"

namespace post3dapp{

UnifiedData *FactoryUnifiedData::getInstance(UnifiedDataType type){
    switch(type){
    case UnifiedDataType::Pstn:
        return UnifiedFixedData::getInstance()->getAnalysisData();
    case UnifiedDataType::Input:
        return UnifiedInputData::getInInstance();
    case UnifiedDataType::FixedInput:
        return UnifiedFixedData::getInstance()->getInputData();
    default :
        qDebug()<<"UnifiedData::getInstance() invalid datatype";
        return nullptr;
    }
}

}// namespace post3dapp
