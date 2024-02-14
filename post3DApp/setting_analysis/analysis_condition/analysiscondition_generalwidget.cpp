#include "analysiscondition_generalwidget.h"

#include <QButtonGroup>
#include <QDoubleValidator>
#include <QFile>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QRadioButton>
#include <QTextCodec>
#include <QTextStream>

#include "analysisconditionreader.h"
#include "generallinecondition.h"
#include "unified_inputdata.h"

namespace post3dapp{


AnalyisisConditionGeneralWidget::AnalyisisConditionGeneralWidget(QWidget *parent)
    : QWidget(parent)
{
    int y = 10;
    int maxWidth = 0;
    const double height = 20.0;
    auto lines = AnalysisConditionReader::DefaultGeneralAnalysisConditionLines();
    foreach(auto str, lines){
        if (!str.startsWith("*")){
            QStringList strlist = str.split(",");
            int num = strlist.at(0).toInt();
            auto condition = GeneralLineCondition::Create(str);
            layoutMap.insert(num, condition);
            maxWidth = qMax(createLayout(y, height, condition), maxWidth);
            y += height;
        }
        else if (str.startsWith("**")){
            QLabel* label = new QLabel(str.split(",").at(0), this);
            QFont font = label->font();
            font.setBold(true);
            label->setFont(font);
            label->setGeometry(10, y, 400, height);
            y += height;
        }else if (str.startsWith("*")){
            QLabel* label = new QLabel(str.split(",").at(0), this);
            QFont font = label->font();
            font.setBold(true);
            label->setFont(font);
            label->setGeometry(25, y, 400, height);
            y += height;
        }
    }

    this->setMinimumHeight(y);
    this->setMinimumWidth(maxWidth);
}

int AnalyisisConditionGeneralWidget::createLayout(const int y, const double height, QSharedPointer<GeneralLineCondition> condition)
{
    int maxWidth = 0;
    QLabel* number = new QLabel(QString::number(condition->number), this);
    number->setGeometry(40, y, 50, height);
    QLabel* comment = new QLabel(condition->comment, this);
    comment->setGeometry(90, y, 400, height);
    if (condition->isChoice){
        QButtonGroup* buttonGroup = new QButtonGroup();
        for (int i = 0; i < condition->choices.count(); i++){
            QRadioButton* b = new QRadioButton(condition->choices.at(i), this);
            b->setObjectName(QString::number(condition->number) + "-" + QString::number(i));
            if (i == condition->defaultChoice){
                QFont font = b->font();
                font.setBold(true);
                b->setFont(font);
                b->setChecked(true);
                connect(b, &QRadioButton::toggled,
                        [=]{
                    QPalette pal = number->palette();
                    if (b->isChecked()){
                        pal.setColor(QPalette::Foreground, Qt::black);
                    }else{
                        pal.setColor(QPalette::Foreground, Qt::red);
                    }
                    number->setPalette(pal);
                    emit dataChanged();
                });
            }
            connect(b, &QRadioButton::clicked,
                    [=]{
                condition->setSelectedNumber(i);
            });

            buttonGroup->addButton(b);
            b->setGeometry(490 + 200 * i, y, 200, height);
            maxWidth = 490 + 200 * (i + 1);
        }
    }else{
        QLineEdit *edit = new QLineEdit(this);
        edit->setObjectName(QString::number(condition->number));
        edit->setText(QString::number(condition->defaultFactor));
        edit->setValidator(new QDoubleValidator());
        edit->setGeometry(490, y, 40, height);
        connect(edit, &QLineEdit::textChanged,
                [=]{
            QPalette pal = number->palette();
            if (edit->text().toDouble() != condition->defaultFactor){
                pal.setColor(QPalette::Foreground, Qt::red);
            }else{
                pal.setColor(QPalette::Foreground, Qt::black);
            }
            number->setPalette(pal);
            condition->setFactor(edit->text().toDouble());
            emit dataChanged();
        });
        QLabel* def = new QLabel("default:" + QString::number(condition->defaultFactor), this);
        def->setGeometry(530, y, 150, height);
        maxWidth = 530 + 150;
    }
    return maxWidth;
}

QString AnalyisisConditionGeneralWidget::write() const{
    QString out;
    Q_FOREACH(auto map, layoutMap){
        out.append(map->write());
    }
    return out;
}

void AnalyisisConditionGeneralWidget::read(const QMap<int, QString>& lists) {
    if (lists.count() == 0){
        setDefault();
        return;
    }

    QMap<int, QSharedPointer<GeneralLineCondition>>::iterator i;
    for (i = layoutMap.begin(); i != layoutMap.end(); ++i){
        if (!lists.contains(i.key()))
            continue;
        QString text = lists.value(i.key());
        bool isOk = true;
        if (i.value()->isChoice){
            int number = text.mid(19,1).toInt(&isOk);
            if (!isOk)
                return;
            QRadioButton* button = this->findChild<QRadioButton *>(QString::number(i.value()->number) + "-" + QString::number(number));
            if (button == nullptr)
                return;
            button->setChecked(true);
            i.value()->setSelectedNumber(number);
        }
        else{
            double factor = text.mid(20, 10).toDouble(&isOk);
            if (!isOk)
                return;
            QLineEdit* edit = this->findChild<QLineEdit *>(QString::number(i.value()->number));
            if (edit == nullptr)
                return;
            edit->setText(QString::number(factor));
        }
    }
}

void AnalyisisConditionGeneralWidget::setDefault(){
    QMap<int, QSharedPointer<GeneralLineCondition>>::iterator i;
    for (i = layoutMap.begin(); i != layoutMap.end(); ++i){
        bool isOk = true;
        if (i.value()->isChoice){
            if (!isOk)
                return;
            QRadioButton* button = this->findChild<QRadioButton *>(QString::number(i.value()->number) + "-" + QString::number(i.value()->defaultChoice));
            if (button == nullptr)
                return;
            button->setChecked(true);
        }else{
            QLineEdit* edit = this->findChild<QLineEdit *>(QString::number(i.value()->number));
            if (edit == nullptr)
                return;
            edit->setText(QString::number(i.value()->defaultFactor));
        }
    }
}

} // namespace post3dapp
