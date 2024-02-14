#ifndef FINDITEMDIALOG_H
#define FINDITEMDIALOG_H

#include <QtWidgets>
#include <QDialog>

#include "factory_unifieddata.h"

class QLineEdit;
class QCheckBox;
class QRadioButton;
class QComboBox;
namespace post3dapp{
struct FINDDIALOGSTATUS {
    explicit FINDDIALOGSTATUS(
        bool _jno_ck = false,
        bool _jfl_ck = false,
        bool _jfr_ck = false,
        bool _mtp_ck = false,
        bool _mst_ck = false,
        bool _mfl_ck = false,
        bool _mfr_ck = false,
        int _jno_j1 = -1,
        int _jno_j2 = -1,
        const QList<QUuid> &_jfl_st = QList<QUuid>(),
        const QList<QUuid> &_jfr_st = QList<QUuid>(),
        DATATYPE _mtp_st = DATATYPE::NODATATYPE,
        const QString &_mst_st = QString(),
        bool _mst_rd = true,
        const QList<QUuid> &_mfl_st = QList<QUuid>(),
        bool _mfl_rd = true,
        const QList<QUuid> &_mfr_st = QList<QUuid>(),
        bool _mfr_rd = true )
        : isJnoChecked(_jno_ck), isJflChecked(_jfl_ck), isJfrChecked(_jfr_ck),
          isMtpChecked(_mtp_ck), isMstChecked(_mst_ck),
          isMflChecked(_mfl_ck), isMfrChecked(_mfr_ck),
          jnoFrom(_jno_j1), jnoTo(_jno_j2), jFloors(_jfl_st), jFrames(_jfr_st),
          dataType(_mtp_st), memberName(_mst_st), mstMatch(_mst_rd),
          mFloors(_mfl_st), mflMatch(_mfl_rd), mFrames(_mfr_st), mfrMatch(_mfr_rd) { }

    bool isJnoChecked;
    bool isJflChecked;
    bool isJfrChecked;

    bool isMtpChecked;
    bool isMstChecked;
    bool isMflChecked;
    bool isMfrChecked;

    int jnoFrom;
    int jnoTo;
    QList<QUuid> jFloors;
    QList<QUuid> jFrames;
    DATATYPE dataType;
    QString memberName;
    bool mstMatch;
    QList<QUuid> mFloors;
    bool mflMatch;
    QList<QUuid> mFrames;
    bool mfrMatch;
};

class FindItemDialog : public QDialog
{

    Q_OBJECT

public:

    FindItemDialog(DATATYPE dtype, QWidget *parent = nullptr , bool isout = false);

    FINDDIALOGSTATUS dialogStatus() const;

    QString checkDialogInput() const;

private:

    QCheckBox *jno_check;
    QLineEdit *jno_line1;
    QLineEdit *jno_line2;

    QCheckBox *jfl_check;
    QLineEdit *jfl_line;

    QCheckBox *jfr_check;
    QLineEdit *jfr_line;

    QCheckBox *mtp_check;
    QComboBox *mtp_comb;

    QCheckBox *mst_check;
    QLineEdit *mst_line;
    QRadioButton *mst_all;

    QCheckBox *mfl_check;
    QLineEdit *mfl_line;
    QRadioButton *mfl_all;

    QCheckBox *mfr_check;
    QLineEdit *mfr_line;
    QRadioButton *mfr_all;

    void createLayout();

    const bool isOutput;

};
} // namespace post3dapp


#endif
