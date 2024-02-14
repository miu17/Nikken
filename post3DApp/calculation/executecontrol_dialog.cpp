#include "executecontrol_dialog.h"

#include <QtGui>
#include <QtWidgets>

#include "calc_load.h"
#include "fixed_data.h"
#include "fixed_tabledata.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "unified_calcload.h"
#include "unified_editingdata.h"
#include "unified_inputdata.h"
#include "unified_project.h"
#include "utility.h"

namespace post3dapp{
ExecuteControlDialog::ExecuteControlDialog(QWidget *parent)
    : QDialog(parent)
{
    this->setFont(QFont(tr("Microsoft JhengHei"), 10));

    messageEditor = new QTextEdit(this);
    messageEditor->setFont(QFont(tr("Microsoft JhengHei"), 9));
    messageEditor->setReadOnly(true);
    messageEditor->setTextColor(QColor(30, 30, 30));
    messageEditor->setMinimumHeight(30);

    QPushButton *cancelButton = new QPushButton(u8"実行中止", this);
    QPushButton *okButton = new QPushButton("OK", this);
//    okButton->setEnabled(false);
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(cancelButton);
    hlay->addStretch();
    hlay->addWidget(okButton);

    connect( okButton, &QAbstractButton::clicked, this, &QDialog::accept );
    connect( cancelButton, &QAbstractButton::clicked, this, &ExecuteControlDialog::slotCancel );

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->setSpacing(5);
    vlay->addWidget(messageEditor);
    vlay->addLayout(hlay);
    this->setLayout(vlay);

    QPalette pal = this->palette();
    pal.setBrush(QPalette::Window, QBrush(Qt::white));
    this->setPalette(pal);
    this->setWindowTitle(u8"解析実行コントロール");
    this->setWindowFlags(Qt::Drawer);
    this->resize(QSize(400, 200));

    run();
}

void ExecuteControlDialog::run(){

    QString line = QDateTime::currentDateTime().toString("<hh:mm:ss> ");
    line += u8"荷重拾い実行中です...\n";
    messageEditor->setPlainText(line);
    messageEditor->moveCursor(QTextCursor::End);

    // ***_outputs/output_loads.csv へのファイル保存
    QString o_fpath = UnifiedEditingData::getInstance()->currentFileDir();
    QString o_dirname = UnifiedEditingData::getInstance()->currentFileBaseName() + "_outputs";
    if ( !QDir(o_fpath + "/" + o_dirname).exists() ) {
        if ( !QDir(o_fpath).mkdir(o_dirname) ) {
            line += u8"アウトプットをファイルに書き込めません。";
            messageEditor->setPlainText(line);
            return;
        }
    }
    o_fpath += "/" + o_dirname + "/output_loads.csv";

    QString errorMessage = UnifiedFixedData::getInstance()->getCalcLoadData()->executeCalculation(o_fpath, nullptr, true);
    if (errorMessage != QString{}){
        line += errorMessage;
        messageEditor->setPlainText(line);
        return;
    }
    else{
        line += u8"荷重拾いが完了しました。\n";
    }

    line += QDateTime::currentDateTime().toString("<hh:mm:ss> ");
    line += u8"形状データをファイルに変換しています...\n";
    messageEditor->setPlainText(line);
    messageEditor->moveCursor(QTextCursor::End);
    qDebug() << "makeIndex";
    makeIndexFile();

    // TODO データチェック
    errorMessage = writeNsv();
    if (errorMessage != QString{}){
        line += errorMessage;
        messageEditor->setPlainText(line);
        return;
    }

    line += QDateTime::currentDateTime().toString("<hh:mm:ss> ");
    line += u8"solver用データを出力しています...\n";
    messageEditor->setPlainText(line);
    messageEditor->moveCursor(QTextCursor::End);
    errorMessage = writeAnalysisDat();
    if (errorMessage != QString{}){
        line += errorMessage;
        messageEditor->setPlainText(line);
        return;
    }

    // 解析実行
    line += QDateTime::currentDateTime().toString("<hh:mm:ss> ");
    line += u8"終了\n";
    messageEditor->setPlainText(line);
    messageEditor->moveCursor(QTextCursor::End);

    //messageEditor->hide();
}

QString ExecuteControlDialog::writeNsv() const{
    QFile file(UnifiedInputData::getInInstance()->fileName().replace(".p3d", ".nsv"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        return u8"ファイル保存に失敗しました。";
    }

    QTextStream outfile(&file);
    auto table = UnifiedFixedData::getInstance()->getUuidTable();
    QString errorMessage = UnifiedFixedData::getInstance()->getInputData()->writeNsvData(outfile, table);
    file.close();
    return errorMessage;
}

QString ExecuteControlDialog::writeAnalysisDat() const{
    if (UnifiedProject::getInstance()->checkAnalysisCondition()){
        return u8"解析条件のフォーマットがおかしい";
    }

    QFile file(UnifiedInputData::getInInstance()->fileName().replace(".p3d", "_analysis.dat"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        return u8"ファイル保存に失敗しました。";
    }

    QTextStream outfile(&file);
    QString errorMessage = UnifiedProject::getInstance()->parseAnalysisCondition(outfile);
    file.close();
    return errorMessage;
}

void ExecuteControlDialog::closeEvent(QCloseEvent *event)
{
    if ( !isOkToClose() ) {
        event->ignore();
        return;
    }
    QDialog::closeEvent(event);
}

void ExecuteControlDialog::slotCancel()
{
    if ( isOkToClose() ) reject();
}

bool ExecuteControlDialog::isOkToClose()
{
    QMessageBox msg(this);
    msg.setWindowTitle(u8"中止の確認");
    msg.setText(u8"解析実行を中止してよろしいですか？");
    msg.setStandardButtons( QMessageBox::Yes | QMessageBox::Cancel );
    msg.setDefaultButton( QMessageBox::Yes );
    msg.button( QMessageBox::Yes )->setText(u8"中止");
    msg.button( QMessageBox::Cancel )->setText(u8"キャンセル");
    return ( msg.exec() == QMessageBox::Yes );
}

void ExecuteControlDialog::makeIndexFile()
{
    QList<JointData *> a_jdlist;
    QList<MemberData *> a_mdlist;

    Q_FOREACH ( JointData *jd, UnifiedInputData::getInInstance()->jointListOfAll() )
        if ( jd->isAnalysisJoint(*UnifiedInputData::getInInstance()) ) a_jdlist.append(jd);
    Q_FOREACH ( MemberData *md, UnifiedInputData::getInInstance()->memberListExceptLoad() )
        if ( md->isAnalysisMember(*UnifiedInputData::getInInstance()) ) a_mdlist.append(md);

    QString o_dirname = UnifiedEditingData::getInstance()->currentFileBaseName() + "_outputs";
    QString o_fpath = UnifiedEditingData::getInstance()->currentFileDir() + "/" + o_dirname + "/id_table.csv";

    QFile o_file(o_fpath);
    if ( !o_file.open(QIODevice::WriteOnly) ) return;
    QTextStream outfile(&o_file);
    outfile << a_jdlist.count() << "\n";
    for ( int i = 0; i < a_jdlist.count(); i++ ) {
        outfile << i + 1 << "," << a_jdlist.at(i)->getUuid().toString() << "\n";
    }
    outfile << a_mdlist.count() << "\n";
    for ( int i = 0; i < a_mdlist.count(); i++ ) {
        outfile << i + 1 << "," << a_mdlist.at(i)->getUuid().toString() << "\n";
    }

//    outfile << endl;

    for ( int i = 0; i < a_jdlist.count(); i++ ) {

        JointData *jd = a_jdlist.at(i);
        if ( !jd->getFirstFloor().isNull() ) {
            outfile << UnifiedInputData::getInInstance()->indexOfJoint(jd) + 1 << ","
                    << UnifiedInputData::getInInstance()->idToName(jd->getFirstFloor(), DATATYPE::TPFLOOR) << "\n" ;
            continue;
        }

        QList<JointData *> l_jdlist = jd->linkedAnalysisJoints(*UnifiedInputData::getInInstance());
        QList<JointData *> d_jdlist = l_jdlist;
        d_jdlist.append(jd);
        QUuid near_id;

        while ( l_jdlist.count() > 0 ) {
            QList<QUuid> j_idlist;
            Q_FOREACH ( JointData *jj, l_jdlist ) {
                QUuid j_id = jj->getFirstFloor();
                if ( !j_id.isNull() && !j_idlist.contains(j_id) ) j_idlist.append(j_id);
            }
            if ( j_idlist.count() > 0 ) {
                near_id = j_idlist.first();
                qreal nh = qAbs( jd->zPos() - UnifiedInputData::getInInstance()->getFloorHeight(j_idlist.first()) );
                for ( int j = 1; j < j_idlist.count(); j++ ) {
                    QUuid uid = j_idlist.at(j);
                    qreal hh = qAbs( jd->zPos() - UnifiedInputData::getInInstance()->getFloorHeight(uid) );
                    if ( hh < nh ) {
                        nh = hh;
                        near_id = uid;
                    }
                }
                break;
            }
            QList<JointData *> m_jdlist;
            Q_FOREACH ( JointData *jj, l_jdlist ) {
                Q_FOREACH ( JointData *j1, jj->linkedAnalysisJoints(*UnifiedInputData::getInInstance()) ) {
                    if ( !d_jdlist.contains(j1) ) {
                        d_jdlist.append(j1);
                        m_jdlist.append(j1);
                    }
                }
            }
            l_jdlist = m_jdlist;
        }

        if ( near_id.isNull() ) {
            outfile << UnifiedInputData::getInInstance()->indexOfJoint(jd) + 1 << ",NOT_FOUND\n";
        } else {
            outfile << UnifiedInputData::getInInstance()->indexOfJoint(jd) + 1 << ","
                    << UnifiedInputData::getInInstance()->idToName(near_id, DATATYPE::TPFLOOR) << "*\n" ;
        }
    }











    o_file.close();
}



QList<QUuid> ExecuteControlDialog::stringToFloorFrameRange( const QString &str, DATATYPE dtype )
{
    if ( dtype != DATATYPE::TPFLOOR && dtype != DATATYPE::TPFRAMEP ) return QList<QUuid>();

    // Range Judge  -> memberID
    // 1F/2F/3F/4F/5F/6F-8F/10F
    // split("/")
    //  -> name exist -> QUuid
    //  -> not exist -> split("-") -> count!=2 -> error
    //                                count==2 QUid_1 QUuid_2
    // F: QList<QUuid> r_floors
    // G1: QList<QUuid> r_frames1
    // G2: QList<QUuid> r_frames2

    QList<QUuid> idlist;

    QStringList words = str.split("/");

    bool isOk = true;
    Q_FOREACH ( QString wrd, words ) {

        if ( UnifiedInputData::getInInstance()->isExistedName(wrd, dtype) ) {
            idlist.append( UnifiedInputData::getInInstance()->nameToID(wrd, dtype) );
        } else {
            QStringList strlist = wrd.split("-");
            if ( strlist.count() != 2 ) {
                isOk = false;
                break;
            }
            if ( !strlist.at(0).isEmpty() &&
                    !UnifiedInputData::getInInstance()->isExistedName(strlist.at(0), dtype) ) {
                isOk = false;
                break;
            }
            if ( !strlist.at(1).isEmpty() &&
                    !UnifiedInputData::getInInstance()->isExistedName(strlist.at(1), dtype) ) {
                isOk = false;
                break;
            }
            QUuid id1 = UnifiedInputData::getInInstance()->nameToID(strlist.at(0), dtype);
            QUuid id2 = UnifiedInputData::getInInstance()->nameToID(strlist.at(1), dtype);

            if ( dtype == DATATYPE::TPFLOOR ) {
                QList<QUuid> ids = UnifiedInputData::getInInstance()->getFloorRange(id1, id2);
                if ( ids.count() == 0 ) {
                    isOk = false;
                    break;
                }
                idlist.append( ids );
            } else if ( dtype == DATATYPE::TPFRAMEP ) {
                QList<QUuid> ids = UnifiedInputData::getInInstance()->getFrameRange(id1, id2); // **** 未

                if ( ids.count() == 0 ) {
                    isOk = false;
                    break;
                }
                idlist.append( ids );
            }
        }

    }

    return idlist;

}
} // namespace post3dapp

