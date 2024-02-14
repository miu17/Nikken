#include <QtGui>
#include <QDebug>

#include "inputjoint_tablewidget.h"

InputJointTableWidget::InputJointTableWidget( const CustomTableDataType &type, QWidget* parent )
    : CustomTableWidget( type, parent )
{
    myMenu=new QMenu(this);
    undoStack = new QUndoStack(this);

    createActions();
    createButtons();
    createModels();
    createViews();
    createWidgets();
    setFilterWidgetsVisible(true);
}

void InputJointTableWidget::createModels()
{
    myTableModel = new CustomTableModel(myType,this);
    myProxyModel = new MySortFilterProxyModel(this);
    myProxyModel->setSourceModel(myTableModel);
    myProxyModel->setDynamicSortFilter(true);
    mySelectionModel=new QItemSelectionModel(myProxyModel);

    connect ( myTableModel, SIGNAL( rowCountChanged(int) ),
              this, SLOT( slotChangeSpinBoxValue(int) ) );
    connect ( myTableModel, SIGNAL( dataCountChanged(int) ),
              this, SLOT( slotChangeSpinBoxMinimum(int) ) );
    connect ( myTableModel, SIGNAL( itemEdited(const QModelIndex&,const QVariant&) ),
              this, SLOT( slotEditItem(const QModelIndex&,const QVariant&) ) );
}

void InputJointTableWidget::createViews()
{
    myTableView=new CustomTableView(this);
    myTableView->setFirstColumn( myType.getTaggedColumn("X") );
    myTableView->setLastColumn( myType.getTaggedColumn("UUID") );

    myTableView->addActions(myActionList);
    myTableView->setMenu(myMenu);

    myTableView->setModel(myProxyModel);
    myTableView->setSelectionModel(mySelectionModel);
    myTableView->setColumnWidth( myType.getTaggedColumn("NO"),60);
    myTableView->setColumnWidth( myType.getTaggedColumn("X"),75);
    myTableView->setColumnWidth( myType.getTaggedColumn("Y"),75);
    myTableView->setColumnWidth( myType.getTaggedColumn("Z"),75);
    myTableView->setColumnWidth( myType.getTaggedColumn("FLOORS"),75);
    myTableView->setColumnWidth( myType.getTaggedColumn("FRAMES"),150);
    myTableView->setColumnWidth( myType.getTaggedColumn("UUID"),120);

    connect ( myTableView, SIGNAL( delKeyClicked() ), this, SLOT( del() ) );
    connect ( myTableView, SIGNAL( selectionBehaviorChanged(bool)),
              this, SLOT( slotRowActionEnable(bool) ) );
}

void InputJointTableWidget::createWidgets()
{
    mySpinBox->setMaximum(99999);

    QVBoxLayout *vlay=new QVBoxLayout;
    vlay->setContentsMargins(0,0,0,0);
    vlay->setSpacing(0);

    vlay->addLayout(topLayout,0);
    vlay->addWidget(myTableView,1);
    this->setLayout(vlay);
}

void InputJointTableWidget::slotDeleteLine()
{
    QModelIndexList rows_idxlist=mySelectionModel->selectedRows();
    bool isDisable=false;
    Q_FOREACH( QModelIndex idx, rows_idxlist ){
        if( myTableModel->isRegisterRow(idx.row()) ) isDisable=true;
    }
    if( isDisable ){
        if( !myTableModel->isEditOn() ) return;
        if( warningLineDelete() ) return;
    }

    CustomTableWidget::slotDeleteLine();
}

void InputJointTableWidget::importJointData( const QList<JointData*> &jdlist )
{
    myTableModel->initializeModel();
    mySpinBox->setValue(jdlist.count());
    for( int i=0; i<jdlist.count(); i++ ){
        QList<QVariant> vlist;
        JointData* jd=jdlist.at(i);
        QString str_fl,str_fr;
        QStringList strlist=UnifiedData::getInstance()->idsToNames(jd->getAttachedFloorList(),TPFLOOR);
        str_fl=strlist.join("/");
        strlist=UnifiedData::getInstance()->idsToNames(jd->getAttachedFrameList(),TPFRAMEP);
        str_fr=strlist.join("/");
        vlist << QVariant() << QVariant(jd->xPos()) << QVariant(jd->yPos()) << QVariant(jd->zPos())
              << QVariant(str_fl) << QVariant(str_fr) << QVariant(jd->getUuid());
        myTableModel->setRowDataDirect(i,vlist,false);
    }
    myTableModel->setDataCount();
    myTableModel->setEditOn(true);
    undoStack->clear();
}

QString InputJointTableWidget::checkJointData() const
{
    QString msg;

    for(int i=0; i < myTableModel->rowCount() ; i++){

        QVariant var1=myTableModel->getData(i,myType.getTaggedColumn("FLOORS"));
        QVariant var2=myTableModel->getData(i,myType.getTaggedColumn("FRAMES"));

        QVariant var3=myTableModel->getData(i,myType.getTaggedColumn("X"));
        QVariant var4=myTableModel->getData(i,myType.getTaggedColumn("Y"));
        QVariant var5=myTableModel->getData(i,myType.getTaggedColumn("Z"));
        if( var3.toString().isEmpty() || var4.toString().isEmpty() || var5.toString().isEmpty() ){
            msg=QString("JOINT No.%1\n").arg(i+1) + tr("Please enter a numeric value in the coordinate field");
            break;
        }

        QStringList fllist=var1.toString().split("/");
        QStringList frlist=var2.toString().split("/");
        if( var1.toString().isEmpty() ) fllist.clear();
        if( var2.toString().isEmpty() ) frlist.clear();

        Q_FOREACH( QString fn, fllist ){
            if( fllist.count(fn)>1 || !UnifiedData::getInstance()->isExistedName(fn,TPFLOOR) ){
                msg=QString("JOINT No.%1\n").arg(i+1) + tr("The belonging floor name is illegal") + " - " +fn;
                break;
            }
        }
        if( !msg.isEmpty() ) break;

        Q_FOREACH( QString fn, frlist ){
            if( frlist.count(fn)>1 || !UnifiedData::getInstance()->isExistedName(fn,TPFRAMEP) ){
                msg=QString("JOINT No.%1\n").arg(i+1) + tr("Affiliation street name is illegal") + " - " +fn;
                break;
            }
        }
        if( !msg.isEmpty() ) break;
    }

    return msg;
}

void InputJointTableWidget::uploadJoints()
{
    QList<QUuid> cj_idlist,nj_idlist; // 変更される節点,新規に作られる節点
    QList<int> cj_indexes,nj_indexes;
    QList<GlobalPoint> cj_gplist,nj_gplist;

    for(int i=0; i < myTableModel->rowCount() ; i++){
        QUuid id=myTableModel->getData(i,myType.getTaggedColumn("UUID")).toString();
        qreal xx=myTableModel->getData(i,myType.getTaggedColumn("X")).toDouble();
        qreal yy=myTableModel->getData(i,myType.getTaggedColumn("Y")).toDouble();
        qreal zz=myTableModel->getData(i,myType.getTaggedColumn("Z")).toDouble();
        QString str_fl=myTableModel->getData(i,myType.getTaggedColumn("FLOORS")).toString();
        QString str_fr=myTableModel->getData(i,myType.getTaggedColumn("FRAMES")).toString();
        QStringList fllist=( str_fl.isEmpty() ) ? QStringList() : str_fl.split("/");
        QStringList frlist=( str_fr.isEmpty() ) ? QStringList() : str_fr.split("/");
        QList<QUuid> flidlist=UnifiedData::getInstance()->namesToIDs(fllist,TPFLOOR);
        QList<QUuid> fridlist=UnifiedData::getInstance()->namesToIDs(frlist,TPFRAMEP);
        GlobalPoint gp(xx,yy,zz);
        if( !flidlist.isEmpty() ) gp.appendAttachedFloor(flidlist);
        if( !fridlist.isEmpty() ) gp.appendAttachedFrame(fridlist);

        if( id.isNull() || !UnifiedData::getInstance()->isExistedID(id,TPJOINT) ){
            nj_idlist.append(id);
            nj_indexes.append(i);
            nj_gplist.append(gp);
        }else{
            cj_idlist.append(id);
            cj_indexes.append(i);
            cj_gplist.append(gp);
        }
    }

    QList<JointData*> all_jdlist=UnifiedData::getInstance()->jointListOfAll();
    QList<JointData*> remove_jdlist,change_jdlist;

    Q_FOREACH( JointData* jd, all_jdlist ){
        ( cj_idlist.contains( jd->getUuid() ) ) ? change_jdlist.append(jd) : remove_jdlist.append(jd);
    }

    // changed_jdlist のソート テーブルに対応
    for( int i=0; i<cj_idlist.count(); i++ ){
        QUuid id=cj_idlist.at(i);
        int idx=-1;
        for( int j=i; j<change_jdlist.count(); j++ ){
            if( id==change_jdlist.at(j)->getUuid() ){ idx=j; break; }
        }
        if( idx<0 ) break;
        change_jdlist.move(idx,i);
    }

    if( !remove_jdlist.isEmpty() )
        UnifiedData::getInstance()->removeJoints(remove_jdlist,true);
    if( !change_jdlist.isEmpty() )
        UnifiedData::getInstance()->changeJoints(change_jdlist,cj_gplist,true);
    QList<JointData*> n_jdlist=UnifiedData::getInstance()->createJoints(nj_gplist,true,nj_idlist);

    for( int i=0; i<cj_indexes.count(); i++ ){
        int c_idx=cj_indexes.at(i);
        n_jdlist.insert(c_idx,change_jdlist.at(i));
    }

    if( UnifiedData::getInstance()->replaceJointIndexes(n_jdlist) ){
        UnifiedData::getInstance()->signalJointChanged(n_jdlist);
    }
}



bool InputJointTableWidget::warningLineDelete()
{
    int ret = QMessageBox::warning(this, "WARNING",
                                   tr("A registered node is selected.") + "\n" +tr("Do you want to delete it?"),
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel );
    return (ret==QMessageBox::Cancel);
}
