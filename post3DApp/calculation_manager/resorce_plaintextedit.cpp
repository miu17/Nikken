#include "resorce_plaintextedit.h"
namespace post3dapp{

ResorcePlainTextEdit::ResorcePlainTextEdit(QWidget *parent)
    :QPlainTextEdit(parent)
{
    setReadOnly(true);
}
} // namespace post3dapp
