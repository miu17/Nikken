#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include <QFrame>
namespace post3dapp{
class CustomLabelPrivate;

/**
 * @brief The CustomLabel class
 */
class CustomLabel : public QFrame
{
    Q_OBJECT
    Q_DISABLE_COPY(CustomLabel)
    Q_DECLARE_PRIVATE(CustomLabel)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(bool autoLineSpacing READ autoLineSpacing WRITE setAutoLineSpacing)
    Q_PROPERTY(int lineSpacing READ lineSpacing WRITE setLineSpacing)
    Q_PROPERTY(int lineCount READ lineCount)
public:
    explicit CustomLabel(QWidget *parent = 0);
    CustomLabel(const QString &text, int lineSpacing = 10, const QMargins &margin = QMargins(5, 5, 5,
                                                                                             5), QWidget *parent = 0);
    ~CustomLabel();

    bool autoLineSpacing() const;
    void setAutoLineSpacing(bool flag);

    int lineSpacing() const;
    void setLineSpacing(int space);

    int lineCount();
    bool isElide();

    QMargins margins() const;
    void setMargins(const QMargins &margin);

    QString text() const;

    QSize sizeHint() const;

public slots:
    void setText(const QString &text);
    void clear();

signals:
    void textChanged();

protected:
    void resizeEvent(QResizeEvent *ev);
    void paintEvent(QPaintEvent *ev);
    void fontChange(const QFont &);

private:
    CustomLabelPrivate *d_ptr;
};
} // namespace post3dapp
#endif // CUSTOMLABEL_H
