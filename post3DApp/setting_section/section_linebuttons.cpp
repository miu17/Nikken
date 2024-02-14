#include "section_linebuttons.h"

#include <QtGui>
#include <QDebug>

#include "reference_combobox.h"
#include "standard_section_combobox.h"
#include "unified_editingdata.h"
#include "unified_inputdata.h"

namespace post3dapp{
SectionLineButtons::SectionLineButtons( const SectionDataType &type, const QList<bool> &filter, QWidget *parent )
    : QWidget( parent ), mySectionType(type)
{
    QFont font(tr("Microsoft JhengHei"), 9);
    font.setStretch(95);
    this->setFont(font);
    createButtons();
    setFilter(filter);
}

void SectionLineButtons::createButtons()
{

    QLabel *lb1 = new QLabel("No.");
    NoSpinBox = new QSpinBox;
    NoSpinBox->setMinimum(1);
    NoSpinBox->setMaximum(1);
    connect(NoSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SectionLineButtons::signalSpinValueChange);

    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addWidget(lb1);
    hlay1->addWidget(NoSpinBox);
    hlay1->addStretch();

    QStringList itemlist = mySectionType.getHeaderStrings();
    for ( int i = 0 ; i < itemlist.count() ; i++ ) {
        QString str = QString("  %1. ").arg(i, 2, 10, QLatin1Char('0')) + itemlist.at(i) + " ";
        str.replace("\n", " ");
        itemlist.replace(i, str);
    }

    QHash<int, QStringList> lmlist = mySectionType.getLimitedStrings();
    QHash<int, DATATYPE> reflist = mySectionType.getReferenceColumns();
    QList<int> sclist = mySectionType.getShapeTypeColumns();
    QList<int> intlist = mySectionType.getIntColumns();
    QList<int> doublelist = mySectionType.getDoubleColumns();

    QSignalMapper *signalMapper = new QSignalMapper(this);
    for ( int i = 0 ; i < mySectionType.count() ; i++) {
        QLabel *lb = new QLabel(itemlist.at(i));
        if (i == 0){
            myLelist.append(new QLineEdit);
            myLblist.append(lb);
            continue;
        }
        if ( sclist.contains(i - 1) ) { //左隣がshapetypecolumnのとき
            StandardSectionComboBox *le = new StandardSectionComboBox;
            le->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle() );
            le->addItems(lmlist.value(i));
            le->setFixedWidth(120);
            myLelist.append(le);
            connect(le, QOverload<int>::of(&QComboBox::activated), signalMapper, QOverload<>::of(&QSignalMapper::map));
            signalMapper->setMapping(le, i);
        } else if ( lmlist.keys().contains(i) ) {
            QComboBox *le = new QComboBox;
            le->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle() );
            le->addItems(lmlist.value(i));
            le->setFixedWidth(120);
            myLelist.append(le);
            connect(le, QOverload<int>::of(&QComboBox::activated), signalMapper, QOverload<>::of(&QSignalMapper::map));
            signalMapper->setMapping(le, i);
        } else if ( reflist.keys().contains(i) ) {
            ReferenceCombobox *le = new ReferenceCombobox(reflist.value(i));
            le->setFixedWidth(120);
            myLelist.append(le);
            connect(le, QOverload<int>::of(&QComboBox::activated), signalMapper, QOverload<>::of(&QSignalMapper::map));
            signalMapper->setMapping(le, i);
        } else {
            QLineEdit *le = new QLineEdit;
            le->setFixedWidth(120);
            myLelist.append(le);
            connect(le, &QLineEdit::editingFinished, signalMapper, QOverload<>::of(&QSignalMapper::map));
            signalMapper->setMapping(le, i);

        }
        myLblist.append(lb);
    }
    connect(signalMapper, QOverload<int>::of(&QSignalMapper::mapped),
            this, &SectionLineButtons::signalChangedValues);

    for ( int i = 0; i < sclist.count(); i++ ) {
        QWidget *wid1 = myLelist.at(sclist.at(i));
        QWidget *wid2 = myLelist.at(sclist.at(i) + 1);
        QComboBox *com1 = qobject_cast<QComboBox *>(wid1);
        StandardSectionComboBox *com2 = qobject_cast<StandardSectionComboBox *>(wid2);
        connect( com1, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
                 com2, &StandardSectionComboBox::updateItemList );
    }

    QGridLayout *glay = new QGridLayout;
    glay->setContentsMargins(0, 0, 0, 0);
    glay->setSpacing(2);
    int sp1 = mySectionType.getSeparateColumn1();
    int sp2 = mySectionType.getSeparateColumn2();
    int sp3 = mySectionType.getSeparateColumn3();
    if ( sp1 < 0 ) {
        sp1 = mySectionType.count();
    }
    if ( sp2 < 0 ) {
        sp2 = mySectionType.count();
    }
    if ( sp3 < 0 ) {
        sp3 = mySectionType.count();
    }

    for (int i = 1; i < sp1 ; i++) {
        glay->addWidget(myLblist.at(i), i - 1, 0, 1, 1);
        glay->addWidget(myLelist.at(i), i - 1, 1, 1, 1);
    }
    for (int i = sp1; i < sp2 ; i++) {
        glay->addWidget(myLblist.at(i), i - sp1, 2, 1, 1);
        glay->addWidget(myLelist.at(i), i - sp1, 3, 1, 1);
    }
    for (int i = sp2; i < sp3 ; i++) {
        glay->addWidget(myLblist.at(i), i - sp2, 4, 1, 1);
        glay->addWidget(myLelist.at(i), i - sp2, 5, 1, 1);
    }
    for (int i = sp3; i < mySectionType.count() ; i++) {
        glay->addWidget(myLblist.at(i), i - sp3, 6, 1, 1);
        glay->addWidget(myLelist.at(i), i - sp3, 7, 1, 1);
    }

    QHBoxLayout *hlay2 = new QHBoxLayout;
    hlay2->addLayout(glay);
    hlay2->addStretch();

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addLayout(hlay1);
    vlay->addLayout(hlay2);

    this->setLayout(vlay);
}

void SectionLineButtons::setLineValues(int row, const QList<QVariant> &values)
{
    myValues = values;
    resetEditerValues(row);
    setStandardLineEdit();
}


void SectionLineButtons::resetEditerValues(int row)
{
    qDebug()<<"section line buttons reset";
    NoSpinBox->setValue(row + 1);
    QHash<int, QStringList> lmlist = mySectionType.getLimitedStrings();
    QHash<int, DATATYPE> reflist = mySectionType.getReferenceColumns();
    QHash<int, QVariant> defaultValues = mySectionType.getDefaultValues();

    for (int i = 1; i < mySectionType.count() ; i++) {
        QString str;
        if ( !myValues.at(i).isValid() ) {
            str = "";
        } else if ( mySectionType.getDoubleColumns().contains(i) ) {
            str = QString("%1").arg(myValues.at(i).toDouble(), 0, 'f', 1);
        } else {
            str = myValues.at(i).toString();
        }

        if ((lmlist.keys().contains(i))) {
            int idx = qobject_cast<QComboBox *>(myLelist.at(i))->findText(str);
            if (idx < 0) idx = 0;
            qobject_cast<QComboBox *>(myLelist.at(i))->setCurrentIndex(idx);
        } else if ((reflist.keys().contains(i))) {
            DATATYPE dtype = reflist.value(i);
            if ( dtype == DATATYPE::TPFLOOR || dtype == DATATYPE::TPFRAMEG || dtype == DATATYPE::TPFRAMEP ) {
                str = UnifiedInputData::getInInstance()->idToName(str, dtype);
            }
            int idx = qobject_cast<QComboBox *>(myLelist.at(i))->findText(str);
            if (idx < 0) idx = 0;
            qobject_cast<QComboBox *>(myLelist.at(i))->setCurrentIndex(idx);
        } else {
            if (str == "" && defaultValues.value(i).isValid()){
                qobject_cast<QLineEdit *>(myLelist.at(i))->setText("");
                qobject_cast<QLineEdit *>(myLelist.at(i))->setPlaceholderText(defaultValues.value(i).toString());
            }
            else{
                qobject_cast<QLineEdit *>(myLelist.at(i))->setText(str);
            }
        }
        qDebug()<<str<<i;
    }
    qDebug()<<"linebutton done";
}

void SectionLineButtons::signalChangedValues(int id)
{
    qDebug()<<"section line buttons sig changed value";
    QHash<int, QStringList> lmlist = mySectionType.getLimitedStrings();
    QHash<int, DATATYPE> reflist = mySectionType.getReferenceColumns();
    QString str1, str2;

    str1 = ( lmlist.keys().contains(id) || reflist.keys().contains(id) ) ?
           qobject_cast<QComboBox *>(myLelist.at(id))->currentText() :
           qobject_cast<QLineEdit *>(myLelist.at(id))->text() ;

    if (myValues.isEmpty()) {
        str2 = "";
    } else if ( !myValues.at(id).isValid() ) {
        str2 = "";
    } else if ( mySectionType.getDoubleColumns().contains(id) ) {
        str2 = QString("%1").arg(myValues.at(id).toDouble(), 0, 'f', 1);
    } else {
        str2 = myValues.at(id).toString();
    }

    if (str1 != str2) {
        emit lineButtonsDataChanged(NoSpinBox->value() - 1, id, str1);
        setStandardLineEdit();
    }
}


void SectionLineButtons::setStandardLineEdit()
{

    QList<int> sclist = mySectionType.getShapeTypeColumns();
    QStringList standardSectionFiles = SectionDataType::standardSectionFileList(StandardSectionTable::listType::STEEL);

    QString st1, st2;
    Q_FOREACH (int col, sclist) {
        st1 = qobject_cast<QComboBox *>(myLelist.at(col))->currentText();
        st2 = qobject_cast<QComboBox *>(myLelist.at(col + 1))->currentText();
        bool flag = ( standardSectionFiles.contains(st1) &&
                      SectionDataType::standardSectionNameList(st1).contains(st2));
        for (int i = 2; i <= 5; i++){
            myLelist.at(col + i)->setEnabled(!flag);
            myLblist.at(col + i)->setEnabled(!flag);
        }
    }
}

void SectionLineButtons::setButtonFocus(int col)
{
    if ( col >= myLelist.count() ) {
        myLelist.last()->setFocus();
        return;
    }
    myLelist.at(col)->setFocus();
}

void SectionLineButtons::slotChangedFilter(const QList<bool> &filter){
    setFilter(filter);
}

void SectionLineButtons::setFilter(const QList<bool> &filter){
    for (int i = 0; i < filter.count(); i++){
        myLblist.at(i)->setEnabled(filter.at(i));
        myLelist.at(i)->setEnabled(filter.at(i));
    }
}
} // namespace post3dapp
