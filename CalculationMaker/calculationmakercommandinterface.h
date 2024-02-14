#ifndef CALCULATIONMAKERCOMMANDINTERFACE_H
#define CALCULATIONMAKERCOMMANDINTERFACE_H

#include <QString>
#ifdef CALCULATIONMAKER_LIB
#define CALCULATIONMAKER_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define CALCULATIONMAKER_CLASS_DLLSPEC Q_DECL_IMPORT
#endif


namespace CalculationMakerLib{

class CALCULATIONMAKER_CLASS_DLLSPEC CalculationMakerCommandInterface
{
public:
    CalculationMakerCommandInterface(const QString &command);
    virtual ~CalculationMakerCommandInterface();

    QString command() const;
    QString errorString() const;

protected:
    void setError(const QString &message);

private:
    QString m_command;
    QString m_error;
};
} // namespace CalculationMakerLib

#endif // CALCULATIONMAKERCOMMANDINTERFACE_H
