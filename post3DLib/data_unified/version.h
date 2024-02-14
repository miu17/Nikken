#ifndef VERSION_H
#define VERSION_H

#include <QVersionNumber>

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
class POST3D_CLASS_DLLSPEC Version
{
public:
    explicit Version(const QString&){}
    virtual QString text() const = 0;
protected:
    QVersionNumber version;

};

///
/// MajorVersion: 大規模な機能追加
/// MinorVersion: UnifiedDataの形式がかわったら
/// MicroVersion: 変更したら
///
class POST3D_CLASS_DLLSPEC LibraryVersion : public Version
{
public:
    explicit LibraryVersion(const QString& ver):Version(ver)
    {
        version = QVersionNumber::fromString(ver);
    }
    static LibraryVersion Current() {return LibraryVersion("1.0.0-Lib");}
    QString text() const override {return version.toString() + "-Lib";}
};


/*
class POST3D_CLASS_DLLSPEC SolverVersion : public Version
{
public:
    explicit SolverVersion(const QString& ver):Version(ver)
    {
        version = QVersionNumber::fromString(ver);
    }
    static SolverVersion Current() {return SolverVersion("1.0.0-Solver");}
    QString text() const override {return version.toString() + "-Solver";}
};
*/

} // namespace post3dapp
#endif // VERSION_H
