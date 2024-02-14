#ifndef ANALYSISCONDITIONDIALOG_H
#define ANALYSISCONDITIONDIALOG_H

#include <QDialog>


class QPushButton;

namespace post3dapp{
enum class CBSTATUS;
class AnalysisTabWidget;
class AppVersion;

class AnalysisConditionDialog : public QDialog
{
    Q_OBJECT
public:
    AnalysisConditionDialog(QWidget *parent = 0);

    void clearData();
    void readBinaryFile(QDataStream &, const AppVersion &);
    void readTextFile(QTextStream&, const AppVersion&);
    void writeBinaryFile (QDataStream &);
    void writeTextFile(QTextStream&);

public slots:
    void slotTextFileOpen();
    void slotTextFileSave();
    void slotTextFileConvert();


private:
    AnalysisTabWidget *tabWidget;
    QPushButton *open_button;
    QPushButton *save_button;
    QPushButton *convert_button; // temporary
    QPushButton *rewind_button;
    QPushButton *upload_button;
    void createButtons();

    void setDataModified(bool);

private slots:
    void slotModelUpload();
    void slotModelRewind();
    void slotDataChanged();

signals:
    void statusChanged(CBSTATUS);
};

} // namespace post3dapp
#endif // ANALYSISCONDITIONDIALOG_H
