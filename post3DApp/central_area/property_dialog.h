#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H

#include <QDialog>
#include "unified_inputdata.h"

class QLineEdit;
class QTabWidget;
class QTextEdit;

namespace post3dapp{

class PropertyInputWidget;
class PropertySelectWidget;

class PropertyDialog : public QDialog
{
    Q_OBJECT

public:

    PropertyDialog(QWidget *parent = nullptr );

protected:

    void closeEvent(QCloseEvent *);
    void keyPressEvent ( QKeyEvent * ) { }

private slots:

    void resetDefinePropertyContents(DATATYPE);
    void resetElementPropertyContents();
    void resetSelectPropertyContents();
    void changeMode(MDITYPE);

signals:

    void windowClosed();

private:

    UnifiedData* myUnifiedData;
    QTabWidget *tabWidget;
    QWidget *defineCountProperty;
    QWidget *elementCountProperty;
    QWidget *selectElementProperty;

    void createDefineCountProperty();
    void createElementCountProperty();
    void createSelectElementProperty();

    void setInputCountText(DATATYPE);

    /* Define Count */
    QLineEdit *d_flLine;
//    QLineEdit *d_frgLine;
    QLineEdit *d_frpLine;
    QLineEdit *d_macLine;
    QLineEdit *d_masLine;
    QLineEdit *d_marLine;
    QLineEdit *d_colLine;
    QLineEdit *d_girLine;
    QLineEdit *d_braLine;
    QLineEdit *d_beamLine;
    QLineEdit *d_slabLine;
    QLineEdit *d_wallLine;
    QLineEdit *d_uwalLine;
    QLineEdit *d_wopenLine;
    QLineEdit *d_baseLine;
    QLineEdit *d_dampLine;
    QLineEdit *d_isoLine;
    QLineEdit *d_jloadLine;
    QLineEdit *d_mloadLine;
    QLineEdit *d_sloadLine;

    /* Element Count */
    PropertyInputWidget *i_jointWidget;
    PropertyInputWidget *i_colWidget;
    PropertyInputWidget *i_girWidget;
    PropertyInputWidget *i_braWidget;
    PropertyInputWidget *i_beamWidget;
    PropertyInputWidget *i_slabWidget;
    PropertyInputWidget *i_wallWidget;
    PropertyInputWidget *i_uwalWidget;
    PropertyInputWidget *i_dampWidget;
    PropertyInputWidget *i_isoWidget;
    PropertyInputWidget *i_jloadWidget;
    PropertyInputWidget *i_mloadWidget;
    PropertyInputWidget *i_sloadWidget;

    PropertySelectWidget *s_jointWidget;
    PropertySelectWidget *s_colWidget;
    PropertySelectWidget *s_girWidget;
    PropertySelectWidget *s_braWidget;
    PropertySelectWidget *s_beamWidget;
    PropertySelectWidget *s_slabWidget;
    PropertySelectWidget *s_wallWidget;
    PropertySelectWidget *s_uwalWidget;
    PropertySelectWidget *s_dampWidget;
    PropertySelectWidget *s_isoWidget;
    PropertySelectWidget *s_jloadWidget;
    PropertySelectWidget *s_mloadWidget;
    PropertySelectWidget *s_sloadWidget;

    void setJointProperty( const QList<JointData *> & );
    void setLineMemberProperty(DATATYPE, const QList<MemberData *> &);
    void setPlaneMemberProperty(DATATYPE, const QList<MemberData *> &);
    void setLoadMemberProperty(DATATYPE, const QList<MemberData *> &);

    QString structureTypeToString( STRUCTTYPE ) const;
    QString renderShapeToString(STRUCTTYPE, RENDERSHAPE, qreal, qreal, qreal, qreal ) const;
    QString planeModelToString(PLANEMODEL , PLANESUBMODEL psubmodel) const;

};

class PropertyInputWidget : public QWidget
{
    Q_OBJECT

public:

    PropertyInputWidget( const QString &, QWidget *parent = nullptr );

    void clearText();
    void setDetailText(const QString &);

private:

    QTextEdit *textEdit;

};

class PropertySelectWidget : public QWidget
{
    Q_OBJECT

public:

    PropertySelectWidget( const QString &, QWidget *parent = nullptr );

    void clearText();
    void setCountText(const QString &);
    void setDetailText(const QString &);

private:

    QLineEdit *lineEdit;
    QTextEdit *textEdit;

};
} // namespace post3dapp


#endif

