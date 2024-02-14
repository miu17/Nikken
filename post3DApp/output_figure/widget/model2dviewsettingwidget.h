#ifndef MODEL2DVIEWSETTINGWIDGET_H
#define MODEL2DVIEWSETTINGWIDGET_H

#include <QWidget>
#include "analysis2dsetting_modelwidget.h"

namespace post3dapp{
class AppVersion;

class OutputFigureModel2DWidget : public Analysis2DSettingModelWidget
{
    Q_OBJECT
public:
    OutputFigureModel2DWidget(const QString&, const QString&, QWidget *parent = 0);

    void writeBinaryFile(QDataStream&) const;
    void readBinaryFile(QDataStream&, const AppVersion&);
};
} // namespace post3dapp

#endif // MODEL2DVIEWSETTINGWIDGET_H
