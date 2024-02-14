#include "operation_element_widget.h"

#include <QtGui>
#include <QDebug>

#include "custom_checkbutton.h"
#include "operation_column_page.h"
#include "operation_beam_page.h"
#include "operation_brace_page.h"
#include "operation_damper_page.h"
#include "operation_girder_page.h"
#include "operation_iso_page.h"
#include "operation_joint_page.h"
#include "operation_jointload_page.h"
#include "operation_memberload_page.h"
#include "operation_slab_page.h"
#include "operation_slabload_page.h"
#include "operation_uwall_page.h"
#include "operation_wall_page.h"
#include "operation_widget_parts.h"
#include "unified_editingdata.h"
#include "vhline.h"

namespace post3dapp{
OperationElementWidget::OperationElementWidget( QWidget *parent )
    : QWidget( parent )
{
    this->setFocusPolicy(Qt::NoFocus);
    elementStackedWidget = new QStackedWidget(this);

    createSelectArea();
    createStackedPages();

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(5);
    vlay->addWidget(selectWidget);
    vlay->addWidget(elementStackedWidget);
    this->setLayout(vlay);
}

void OperationElementWidget::createSelectArea()
{
    elementComb = new QComboBox(this);
    elementComb->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle() );
    elementComb->setMaxVisibleItems(17);
    const QStringList slist{u8"形状節点", u8"柱", u8"大梁", u8"ブレース", u8"小梁", u8"スラブ",
                            u8"壁", u8"地下外壁", u8"制振要素", u8"免震要素", u8"節点荷重", u8"部材荷重", u8"床荷重"};
    elementComb->addItems(slist);
    elementComb->insertSeparator(10);
    elementComb->insertSeparator(8);
    elementComb->insertSeparator(5);
    elementComb->insertSeparator(1);

    CustomCheckButton *add_button = new CustomCheckButton(u8"追 加", this);
    CustomCheckButton *edit_button = new CustomCheckButton(u8"編 集", this);
    elementOpMode = new QButtonGroup;
    elementOpMode->addButton(add_button, 0);
    elementOpMode->addButton(edit_button, 1);

    connect(elementOpMode, QOverload<int>::of(&QButtonGroup::buttonClicked),
            this, &OperationElementWidget::slotChangeOperationMode);

    add_button->setFixedHeight(25);
    edit_button->setFixedHeight(25);
    add_button->setChecked(true);

    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->setSpacing(3);
    hlay->addWidget(add_button);
    hlay->addWidget(edit_button);

    selectWidget = new QWidget(this);
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(3);
    vlay->addWidget(elementComb);
    vlay->addLayout(hlay);
    vlay->addSpacing(5);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addSpacing(5);
    selectWidget->setLayout(vlay);
}

void OperationElementWidget::createStackedPages()
{
    OperationJointPage *jointPage = new OperationJointPage(this);
    OperationColumnPage *columnPage = new OperationColumnPage(this);
    OperationGirderPage *girderPage = new OperationGirderPage(this);
    OperationBracePage *bracePage = new OperationBracePage(this);
    OperationBeamPage *beamPage = new OperationBeamPage(this);
    OperationSlabPage *slabPage = new OperationSlabPage(this);
    OperationWallPage *wallPage = new OperationWallPage(this);
    OperationUWallPage *uwallPage = new OperationUWallPage(this);
    OperationDamperPage *damperPage = new OperationDamperPage(this);
    OperationIsoPage *isoPage = new OperationIsoPage(this);
    OperationJointLoadPage *jointLoadPage = new OperationJointLoadPage(this);
    OperationMemberLoadPage *memberLoadPage = new OperationMemberLoadPage(this);
    OperationSlabLoadPage *slabLoadPage = new OperationSlabLoadPage(this);

    elementPageList.append(jointPage);
    elementPageList.append(columnPage);
    elementPageList.append(girderPage);
    elementPageList.append(bracePage);
    elementPageList.append(beamPage);
    elementPageList.append(slabPage);
    elementPageList.append(wallPage);
    elementPageList.append(uwallPage);
    elementPageList.append(damperPage);
    elementPageList.append(isoPage);
    elementPageList.append(jointLoadPage);
    elementPageList.append(memberLoadPage);
    elementPageList.append(slabLoadPage);

    for (int i = 0; i < elementPageList.count() ; i++ ) {
        elementStackedWidget->addWidget(elementPageList.at(i));
    }
    elementStackedWidget->setCurrentIndex(0);

    connect(elementComb, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &OperationElementWidget::slotChangeElementLayout);
}

void OperationElementWidget::slotChangeElementLayout(int ie)
{
    if (ie > 13) ie--;
    if (ie > 10) ie--;
    if (ie > 6) ie--;
    if (ie > 1) ie--;

    elementStackedWidget->setCurrentIndex(ie);
    int id = elementOpMode->checkedId();
    elementPageList.at(ie)->slotChangeAddEditLayout(id);
    UnifiedEditingData::getInstance()->changeCurrentElement(ie);
}

void OperationElementWidget::slotChangeOperationMode(int id)
{
    int ie = elementComb->currentIndex();
    if (ie > 13) ie--;
    if (ie > 10) ie--;
    if (ie > 6) ie--;
    if (ie > 1) ie--;
    elementPageList.at(ie)->slotChangeAddEditLayout(id);
}
} // namespace post3dapp
