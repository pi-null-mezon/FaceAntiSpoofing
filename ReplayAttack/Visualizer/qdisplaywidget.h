#ifndef QDISPLAYWIDGET_H
#define QDISPLAYWIDGET_H

#include <QWidget>

class QDisplayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QDisplayWidget(QWidget *parent = nullptr);

signals:
    void imageHasBeenDrawn();

public slots:
    void updateImage(const QImage &_img);
    void updateInfo(const QString &_info);

protected:
    void paintEvent(QPaintEvent *_event);

private:
    QImage  qimg;
    QString infostr;
};

#endif // QDISPLAYWIDGET_H
