#include "outputtext_tabwidget.h"

#include "abstracttext.h"
#include "app_version.h"
#include "fixed_data.h"
#include "outputtextparser.h"
#include "outputtext_conditionwidget.h"
#include "outputtext_plaintextedit.h"
#include "outputtext_reader.h"
#include "outputtext_resultwidget.h"
#include "unified_analysisdata.h"

namespace post3dapp {

OutputTextTabWidget::OutputTextTabWidget(QWidget *parent)
    :QTabWidget(parent)
{
    //this->setMinimumWidth(OutputTextResultWidget::ConditionFixWidth + OutputTextResultWidget::ResultMinimumWidth);
    condition = new OutputTextConditionWidget();
    this->addTab(condition, "condition");

    connect(condition->runButton, &QPushButton::clicked, this, &OutputTextTabWidget::slotRunButtonClicked);

}

void OutputTextTabWidget::slotRunButtonClicked(){
    for (int i = this->count(); 0 < i; i--){
        this->removeTab(i);
    }
    result = new OutputTextResultWidget("result");

    QTextEdit* column = nullptr;
    QTextEdit* girder = nullptr;
    QTextEdit* brace = nullptr;
    QTextEdit* wall = nullptr;

    OutputTextReader reader;
    /*
    auto parseCard = reader.parse(condition->textEdit->toPlainText().split("\n"));
    OutputTextParser parser{*UnifiedFixedData::getInstance()->getPstnData()};
    auto text = parser.parse(parseCard);

    Q_FOREACH(auto list, text){
        auto command = list.first;
        auto ret = list.second;
        result->appendResult(ret.first, ret.second);
        if (command.startsWith(LineOutputText::outputTextStatusToCommand(OutputTextStatus::Chck_ColumnCheck))){
            column = new QTextEdit();
            column->setFont(QFont(u8"ＭＳ 明朝"));
            column->setPlainText(ret.second);
        }
        if (command.startsWith(LineOutputText::outputTextStatusToCommand(OutputTextStatus::Chck_GirderCheck))){
            girder = new QTextEdit();
            girder->setPlainText(ret.second);
        }
        if (command.startsWith(LineOutputText::outputTextStatusToCommand(OutputTextStatus::Chck_BraceCheck))){
            brace = new QTextEdit();
            brace->setPlainText(ret.second);
        }
        if (command.startsWith(LineOutputText::outputTextStatusToCommand(OutputTextStatus::Chck_WallCheck))){
            wall = new QTextEdit();
            wall->setPlainText(ret.second);
        }
    }
    */
    this->addTab(result, result->getLabel());
    if (column != nullptr) this->addTab(column, "Column");
    if (girder != nullptr) this->addTab(girder, "Girder");
    if (brace != nullptr) this->addTab(brace, "Brace");
    if (wall != nullptr) this->addTab(wall, "Wall");

    this->setCurrentWidget(result);
    result->setCursorTop();
}

void OutputTextTabWidget::writeBinaryFile(QDataStream &outfile) const{
    condition->writeBinaryFile(outfile);
}
void OutputTextTabWidget::readBinaryFile(QDataStream &infile, const AppVersion &version){
    condition->readBinaryFile(infile, version);
}


} // namespace post3dapp
