#ifndef ANALYSISTABWIDGET_H
#define ANALYSISTABWIDGET_H

#include <QTabWidget>
class QStatusBar;

namespace post3dapp {
class AnalyisisConditionGeneralWidget;
class AnalysisPlainTextEdit;

class AnalysisTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    AnalysisTabWidget(QWidget *parent = 0);

    void read (QTextStream&);
    void read (const QPair<QString, QString>&);
    void read (const QString&);
    QString write() const;
    bool compare(const QPair<QString, QString>&) const;

    void modelUpload();
    void modelRewind();
    bool hasFormatError() const;
    void clearData();

private:
    AnalyisisConditionGeneralWidget *generalWidget;
    AnalysisPlainTextEdit *textEdit;
    QStatusBar *statusBar;

private slots:
    void slotColChanged();

signals:
    void dataChanged();

};

} // namespace post3dapp
#endif // ANALYSISTABWIDGET_H
