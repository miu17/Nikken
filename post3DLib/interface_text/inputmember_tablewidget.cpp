#include <QtGui>
#include <QDebug>

#include "inputmember_tablewidget.h"

InputMemberTableWidget::InputMemberTableWidget( const CustomTableDataType &type, QWidget* parent )
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

void InputMemberTableWidget::createModels()
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

void InputMemberTableWidget::createViews()
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
    myTableView->setColumnWidth( myType.getTaggedColumn("WALLOPEN"),75);
    myTableView->setColumnWidth( myType.getTaggedColumn("JOINTS"),150);
    myTableView->setColumnWidth( myType.getTaggedColumn("FLOORS"),75);
    myTableView->setColumnWidth( myType.getTaggedColumn("FRAMES"),75);
    myTableView->setColumnWidth( myType.getTaggedColumn("ANGLE"),75);
    myTableView->setColumnWidth( myType.getTaggedColumn("IFACERULE"),75);
    myTableView->setColumnWidth( myType.getTaggedColumn("IFACELEN"),75);
    myTableView->setColumnWidth( myType.getTaggedColumn("JFACERULE"),75);
    myTableView->setColumnWidth( myType.getTaggedColumn("JFACELEN"),75);
    myTableView->setColumnWidth( myType.getTaggedColumn("SHIFTY"),75);
    myTableView->setColumnWidth( myType.getTaggedColumn("SHIFTZ"),75);
    myTableView->setColumnWidth( myType.getTaggedColumn("UUID"),120);


    QHash<int,QStringList> lmlist=myType.getLimitedStrings();
    ComboBoxDelegate* deleg1=new ComboBoxDelegate(lmlist.value(myType.getTaggedColumn("TYPE")));
    myTableView->setItemDelegateForColumn(myType.getTaggedColumn("TYPE"),deleg1);
    ComboBoxDelegate* deleg2=new ComboBoxDelegate(lmlist.value(myType.getTaggedColumn("IFACERULE")));
    myTableView->setItemDelegateForColumn(myType.getTaggedColumn("IFACERULE"),deleg2);
    ComboBoxDelegate* deleg3=new ComboBoxDelegate(lmlist.value(myType.getTaggedColumn("JFACERULE")));
    myTableView->setItemDelegateForColumn(myType.getTaggedColumn("JFACERULE"),deleg3);

    connect ( myTableView, SIGNAL( delKeyClicked() ), this, SLOT( del() ) );
    connect ( myTableView, SIGNAL( selectionBehaviorChanged(bool)),
              this, SLOT( slotRowActionEnable(bool) ) );
}

void InputMemberTableWidget::createWidgets()
{
    mySpinBox->setMaximum(99999);

    QVBoxLayout *vlay=new QVBoxLayout;
    vlay->setContentsMargins(0,0,0,0);
    vlay->setSpacing(0);

    vlay->addLayout(topLayout,0);
    vlay->addWidget(myTableView,1);
    this->setLayout(vlay);
}

void InputMemberTableWidget::slotDeleteLine()
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

void InputMemberTableWidget::importMemberData( const QList<MemberData*> &mdlist )
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
              << QVariant( md->wallOpenName() )
              << QVariant( str_jd )
              << QVariant( floor )
              << QVariant( frames )
              << QVariant( md->codeAngle()*180.0/M_PI )
              << QVariant( (md->getIFaceRule()==FACE_AUTO) ? tr("Auto") : tr("Value specification") )
              << QVariant( md->getIFace() )
              << QVariant( (md->getJFaceRule()==FACE_AUTO) ? tr("Auto") : tr("Value specification") )
              << QVariant( md->getJFace() )
              << QVariant( md->getShiftY() )
              << QVariant( md->getShiftZ() )
              << QVariant( md->getUuid() );
        myTableModel->setRowDataDirect(i,vlist,false);
    }
    myTableModel->setDataCount();
    myTableModel->setEditOn(true);
    undoStack->clear();
}


QString InputMemberTableWidget::checkMemberData(int jcount) const
{
    QString msg;

    for(int i=0; i < myTableModel->rowCount() ; i++){

        QVariant var1=myTableModel->getData(i,myType.getTaggedColumn("TYPE"));
        QVariant var2=myTableModel->getData(i,myType.getTaggedColumn("NAME"));
        QVariant var3=myTableModel->getData(i,myType.getTaggedColumn("WALLOPEN"));
        QVariant var4=myTableModel->getData(i,myType.getTaggedColumn("JOINTS"));

        if( var1.toString().isEmpty() ){
            msg=QString("MEMBER No.%1\n").arg(i+1) + tr("Member type is blank");
            break;
        }

        DATATYPE dtype=UnifiedData::getInstance()->stringToDataType( var1.toString() );
        QString name=var2.toString();
        QStringList nameList=UnifiedData::getInstance()->getNameList(dtype);
        QString wname=var3.toString();

        if( !name.isEmpty() && !nameList.contains(name) ){
            msg=QString("MEMBER No.%1\n").arg(i+1) + tr("Corresponding member code is missing");
            break;
        }
        if( !wname.isEmpty() && !UnifiedData::getInstance()->isExistedName(wname,TPWALLOPEN) ){
            msg=QString("MEMBER No.%1\n").arg(i+1) + tr("There is no corresponding aperture sign");
            break;
        }

        QString jstr=var4.toString();
        if( jstr.isEmpty() ){
            msg=QString("MEMBER No.%1\n").arg(i+1) + tr("Nodal composition is blank");
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
            msg=QString("MEMBER No.%1\n").arg(i+1) + tr("Nodal structure has non-numeric characters");
            break;
        }
        if( !ok2 ){
            msg=QString("MEMBER No.%1\n").arg(i+1) + tr("The node structure does not correspond to the existing node No.");
            break;
        }
        if( !ok3 ){
            msg=QString("MEMBER No.%1\n").arg(i+1) + tr("The same node is used for composition");
            break;
        }
        if( dtype==TPSLAB || dtype==TPWALL || dtype==TPUWALL ){
            if( jnolist.count()<3 ){
                msg=QString("MEMBER No.%1\n").arg(i+1) + tr("Please specify at least 3 nodes for the face material");
                break;
            }
        }else{
            if( jnolist.count()<2 ){
                msg=QString("MEMBER No.%1\n").arg(i+1) + tr("Please specify at least 2 nodes for the wire");
                break;
            }
        }

    }

    return msg;
}

void InputMemberTableWidget::uploadMembers()
{
    UnifiedData::getInstance()->removeMembers( UnifiedData::getInstance()->memberListExceptLoad() );

    QList<MemberData*> created_mdlist;

    for(int i=0; i < myTableModel->rowCount() ; i++){

        QString str1=myTableModel->getData(i,myType.getTaggedColumn("TYPE")).toString();
        DATATYPE dtype=UnifiedData::getInstance()->stringToDataType(str1);
        QString name=myTableModel->getData(i,myType.getTaggedColumn("NAME")).toString();
        QString wname=myTableModel->getData(i,myType.getTaggedColumn("WALLOPEN")).toString();

        QList<JointData*> jdlist;
        QString str2=myTableModel->getData(i,myType.getTaggedColumn("JOINTS")).toString();
        QStringList strlist=str2.split("-");
        Q_FOREACH( QString str, strlist ){
            jdlist.append( UnifiedData::getInstance()->jointIndexToPointer(str.toInt()-1) );
        }
        qreal angle=myTableModel->getData(i,myType.getTaggedColumn("ANGLE")).toDouble();
        QString str3=myTableModel->getData(i,myType.getTaggedColumn("IFACERULE")).toString();
        FACERULE ifrule=( str3==tr("Value specification") ) ? FACE_VALUE : FACE_AUTO ;
        qreal iface=myTableModel->getData(i,myType.getTaggedColumn("IFACELEN")).toDouble();
        QString str4=myTableModel->getData(i,myType.getTaggedColumn("JFACERULE")).toString();
        FACERULE jfrule=( str4==tr("Value specification") ) ? FACE_VALUE : FACE_AUTO ;
        qreal jface=myTableModel->getData(i,myType.getTaggedColumn("JFACELEN")).toDouble();
        qreal shy=myTableModel->getData(i,myType.getTaggedColumn("SHIFTY")).toDouble();
        qreal shz=myTableModel->getData(i,myType.getTaggedColumn("SHIFTZ")).toDouble();

        QUuid uid=myTableModel->getData(i,myType.getTaggedColumn("UUID")).toString();

        MemberData* md=UnifiedData::getInstance()->createMember(jdlist,dtype,name,uid);
        md->copyValues( MEMBERVALUES(dtype,name,angle,shy,shz,ifrule,jfrule,iface,jface,
                                     PART_INTERNAL,PART_RATIO,1200.0,1200.0,0.25,0.25,wname) );
        created_mdlist.append(md);

    }

    if( created_mdlist.count()>0 ){
        UnifiedData::getInstance()->signalMemberAppended(created_mdlist);
    }
}

bool InputMemberTableWidget::warningLineDelete()
{
    int ret = QMessageBox::warning(this, "WARNING",
                                   tr("Registered members are selected.") + "\n" + tr("Do you want to delete it?"),
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel );
    return (ret==QMessageBox::Cancel);
}
