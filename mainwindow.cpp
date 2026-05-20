#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCompleter>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QMap>
#include <QDebug>
#include <QStringList>
#include <QRandomGenerator>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupBasicUI();
    initStyles();
    loadJsonData();
    setupConnections();
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupBasicUI()
{
    QTime time = QTime::currentTime();
    QString time_text = time.toString("hh : mm : ss");

    QFont timeFont("Arial", 20, QFont::Bold);
    QString full_text = QString(
                            "<span style='font-size:24px; font-weight:bold;'>%1</span>"
                            "<br><br>"
                            "<span style='font-size:23px; color:#a0a0a0;'>Hà Nội, Việt Nam</span>"
                            ).arg(time_text);

    ui->labelLocation->setText(full_text);
    ui->labelLocation->setAlignment(Qt::AlignCenter);
    ui->labelLocation->setFont(timeFont);

    ui->stackedWidget->setCurrentIndex(0);

    QPixmap pix(":/weather2.png");
    ui->labelIcon->setPixmap(pix);

    ui->btnSearch->setEnabled(false);
    ui->comboBox->setCurrentIndex(-1);
}

void MainWindow::initStyles()
{
    m_darkStyle =
        "* { color: white; }"
        "QMainWindow { background-color: #1e1e2f; }"
        "QGroupBox { color: white; font-weight: bold; }"
        "QLineEdit { "
        "border-radius: 15px; "
        "padding: 5px 15px; "
        "background-color: #3d3d5c; "
        "color: white; "
        "border: 1px solid #555; "
        "}"
        "QComboBox { "
        "border-radius: 15px; "
        "padding: 5px 15px; "
        "background-color: #3d3d5c; "
        "color: white; "
        "border: 1px solid #555; "
        "}";
    m_lightStyle =
        "QMainWindow { background-color: #f0f0f6; }"
        "QLabel { color: #1e1e2f; }"
        "QGroupBox { color: #1e1e2f; font-weight: bold; }"
        "QRadioButton { color: #1e1e2f; }"
        "QToolButton { color: #1e1e2f; }"
        "QLineEdit { "
        "border-radius: 15px; "
        "padding: 5px 15px; "
        "background-color: #ffffff; "
        "color: #1e1e2f; "
        "border: 1px solid #cccccc; "
        "}"
        "QComboBox { "
        "border-radius: 15px; "
        "padding: 5px 15px; "
        "background-color: #ffffff; "
        "color: #1e1e2f; "
        "border: 1px solid #cccccc; "
        "}";
    }

void MainWindow::loadJsonData()
{
    QFile file(":/data.json");
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        m_wardsData = doc.object().value("wards").toObject();
        file.close();
    }
}

void MainWindow::setupConnections()
{

    connect(ui->darkButton, &QRadioButton::clicked, this, [=]() {
        this->setStyleSheet(m_darkStyle);
    });

    connect(ui->lightButton, &QRadioButton::clicked, this, [=]() {
        this->setStyleSheet(m_lightStyle);
    });

    connect(ui->settingButton, &QToolButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->settingButton2, &QToolButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
    });
    QCompleter *completer = new QCompleter(m_wardsData.keys(), this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEditCity->setCompleter(completer);

    connect(ui->lineEditCity, &QLineEdit::textChanged, this, [=](const QString &text) {
        ui->comboBox->clear();
        if (m_wardsData.contains(text)) {
            const QJsonArray wardsArray = m_wardsData[text].toArray();
            QStringList wardList;
            for (const QJsonValue &val : wardsArray) {
                wardList << val.toString();
            }
            ui->comboBox->addItems(wardList);
        }
    });

    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, [=](int index) {
        if (index >= 0) {
            ui->btnSearch->setEnabled(true);
        } else {
            ui->btnSearch->setEnabled(false);
        }
    });

    connect(ui->fButton, &QRadioButton::clicked, this, [=]() {
        ui->btnSearch->click();
    });
    connect(ui->cButton, &QRadioButton::clicked, this, [=]() {
        ui->btnSearch->click();
    });

    connect(ui->btnSearch, &QPushButton::clicked, this,  [=]() {
        QString city = ui->lineEditCity->text();
        QMap<QString, QString> cityMap = {
            {"Hà Nội", "🇻🇳 Hà Nội - Việt Nam"},
            {"London", "🇬🇧 London - Anh"},
            {"Tokyo", "🇯🇵 Tokyo - Nhật Bản"},
            {"New York", "🇺🇸 New York - Hoa Kỳ"},
            {"Sydney", "🇦🇺 Sydney - Úc"}
        };

        if (cityMap.contains(city)) {

            ui->labelCity->setText(cityMap[city]);

            static QMap<QString, int> savedTemps;
            static QMap<QString, int> savedWeathers;

            if (!savedTemps.contains(city)) {
                savedTemps[city] = QRandomGenerator::global()->bounded(10, 35);
                savedWeathers[city] = QRandomGenerator::global()->bounded(3);
            }

            int tempC = savedTemps[city];
            int randomWeather = savedWeathers[city];

            QStringList weathers = {"☁️ Mây Mù", "☀️ Trời Nắng", "🌧️ Mưa Rào"};
            ui->labelWeather->setText(weathers[randomWeather]);

            QString icon = (tempC >= 25) ? "🔥" : "❄️";

            if (ui->fButton->isChecked()) {
                double tempF = (tempC * 1.8) + 32.0;
                ui->labelTemp->setText(QString("%1 %2°F").arg(icon).arg(tempF, 0, 'f', 1));
            } else {
                ui->labelTemp->setText(QString("%1 %2°C").arg(icon).arg(tempC));
            }
        }
        else {
            ui->labelCity->setText("Hãy nhập tên hợp lệ");
            ui->labelTemp->setText("--");
            ui->labelWeather->setText("--");
        }
    });
}