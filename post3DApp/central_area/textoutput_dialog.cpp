#include <QtGui>
#include <QtWidgets>

#include "figure_template.h"
#include "figure_template_factory.h"
#include "fixed_data.h"
#include "output_control.h"
#include "textoutput_dialog.h"
#include "unified_analysisdata.h"
#include "unified_data.h"
#include "unified_settings.h"

namespace post3dapp{
TextOutputDialog::TextOutputDialog(QWidget *parent)
    : QDialog(parent)
{
    createLayout();

    setFont(QFont(QString::fromLocal8Bit("Microsoft JhengHei"), 9));
    setWindowTitle(u8"解析結果 テキスト表示");
    setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
}

void TextOutputDialog::createLayout()
{
    textEditor = new QTextEdit(this);

    QPushButton *okButton = new QPushButton("O K", this);
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addStretch();
    hlay->addWidget(okButton);

    connect( okButton, &QAbstractButton::clicked, this, &TextOutputDialog::reject );

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->setSpacing(5);
    vlay->addWidget(textEditor, 1);
    vlay->addLayout(hlay, 0);
    this->setLayout(vlay);
}

void TextOutputDialog::setEditorText(const FIGURE_TYPE_PROPERTY& otype, const QList<QUuid> &jdlist,
                                      const QList<QUuid> &mdlist  , const QUuid &id)
{
    textEditor->clear();

    //ElevかPlanかで分岐？
    FigureTemplate *tp;
    UnifiedAnalysisData* adata = UnifiedFixedData::getInstance()->getAnalysisData();
    if (adata->isExistedID(id,DATATYPE::TPFRAMEP)){
        tp = FigureTemplateFactory::CreateElevTemplate(UnifiedSettings::getInstance()->getOutput2DViewSettings(), otype,id);
    }else if (adata->isExistedID(id,DATATYPE::TPFLOOR)){
        tp = FigureTemplateFactory::CreatePlanTemplate(UnifiedSettings::getInstance()->getOutput2DViewSettings(), otype,id);
    }else{
        qDebug()<<"TextOutputDialog::unknown FigureTemplate";
        return;
    }
    QString str;
    str += u8"選択部材数　　　：　";
    str += QString::number(mdlist.count());
    str += u8" , 選択節点数　　　：　";
    str += QString::number(jdlist.count());
    str += u8"\n\n";

    for(int nm=0;nm<mdlist.count();nm++){
        str += tp->getMemberInfo(mdlist.at(nm));
        str += u8"\n\n";
    }
    for(int nj=0;nj<jdlist.count();nj++){
        str += tp->getJointInfo(jdlist.at(nj));
        str += u8"\n\n";
    }

    textEditor->setPlainText(str);
}

} // namespace post3dapp
