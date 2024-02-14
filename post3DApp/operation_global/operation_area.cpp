#include "operation_area.h"

#include <QtGui>

#include "custom_checkbutton.h"
#include "operation_changeall_widget.h"
#include "operation_editall_widget.h"
#include "operation_element_widget.h"
#include "unified_editingdata.h"
#include "vhline.h"

namespace post3dapp{
OperationArea::OperationArea( QWidget *parent )
    : QScrollArea( parent )
{
    elementWidget = new OperationElementWidget(this);
    editAllWidget = new OperationEditAllWidget(this);
    changeAllWidget = new OperationChangeAllWidget(this);

    QStackedWidget *topStackedWidget = new QStackedWidget(this);
    topStackedWidget->addWidget(elementWidget);
    topStackedWidget->addWidget(editAllWidget);
    topStackedWidget->addWidget(changeAllWidget);

    CustomCheckButton *element_button = new CustomCheckButton(u8"部材別・荷重別の追加・編集", this);
    CustomCheckButton *editall_button = new CustomCheckButton(u8"全 体 編 集", this);
    CustomCheckButton *changeall_button = new CustomCheckButton(u8"一 括 操 作", this);

    element_button->setFixedHeight(25);
    editall_button->setFixedHeight(25);
    changeall_button->setFixedHeight(25);
    element_button->setChecked(true);

    QButtonGroup *operationButtons = new QButtonGroup;
    operationButtons->addButton(element_button, 0);
    operationButtons->addButton(editall_button, 1);
    operationButtons->addButton(changeall_button, 2);

    connect(operationButtons, QOverload<int>::of(&QButtonGroup::buttonClicked),
            topStackedWidget, QOverload<int>::of(&QStackedWidget::setCurrentIndex));
    connect(operationButtons, QOverload<int>::of(&QButtonGroup::buttonClicked),
            UnifiedEditingData::getInstance(), &UnifiedEditingData::changeCurrentOperation);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 0);
    vlay->setSpacing(3);
    vlay->addSpacing(5);
    vlay->addWidget(element_button);
    vlay->addWidget(editall_button);
    vlay->addWidget(changeall_button);
    vlay->addSpacing(5);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addSpacing(5);
    vlay->addWidget(topStackedWidget);

    QWidget *op_wid = new QWidget(this);
    op_wid->setFixedSize(255, 650);
    op_wid->setLayout(vlay);

    this->setWidget(op_wid);

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Window, QBrush(Qt::white));
    this->setPalette(pal);
    this->setMaximumWidth(260);
}
} // namespace post3dapp
