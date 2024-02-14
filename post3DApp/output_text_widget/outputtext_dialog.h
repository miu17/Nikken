#ifndef OUTPUTTEXTDIALOG_H
#define OUTPUTTEXTDIALOG_H

#include <QDialog>
namespace post3dapp{
class AppVersion;
class OutputTextTabWidget;

class OutputTextDialog : public QDialog
{
    Q_OBJECT
public:
    OutputTextDialog(QWidget *parent = 0);
    void writeBinaryFile(QDataStream&) const;
    void readBinaryFile(QDataStream&, const AppVersion&);

private:
    OutputTextTabWidget* tabWidget;
};
} // namespace post3dapp

#endif // OUTPUTTEXTDIALOG_H
