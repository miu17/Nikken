#include "inputload_tablewidget.h"

#include <QtGui>
#include <QBoxLayout>
#include <QDebug>
#include <QMenu>
#include <QSpinBox>
#include <QUndoStack>

#include "combobox_delegate.h"
#include "customtable_model.h"
#include "customtable_view.h"
#include "define_textinput.h"
#include "manage_memberdata.h"
#include "sortfilter_proxymodel.h"
#include "unified_inputdata.h"
#include "utility.h"

namespace post3dapp{
InputLoadTableWidget::InputLoadTableWidget( const CustomTableDataType &type, QWidget *parent )
    : CustomTableWidget( type, parent )
{
    myMenu = new QMenu(this);
    undoStack = new QUndoStack(this);

    createActions();
    createButtons();
    createModels();
    createViews();
    createWidgets();
    setFilterWidgetsVisible(true);
}

void InputLoadTableWidget::createModels()
{
    myTableModel = new CustomTableModel(myType, this);
    myProxyModel = new MySortFilterProxyModel(this);
    myProxyModel->setSourceModel(myTableModel);
    myProxyModel->setDynamicSortFilter(true);
    mySelectionModel = new QItemSelectionModel(myProxyModel);

    connect ( myTableModel, &CustomTableModel::rowCountChanged,
              this, &CustomTableWidget::slotChangeSpinBoxValue );
    connect ( myTableModel, &CustomTableModel::dataCountChanged,
              this, &CustomTableWidget::slotChangeSpinBoxMinimum );
    connect ( myTableModel, &CustomTableModel::itemEdited,
              this, &CustomTableWidget::slotEditItem );
}

void InputLoadTableWidget::createViews()
{
    myTableView = new CustomTableView(this);
    myTableView->setFirstColumn( myType.getTaggedColumn("TYPE") );
    myTableView->setLastColumn( myType.getTaggedColumn("UUID") );

    myTableView->addActions(myActionList);
    myTableView->setMenu(myMenu);

    myTableView->setModel(myProxyModel);
    myTableView->setSelectionModel(mySelectionModel);
    myTableView->setColumnWidth( myType.getTaggedColumn("NO"), 60);
    myTableView->setColumnWidth( myType.getTaggedColumn("TYPE"), 75);
    myTableView->setColumnWidth( myType.getTaggedColumn("NAME"), 75);
    myTableView->setColumnWidth( myType.getTaggedColumn("JOINTS"), 150);
    myTableView->setColumnWidth( myType.getTaggedColumn("FLOORS"), 75);
    myTableView->setColumnWidth( myType.getTaggedColumn("FRAMES"), 75);
    myTableView->setColumnWidth( myType.getTaggedColumn("UUID"), 120);

    QHash<int, QStringList> lmlist = myType.getLimitedStrings();
    QList<int> collist = lmlist.keys();
    Q_FOREACH (int col, collist) {
        ComboBoxDelegate *deleg = new ComboBoxDelegate(lmlist.value(col));
        myTableView->setItemDelegateForColumn(col, deleg);
    }

    connect ( myTableView, QOverload<>::of(&CustomTableView::delKeyClicked), this, &CustomTableWidget::del );
    connect ( myTableView, &CustomTableView::selectionBehaviorChanged,
              this, &CustomTableWidget::slotRowActionEnable );
}

void InputLoadTableWidget::createWidgets()
{
    mySpinBox->setMaximum(99999);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(0);

    vlay->addLayout(topLayout, 0);
    vlay->addWidget(myTableView, 1);
    this->setLayout(vlay);
}

void InputLoadTableWidget::slotDeleteLine()
{

    QModelIndexList rows_idxlist = mySelectionModel->selectedRows();
    bool isDisable = false;
    Q_FOREACH (QModelIndex idx, rows_idxlist) {
        if (myTableModel->isRegisterRow(idx.row())) isDisable = true;
    }
    if ( isDisable ) {
        if ( ! myTableModel->isEditOn() ) return;
        if ( warningLineDelete() ) return;
    }

    CustomTableWidget::slotDeleteLine();
}

void InputLoadTableWidget::importMemberData( const QList<MemberData *> &mdlist )
{
    myTableModel->initializeModel();
    mySpinBox->setValue(mdlist.count());
    for ( int i = 0; i < mdlist.count(); i++ ) {
        QList<QVariant> vlist;
        MemberData *md = mdlist.at(i);
        QStringList strlist;
        Q_FOREACH ( JointData *jd, md->getJointList() )
            strlist.append( QString("%1").arg(UnifiedInputData::getInInstance()->indexOfJoint(jd) + 1 ) );
        QString str_jd = strlist.join("-");
        QString floor = UnifiedInputData::getInInstance()->idToName(md->getAttachedFloor(), DATATYPE::TPFLOOR);
        QStringList frlist = UnifiedInputData::getInInstance()->idsToNames(md->getAttachedFrameList(), DATATYPE::TPFRAMEP);
        QString frames = frlist.join("/");
        vlist << QVariant()
              << QVariant( UnifiedInputData::getInInstance()->dataTypeToJapanese(md->dataType()) )
              << QVariant( md->sectionName() )
              << QVariant( str_jd )
              << QVariant( floor )
              << QVariant( frames )
              << QVariant( md->getUuid() );
        myTableModel->setRowDataDirect(i, vlist, false);
    }
    myTableModel->setDataCount();
    myTableModel->setEditOn(true);
    undoStack->clear();
}

QString InputLoadTableWidget::checkLoadData(int jcount) const
{
    QString msg;

    for (int i = 0; i < myTableModel->rowCount() ; i++) {

        QVariant var1 = myTableModel->getData(i, myType.getTaggedColumn("TYPE"));
        QVariant var2 = myTableModel->getData(i, myType.getTaggedColumn("NAME"));
        QVariant var3 = myTableModel->getData(i, myType.getTaggedColumn("JOINTS"));

        if ( var1.toString().isEmpty() ) {
            msg = QString("LOAD No.%1\n").arg(i + 1) + u8"荷重種別が未記入です";
            break;
        }

        DATATYPE dtype = UnifiedInputData::getInInstance()->stringToDataType( var1.toString() );
        QString name = var2.toString();
        QStringList nameList = UnifiedInputData::getInInstance()->getNameList(dtype);

        if ( !name.isEmpty() && !nameList.contains(name) ) {
            msg = QString("LOAD No.%1\n").arg(i + 1) + u8"対応する部材符号がありません";
            break;
        }

        QString jstr = var3.toString();
        if ( jstr.isEmpty() ) {
            msg = QString("LOAD No.%1\n").arg(i + 1) + u8"節点構成が無記入です";
            break;
        }

        QStringList jstrlist = jstr.split("-");
        QList<int> jnolist;
        bool ok1 = true, ok2 = true, ok3 = true;
        Q_FOREACH ( QString str, jstrlist ) {
            int jn = str.toInt(&ok1);
            if ( !ok1 ) break;
            jnolist.append(jn);
            if ( jn < 1 || jn > jcount ) {
                ok2 = false;
                break;
            }
            if ( jnolist.count(jn) > 1 ) {
                ok3 = false;
                break;
            }
        }
        if ( !ok1 ) {
            msg = QString("LOAD No.%1\n").arg(i + 1) + u8"節点構成に数値以外の文字があります";
            break;
        }
        if ( !ok2 ) {
            msg = QString("LOAD No.%1\n").arg(i + 1) + u8"節点構成が既存節点No.に対応していません";
            break;
        }
        if ( !ok3 ) {
            msg = QString("LOAD No.%1\n").arg(i + 1) + u8"同じ節点が構成に使われています";
            break;
        }
        if ( dtype == DATATYPE::TPJOINTLOAD ) {
            if ( jnolist.count() != 1 ) {
                msg = QString("LOAD No.%1\n").arg(i + 1) + u8"節点荷重は1点の節点を指定してください";
                break;
            }
        } else if ( dtype == DATATYPE::TPMEMBERLOAD ) {
            if ( jnolist.count() < 2 ) {
                msg = QString("LOAD No.%1\n").arg(i + 1) + u8"部材荷重は2点以上の節点を指定してください";
                break;
            }
        } else {
            if ( jnolist.count() < 3 ) {
                msg = QString("LOAD No.%1\n").arg(i + 1) + u8"床荷重は3点以上の節点を指定してください";
                break;
            }
        }

    }

    return msg;
}

void InputLoadTableWidget::uploadLoads()
{
    QList<DATATYPE> loadTypes;
    loadTypes << DATATYPE::TPJOINTLOAD << DATATYPE::TPMEMBERLOAD << DATATYPE::TPSLABLOAD;
    UnifiedInputData::getInInstance()->removeMembers( UnifiedInputData::getInInstance()->memberListOfType(
                                                   loadTypes) );

    QList<MemberData *> created_mdlist;

    for (int i = 0; i < myTableModel->rowCount() ; i++) {

        QString str1 = myTableModel->getData(i, myType.getTaggedColumn("TYPE")).toString();
        DATATYPE dtype = UnifiedInputData::getInInstance()->stringToDataType(str1);
        QString name = myTableModel->getData(i, myType.getTaggedColumn("NAME")).toString();

        QList<JointData *> jdlist;
        QString str2 = myTableModel->getData(i, myType.getTaggedColumn("JOINTS")).toString();
        QStringList strlist = str2.split("-");
        Q_FOREACH ( QString str, strlist ) {
            jdlist.append( UnifiedInputData::getInInstance()->jointIndexToPointer(str.toInt() - 1) );
        }

        QUuid uid = myTableModel->getData(i, myType.getTaggedColumn("UUID")).toString();

        MemberData *md = UnifiedInputData::getInInstance()->createMember(jdlist, dtype, name, uid);
        created_mdlist.append(md);

    }

    if ( created_mdlist.count() > 0 ) {
        UnifiedInputData::getInInstance()->signalMemberAppended(created_mdlist);
    }
}

bool InputLoadTableWidget::warningLineDelete()
{
    return Utility::showWarningMessage2(this, u8"登録済の荷重が選択されています。\n削除しますか？");
}

bool post3dapp::InputLoadTableWidget::readTextFile(QTextStream &infile, bool flg)
{
    if ( ! myTableModel->readTextData(infile, flg) ) return false;
    undoStack->clear();
    return true;
}

void InputLoadTableWidget::writeTextFile(QTextStream &outfile)
{
    QList<int> list;
    list.append( myType.getTaggedColumn("UUID") );
    myTableModel->writeTextData(outfile, list);
}

} // namespace post3dapp
