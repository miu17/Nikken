#ifndef CALCULATIONMANAGERDIALOG_H
#define CALCULATIONMANAGERDIALOG_H

#include <QDialog>

class QPushButton;
class QLineEdit;

namespace post3dapp{
class AppVersion;
class CalculationTabWidget;

class CalculationManagerDialog : public QDialog
{
    Q_OBJECT
public:
    CalculationManagerDialog(QWidget *parent = 0);

    void writeBinaryFile(QDataStream&) const;
    void readBinaryFile(QDataStream&, const AppVersion&);

public slots:
    void slotReference();
    void slotRun();
    void slotModelUpload();
    void slotModelRewind();
    void slotFilenameChanged();

private:
    void createButtons();

    QPushButton *rewind_button;
    QPushButton *upload_button;
    CalculationTabWidget *tabWidget;
    QLineEdit *pathLineEdit;
    QPushButton *reference_button;
    QPushButton *run_button;

};
} // namespace post3dapp

#endif // CALCULATIONMANAGERDIALOG_H
