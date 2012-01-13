#ifndef APNHELPER_H
#define APNHELPER_H

#include <QObject>
#include <QProcess>
#include <QSystemDeviceInfo>
#include <QStringList>

#include <gconfitem.h>

QTM_USE_NAMESPACE

class APNHelper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList apnList READ apnList NOTIFY listChanged)

public:
    APNHelper(QObject *parent = 0);
    virtual ~APNHelper();

    Q_INVOKABLE void createAPN(const QString& apname, const QString& iap);
    Q_INVOKABLE void removeAPN(const QString& apname);
    QStringList apnList();
    Q_INVOKABLE QString apnPrettyName(const QString& id) const;
    Q_INVOKABLE QString apnAccessPointName(const QString& id) const;

signals:

    void listChanged();

public slots:
    void valueChanged();


private:
    QStringList removePathFromDir(const QStringList dirs) const;
    QProcess *m_process;

    GConfItem *m_rootDir;
    QSystemDeviceInfo *m_devinfo;

    QStringList m_apnlist;
    QString m_gconfpath;
};

#endif // APNHELPER_H
