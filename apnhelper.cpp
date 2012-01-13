#include "apnhelper.h"
#include <QDebug>
#include <QVariant>

#include <gconfitem.h>

#define IAP_PATH QString("/system/osso/connectivity/IAP")

APNHelper::APNHelper(QObject *parent) :
    QObject(parent),
    m_process()
{
    qDebug() << __PRETTY_FUNCTION__;
    m_gconfpath = "/usr/bin/gconftool-2";
    m_devinfo = new QSystemDeviceInfo(parent);
    m_rootDir = new GConfItem(IAP_PATH);

    connect(m_rootDir, SIGNAL(valueChanged()), this, SLOT(valueChanged()));
    m_apnlist = removePathFromDir(m_rootDir->listDirs());
}

APNHelper::~APNHelper()
{
    if(m_devinfo) delete m_devinfo;
    if(m_rootDir) delete m_rootDir;
}

void APNHelper::createAPN(const QString &apname, const QString &iap)
{
    /* ugliest fucking hack but libqgconf is pretty lacking */
    qDebug() << __PRETTY_FUNCTION__;
    QString pathname = apname;
    pathname = pathname.replace(" ", "@32@");
    pathname = pathname.replace(".", "@46@");
    QString imsi = m_devinfo->imsi();
    QString type = "--set /system/osso/connectivity/IAP/" + pathname + "/type --type string GPRS";
    QString name = "--set /system/osso/connectivity/IAP/" + pathname + "/name --type string " + apname;
    QString gprsiap = "--set /system/osso/connectivity/IAP/" + pathname + "/gprs_accesspointname --type string " + iap;
    QString ip4type = "--set /system/osso/connectivity/IAP/" + pathname + "/ipv4_type --type string AUTO";
    QString simimsi = "--set /system/osso/connectivity/IAP/" + pathname + "/sim_imsi --type string " + imsi;

    QStringList cmds;
    cmds << type << name << gprsiap << ip4type << simimsi;

    QStringList arguments;
    int ret;

    m_process = new QProcess();
    foreach(const QString& cmd, cmds) {
        arguments = cmd.split(" ");
        qDebug() << "Executing:" << m_gconfpath << arguments;
        ret = m_process->execute(m_gconfpath, arguments);
        //qDebug() << "Process returned:" << ret;
    }

    valueChanged();
}

void APNHelper::removeAPN(const QString& apname)
{
    /* ugliest fucking hack but libqgconf is pretty lacking */
    qDebug() << __PRETTY_FUNCTION__;
    QString path = IAP_PATH + "/" + apname;
    m_process = new QProcess();

    QString cmd = m_gconfpath + " --recursive-unset " + path;
    qDebug() << "Executing:" << cmd;
    int ret = m_process->execute(cmd);

    cmd = m_gconfpath + " --all-dirs " + IAP_PATH;
    ret = m_process->execute(cmd);

    valueChanged();
}


void APNHelper::valueChanged()
{
    qDebug() << __PRETTY_FUNCTION__;
    m_apnlist = removePathFromDir(m_rootDir->listDirs());
    emit listChanged();
}

QStringList APNHelper::removePathFromDir(const QStringList dirs) const
{
    qDebug() << __PRETTY_FUNCTION__;
    QStringList retList;
    foreach(QString d, dirs) {
        retList << d.remove(IAP_PATH + "/");
    }
    qDebug() << retList;
    return retList;
}

QStringList APNHelper::apnList()
{
    qDebug() << __PRETTY_FUNCTION__;
    return m_apnlist;
}

QString APNHelper::apnPrettyName(const QString &id) const
{
    qDebug() << __PRETTY_FUNCTION__;
    QString path = IAP_PATH + "/" + id + "/name";
    GConfItem* details = new GConfItem(path);
    QString name = details->value().toString();

    return name;
}

QString APNHelper::apnAccessPointName(const QString &id) const
{
    qDebug() << __PRETTY_FUNCTION__;
    QString path = IAP_PATH + "/" + id + "/gprs_accesspointname";
    GConfItem* details = new GConfItem(path);
    QString apn = details->value().toString();

    return apn;
}
