#ifndef OUTPUTTEXTCONDITIONWIDGET_H
#define OUTPUTTEXTCONDITIONWIDGET_H

#include <QWidget>

class QPushButton;

namespace post3dapp{
class AppVersion;
class OutputTextPlainTextEdit;

class OutputTextConditionWidget : public QWidget
{
    Q_OBJECT
public:
    OutputTextConditionWidget(QWidget* parent = 0);
    OutputTextPlainTextEdit* textEdit;
    QPushButton* runButton;

    void writeBinaryFile(QDataStream&) const;
    void readBinaryFile(QDataStream&, const AppVersion&);
};
} // namespace post3dapp

#endif // OUTPUTTEXTCONDITIONWIDGET_H
