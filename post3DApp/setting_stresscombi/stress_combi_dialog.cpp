#include "stress_combi_dialog.h"

#include "combobox_delegate.h"
#include "fixed_data.h"
#include "unified_editingdata.h"
#include "unified_output.h"
#include "unified_project.h"

namespace post3dapp{
/*memo
 * 構造体を定義して、リードとライト。管轄はUnifiedData？データがある場合はそちら優先、ない場合はデフォルト。
 * 確定押したらセーブされ、キャンセルで破棄。この処理はtopmenuwidgetでやってもよい。
 * pstnに含まれない場合の、描画処理での例外に気を付ける
 * 文字列をu8で描いているのをどうにかする
 * ウィジェットで編集できるが、ウィジェットを初めて開く前でもデフォルト値は存在するはず。先に図の表示に行ったときはどうなのか。
 * signalをfunctor系で書き直す
 */
StressCombiDialog::StressCombiDialog(QWidget* p):QDialog(p)
{
    this->setWindowFlags(Qt::Dialog
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint);
    this->setFont(QFont(tr("Microsoft JhengHei"), 10));
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //プルダウン反映・有無のチェック用にケースリストを読む
    existCase = UnifiedFixedData::getInstance()->getPstnData()->getStressCaseList();
    createLayout();

    setData();

}
void StressCombiDialog::confirm(){
    //データチェック、UnifiedOutputの更新、Widgetの更新
//    QStringList combi;
//    QHash<QString,QList<qreal>> coef;
//    QHash<QString,QStringList> stress;
//    QList<int> step;

    STRESSCOMBISETTINGS set;
    for (int i=0;i<combitables.count();i++){
        QString combiname = table1->item(i,0)->text();
        set.combi<<combiname;
        set.step<<stepcase.at(i)->currentIndex();
        QStringList tmp1;
        QList<qreal > tmp2;
        for(int j=0;j<combitables.at(i)->rowCount();j++){
            tmp1<<combitables.at(i)->item(j,0)->text();
            tmp2<<combitables.at(i)->item(j,1)->text().toDouble();
        }
        set.stress.insert(combiname,tmp1);
        set.coef.insert(combiname,tmp2);
        qDebug()<<"insert combi stress"<<combiname<<tmp1<<tmp2;
    }
    qDebug()<<"call register";
    UnifiedProject::getInstance()->registerTempStressCombiData(set);
    qDebug()<<"call register done";
//    registerTempStressCombiData(st);

//    qDebug()<<UnifiedInputData::getInInstance()->getStressCombinationSetting().stress.value(u8"CaseDL");
//    qDebug()<<UnifiedInputData::getInInstance()->getStressCombinationSetting().coef.value(u8"CaseDL");
    emit statusChanged();
    this->close();

}
void StressCombiDialog::cancel(){
    this->close();
}


void StressCombiDialog::createLayout()
{

    table1 = new QTableWidget(0,1);
    table1->horizontalHeader()->setStretchLastSection(true);
    table1->setHorizontalHeaderLabels(QStringList()<<tr(u8"応力組み合わせ名"));
    table1->setMinimumHeight(400);
    QPushButton* button_plus1 = new QPushButton(tr(u8"＋"));
    QPushButton* button_minus1= new QPushButton(tr(u8"－"));

    stack= new QStackedLayout;

    QPushButton* button_confirm =new QPushButton(tr(u8"確定"));
    QPushButton* button_cancel =new QPushButton(tr(u8"キャンセル"));

    QVBoxLayout* lay_whole = new QVBoxLayout;
    lay_whole->setContentsMargins(10, 10, 10, 10);

    QHBoxLayout* lay_buttons= new QHBoxLayout;
    QHBoxLayout* lay_tables= new QHBoxLayout;
    QVBoxLayout* lay_plusminus1 = new QVBoxLayout;
    lay_plusminus1->addWidget(button_plus1);
    lay_plusminus1->addWidget(button_minus1);
    lay_plusminus1->addStretch();


    lay_tables->addWidget(table1);
    lay_tables->addLayout(lay_plusminus1);
    lay_tables->addLayout(stack);
    lay_buttons->addWidget(button_confirm);
    lay_buttons->addWidget(button_cancel);
    lay_buttons->addStretch();
    lay_whole->addLayout(lay_buttons);
    lay_whole->addLayout(lay_tables);
    this->setLayout(lay_whole);

    connect ( button_confirm, &QAbstractButton::clicked, this, &StressCombiDialog::confirm );
    connect ( button_cancel, &QAbstractButton::clicked, this, &StressCombiDialog::cancel );
    connect ( button_plus1, &QAbstractButton::clicked, this, &StressCombiDialog::addCombination );
    connect ( button_minus1, &QAbstractButton::clicked, this, &StressCombiDialog::deleteCombination );
    connect ( table1, &QTableWidget::cellClicked, this, &StressCombiDialog::changeStackWidget );


}
void StressCombiDialog::insertSubWidget(int index)
{
    QTableWidget* table= new QTableWidget(0,2);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setHorizontalHeaderLabels(QStringList()<<tr(u8"応力ケース名")<<tr(u8"係数"));
    ComboBoxDelegate* delegate = new ComboBoxDelegate(defaultCases());
    table->setItemDelegateForColumn(0,delegate);
    table->setColumnWidth(0,120);

    QPushButton* button_plus2= new QPushButton(tr(u8"＋"));
    QPushButton* button_minus2= new QPushButton(tr(u8"－"));

    QLabel * labelstep = new QLabel(tr(u8"ステップを考慮する応力ケース"));
    QComboBox* combostep = new QComboBox();
    QHBoxLayout* lay_tables= new QHBoxLayout;
    QVBoxLayout* lay_combitable = new QVBoxLayout;
    QVBoxLayout* lay_plusminus2= new QVBoxLayout;
    lay_tables->setContentsMargins(0, 0, 0, 0);

    lay_combitable->addWidget(table);
    lay_combitable->addWidget(labelstep);
    lay_combitable->addWidget(combostep);

    lay_plusminus2->addWidget(button_plus2);
    lay_plusminus2->addWidget(button_minus2);
    lay_plusminus2->addStretch();

    lay_tables->addLayout(lay_combitable);
    lay_tables->addLayout(lay_plusminus2);

    connect ( button_plus2, &QAbstractButton::clicked, this, &StressCombiDialog::addStressCase );
    connect ( button_minus2, &QAbstractButton::clicked, this, &StressCombiDialog::deleteStressCase );
    connect ( table, &QTableWidget::cellChanged, this, &StressCombiDialog::checkCase );
    combitables.insert(index,table);
    stepcase.insert(index,combostep);

    QWidget* w = new QWidget(this);
    w->setLayout(lay_tables);
    stack->insertWidget(index,w);
}
void StressCombiDialog::removeSubWidget(int index){
    if(index==stack->currentIndex()){
        table1->removeRow(index);
        combitables.removeAt(index);
        stepcase.removeAt(index);
        stack->removeWidget(stack->currentWidget());
    }

}

void StressCombiDialog::addCombination()
{
    int index = table1->currentRow();
    table1->insertRow(index+1);
    insertSubWidget(index+1);
}
void StressCombiDialog::deleteCombination()
{
    int index = table1->currentRow();
    removeSubWidget(index);
}
void StressCombiDialog::addStressCase()
{
    int index = stack->currentIndex();
    QTableWidget* table=combitables.at(index);
    table->insertRow(table->currentRow()+1);

    QTableWidgetItem* stitem= new QTableWidgetItem();
    stitem->setTextAlignment(Qt::AlignCenter);
    table->setItem(table->currentRow()+1,0,stitem);

    QTableWidgetItem* coefitem= new QTableWidgetItem(QString::number(1.00,'f',2));
    coefitem->setTextAlignment(Qt::AlignCenter);
    table->setItem(table->currentRow()+1,1,coefitem);


}

void StressCombiDialog::deleteStressCase()
{
    int index = stack->currentIndex();
    int row = combitables.at(index)->currentRow();
    combitables.at(index)->removeRow(row);
    checkCase(0,0); //ステップボックスの再作成など
//    stepcase.at(index)->removeItem(row+1);
}

void StressCombiDialog::changeStackWidget(int row,int col)
{
    Q_UNUSED(col);
    stack->setCurrentIndex(row);
}



void StressCombiDialog::setOneData(QString name,int stepindex,QList<qreal> coef,QStringList stcase){
    int index = combitables.count();
    insertSubWidget(index);

    if(index>=table1->rowCount()){
        table1->setRowCount(index+1);
    }else if(index<0){
        return;
    }
    QTableWidgetItem* item = new QTableWidgetItem(name);
    table1->setItem(index,0,item);

    for(int i=0;i<coef.count();i++){
        QTableWidgetItem* coefitem= new QTableWidgetItem(QString::number(coef.at(i),'f',2));
        QTableWidgetItem* stitem= new QTableWidgetItem(stcase.at(i));
        stitem->setTextAlignment(Qt::AlignCenter);
        coefitem->setTextAlignment(Qt::AlignCenter);
        combitables.at(index)->insertRow(i);
        combitables.at(index)->setItem(i,0,stitem);
        combitables.at(index)->setItem(i,1,coefitem);
        if(!existData(stcase.at(i))){
            stitem->setForeground(QBrush(Qt::red));
            item->setForeground(QBrush(Qt::red));
        }

    }
    stepcase.at(index)->clear();
    stcase.push_front("");
    stepcase.at(index)->insertItems(0,stcase);
    if( stepindex >= 0 && stepindex < stcase.count()){
         stepcase.at(index)->setCurrentIndex(stepindex);
    }

}
void StressCombiDialog::setData(){
    STRESSCOMBISETTINGS scset = UnifiedProject::getInstance()->getStressCombinationSetting();
    for(int i = 0;i<scset.combi.count();i++){
        QString key = scset.combi.at(i);
        setOneData(key,scset.step.at(i),scset.coef.value(key),scset.stress.value(key));
    }
}

bool StressCombiDialog::existData(QString str)
{
    return existCase.contains(str);
}
QStringList StressCombiDialog::defaultCases()
{
    QStringList list;
    list<<u8"RSTDL"<<u8"RSTLL1"<<u8"RSTLL2"<<u8"RSTLL3"<<u8"RSTLL4"<<u8"RSTE1"<<u8"RSTE2"<<u8"RSTU1"
        <<u8"RSTU2"<<u8"RSTU3"<<u8"RSTU4"<<u8"RSTU5"<<u8"RSTU6"<<u8"RSTU7"<<u8"RSTU8";
    Q_FOREACH(QString str,existCase){
        if(!list.contains(str))list.append(str);
    }
    return list;
}

void StressCombiDialog::checkCase(int row,int col)
{
    if(col == 0){
        //変わったデータの有無をチェック
        int index = stack->currentIndex();
        QTableWidget* table = combitables.at(index);
        if(table==nullptr)return;
        QTableWidgetItem* csitem = table1->item(index,0);
        if(csitem==nullptr)return;
        csitem->setForeground(QBrush(Qt::black));
        for(int i=0;i<table->rowCount();i++){
            QTableWidgetItem* item =table->item(i,0);
            QString data = item->text();
            if(existData(data)){
                item->setForeground(QBrush(Qt::black));
            }else{
                csitem->setForeground(QBrush(Qt::red));
                item->setForeground(QBrush(Qt::red));
            }

        }


        //ステップコンボボックスの再構成
        QStringList list;
        list<<"";
        for(int i=0;i<table->rowCount();i++){
            if(table->item(i,0)==nullptr)continue;
            list << table->item(i,0)->text();
        }
        QComboBox* combo = stepcase.at(index);
        if(combo == nullptr)return;
        QString prev = combo->currentText();
        combo->clear();
        combo->insertItems(0,list);

        combo->setCurrentIndex(combo->findText(prev));
    }
}
}

