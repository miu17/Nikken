#include "outputtext_resultwidget.h"

#include <QBoxLayout>
#include <QTextEdit>
#include <QScrollArea>

#include "abstracttext.h"
namespace post3dapp{

ClickableLabel::ClickableLabel(const QString& text, QWidget* parent)
    : QLabel(text, parent)
{}

void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
    emit clicked(this->text());
}

ClickableLabel::~ClickableLabel()
{}

OutputTextResultWidget::OutputTextResultWidget(const QString& _label, QWidget* parent)
    :label(_label), QWidget(parent)
{
    this->setFont(QFont(tr("Microsoft JhengHei"), 10));
    QHBoxLayout *lay = new QHBoxLayout();
    QScrollArea* area = new QScrollArea();
    area->setFixedWidth(ConditionFixWidth);
    lay->addWidget(area);
    QWidget* bookmark = new QWidget();
    area->setWidget(bookmark);
    vlay = new QVBoxLayout(bookmark);
    vlay->setSizeConstraint(QLayout::SetMinAndMaxSize);
    vlay->addStretch();
    textBrowser = new QTextEdit();
    textBrowser->setFont(QFont(u8"ＭＳ 明朝"));
    textBrowser->setMinimumWidth(ResultMinimumWidth);
    lay->addWidget(textBrowser);

    // textBrowser = new QPlainTextEdit();
    // QFont osaka = QFontDatabase().font("Osaka", "Regular-Mono", 20);
    // textBrowser->setFont(osaka);

    this->setLayout(lay);
}

int OutputTextResultWidget::ConditionFixWidth = 480;
int OutputTextResultWidget::ResultMinimumWidth = 900;


void OutputTextResultWidget::slotJump(const QString& label) const{
    textBrowser->scrollToAnchor(label);
}

ClickableLabel* OutputTextResultWidget::createLabel(const QString& label){
    ClickableLabel* clickableLabel = new ClickableLabel(label);
    connect(clickableLabel, &ClickableLabel::clicked, this, &OutputTextResultWidget::slotJump);
    return clickableLabel;
}

void OutputTextResultWidget::appendResult(const QPair<QString, QString>& label, const QString& text){
    // strecth分残してインサート
    const QString command = label.first;
    const QString japanese = label.second;
    const QString ref = japanese + "(" + command + ")";
    if (command.startsWith("Section")){
        auto red = createLabel(ref);
        QPalette palette = red->palette();
        palette.setColor(QPalette::WindowText, Qt::red);
        red->setPalette(palette);
        vlay->insertWidget(vlay->count() - 1 , red);
        return;
    }

    vlay->insertWidget(vlay->count() - 1, createLabel(ref));

    // textBrowser->appendPlainText(text);
    QString t(u8"■" + japanese + "\n");
    t.append(text).append("\n");
    QString html = "<a name=\"" + ref + "\">" + t.replace("\n","<br>").replace(" ", "&nbsp;") + "</a>";
    textBrowser->append(html);
    /*
    textBrowser->appendHtml(html);
    */
}
void OutputTextResultWidget::setCursorTop() const
{
    this->textBrowser->verticalScrollBar()->setValue(0);
}


} // namespace post3dapp
