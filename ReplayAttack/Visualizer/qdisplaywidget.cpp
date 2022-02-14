#include "qdisplaywidget.h"

#include <QPainter>
#include <QTimer>

int getRandomInt(int _from, int _to)
{
    return _from + (_to - _from) * (qreal)qrand() / RAND_MAX;
}

QDisplayWidget::QDisplayWidget(QWidget *parent) : QWidget(parent)
{    
}

void QDisplayWidget::updateImage(const QImage &_img)
{
    qimg = _img;
    update();
}

void QDisplayWidget::updateInfo(const QString &_info)
{
    infostr = _info;
    update();
}

void QDisplayWidget::paintEvent(QPaintEvent *_event)
{
    Q_UNUSED(_event);
    QPainter painter(this);

    if(!qimg.isNull()) {
        int _rndlvl = getRandomInt(0,200);
        painter.fillRect(rect(),QColor(_rndlvl,_rndlvl,_rndlvl));
        QRectF _viewrect = rect();
        if(_viewrect.width()/_viewrect.height() > (qreal)qimg.width()/qimg.height()) {
            _viewrect.setWidth((qreal)_viewrect.height()*qimg.width()/qimg.height());
            _viewrect.moveLeft((rect().width()-_viewrect.width())/2);
        } else {
            _viewrect.setHeight((qreal)_viewrect.width()*qimg.height()/qimg.width());
            _viewrect.moveTop((rect().height()-_viewrect.height())/2);
        }
        painter.drawImage(_viewrect,qimg);
    } else {
        painter.setPen(Qt::lightGray);
        QFont _font = font();
        if(logicalDpiX() > 96) {
            _font.setPointSizeF(0.015*(rect().width() + rect().height()) + 2.5);
        } else {
            _font.setPointSizeF(0.03*(rect().width() + rect().height()) + 5.0);
        }
        painter.setFont(_font);
        painter.drawText(rect(),Qt::AlignCenter,infostr);
    }
}
