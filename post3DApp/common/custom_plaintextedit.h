#ifndef CUSTOMPLAINTEXTEDIT_H
#define CUSTOMPLAINTEXTEDIT_H

#include <QPlainTextEdit>

namespace post3dapp{
class TextDataUnit;

class CustomPlainTextEdit : public QPlainTextEdit
{
public:
    CustomPlainTextEdit(QWidget *parent = nullptr);
    QString write() const;
    virtual bool hasFormatError() const = 0;
    virtual QString getStatusMessage() const = 0;

protected:
    bool eventFilter(QObject *obj,QEvent *event) override = 0 ;
    void paintEvent( QPaintEvent* ) override;
    void resizeEvent( QResizeEvent * ) override;
    void decorateUnit(const QTextBlock&, const TextDataUnit&) const;
    void drawRowNumber(const QTextBlock&, QPainter*) const;
    void drawRedCircle(const QTextBlock&, QPainter*) const;

    QWidget* rowCounter;
    int fontHeight;
    int fontWidth;
    int rowCountWidth;
    int rowCountAreaWidth;


private:
    void drawEOB(QPainter* painter) const;
    virtual void decorateCells() const = 0;


    void drawHorizontalRuledLine(QPainter* painter) const;
    void drawVerticalRuledLine(QPainter* painter);
    int maxRowPos;

};
} // namespace post3dapp

#endif // CUSTOMPLAINTEXTEDIT_H
