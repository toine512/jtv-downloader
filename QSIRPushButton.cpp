#include "QSIRPushButton.h"

QSIRPushButton::QSIRPushButton(const QString &pixmap_file, QWidget *parent) :
    QPushButton(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    container = new QLabel;
    container->setPixmap(QPixmap(pixmap_file));
    container->setScaledContents(true);
    setLayout(new QHBoxLayout);
    layout()->addWidget(container);
}

QSize QSIRPushButton::sizeHint() const
{
    QSize size = QPushButton::sizeHint();
    size.setWidth(height());
    return size;
}
