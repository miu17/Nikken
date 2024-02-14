#include "framesetting_xydialog.h"

#include <QtGui>
#include <QBoxLayout>
#include <QDebug>
#include <QMenu>
#include <QSpinBox>
#include <QUndoStack>

#include "customtable_model.h"
#include "customtable_view.h"
#include "define_framesettingtable.h"
#include "sortfilter_proxymodel.h"

namespace post3dapp{
FrameSettingXYWidget::FrameSettingXYWidget( const CustomTableDataType &type, QWidget *parent )
    : CustomTableWidget( type, parent )
{
    myMenu = new QMenu(this);
    undoStack = new QUndoStack(this);
    createActions();
    createButtons();
    createModels();
    createViews();
    createWidgets();
    setFilterWidgetsVisible(false);
}

void FrameSettingXYWidget::createModels()
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

void FrameSettingXYWidget::createViews()
{
    myTableView = new CustomTableView(this);
    myTableView->setFirstColumn(myType.getTaggedColumn("X"));
    myTableView->setLastColumn(myType.getTaggedColumn("Y"));

    myTableView->addActions(myActionList);
    myTableView->setMenu(myMenu);

    myTableView->setModel(myProxyModel);
    myTableView->setSelectionModel(mySelectionModel);
    myTableView->setColumnWidth(myType.getTaggedColumn("NO"), 60);
    myTableView->setColumnWidth(myType.getTaggedColumn("X"), 80);
    myTableView->setColumnWidth(myType.getTaggedColumn("Y"), 80);

    connect ( myTableView, QOverload<>::of(&CustomTableView::delKeyClicked), this, &CustomTableWidget::del );
}

void FrameSettingXYWidget::createWidgets()
{
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(0);
    vlay->addLayout(topLayout, 0);
    vlay->addWidget(myTableView, 1);

    this->setLayout(vlay);
}

void FrameSettingXYWidget::setXYList(const QString &xystring)
{

    QStringList strlist = xystring.split(',');
    int scount = strlist.count() - 1;
    int count = ( scount % 2 == 0 ) ? scount / 2 : (scount + 1) / 2 ;
    mySpinBox->setValue(count);
    mySpinBox->setMinimum(count);
    myTableModel->setRowCount(count);

    bool ok1, ok2;
    for (int i = 0; i < count; i++) {
        qreal xx = strlist.at(i * 2).toDouble(&ok1);
        if (!ok1) xx = 0.0;
        myTableModel->setDataDirect(myTableModel->index(i, myType.getTaggedColumn("X")), QVariant(xx));
        if ( i == count - 1 && scount % 2 != 0 ) break;
        qreal yy = strlist.at(i * 2 + 1).toDouble(&ok2);
        if (!ok2) yy = 0.0;
        myTableModel->setDataDirect(myTableModel->index(i, myType.getTaggedColumn("Y")), QVariant(yy));
    }
    undoStack->clear();
}

QString FrameSettingXYWidget::xyString() const
{
    QString xystring;
    for (int i = 0; i < myTableModel->rowCount(); i++) {
        qreal xx = myTableModel->data(myTableModel->index(i, 1), Qt::EditRole).toDouble();
        qreal yy = myTableModel->data(myTableModel->index(i, 2), Qt::EditRole).toDouble();
        QString str = QString("%1,%2,").arg(xx, 0, 'f', 3).arg(yy, 0, 'f', 3);
        xystring += str;
    }
    return xystring;
}

FrameSettingXYDialog::FrameSettingXYDialog( const QString &title, const QString &str,
                                            QWidget *parent )
    : QDialog(parent)
{
    FrameXYDataType my_type;
    myWidget = new FrameSettingXYWidget(my_type, this);
    myWidget->setXYList(str);
    this->setWindowTitle(title);

    QDialogButtonBox *buttons
        = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
    connect( buttons, &QDialogButtonBox::accepted, this, &QDialog::accept );
    connect( buttons, &QDialogButtonBox::rejected, this, &QDialog::reject );

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(myWidget);
    lay->addWidget(buttons);
    this->setLayout(lay);

    setFont(QFont(tr("Microsoft JhengHei"), 9));
    setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
}

QString FrameSettingXYDialog::xyString() const
{
    return myWidget->xyString();
}
} // namespace post3dapp


