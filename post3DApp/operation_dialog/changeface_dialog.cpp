#include "changeface_dialog.h"

#include <QtGui>

namespace post3dapp{
ChangeFaceDialog::ChangeFaceDialog(int ii, const QString &istr,
                                   int jj, const QString &jstr,
                                   QWidget *parent)
    : QDialog(parent)
{
    createLayout();

    ifaceRuleGroup->button(ii)->setChecked(true);
    enableIFaceLineEdit(ii);
    ifaceLine->setText(istr);
    jfaceRuleGroup->button(jj)->setChecked(true);
    enableJFaceLineEdit(jj);
    jfaceLine->setText(jstr);

    setFont(QFont(tr("Microsoft JhengHei"), 9));
    setWindowTitle(u8"フェイス長さの変更");
    setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
}

void ChangeFaceDialog::createLayout()
{
    QLabel *ifaceRuleLabel = new QLabel(u8"i端 フェイス長さルール:", this);
    QRadioButton *ifaceAuto = new QRadioButton(u8"自動", this);
    QRadioButton *ifaceManual = new QRadioButton(u8"手動", this);
    QRadioButton *ifaceIrregular = new QRadioButton(u8"不定", this);
    ifaceRuleGroup = new QButtonGroup;
    ifaceRuleGroup->addButton(ifaceAuto, 0);
    ifaceRuleGroup->addButton(ifaceManual, 1);
    ifaceRuleGroup->addButton(ifaceIrregular, 2);
    ifaceRuleGroup->button(2)->setChecked(true);

    QLabel *ifaceLabel = new QLabel(u8"i端 フェイス長さ(mm):", this);
    ifaceLine = new QLineEdit(this);
    ifaceLine->setValidator(new QDoubleValidator(-99999.999, 99999.999, 3, this));
    ifaceLine->setAlignment(Qt::AlignRight);
    ifaceLine->setEnabled(false);

    connect(ifaceRuleGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &ChangeFaceDialog::enableIFaceLineEdit);

    QLabel *jfaceRuleLabel = new QLabel(u8"j端 フェイス長さルール:", this);
    QRadioButton *jfaceAuto = new QRadioButton(u8"自動", this);
    QRadioButton *jfaceManual = new QRadioButton(u8"手動", this);
    QRadioButton *jfaceIrregular = new QRadioButton(u8"不定", this);
    jfaceRuleGroup = new QButtonGroup;
    jfaceRuleGroup->addButton(jfaceAuto, 0);
    jfaceRuleGroup->addButton(jfaceManual, 1);
    jfaceRuleGroup->addButton(jfaceIrregular, 2);
    jfaceRuleGroup->button(2)->setChecked(true);

    QLabel *jfaceLabel = new QLabel(u8"j端 フェイス長さ(mm):", this);
    jfaceLine = new QLineEdit(this);
    jfaceLine->setValidator(new QDoubleValidator(-99999.999, 99999.999, 3, this));
    jfaceLine->setAlignment(Qt::AlignRight);
    jfaceLine->setEnabled(false);

    connect(jfaceRuleGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &ChangeFaceDialog::enableJFaceLineEdit);

    QGridLayout *glay = new QGridLayout;
    glay->setContentsMargins(0, 0, 0, 0);
    glay->setSpacing(2);
    glay->addWidget(ifaceRuleLabel, 0, 0, 1, 1);
    glay->addWidget(ifaceAuto, 0, 1, 1, 1);
    glay->addWidget(ifaceManual, 0, 2, 1, 1);
    glay->addWidget(ifaceIrregular, 0, 3, 1, 1);
    glay->addWidget(ifaceLabel, 1, 0, 1, 1);
    glay->addWidget(ifaceLine, 1, 1, 1, 3);
    glay->addWidget(jfaceRuleLabel, 2, 0, 1, 1);
    glay->addWidget(jfaceAuto, 2, 1, 1, 1);
    glay->addWidget(jfaceManual, 2, 2, 1, 1);
    glay->addWidget(jfaceIrregular, 2, 3, 1, 1);
    glay->addWidget(jfaceLabel, 3, 0, 1, 1);
    glay->addWidget(jfaceLine, 3, 1, 1, 3);

    QPushButton *okButton = new QPushButton(u8"変　更", this);
    QPushButton *cancelButton = new QPushButton(u8"キャンセル", this);

    connect( okButton, &QAbstractButton::clicked, this, &QDialog::accept );
    connect( cancelButton, &QAbstractButton::clicked, this, &QDialog::reject );

    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addStretch();
    hlay->addWidget(okButton);
    hlay->addWidget(cancelButton);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addLayout(glay);
    vlay->addLayout(hlay);
    vlay->addStretch();
    this->setLayout(vlay);

}


void ChangeFaceDialog::enableIFaceLineEdit(int id)
{
    ifaceLine->setEnabled(id == 1);
}

void ChangeFaceDialog::enableJFaceLineEdit(int id)
{
    jfaceLine->setEnabled(id == 1);
}

int ChangeFaceDialog::ifaceRuleCheckedID() const
{
    return ifaceRuleGroup->checkedId();
}

int ChangeFaceDialog::jfaceRuleCheckedID() const
{
    return jfaceRuleGroup->checkedId();
}

QString ChangeFaceDialog::ifaceValueText() const
{
    return ifaceLine->text();
}

QString ChangeFaceDialog::jfaceValueText() const
{
    return jfaceLine->text();
}
} // namespace post3dapp
