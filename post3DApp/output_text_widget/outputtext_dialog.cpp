#include "outputtext_dialog.h"

#include <QBoxLayout>

#include "app_version.h"
#include "outputtext_tabwidget.h"

namespace post3dapp{

OutputTextDialog::OutputTextDialog(QWidget *parent)
    :QDialog(parent)
{
    this->setWindowTitle(u8"任意テキスト出力");
    this->setWindowFlags(Qt::Dialog
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint);
    this->setFont(QFont(tr("Microsoft JhengHei"), 10));
    this->setGeometry(0, 0, 500, 500);
    QVBoxLayout *lay = new QVBoxLayout(this);
    tabWidget = new OutputTextTabWidget();
    tabWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    lay->addWidget(tabWidget);
    this->setLayout(lay);
}

void OutputTextDialog::writeBinaryFile(QDataStream &outfile) const{
    tabWidget->writeBinaryFile(outfile);
}
void OutputTextDialog::readBinaryFile(QDataStream &infile, const AppVersion& version){
    tabWidget->readBinaryFile(infile, version);
}


} // namespace post3dapp
