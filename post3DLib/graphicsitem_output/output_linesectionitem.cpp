#include "output_linesectionitem.h"

#include <QBrush>
#include <QDebug>
#include <QPaintEngine>
#include <QPainter>
#include <QStyleOption>
#include "define_npstn.h"
#include "fixed_data.h"
#include "define_draw_figure.h"
#include "manage_jointdata.h"
#include "unified_analysisdata.h"
#include "unified_output.h"

namespace post3dapp {

OutputLineSectionItem::OutputLineSectionItem( FigureTemplate* fig, DATATYPE dtype,STRUCTTYPE stype, const QUuid &uid )
    : OutputGraphicsItem( fig, dtype,stype, uid)
{
}

} // namespace post3dapp

