#include <QtGui/QApplication>
//#include "qmlapplicationviewer.h"
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDeclarativeView>
#include <QUrl>
#include "apnhelper.h"

#include <applauncherd/MDeclarativeCache>


Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QApplication *app = MDeclarativeCache::qApplication(argc, argv);
    app->setOrganizationDomain("frals.se");
    app->setApplicationName("fapn");

    QFont font = QFont("Nokia Pure Text Light");
    app->setFont(font);

    QDeclarativeView *view = MDeclarativeCache::qDeclarativeView();
    QDeclarativeContext *ctx = view->rootContext();

    view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view->setInputMethodHints(Qt::ImhNoPredictiveText);

    APNHelper *a = new APNHelper();
    ctx->setContextProperty("apnhelper", a);

//    QString pathInInstallDir = QCoreApplication::applicationDirPath()
//            + QLatin1String("/../") + "qml/fAPN";

//    view->setSource(QUrl(pathInInstallDir + "/main.qml"));
    view->setSource(QUrl("qrc:/qml/main.qml"));
    view->showFullScreen();

    //QmlApplicationViewer viewer;
//    viewer.rootContext()->setContextProperty("apnhelper", a);

//    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
//    viewer.setMainQmlFile(QLatin1String("qrc:/qml/main.qml"));

//    viewer.showExpanded();



    return app->exec();
}
