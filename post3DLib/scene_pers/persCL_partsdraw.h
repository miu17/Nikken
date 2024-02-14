#ifndef PERSCLPARTSDRAW_H
#define PERSCLPARTSDRAW_H

#include <QObject>
#include "define_unifieddata.h"
namespace post3dapp{
enum class LOADSUMTYPE;
enum class CALCOUTPUTDIR;
class JointData;
class MemberData;
class PersCalcLoadGLViewer;
class PersCalcLoadPartsDrawing : public QObject
{

    Q_OBJECT

public:

    PersCalcLoadPartsDrawing( QObject *parent = nullptr );

    static void drawLineOutput( MemberData *, LoadType::LOADTYPE, LOADSUMTYPE, CALCOUTPUTDIR );
    static void drawJointOutput( JointData *, LoadType::LOADTYPE, LOADSUMTYPE);

    static void drawColorBar( PersCalcLoadGLViewer *);

public slots:

signals:

private :

    static void drawLineLoad( MemberData *, const QList<JointData *> &, const QList<qreal> &,
                              LoadType::LOADTYPE, LOADSUMTYPE, CALCOUTPUTDIR );

    static QColor valueToColor( qreal, const QColor &, qreal, const QColor &, qreal );


};
} // namespace post3dapp
#endif


