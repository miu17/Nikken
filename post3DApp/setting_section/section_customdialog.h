#ifndef SECTION_CUSTOMDIALOG_H
#define SECTION_CUSTOMDIALOG_H

#include <QDialog>

class QPushButton;
namespace post3dapp{
enum class CBSTATUS;
class AppVersion;
class SectionFilterDialog;
class SectionTableWidget;

class SectionCustomDialog : public QDialog
{
    Q_OBJECT

public:

    SectionCustomDialog( QWidget *parent = nullptr );

    bool checkBinaryFile ( QDataStream &infile, const AppVersion &ver) const;
    void readBinaryFile ( QDataStream &infile, const AppVersion &ver);
    void readTextFile (QTextStream &infile, const AppVersion &ver);
    void writeBinaryFile ( QDataStream &outfile) const;
    void writeTextFile (QTextStream &outfile) const;
    void clearData();

    void reflectUnifiedData();
    void replaceMatchingName( const QString &str_a, const QString &str_b );
protected:

public slots:

    void slotTextFileOpen();
    void slotTextFileSave();

    virtual void slotModelUpload();
    void slotModelRewind();

    void slotModifiedStateOn()
    {
        setDataModified(true);
    }

    void slotOpenFilterDialog();

signals:

    void statusChanged(CBSTATUS);

protected:

    QHBoxLayout *buttonsLayout;
    QPushButton *rewind_button;
    QPushButton *upload_button;
    SectionFilterDialog *filterDialog;

    QString sectionWindowTitle;
    QList<SectionTableWidget *> childWidgets;

    void createButtons(bool);
    void setDataModified(bool);
    void initializeWindowTitle(const QString &title )
    {
        sectionWindowTitle = title;
        setWindowTitle(sectionWindowTitle);
    }

};
} // namespace post3dapp
#endif
