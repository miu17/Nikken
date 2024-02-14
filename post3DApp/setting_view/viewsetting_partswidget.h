#ifndef VIEWSETTINGPARTS_H
#define VIEWSETTINGPARTS_H

#include <QWidget>

class QCheckBox;
class QSpinBox;
class QDoubleSpinBox;
class QSettings;

namespace post3dapp{
class TypicalColorComboBox;
struct COLORBRUSHSETTING;
struct DOWNSETTING;
struct ELEMENTVIEW;

/* 要素の設定ウィジェット  */
class ElementWidgetGroup : public QWidget
{
    Q_OBJECT

public:

    ElementWidgetGroup(const QString &, QWidget *widget = 0);

    ELEMENTVIEW getElementSetting() const;
    void setElementSetting( const ELEMENTVIEW & );

public slots:

    void slotEnableAllWidgets(bool);

private:

    QCheckBox *elementCheckBox;
    QCheckBox *nameCheckBox;
    QLabel *lineLabel;
    TypicalColorComboBox *lineComboBox;
    QLabel *brushLabel;
    TypicalColorComboBox *brushComboBox;
    QLabel *transLabel;
    QSpinBox *transSpinBox;
};


/* 要素状態の設定ウィジェット  */
class ElementStateWidgetGroup : public QWidget
{
    Q_OBJECT

public:

    ElementStateWidgetGroup(const QString &, QWidget *widget = 0);

    COLORBRUSHSETTING getElementSetting() const;
    void setElementSetting( const COLORBRUSHSETTING & );

private:

    TypicalColorComboBox *lineComboBox;
    TypicalColorComboBox *brushComboBox;
    QSpinBox *transSpinBox;
};

/* Down設定ウィジェット */
class DownWidgetGroup : public QWidget
{
    Q_OBJECT

public:

    DownWidgetGroup(const QString &, QWidget *widget = 0);

    DOWNSETTING getElementSetting() const;
    void setElementSetting( const DOWNSETTING & );

private:
    QCheckBox *nameCheckBox;
    TypicalColorComboBox *lineComboBox;
    TypicalColorComboBox *brushComboBox;
    QSpinBox *transSpinBox;

};


/* ５サイズコンボボックス  */
class FiveSizeComboBox : public QComboBox
{
    Q_OBJECT

public:

    FiveSizeComboBox(QWidget *widget = 0);
    int size() const
    {
        return currentIndex();
    }
    void setSize(int idx)
    {
        setCurrentIndex(idx);
    }

private:

};

/*線種の指定ウィジェット*/
class LineStyleWidget : public QWidget
{
    Q_OBJECT

public:

    LineStyleWidget(const QString &, QWidget *widget = 0);

    bool getLineVisible() const;
    QColor getLineColor() const;
    Qt::PenStyle getLineStyle() const;
    static Qt::PenStyle toPenStyle(QString str) ;//todo
    static QString toString(Qt::PenStyle style);//todo
    void setSetting( bool visible,const QColor& color,Qt::PenStyle pen);
public slots:
    void slotEnableMinMax(bool);

private:
    QCheckBox* chk_visible;

    TypicalColorComboBox *lineColorComboBox;
    QComboBox *lineStyleComboBox;
};

/*最大最小値の指定ウィジェット*/
class AxisMinMaxWidget : public QWidget
{
    Q_OBJECT

public:

    AxisMinMaxWidget(const QString &, QWidget *widget = 0);

    bool isAutoScale() const;
    qreal minValue() const;
    qreal maxValue() const;
    void setSetting( bool isAuto,qreal min,qreal max);
public slots:
    void slotEnableMinMax(bool);
    void slotDisableMinMax(bool);

private:
    QButtonGroup* radio_autoMinMax;

    QDoubleSpinBox *minSpinBox;
    QDoubleSpinBox *maxSpinBox;
};
///* 荷重分布の設定ウィジェット 2D  */
//class Load2DWidgetGroup : public QFrame
//{
//    Q_OBJECT

//public:

//    Load2DWidgetGroup(QWidget *widget = 0);

//    OLD_LOAD2DFIGURESETTINGS getViewSettings() const;
//    void setViewSettings( const OLD_LOAD2DFIGURESETTINGS & );
//    void saveSettings( QSettings & );
//    void readSettings( QSettings & );
//    void initializeSettings();

//private:

//    FiveSizeComboBox *stringSizeCombobox;
//    QCheckBox *jCheck;
//    TypicalColorComboBox *jColorCombobox;
//    FiveSizeComboBox *jSizeCombobox;
//    QCheckBox *pCheck;
//    TypicalColorComboBox *pColorCombobox;
//    FiveSizeComboBox *pSizeCombobox;
//    QCheckBox *wCheck;
//    TypicalColorComboBox *wColorCombobox;
//    QDoubleSpinBox *wScaleSpin;
//    QCheckBox *dCheck;
//    TypicalColorComboBox *dColorCombobox;
//    QDoubleSpinBox *dScaleSpin;

//};


///* 荷重分布の設定ウィジェット 3D  */
//class Load3DWidgetGroup : public QFrame
//{
//    Q_OBJECT

//public:

//    Load3DWidgetGroup(QWidget *widget = 0);

//    OLD_LOAD3DFIGURESETTINGS getViewSettings() const;
//    void setViewSettings( const OLD_LOAD3DFIGURESETTINGS & );
//    void saveSettings( QSettings & );
//    void readSettings( QSettings & );
//    void initializeSettings();

//private:

//    FiveSizeComboBox *jloadSizeCombobox;
//    QDoubleSpinBox *mloadSizeSpin;

//    QCheckBox *jloadCheck;
//    QDoubleSpinBox *jloadMinSpin;
//    TypicalColorComboBox *jloadMinColor;
//    QDoubleSpinBox *jloadMaxSpin;
//    TypicalColorComboBox *jloadMaxColor;

//    QCheckBox *ploadCheck;
//    QDoubleSpinBox *ploadMinSpin;
//    TypicalColorComboBox *ploadMinColor;
//    QDoubleSpinBox *ploadMaxSpin;
//    TypicalColorComboBox *ploadMaxColor;

//    QCheckBox *wloadCheck;
//    QDoubleSpinBox *wloadMinSpin;
//    TypicalColorComboBox *wloadMinColor;
//    QDoubleSpinBox *wloadMaxSpin;
//    TypicalColorComboBox *wloadMaxColor;

//    QCheckBox *dloadCheck;
//    QDoubleSpinBox *dloadMinSpin;
//    TypicalColorComboBox *dloadMinColor;
//    QDoubleSpinBox *dloadMaxSpin;
//    TypicalColorComboBox *dloadMaxColor;

//};
///* 節点応力図の設定ウィジェット 2D  */
//class NStress2DWidgetGroup : public QFrame
//{
//    Q_OBJECT

//public:

//    NStress2DWidgetGroup(QWidget *widget = 0);

//    OLD_NSTRESS2DFIGURESETTINGS getViewSettings() const;
//    void setViewSettings( const OLD_NSTRESS2DFIGURESETTINGS & );
//    void saveSettings( QSettings & );
//    void readSettings( QSettings & );

//private:

////    FiveSizeComboBox* stringSizeCombobox;
//    FiveSizeComboBox *stressSizeCombobox;
//    QCheckBox *wallgirderCheck;
//    TypicalColorComboBox *dColorCombobox;
//    QCheckBox *cCheck;
//    TypicalColorComboBox *cColorCombobox;
//    QCheckBox *gCheck;
//    TypicalColorComboBox *gColorCombobox;
//    QCheckBox *bCheck;
//    TypicalColorComboBox *bColorCombobox;
//    QCheckBox *sCheck;
//    TypicalColorComboBox *sColorCombobox;
//    QCheckBox *wCheck;
//    TypicalColorComboBox *wColorCombobox;
//    QDoubleSpinBox *dScaleSpin;

//};
///* フェイス応力図の設定ウィジェット 2D  */
//class FStress2DWidgetGroup : public QFrame
//{
//    Q_OBJECT

//public:

//    FStress2DWidgetGroup(QWidget *widget = 0);

//    OLD_FSTRESS2DFIGURESETTINGS getViewSettings() const;
//    void setViewSettings( const OLD_FSTRESS2DFIGURESETTINGS & );
//    void saveSettings( QSettings & );
//    void readSettings( QSettings & );

//private:

////    FiveSizeComboBox* stringSizeCombobox;
//    FiveSizeComboBox *stressSizeCombobox;
//    QCheckBox *wallgirderCheck;
//    TypicalColorComboBox *dColorCombobox;
//    QCheckBox *cCheck;
//    TypicalColorComboBox *cColorCombobox;
//    QCheckBox *gCheck;
//    TypicalColorComboBox *gColorCombobox;
//    QCheckBox *bCheck;
//    TypicalColorComboBox *bColorCombobox;
//    QCheckBox *sCheck;
//    TypicalColorComboBox *sColorCombobox;
//    QCheckBox *wCheck;
//    TypicalColorComboBox *wColorCombobox;
//    QDoubleSpinBox *dScaleSpin;

//};

///* 部材長図の設定ウィジェット 2D  */
//class Axial2DWidgetGroup : public QFrame
//{
//    Q_OBJECT

//public:

//    Axial2DWidgetGroup(QWidget *widget = 0);

//    OLD_AXIAL2DFIGURESETTINGS getViewSettings() const;
//    void setViewSettings( const OLD_AXIAL2DFIGURESETTINGS & );
//    void saveSettings( QSettings & );
//    void readSettings( QSettings & );

//private:

////    FiveSizeComboBox* stringSizeCombobox;
////    FiveSizeComboBox* stringSizeCombobox;
//    QCheckBox *checkVisible;
//    TypicalColorComboBox *NegativeColorCombobox;
//    TypicalColorComboBox *PositiveColorCombobox;
//    QDoubleSpinBox *dScaleSpin;

//};
///* 節点座標図の設定ウィジェット 2D  */
//class Coordinate2DWidgetGroup : public QFrame
//{
//    Q_OBJECT

//public:

//    Coordinate2DWidgetGroup(QWidget *widget = 0);

//    OLD_COORDINATE2DFIGURESETTINGS getViewSettings() const;
//    void setViewSettings( const OLD_COORDINATE2DFIGURESETTINGS & );
//    void saveSettings( QSettings & );
//    void readSettings( QSettings & );

//private:

////    FiveSizeComboBox* stringSizeCombobox;
//    FiveSizeComboBox *stringSizeCombobox;
//    TypicalColorComboBox *colorCombobox;

//};
} // namespace post3dapp
#endif
