#include "textcalcload_dialog.h"

#include <QtGui>
#include <QtWidgets>

#include "define_output.h"
#include "fixed_data.h"
#include "manage_memberdata.h"
#include "unified_inputdata.h"

namespace post3dapp{
TextCalcLoadDialog::TextCalcLoadDialog(QWidget *parent)
    : QDialog(parent)
{
    createLayout();

    setFont(QFont(QString::fromLocal8Bit("Microsoft JhengHei"), 9));
    setWindowTitle(u8"解析結果 テキスト表示");
    setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
}

void TextCalcLoadDialog::createLayout()
{
    textEditor = new QTextEdit(this);

    QPushButton *okButton = new QPushButton("O K", this);
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addStretch();
    hlay->addWidget(okButton);

    connect( okButton, &QAbstractButton::clicked, this, &QDialog::reject );

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->setSpacing(5);
    vlay->addWidget(textEditor, 1);
    vlay->addLayout(hlay, 0);
    this->setLayout(vlay);
}

void TextCalcLoadDialog::setEditorText(const QList<JointData *> &jdlist, const QList<MemberData *> &mdlist)
{
    textEditor->clear();
    textEditor->setPlainText(getLoadOutput(jdlist, mdlist));
}

QString TextCalcLoadDialog::getLoadOutput(const QList<JointData *> &jdlist, const QList<MemberData *> &mdlist) const
{
    QString str;

    Q_FOREACH ( JointData *jd, jdlist) {
        if ( jd == nullptr ) continue;
        int jno = UnifiedFixedData::getInstance()->getInputData()->indexOfJoint(jd) + 1;
        Q_FOREACH ( JLOADSUM *jload, UnifiedFixedData::getInstance()->getCalcLoadData()->loadOfJoint( jd) ) {
            str += QString("$JOINT,No.%1,").arg(jno);
            str += LoadType::enumToString(jload->loadType) + ",";
            str += ( jload->loadName.isEmpty() ) ? "(auto)" : jload->loadName;
            str += ",";
            switch ( jload->loadDir ) {
            case LOADDIR::LDIR_X:
                str += QString("%1kN(X)").arg(jload->concentratedLoad, 0, 'f', 2);
                break;
            case LOADDIR::LDIR_Y:
                str += QString("%1kN(Y)").arg(jload->concentratedLoad, 0, 'f', 2);
                break;
            case LOADDIR::LDIR_Z:
                str += QString("%1kN(Z)").arg(jload->concentratedLoad, 0, 'f', 2);
                break;
            case LOADDIR::LDIR_MX:
                str += QString("%1kN*m(MX)").arg(jload->concentratedLoad, 0, 'f', 2);
                break;
            case LOADDIR::LDIR_MY:
                str += QString("%1kN*m(MY)").arg(jload->concentratedLoad, 0, 'f', 2);
            case LOADDIR::LDIR_MZ:
                str += QString("%1kN*m(MZ)").arg(jload->concentratedLoad, 0, 'f', 2);
                break;
            default:
                break;
            }
            str += "\n";
        }
    }

    Q_FOREACH ( MemberData *md, mdlist) {
        if ( md == nullptr ) continue;
        QString str1;
        str1 += "$" + UnifiedFixedData::getInstance()->getInputData()->dataTypeToEnglish(md->dataType()) + ",";
        str1 += md->sectionName() + ",";
        int ino = UnifiedFixedData::getInstance()->getInputData()->indexOfJoint(md->iJoint()) + 1;
        int jno = UnifiedFixedData::getInstance()->getInputData()->indexOfJoint(md->jJoint()) + 1;
        str1 += QString("iNo.%1,jNo.%2,").arg(ino).arg(jno);

        Q_FOREACH ( MLOADSUM *mload, UnifiedFixedData::getInstance()->getCalcLoadData()->loadOfMember( md) ) {

            QString str2;
            str2 += str1;
            str2 += LoadType::enumToString(mload->loadType) + ",";
            str2 += ( mload->loadName.isEmpty() ) ? "(auto)" : mload->loadName;
            str2 += ",";
            bool isMoment = false;
            switch ( mload->loadDir ) {
            case LOADDIR::LDIR_X:
                str2 += QString("x-dir,");
                break;
            case LOADDIR::LDIR_Y:
                str2 += QString("y-dir,");
                break;
            case LOADDIR::LDIR_Z:
                str2 += QString("z-dir,");
                break;
            case LOADDIR::LDIR_MX:
                str2 += QString("mx-dir,");
                isMoment = true;
                break;
            case LOADDIR::LDIR_MY:
                str2 += QString("my-dir,");
                isMoment = true;
                break;
            case LOADDIR::LDIR_MZ:
                str2 += QString("mz-dir,");
                isMoment = true;
                break;
            default:
                break;
            }

            if ( !mload->sumUpLoad.isEmpty() ) {
                str += str2;
                str += ( isMoment ) ? "SumUp(kN*m)," : "SumUp(kN)," ;
                str += "(";
                Q_FOREACH ( qreal vv, mload->sumUpLoad ) {
                    str += QString("%1,").arg(vv, 0, 'f', 2);
                }
                str.remove(str.count() - 1, 1);
                str += "),";
                str += "\n";
            }
            if ( !mload->concentratedLoad.isEmpty() ) {
                str += str2;
                str += ( isMoment ) ? "Concentrated(kN*m)," : "Concentrated(kN)," ;
                Q_FOREACH ( QPointF pp, mload->concentratedLoad ) {
                    str += QString("p(%1,%2),").arg(pp.x(), 0, 'f', 2).arg(pp.y(), 0, 'f', 2);
                }
                str += "\n";
            }
            if (  qAbs(mload->loadWAB.loadWA.y()) > 1.0e-3
                    || qAbs(mload->loadWAB.loadWB.y()) > 1.0e-3 ) {
                str += str2;
                str += ( isMoment ) ? "Distributed(kN*m)," : "Distributed(kN)," ;
                str += QString("wA(%1,%2),").arg(mload->loadWAB.loadWA.x(), 0, 'f', 2)
                       .arg(mload->loadWAB.loadWA.y(), 0, 'f', 2);

                str += QString("wB(%1,%2),").arg(mload->loadWAB.loadWB.x(), 0, 'f', 2)
                       .arg(mload->loadWAB.loadWB.y(), 0, 'f', 2);
                str += "\n";
            }
        }
    }

    return str;

}
} // namespace post3dapp
