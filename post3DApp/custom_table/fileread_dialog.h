#ifndef FILEREADDIALOG_H
#define FILEREADDIALOG_H

#include <QButtonGroup>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QVBoxLayout>

namespace post3dapp{
class FileReadDialog : public QDialog
{
    Q_OBJECT

public:

    FileReadDialog( QWidget *parent = 0 ) : QDialog(parent)
    {

        oldDataGroupBox = new QGroupBox(u8"読込み方法の選択");
        overwriteButton = new QRadioButton(u8"既存データを破棄して上書きする");
        appendButton = new QRadioButton(u8"既存データの後ろに追加書きする");
        overwriteButton->setChecked(true);
        QVBoxLayout *vlay1 = new QVBoxLayout;
        vlay1->addWidget(overwriteButton);
        vlay1->addWidget(appendButton);
        oldDataGroupBox->setLayout(vlay1);
        /*
                newDataGroupBox=new QGroupBox(tr("読込みデータの扱い"));
                unregisteredButton=new QRadioButton(tr("未登録データとして読み込む"));
                registeredButton=new QRadioButton(tr("登録データとしてして読み込む"));
                unregisteredButton->setChecked(true);
                QVBoxLayout *vlay2=new QVBoxLayout;
                vlay2->addWidget(unregisteredButton);
                vlay2->addWidget(registeredButton);
                newDataGroupBox->setLayout(vlay2);
        */
        QDialogButtonBox *buttons
            = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
        connect( buttons, &QDialogButtonBox::accepted, this, &FileReadDialog::accept );
        connect( buttons, &QDialogButtonBox::rejected, this, &FileReadDialog::reject );

        QVBoxLayout *vlay = new QVBoxLayout;
        vlay->addWidget(oldDataGroupBox);
//        vlay->addWidget(newDataGroupBox);
        vlay->addStretch();
        vlay->addWidget(buttons);
        this->setLayout(vlay);

        this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
        this->setFont(QFont(tr("Microsoft JhengHei"), 9));
        this->setWindowTitle(u8"ファイルの読込み");
        this->setWindowIcon(QIcon(":/icons/c2_yellow.png"));
        this->resize(QSize(300, 150));

    }

    bool overWriteButtonState()
    {
        return overwriteButton->isChecked();
    }
//    bool registeredButtonState(){ return registeredButton->isChecked(); }

private slots:

    void accept()
    {
        QDialog::accept();
    }
    void reject()
    {
        QDialog::reject();
    }

private:

    QGroupBox *oldDataGroupBox;
    QRadioButton *overwriteButton;
    QRadioButton *appendButton;
//    QGroupBox* newDataGroupBox;
//    QRadioButton* unregisteredButton;
//    QRadioButton* registeredButton;
};
} // namespace post3dapp


#endif
