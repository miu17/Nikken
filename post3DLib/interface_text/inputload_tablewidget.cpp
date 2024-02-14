#include <QtGui>
#include <QDebug>

#include "inputload_tablewidget.h"

InputLoadTableWidget::InputLoadTableWidget( const CustomTableDataType &type, QWidget* parent )
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

void InputLoadTableWidget::createModels()
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

void InputLoadTableWidget::createViews()
{
    myTableView=new CustomTableView(this);
    myTableView->setFirstColumn( myType.getTaggedColumn("TYPE") );
    myTableView->setLastColumn( myType.getTaggedColumn("UUID") );

    myTableView->addActions(myActionList);
    myTableView->setMenu(myMenu);

    myTableView->setModel(myProxyModel);
    myTableView->setSelectionModel(mySelectionModel);
    myTableView->setColumnWidth( myType.getTaggedColumn("NO"),60);
    myTableView->setColumnWidth( myType.getTaggedColumn("TYPE"),75);
    myTableView->setColumnWidth( myType.getTaggedColumn("NAME"),75);
    myTableView->setColumnWidth( myType.getTaggedColumn("JOINTS"),150);
    myTableView->setColumnWidth( myType.getTaggedColumn("FLOORS"),75);
    myTableView->setColumnWidth( myType.getTaggedColumn("FRAMES"),75);
    myTableView->setColumnWidth( myType.getTaggedColumn("UUID"),120);


    QHash<int,QStringList> lmlist=myType.getLimitedStrings();
    ComboBoxDelegate* deleg1=new ComboBoxDelegate(lmlist.value(myType.getTaggedColumn("TYPE")));
    myTableView->setItemDelegateForColumn(myType.getTaggedColumn("TYPE"),deleg1);

    connect ( myTableView, SIGNAL( delKeyClicked() ), this, SLOT( del() ) );
    connect ( myTableView, SIGNAL( selectionBehaviorChanged(bool)),
              this, SLOT( slotRowActionEnable(bool) ) );
}

void InputLoadTableWidget::createWidgets()
{
    mySpinBox->setMaximum(99999);

    QVBoxLayout *vlay=new QVBoxLayout;
    vlay->setContentsMargins(0,0,0,0);
    vlay->setSpacing(0);

    vlay->addLayout(topLayout,0);
    vlay->addWidget(myTableView,1);
    this->setLayout(vlay);
}

void InputLoadTableWidget::slotDeleteLine()
{

    QModelIndexList rows_idxlist=mySelectionModel->selectedRows();
    bool isDisable=false;
    Q_FOREACH(QModelIndex idx, rows_idxlist){
        if(myTableModel->isRegisterRow(idx.row())) isDisable=true;
    }
    if( isDisable ){
        if( ! myTableModel->isEditOn() ) return;
        if( warningLineDelete() ) return;
    }

    CustomTableWidget::slotDeleteLine();
}

void InputLoadTableWidget::importMemberData( const QList<MemberData*> &mdlist )
{
    myTableModel->initializeModel();
    mySpinBox->setValue(mdlist.count());
    for( int i=0; i<mdlist.count(); i++ ){
        QList<QVariant> vlist;
        MemberData* md=mdlist.at(i);
        QStringList strlist;
        Q_FOREACH( JointData* jd, md->getJointList() )
            strlist.append( QString("%1").arg(UnifiedData::getInstance()->indexOfJoint(jd)+1 ) );
        QString str_jd=strlist.join("-");
        QString floor=UnifiedData::getInstance()->idToName(md->getAttachedFloor(),TPFLOOR);
        QStringList frlist=UnifiedData::getInstance()->idsToNames(md->getAttachedFrameList(),TPFRAMEP);
        QString frames=frlist.join("/");
        vlist << QVariant()
              << QVariant( UnifiedData::getInstance()->dataTypeToJapanese(md->dataType()) )
              << QVariant( md->sectionName() )
              << QVariant( str_jd )
              << QVariant( floor )
              << QVariant( frames )
              << QVariant( md->getUuid() );
        myTableModel->setRowDataDirect(i,vlist,false);
    }
    myTableModel->setDataCount();
    myTableModel->setEditOn(true);
    undoStack->clear();
}

QString InputLoadTableWidget::checkLoadData(int jcount) const
{
    QString msg;

    for(int i=0; i < myTableModel->rowCount() ; i++){

        QVariant var1=myTableModel->getData(i,myType.getTaggedColumn("TYPE"));
        QVariant var2=myTableModel->getData(i,myType.getTaggedColumn("NAME"));
        QVariant var3=myTableModel->getData(i,myType.getTaggedColumn("JOINTS"));

        if( var1.toString().isEmpty() ){
            msg=QString("LOAD No.%1\n").arg(i+1) + tr("Load type is blank");
            break;
        }

        DATATYPE dtype=UnifiedData::getInstance()->stringToDataType( var1.toString() );
        QString name=var2.toString();
        QStringList nameList=UnifiedData::getInstance()->getNameList(dtype);

        if( !name.isEmpty() && !nameList.contains(name) ){
            msg=QString("LOAD No.%1\n").arg(i+1) + tr("Corresponding member code is missing");
            break;
        }

        QString jstr=var3.toString();
        if( jstr.isEmpty() ){
            msg=QString("LOAD No.%1\n").arg(i+1) + tr("Nodal composition is blank");
            break;
        }

        QStringList jstrlist=jstr.split("-");
        QList<int> jnolist;
        bool ok1=true,ok2=true,ok3=true;
        Q_FOREACH( QString str, jstrlist ){
            int jn=str.toInt(&ok1);
            if( !ok1 ) break;
            jnolist.append(jn);
            if( jn<1 || jn>jcount ){ ok2=false; break; }
            if( jnolist.count(jn)>1 ){ ok3=false; break; }
        }
        if( !ok1 ){
            msg=QString("LOAD No.%1\n").arg(i+1) + tr("Nodal structure has non-numeric characters");
            break;
        }
        if( !ok2 ){
            msg=QString("LOAD No.%1\n").arg(i+1) + tr("The node structure does not correspond to the existing node No.");
            break;
        }
        if( !ok3 ){
            msg=QString("LOAD No.%1\n").arg(i+1) + tr("The same node is used for composition");
            break;
        }
        if( dtype==TPJOINTLOAD ){
            if( jnolist.count()!=1 ){
                msg=QString("LOAD No.%1\n").arg(i+1) + tr("Please specify the node of 1 point for the nodal load");
                break;
            }
        }else if( dtype==TPMEMBERLOAD ){
            if( jnolist.count()<2 ){
                msg=QString("LOAD No.%1\n").arg(i+1) + tr("Please specify a node with 2 or more points for member load");
                break;
            }
        }else{
            if( jnolist.count()<3 ){
                msg=QString("LOAD No.%1\n").arg(i+1) + tr("Please specify a node with 3 or more floor loads");
                break;
            }
        }

    }

    return msg;
}

void InputLoadTableWidget::uploadLoads()
{
    QList<DATATYPE> loadTypes;
    loadTypes << TPJOINTLOAD << TPMEMBERLOAD << TPSLABLOAD;
    UnifiedData::getInstance()->removeMembers( UnifiedData::getInstance()->memberListOfType(loadTypes) );

    QList<MemberData*> created_mdlist;

    for(int i=0; i < myTableModel->rowCount() ; i++){

        QString str1=myTableModel->getData(i,myType.getTaggedColumn("TYPE")).toString();
        DATATYPE dtype=UnifiedData::getInstance()->stringToDataType(str1);
        QString name=myTableModel->getData(i,myType.getTaggedColumn("NAME")).toString();

        QList<JointData*> jdlist;
        QString str2=myTableModel->getData(i,myType.getTaggedColumn("JOINTS")).toString();
        QStringList strlist=str2.split("-");
        Q_FOREACH( QString str, strlist ){
            jdlist.append( UnifiedData::getInstance()->jointIndexToPointer(str.toInt()-1) );
        }

        QUuid uid=myTableModel->getData(i,myType.getTaggedColumn("UUID")).toString();

        MemberData* md=UnifiedData::getInstance()->createMember(jdlist,dtype,name,uid);
        created_mdlist.append(md);

    }

    if( created_mdlist.count()>0 ){
        UnifiedData::getInstance()->signalMemberAppended(created_mdlist);
    }
}

bool InputLoadTableWidget::warningLineDelete()
{
    int ret = QMessageBox::warning(this, "WARNING",
                                   tr("Registered load is selected.") + "\n" + tr("Do you want to delete it?"),
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel );
    return (ret==QMessageBox::Cancel);
}
