#ifndef NAMESELECTDIALOG_H
#define NAMESELECTDIALOG_H

#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>


namespace post3dapp{
class NameSelectDialog : public QDialog
{
    Q_OBJECT

public:

    NameSelectDialog( const QStringList &all, const QStringList &select, QWidget *parent = 0 )
        : QDialog(parent), allList(all), selectedList(select)
    {
        QPushButton *select_button = new QPushButton(u8"全て選択", this);
        QPushButton *deselect_button = new QPushButton(u8"全て解除", this);

        connect( select_button, &QAbstractButton::clicked, this, &NameSelectDialog::selectAllItems );
        connect( deselect_button, &QAbstractButton::clicked, this, &NameSelectDialog::deselectAllItems );

        listWidget = new QListWidget(this);
        listWidget->addItems(allList);
        listWidget->setSelectionMode(QAbstractItemView::MultiSelection);

        for (int i = 0; i < selectedList.count(); i++) {
            QString str = selectedList.at(i);
            if ( allList.contains(str) ) {
                int row = allList.indexOf(str);
                QListWidgetItem *item = listWidget->item(row);
                item->setSelected(true);
            }
        }

        QDialogButtonBox *buttons
            = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
        connect( buttons, &QDialogButtonBox::accepted, this, &NameSelectDialog::accept );
        connect( buttons, &QDialogButtonBox::rejected, this, &NameSelectDialog::reject );


        QHBoxLayout *hlay = new QHBoxLayout;
        hlay->addWidget(select_button);
        hlay->addWidget(deselect_button);

        QVBoxLayout *lay = new QVBoxLayout;
        lay->setContentsMargins(3, 3, 3, 3);
        lay->setSpacing(3);
        lay->addLayout(hlay);
        lay->addWidget(listWidget);
        lay->addWidget(buttons);
        this->setLayout(lay);

        this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
        this->setFont(QFont(QString::fromLocal8Bit("Microsoft JhengHei"), 9));
        this->resize(QSize(150, 400));

    }

    void resetNameList(const QStringList &strlist)
    {
        allList = strlist;
        listWidget->clear();
        listWidget->addItems(allList);
    }

    QStringList selectedNames()
    {
        QList<QListWidgetItem *> items = listWidget->selectedItems();

        QList<int> rowlist;
        Q_FOREACH (QListWidgetItem *item, items)
            rowlist.append( listWidget->row(item) );
        qSort(rowlist.begin(), rowlist.end());

        QStringList strlist;
        Q_FOREACH (int row, rowlist)
            strlist << listWidget->item(row)->text();

        return strlist;
    }

private slots:

    void selectAllItems()
    {
        for (int i = 0; i < listWidget->count(); i++) {
            QListWidgetItem *item = listWidget->item(i);
            item->setSelected(true);
        }
        listWidget->setFocus();
    }

    void deselectAllItems()
    {
        for (int i = 0; i < listWidget->count(); i++) {
            QListWidgetItem *item = listWidget->item(i);
            item->setSelected(false);
        }
    }

    void accept() override
    {
        QDialog::accept();
    }
    void reject() override
    {
        QDialog::reject();
    }

private:

    QStringList allList;
    QStringList selectedList;
    QListWidget *listWidget;
};

} // namespace post3dapp

#endif
