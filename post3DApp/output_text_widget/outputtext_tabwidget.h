#ifndef OUTPUTTEXTTABWIDGET_H
#define OUTPUTTEXTTABWIDGET_H
#include <QTabWidget>
namespace post3dapp {
class AppVersion;
class OutputTextConditionWidget;
class OutputTextResultWidget;

class OutputTextTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    OutputTextTabWidget(QWidget *parent = 0);

    void writeBinaryFile(QDataStream&) const;
    void readBinaryFile(QDataStream&, const AppVersion&);

public slots:
    void slotRunButtonClicked();

private:
    OutputTextConditionWidget* condition;
    OutputTextResultWidget* result;

    QList<QStringList> parse(const QStringList&) const;
    QString parseLine(const QStringList&) const;
};
} // namespace post3dapp

#endif // OUTPUTTEXTTABWIDGET_H
