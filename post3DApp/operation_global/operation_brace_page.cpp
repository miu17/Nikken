#include "operation_brace_page.h"

#include <QtGui>

#include "custom_checkbutton.h"
#include "reference_combobox.h"

namespace post3dapp{
OperationBracePage::OperationBracePage( QWidget *parent )
    : AbstractElementPage( parent )
{
    myElementType = ELBRACE;

    ReferenceCombobox *fugo_combo = new ReferenceCombobox(DATATYPE::TPBRACE, UnifiedDataType::Input, this);
    fugo_combo->setMinimumWidth(125);
    connect(fugo_combo, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
            this, &AbstractElementPage::signalCurrentInputName);

    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addWidget(new QLabel(u8"・部材符号：", this));
    hlay1->addWidget(fugo_combo);


    /* 追加　レイアウト */
    QVBoxLayout *vlay1 = new QVBoxLayout;
    vlay1->setContentsMargins(0, 0, 0, 0);
    vlay1->addLayout(hlay1);
    vlay1->addSpacing(10);
    vlay1->addWidget(new SnapJointWidget(this));
    vlay1->addSpacing(10);
    vlay1->addWidget(new AddMoveWidget(this));
    vlay1->addSpacing(10);
    vlay1->addWidget(new AddedMovedWidget(this));
    vlay1->addStretch();

    myAddWidget->setLayout(vlay1);

    /* 編集　レイアウト */
    QVBoxLayout *vlay2 = new QVBoxLayout;
    vlay2->setContentsMargins(0, 0, 0, 0);
    vlay2->addWidget(new SnapJointWidget(this));
    vlay2->addSpacing(10);
    vlay2->addWidget(new AddMoveWidget(this));
    vlay2->addSpacing(10);
    vlay2->addWidget(new AddedMovedWidget(this));
    vlay2->addSpacing(10);
    vlay2->addWidget(new DeleteWidget(this));
    vlay2->addStretch();

    myEditWidget->setLayout(vlay2);
}

void OperationBracePage::slotChangeAddEditLayout(int id)
{
    addEditWidget->setCurrentIndex(id);
    (id == 0) ? signalCurrentMode(POINTHITMODE) :
    signalCurrentMode(EDITMODE);
}
} // namespace post3dapp
