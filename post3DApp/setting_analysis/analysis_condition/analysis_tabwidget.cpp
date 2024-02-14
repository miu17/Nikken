#include "analysis_tabwidget.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QStatusBar>

#include "analysis_plaintextedit.h"
#include "analysiscondition_generalwidget.h"
#include "analysisconditionreader.h"
#include "unified_inputdata.h"
#include "unified_project.h"
#include "utility.h"

namespace post3dapp {

AnalysisTabWidget::AnalysisTabWidget(QWidget *parent)
    :QTabWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setFixedWidth(995);
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    generalWidget = new AnalyisisConditionGeneralWidget();
    scrollArea->setWidget(generalWidget);
    this->addTab(scrollArea, "GENE");

    QVBoxLayout *vlay = new QVBoxLayout();
    textEdit = new AnalysisPlainTextEdit();
    vlay->addWidget(textEdit);
    statusBar = new QStatusBar();
    connect(textEdit, &AnalysisPlainTextEdit::cursorPositionChanged, this, &AnalysisTabWidget::slotColChanged);
    statusBar->setFixedHeight(25);
    statusBar->setContentsMargins(20, 0, 0, 5);
    vlay->addWidget(statusBar);

    QWidget* otherWidget = new QWidget();
    otherWidget->setLayout(vlay);
    this->addTab(otherWidget, "OTHER");
    connect(generalWidget, &AnalyisisConditionGeneralWidget::dataChanged, this, &AnalysisTabWidget::dataChanged);
    connect(textEdit, &QPlainTextEdit::textChanged, this, &AnalysisTabWidget::dataChanged);
}

void AnalysisTabWidget::read(QTextStream &infile)
{
    QStringList gene;
    while(true){
        QString line = infile.readLine();
        if (line == "**ANALYSISCONDITION"){
            break;
        }
        gene.append(line);
    }
    generalWidget->read(AnalysisConditionReader::ReadGeneralAnalysisCondition(gene));

    QString analysisCondition;
    int lineCount = infile.readLine().toInt();
    for (int i = 0; i < lineCount; i++){
        analysisCondition.append(infile.readLine());
        if (i != lineCount - 1)
            analysisCondition.append("\n");
    }
    textEdit->setPlainText(analysisCondition);
}

void AnalysisTabWidget::read(const QString& txt){
    QStringList list = txt.split("\n");

    if (list.count() == 0){
        QString message = u8"empty";
        Utility::showWarningMessage(this, message);
        return;
    }

    QStringList gene;
    int count = 0;
    for(int i = 0; i < list.count(); i++){
        if (list.at(i) == "**ANALYSISCONDITION"){
            count = i + 1;
            break;
        }
        gene.append(list.at(i));
    }

    generalWidget->read(AnalysisConditionReader::ReadGeneralAnalysisCondition(gene));

    QString analysisCondition;
    int lineCount = list.at(count).toInt();
    count++;
    for (int i = count; i < count + lineCount; i++){
        analysisCondition.append(list.at(i));
        if (i != count + lineCount - 1)
            analysisCondition.append("\n");
    }

    textEdit->setPlainText(analysisCondition);
}

void AnalysisTabWidget::read(const QPair<QString, QString>& txt){
    QStringList list = txt.first.split("\n");
    generalWidget->read(AnalysisConditionReader::ReadGeneralAnalysisCondition(list));
    textEdit->setPlainText(txt.second);
}

QString AnalysisTabWidget::write() const
{
    QString txt;
    txt.append("**GENE\n");
    txt.append(generalWidget->write());
    txt.append("**ANALYSISCONDITION").append("\n");
    const QString edit = textEdit->write();
    int count = edit == QString{} ? 0 : edit.split("\n").count();
    txt.append(QString::number(count)).append("\n");
    if (edit != QString{})
        txt.append(edit).append("\n");
    return txt;
}

bool AnalysisTabWidget::compare(const QPair<QString, QString>& txt) const
{
    return generalWidget->write() == txt.first && textEdit->write() == txt.second;
}


void AnalysisTabWidget::slotColChanged(){
    statusBar->showMessage(textEdit->getStatusMessage());
}


void AnalysisTabWidget::modelUpload(){
    UnifiedProject::getInstance()->registerTempAnalyisisData(generalWidget->write(), textEdit->write());
}
void AnalysisTabWidget::modelRewind(){
    this->read(UnifiedProject::getInstance()->getAnalysisCondition());
}

bool AnalysisTabWidget::hasFormatError() const{
    return textEdit->hasFormatError();
}

void AnalysisTabWidget::clearData(){
    generalWidget->setDefault();
    textEdit->setPlainText(QString());
    UnifiedProject::getInstance()->registerTempAnalyisisData(QString(), QString());

}

} // namespace post3dapp

