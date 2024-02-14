#ifndef OUTPUTCONTROL_H
#define OUTPUTCONTROL_H

#include <QStringList>

namespace post3dapp{
enum class OUTFIGTYPE;
enum class SCENETYPE;
enum class LISTTYPE;
class FigureTemplate;
struct FIGURE_TYPE_PROPERTY;
struct OUTPUT2DVIEWSETTINGS;


/* フラグ　..その他の出力（伏）、その他の出力（軸）、その他の出力（パース）、グラフ、テキスト */
enum class OUTFIGCATEGORY { UNDEFINED, PLAN, ELEVATION, PERS, GRAPH, TEXT };

class OutFigString{
public:
    OutFigString(){}
    explicit OutFigString(OUTFIGTYPE _type,
                 bool _isOutput = true,
                 bool _hasPlan = true,
                 bool _hasSection = true,
                 bool _hasPers = true,
                 bool _hasGraph = true)
        :type(_type), isOutput(_isOutput), hasPlan(_hasPlan), hasSection(_hasSection), hasPers(_hasPers), hasGraph(_hasGraph){}
    QString japanese() const;
    QString english() const;
    OUTFIGTYPE type;
    bool isOutput;
    bool hasPlan;
    bool hasSection;
    bool hasPers;
    bool hasGraph;
private:

};

class OutputControl : public QObject
{
    Q_OBJECT

public:

    static OutputControl *getInstance()
    {
        if (myInstance == nullptr) myInstance = new OutputControl;
        return myInstance;
    }

    static void deleteInstance()
    {
        if (myInstance != nullptr) delete myInstance;
        myInstance = nullptr;
    }

    static QStringList outputTypeStringList(OUTFIGCATEGORY);
    static QList<OUTFIGTYPE> outputTypeList(OUTFIGCATEGORY);
    static LISTTYPE listType(int,OUTFIGCATEGORY);

    static QString getFigureTypeTitle(SCENETYPE scene, const QString&, FIGURE_TYPE_PROPERTY);
    static const QMap<OUTFIGTYPE, OutFigString> outfigStrings;

    QStringList getOutfigStringList(LISTTYPE, int idx=2)const;
    FIGURE_TYPE_PROPERTY makeOutputFigureTypeProperty(SCENETYPE, const QUuid &uid, OUTFIGTYPE myFigureType, const QVector<int> &idx, int step) const;
    static FIGURE_TYPE_PROPERTY ChangeCalcLoadFigureTypeProperty(const FIGURE_TYPE_PROPERTY&, int, int, int);

public slots:

    void slotOutputFileRead();
signals:


private:

    static OutputControl *myInstance;

    OutputControl();
    OutputControl(const OutputControl &);
    OutputControl &operator=(const OutputControl &);

    QHash<int,QStringList> outfigPulldownList;


};
} // namespace post3dapp
#endif
