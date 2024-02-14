#ifndef TOPMENUWIDGET_H
#define TOPMENUWIDGET_H

#include <QIcon>
#include <QThread>
#include <QWidget>
//#include "autosave.h"
class QFrame;
class QTabWidget;
class QTextStream;
//class AutoSave;

namespace post3dapp {
enum class DATATYPE;
class Analysis2DSettingDialog;
class Analysis3DSettingDialog;
class AnalysisConditionDialog;
class AnalysisFloorDialog;
class AnalysisGroupDialog;
class AnalysisGroupDialog;
class CalculationLoad2DSettingDialog;
class CalculationLoad3DSettingDialog;
class CalculationManagerDialog;
class FixedTableData;
class FloorSettingDialog;
class FrameSettingDialog;
class handleNPstn;
class Input2DSettingDialog;
class Input3DSettingDialog;
class JointLoadSettingDialog;
class MaterialSettingDialog;
class MemberLoadSettingDialog;
class MenuButton;
class OutputTextDialog;
class OutputFigureDialog;
class SectionBaseDialog;
class SectionBeamDialog;
class SectionBraceDialog;
class SectionColumnDialog;
class SectionDamperDialog;
class SectionGirderDialog;
class SectionIsolatorDialog;
class SectionOpeningDialog;
class SectionSlabDialog;
class SectionWallDialog;
class SectionUWallDialog;
class SlabLoadSettingDialog;
class StressCombiDialog;
class TextInputDialog;
class TopMenuWidget : public QWidget
{
    Q_OBJECT

public:

    TopMenuWidget( QWidget *parent = 0 );

    void enableSaveAsButton(bool);

public slots:

    void slotFileSave();
    void slotFileSaveAs();
    void slotFileOpen();
    void slotNewFileOpen();

    void slotAdditionalFileOpen();
    void slotDivideFileSave();
    void slotPstnOpen();
    void slotWritePsv();
    void slotEstimate();

    void slotAnalysisGroupDialogShow();
    void slotCalculationLoad();
    void slotExecuteAnalysis();
    void slotStressCombiDialogShow();
    void slotStructuralStatement();
    void slotCalculationTextDialogShow();
    void slotCalculationFigureDialogShow();

    void slotChangeTableName(DATATYPE, const QString &, const QString &);

signals:

    void saveFileChanged();

private:
    //QThread thread_autosave;
    //AutoSave threadA;
    QTabWidget *topMenuWidget;
    QIcon tabIcon;
    handleNPstn *npstn;

    MenuButton *saveAs_button;

    FloorSettingDialog *floorSettingDialog;
    FrameSettingDialog *frameSettingDialog;
    MaterialSettingDialog *materialSettingDialog;

    SectionColumnDialog *columnSettingDialog;
    SectionGirderDialog *girderSettingDialog;
    SectionBraceDialog *braceSettingDialog;
    SectionBeamDialog *beamSettingDialog;
    SectionSlabDialog *slabSettingDialog;
    SectionWallDialog *wallSettingDialog;
    SectionUWallDialog *uwallSettingDialog;
    SectionBaseDialog *baseSettingDialog;
    SectionOpeningDialog *openSettingDialog;
    SectionDamperDialog *damperSettingDialog;
    SectionIsolatorDialog *isoSettingDialog;

    JointLoadSettingDialog *jloadSettingDialog;
    MemberLoadSettingDialog *mloadSettingDialog;
    SlabLoadSettingDialog *sloadSettingDialog;

    MenuButton *anagroup_button;
    AnalysisFloorDialog *analysisFloorDialog;
    AnalysisConditionDialog *analysisConditionDialog;
    OutputTextDialog *outputTextDialog;
    OutputFigureDialog *outputFigureDialog;
    CalculationManagerDialog *calculationManagerDialog;

    Input2DSettingDialog *input2DSettingDialog;
    Input3DSettingDialog *input3DSettingDialog;

    CalculationLoad2DSettingDialog *calculation2DSettingDialog;
    CalculationLoad3DSettingDialog *calculation3DSettingDialog;

    Analysis2DSettingDialog *analysis2DSettingDialog;
    Analysis3DSettingDialog *analysis3DSettingDialog;

//    OLD_Analysis2DSettingDialog *old_analysis2DSettingDialog;
//    OLD_Analysis3DSettingDialog *old_analysis3DSettingDialog;

    StressCombiDialog *stressCombiDialog;
    TextInputDialog *textInputDialog;

    void createFileTabMenu();
    void createBaseTabMenu();
    void createSectionTabMenu();
    void createLoadTabMenu();
    void createAnalysisTabMenu();
    void createViewSettingTabMenu();
    void createOtherTabMenu();

    bool checkDataFile( QDataStream &infile ) const;
    void writeP3dDataFile( QDataStream &outfile ) const;
    void writeListP3dData( QDataStream &outfile, QHash<QString, qint64> & ) const;
    void writeWidgetP3dData( QDataStream &outfile, QHash<QString, qint64> &) const;
    void readP3dFile( QDataStream &infile );
    void writeListPsvData(QTextStream &outfile) const;
    void readPsvFile( QTextStream &infile);

    void reflectUnifiedData();

    void setTableModel(FixedTableData*);
};

} // namespace post3dapp
#endif
