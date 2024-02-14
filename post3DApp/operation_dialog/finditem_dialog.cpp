#include "finditem_dialog.h"

#include <QtGui>

#include "factory_unifieddata.h"
#include "unified_data.h"
#include "unified_editingdata.h"
#include "vhline.h"

namespace post3dapp{
FindItemDialog::FindItemDialog( DATATYPE dtype, QWidget *parent,UnifiedDataType isout )
    : QDialog(parent), isOutput(isout)
{
    createLayout();

    if ( dtype != DATATYPE::TPJOINT && dtype != DATATYPE::NODATATYPE ) {
        mtp_check->setChecked(true);
        int idx = 0;
        for ( int i = static_cast<int>(DATATYPE::TPCOLUMN); i <= static_cast<int>(DATATYPE::TPSLABLOAD); i++ ) {
            if ( static_cast<DATATYPE>(i) == DATATYPE::TPBASE || static_cast<DATATYPE>(i) == DATATYPE::TPWALLOPEN ) continue;
            if ( static_cast<DATATYPE>(i) == dtype ) break;
            idx++;
        }
        mtp_comb->setCurrentIndex(idx);
    }

    setFont(QFont(tr("Microsoft JhengHei"), 9));
    setWindowTitle(u8"検索");
    setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
}

void FindItemDialog::createLayout()
{
    // 節点　ラベル
    QHBoxLayout *hlay_joint = new QHBoxLayout;
    hlay_joint->setContentsMargins(0, 0, 0, 0);
    hlay_joint->addWidget(new QLabel(u8"【 節点の検索 】", this));
    hlay_joint->addStretch();

    // 節点　節点番号

    jno_check = new QCheckBox(u8"節点番号", this);
    jno_check->setFixedWidth(80);
    jno_line1 = new QLineEdit(this);
    jno_line1->setFixedWidth(60);
    jno_line2 = new QLineEdit(this);
    jno_line2->setFixedWidth(60);
    QLabel *jno_label = new QLabel("-", this);

    jno_line1->setEnabled(false);
    jno_line2->setEnabled(false);
    jno_label->setEnabled(false);

    connect( jno_check, &QAbstractButton::toggled, jno_line1, &QWidget::setEnabled );
    connect( jno_check, &QAbstractButton::toggled, jno_line2, &QWidget::setEnabled );
    connect( jno_check, &QAbstractButton::toggled, jno_label, &QWidget::setEnabled );

    QHBoxLayout *hlay_jno = new QHBoxLayout;
    hlay_jno->setContentsMargins(0, 0, 0, 0);
    hlay_jno->addWidget(jno_check);
    hlay_jno->addWidget(jno_line1);
    hlay_jno->addWidget(jno_label);
    hlay_jno->addWidget(jno_line2);
    hlay_jno->addStretch();

    // 節点　所属階

    jfl_check = new QCheckBox(u8"所属階", this);
    jfl_check->setFixedWidth(80);
    jfl_line = new QLineEdit(this);
    jfl_line->setFixedWidth(100);

    jfl_line->setEnabled(false);

    connect( jfl_check, &QAbstractButton::toggled, jfl_line, &QWidget::setEnabled );

    QHBoxLayout *hlay_jfl = new QHBoxLayout;
    hlay_jfl->setContentsMargins(0, 0, 0, 0);
    hlay_jfl->addWidget(jfl_check);
    hlay_jfl->addWidget(jfl_line);
    hlay_jfl->addStretch();

    // 節点　所属通り

    jfr_check = new QCheckBox(u8"所属通り", this);
    jfr_check->setFixedWidth(80);
    jfr_line = new QLineEdit(this);
    jfr_line->setFixedWidth(100);

    jfr_line->setEnabled(false);

    connect( jfr_check, &QAbstractButton::toggled, jfr_line, &QWidget::setEnabled );

    QHBoxLayout *hlay_jfr = new QHBoxLayout;
    hlay_jfr->setContentsMargins(0, 0, 0, 0);
    hlay_jfr->addWidget(jfr_check);
    hlay_jfr->addWidget(jfr_line);
    hlay_jfr->addStretch();

    // 部材　ラベル
    QHBoxLayout *hlay_member = new QHBoxLayout;
    hlay_member->setContentsMargins(0, 0, 0, 0);
    hlay_member->addWidget(new QLabel(u8"【 部材・荷重の検索 】", this));
    hlay_member->addStretch();

    // 部材　種類

    mtp_check = new QCheckBox(u8"種類", this);
    mtp_check->setFixedWidth(80);
    mtp_comb = new QComboBox(this);
    mtp_comb->setStyle( UnifiedEditingData::getInstance()->getWindowXPStyle() );
    mtp_comb->setFixedWidth(100);

    for ( int i = static_cast<int>(DATATYPE::TPCOLUMN); i <= static_cast<int>(DATATYPE::TPSLABLOAD); i++ ) {
        if ( static_cast<DATATYPE>(i) == DATATYPE::TPBASE || static_cast<DATATYPE>(i) == DATATYPE::TPWALLOPEN ) continue;
        mtp_comb->addItem( FactoryUnifiedData::getInstance(isOutput)->dataTypeToJapanese( static_cast<DATATYPE>(i) ) );
    }

    mtp_comb->setEnabled(false);

    connect( mtp_check, &QAbstractButton::toggled, mtp_comb, &QWidget::setEnabled );

    QHBoxLayout *hlay_mtp = new QHBoxLayout;
    hlay_mtp->setContentsMargins(0, 0, 0, 0);
    hlay_mtp->addWidget(mtp_check);
    hlay_mtp->addWidget(mtp_comb);
    hlay_mtp->addStretch();

    // 部材　符号

    mst_check = new QCheckBox(u8"符号", this);
    mst_check->setFixedWidth(80);
    mst_line = new QLineEdit(this);
    mst_line->setFixedWidth(100);
    QButtonGroup *mst_group = new QButtonGroup(this);
    mst_all = new QRadioButton(u8"完全一致", this);
    mst_all->setFixedWidth(80);
    QRadioButton *mst_part = new QRadioButton(u8"部分一致", this);
    mst_part->setFixedWidth(80);
    mst_group->addButton(mst_all);
    mst_group->addButton(mst_part);
    mst_all->setChecked(true);

    mst_line->setEnabled(false);
    mst_all->setEnabled(false);
    mst_part->setEnabled(false);

    connect( mst_check, &QAbstractButton::toggled, mst_line, &QWidget::setEnabled );
    connect( mst_check, &QAbstractButton::toggled, mst_all,  &QWidget::setEnabled );
    connect( mst_check, &QAbstractButton::toggled, mst_part, &QWidget::setEnabled );

    QHBoxLayout *hlay_mst = new QHBoxLayout;
    hlay_mst->setContentsMargins(0, 0, 0, 0);
    hlay_mst->addWidget(mst_check);
    hlay_mst->addWidget(mst_line);
    hlay_mst->addSpacing(15);
    hlay_mst->addWidget(mst_all);
    hlay_mst->addWidget(mst_part);
    hlay_mst->addStretch();

    // 部材　所属階

    mfl_check = new QCheckBox(u8"所属階", this);
    mfl_check->setFixedWidth(80);
    mfl_line = new QLineEdit(this);
    mfl_line->setFixedWidth(100);
    QButtonGroup *mfl_group = new QButtonGroup(this);
    mfl_all = new QRadioButton(u8"全体所属", this);
    mfl_all->setFixedWidth(80);
    QRadioButton *mfl_part = new QRadioButton(u8"部分所属", this);
    mfl_part->setFixedWidth(80);
    mfl_group->addButton(mfl_all);
    mfl_group->addButton(mfl_part);
    mfl_all->setChecked(true);

    mfl_line->setEnabled(false);
    mfl_all->setEnabled(false);
    mfl_part->setEnabled(false);

    connect( mfl_check, &QAbstractButton::toggled, mfl_line, &QWidget::setEnabled );
    connect( mfl_check, &QAbstractButton::toggled, mfl_all, &QWidget::setEnabled );
    connect( mfl_check, &QAbstractButton::toggled, mfl_part, &QWidget::setEnabled );

    QHBoxLayout *hlay_mfl = new QHBoxLayout;
    hlay_mfl->setContentsMargins(0, 0, 0, 0);
    hlay_mfl->addWidget(mfl_check);
    hlay_mfl->addWidget(mfl_line);
    hlay_mfl->addSpacing(15);
    hlay_mfl->addWidget(mfl_all);
    hlay_mfl->addWidget(mfl_part);
    hlay_mfl->addStretch();

    // 部材　所属通り

    mfr_check = new QCheckBox(u8"所属通り", this);
    mfr_check->setFixedWidth(80);
    mfr_line = new QLineEdit(this);
    mfr_line->setFixedWidth(100);
    QButtonGroup *mfr_group = new QButtonGroup(this);
    mfr_all = new QRadioButton(u8"全体所属", this);
    mfr_all->setFixedWidth(80);
    QRadioButton *mfr_part = new QRadioButton(u8"部分所属", this);
    mfr_part->setFixedWidth(80);
    mfr_group->addButton(mfr_all);
    mfr_group->addButton(mfr_part);
    mfr_all->setChecked(true);

    mfr_line->setEnabled(false);
    mfr_all->setEnabled(false);
    mfr_part->setEnabled(false);

    connect( mfr_check, &QAbstractButton::toggled, mfr_line, &QWidget::setEnabled );
    connect( mfr_check, &QAbstractButton::toggled, mfr_all,  &QWidget::setEnabled );
    connect( mfr_check, &QAbstractButton::toggled, mfr_part, &QWidget::setEnabled );

    QHBoxLayout *hlay_mfr = new QHBoxLayout;
    hlay_mfr->setContentsMargins(0, 0, 0, 0);
    hlay_mfr->addWidget(mfr_check);
    hlay_mfr->addWidget(mfr_line);
    hlay_mfr->addSpacing(15);
    hlay_mfr->addWidget(mfr_all);
    hlay_mfr->addWidget(mfr_part);
    hlay_mfr->addStretch();

    // OK CANCEL

    QPushButton *okButton = new QPushButton(u8"検索", this);
    QPushButton *cancelButton = new QPushButton(u8"キャンセル", this);

    connect( okButton, &QAbstractButton::clicked, this, &QDialog::accept );
    connect( cancelButton, &QAbstractButton::clicked, this, &QDialog::reject );

    QHBoxLayout *hlay2 = new QHBoxLayout;
    hlay2->addStretch();
    hlay2->addWidget(okButton);
    hlay2->addWidget(cancelButton);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setSpacing(5);
    vlay->addLayout(hlay_joint);
    vlay->addSpacing(5);
    vlay->addLayout(hlay_jno);
    vlay->addLayout(hlay_jfl);
    vlay->addLayout(hlay_jfr);
    vlay->addSpacing(5);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addSpacing(5);
    vlay->addLayout(hlay_member);
    vlay->addSpacing(5);
    vlay->addLayout(hlay_mtp);
    vlay->addLayout(hlay_mst);
    vlay->addLayout(hlay_mfl);
    vlay->addLayout(hlay_mfr);
    vlay->addStretch();
    vlay->addSpacing(5);
    vlay->addWidget(new HFLine(Qt::lightGray, this));
    vlay->addSpacing(5);
    vlay->addLayout(hlay2);

    this->setLayout(vlay);
}

FINDDIALOGSTATUS FindItemDialog::dialogStatus() const
{
    FINDDIALOGSTATUS status(jno_check->isChecked(), jfl_check->isChecked(), jfr_check->isChecked(),
                            mtp_check->isChecked(), mst_check->isChecked(),
                            mfl_check->isChecked(), mfr_check->isChecked());

    if ( status.isJnoChecked ) {
        status.jnoFrom = jno_line1->text().toInt();
        status.jnoTo = ( jno_line2->text().isEmpty() ) ? status.jnoFrom : jno_line2->text().toInt();
    }

    if ( status.isJflChecked ) {
        QList<QUuid> floors;
        Q_FOREACH ( QString str, jfl_line->text().split(',') ) {
            floors.append( FactoryUnifiedData::getInstance(isOutput)->nameToID(str, DATATYPE::TPFLOOR) );
        }
        status.jFloors = floors;
    }
    if ( status.isJfrChecked ) {
        QList<QUuid> frames;
        Q_FOREACH ( QString str, jfr_line->text().split(',') ) {
            frames.append( FactoryUnifiedData::getInstance(isOutput)->nameToID(str, DATATYPE::TPFRAMEP) );
        }
        status.jFrames = frames;
    }
    if ( status.isMtpChecked ) {
        status.dataType = FactoryUnifiedData::getInstance(isOutput)->stringToDataType( mtp_comb->currentText() );
    }
    if ( status.isMstChecked ) {
        status.memberName = mst_line->text();
        status.mstMatch = mst_all->isChecked();
    }
    if ( status.isMflChecked ) {
        QList<QUuid> floors;
        Q_FOREACH ( QString str, mfl_line->text().split(',') ) {
            floors.append( FactoryUnifiedData::getInstance(isOutput)->nameToID(str, DATATYPE::TPFLOOR) );
        }
        status.mFloors = floors;
        status.mflMatch = mfl_all->isChecked();
    }
    if ( status.isMfrChecked ) {
        QList<QUuid> frames;
        Q_FOREACH ( QString str, mfr_line->text().split(',') ) {
            frames.append( FactoryUnifiedData::getInstance(isOutput)->nameToID(str, DATATYPE::TPFRAMEP) );
        }
        status.mFrames = frames;
        status.mfrMatch = mfr_all->isChecked();
    }

    return status;
}

QString FindItemDialog::checkDialogInput() const
{
    bool ok = true;
    if ( jno_check->isChecked() ) {
        int j1 = jno_line1->text().toInt(&ok);
        int j2 = ( ok && jno_line2->text().isEmpty() ) ? j1 : jno_line2->text().toInt(&ok);
        if ( !ok ) {
            return u8"節点番号の指定が正しくありません。";
        }
        if ( j1 > j2 ) {
            return u8"節点番号の指定範囲が正しくありません。";
        }
    }
    if ( jfl_check->isChecked() ) {
        QStringList strlist = jfl_line->text().split(',');
        Q_FOREACH ( QString str, strlist ) {
            if ( !FactoryUnifiedData::getInstance(isOutput)->isExistedName(str, DATATYPE::TPFLOOR) ) {
                ok = false;
                break;
            }
        }
        if ( !ok ) {
            return u8"節点の所属階の指定が正しくありません。";
        }
    }
    if ( jfr_check->isChecked() ) {
        QStringList strlist = jfr_line->text().split(',');
        Q_FOREACH ( QString str, strlist ) {
            if ( !FactoryUnifiedData::getInstance(isOutput)->isExistedName(str, DATATYPE::TPFRAMEP) ) {
                ok = false;
                break;
            }
        }
        if ( !ok ) {
            return u8"節点の所属通りの指定が正しくありません。";
        }
    }
    if ( mfl_check->isChecked() ) {
        QStringList strlist = mfl_line->text().split(',');
        Q_FOREACH ( QString str, strlist ) {
            if ( !FactoryUnifiedData::getInstance(isOutput)->isExistedName(str, DATATYPE::TPFLOOR) ) {
                ok = false;
                break;
            }
        }
        if ( !ok ) {
            return u8"部材・荷重の所属階の指定が正しくありません。";
        }
    }
    if ( mfr_check->isChecked() ) {
        QStringList strlist = mfr_line->text().split(',');
        Q_FOREACH ( QString str, strlist ) {
            if ( !FactoryUnifiedData::getInstance(isOutput)->isExistedName(str, DATATYPE::TPFRAMEP) ) {
                ok = false;
                break;
            }
        }
        if ( !ok ) {
            return u8"部材・荷重の所属通りの指定が正しくありません。";
        }
    }
    return QString();
}
} // namespace post3dapp
