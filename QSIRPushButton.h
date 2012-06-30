#ifndef QSIRPUSHBUTTON_H
#define QSIRPUSHBUTTON_H

#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>

class QSIRPushButton : public QPushButton
{
    Q_OBJECT

public:
    explicit QSIRPushButton(const QString &pixmap_file, QWidget *parent = 0);
    QSize sizeHint() const;
    
signals:
    
public slots:

protected:
    QLabel *container;
    
};

#endif // QSIRPUSHBUTTON_H
