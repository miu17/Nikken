#ifndef PERSOUTPUTPARTSDRAW_H
#define PERSOUTPUTPARTSDRAW_H

#include <QObject>

namespace post3dapp{
enum class OUTFIGTYPE;
class JointData;
class MemberData;
class PersOutputGLViewer;

class PersOutputPartsDrawing : public QObject
{

    Q_OBJECT

public:

    PersOutputPartsDrawing( QObject *parent = nullptr );

    static void drawLineOutput( MemberData *, OUTFIGTYPE, int, int, int );
    static void drawJointOutput( JointData *, OUTFIGTYPE, int, int, int );

    static void drawColorBar( PersOutputGLViewer *, OUTFIGTYPE );

public slots:

signals:

private :

    static QColor valueToColor( qreal, const QColor &, qreal, const QColor &, qreal );


};
} // namespace post3dapp
#endif


