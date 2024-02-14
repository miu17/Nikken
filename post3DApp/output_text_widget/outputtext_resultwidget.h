#ifndef OUTPUTEXTRESULTWIDGET_H
#define OUTPUTEXTRESULTWIDGET_H

#include <QWidget>
#include <QLabel>

class QTextEdit;
class QVBoxLayout;

namespace post3dapp{

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel(const QString& text="", QWidget* parent=0);
    ~ClickableLabel();

signals:
    void clicked(const QString&);

protected:
    void mousePressEvent(QMouseEvent* event);
};

class OutputTextResultWidget : public QWidget
{
    Q_OBJECT
public:
    OutputTextResultWidget(const QString&, QWidget* parent = 0);
    void appendResult(const QPair<QString, QString>&, const QString&);
    QString getLabel() const{return label;}
    void setCursorTop() const;
    static int ConditionFixWidth;
    static int ResultMinimumWidth;

public slots:
    void slotJump(const QString&) const;

private:
    const QString label;
    ClickableLabel* createLabel(const QString&);
    QTextEdit* textBrowser;
    //QPlainTextEdit *textBrowser;
    QVBoxLayout *vlay;
};
} // namespace post3dapp

#endif // OUTPUTEXTRESULTWIDGET_H
