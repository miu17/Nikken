#ifndef FACTORY_UNIFIEDDATA_H
#define FACTORY_UNIFIEDDATA_H

#include <QObject>

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
class UnifiedData;
enum class UnifiedDataType;

class POST3D_CLASS_DLLSPEC FactoryUnifiedData :QObject
{
    Q_OBJECT
public:
    static UnifiedData *getInstance(UnifiedDataType type);
};
} // namespace post3dapp

#endif // FACTORY_UNIFIEDDATA_H
