#ifndef OUTPUTFIGUREDIALOG_H
#define OUTPUTFIGUREDIALOG_H

#include <QDialog>

namespace post3dapp{
class AppVersion;
class OutputFigureTabWidget;

class OutputFigureDialog: public QDialog
{
public:
    OutputFigureDialog(QWidget *parent = 0);

    void writeBinaryFile(QDataStream&) const;
    void readBinaryFile(QDataStream&, const AppVersion&);

public slots:
    void setPrinter();

private:
    QPushButton *run_button;
    OutputFigureTabWidget *tabWidget;



};
} // namespace post3dapp

#endif // OUTPUTFIGUREDIALOG_H
