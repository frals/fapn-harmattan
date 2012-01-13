#ifndef GCONFWRAPPER_H
#define GCONFWRAPPER_H

#include <QObject>


QTM_USE_NAMESPACE

class GConfWrapper : public QObject
{
    Q_OBJECT
public:
    explicit GConfWrapper(QObject *parent = 0);

signals:

public slots:

};


#endif // GCONFWRAPPER_H
