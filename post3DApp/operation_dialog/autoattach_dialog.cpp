#include "autoattach_dialog.h"

#include <QtGui>

namespace post3dapp{
AutoAttachDialog::AutoAttachDialog( QWidget *parent )
    : QDialog(parent)
{
    createLayout();

    setFont(QFont(tr("Microsoft JhengHei"), 9));
    setWindowTitle(u8"属性の上書き／追加の確認");
    setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
}

void AutoAttachDialog::createLayout()
{
    QRadioButton *yesButton = new QRadioButton(u8"現在の属性をクリアして上書きする。", this);
    QRadioButton *noButton = new QRadioButton(u8"現在の属性を維持したまま追加する。", this);
    yesButton->setChecked(true);

    overwriteGroup = new QButtonGroup;
    overwriteGroup->addButton(yesButton, 0);
    overwriteGroup->addButton(noButton, 1);

    QVBoxLayout *vlay_bg = new QVBoxLayout;
    vlay_bg->addWidget(yesButton);
    vlay_bg->addWidget(noButton);

    QPushButton *okButton = new QPushButton(u8"実　行", this);
    QPushButton *cancelButton = new QPushButton(u8"キャンセル", this);

    connect( okButton, &QAbstractButton::clicked, this, &QDialog::accept );
    connect( cancelButton, &QAbstractButton::clicked, this, &QDialog::reject );

    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addStretch();
    hlay->addWidget(okButton);
    hlay->addWidget(cancelButton);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addLayout(vlay_bg);
    vlay->addLayout(hlay);
    vlay->addStretch();

    this->setLayout(vlay);

}
} // namespace post3dapp
