#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore/qjsonobject.h>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    QJsonObject m_wardsData;
    QString m_darkStyle;
    QString m_lightStyle;

    void setupBasicUI();
    void initStyles();
    void loadJsonData();
    void setupConnections();
};
#endif // MAINWINDOW_H
