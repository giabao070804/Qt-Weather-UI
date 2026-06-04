#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QApplication"
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
#include <QIcon>
#include <QTimer>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupBasicUI();
    initStyles();
    loadJsonData();
    setupConnections();

    //set giao diện tối khi vừa chạy
    qApp->setStyleSheet(m_darkStyle);
    ui->darkButton->setCheckable(true);

    //tạo Qtimer đo thời giant thực
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateClock);
    timer->start(100);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupBasicUI()
{
       //set hiển thị trang đầu khi chạy
    ui->stackedWidget->setCurrentIndex(0);

    //set ảnh icon nhỏ
    QPixmap pix(":/weather.png");
    ui->labelIcon->setPixmap(pix);

    //set icon
    this->setWindowIcon(QIcon(":/icon.png"));

    //tắt nút search và set combobox về trạng thái trống
    ui->btnSearch->setEnabled(false);
    ui->comboBox->setCurrentIndex(-1);
}

void MainWindow::initStyles()
{
    //set giao diện tối
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

    //set giao diện sáng
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

    //đọc file các phường trong file json
    QFile file(":/data.json");
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        m_wardsData = doc.object().value("wards").toObject();
        file.close();
    }
}
    void MainWindow::updateClock()
{
    //Gắn thời gian thực vào label để hiện thị thời gian và địa điểm "Hà Nội, Việt Nam"
    QTime time = QTime::currentTime();
    QString time1_text = time.toString("hh : mm : ss");
    QString time2_text = time.toString("hh : mm : ss ap");
    QFont timeFont("Arial", 20, QFont::Bold);
    QString full_text;
    //set 2 mode định dạng thời gian
    if (ui->twofourButton->isChecked()) {
      full_text = QString(
                            "<span style='font-size:30px; font-weight:bold;'>%1</span>"
                            "<br>" "<br>"
                            "<span style='font-size:20px; color:#a0a0a0;'>Hà Nội, Việt Nam (UTC +7)</span>"
                            ).arg(time1_text);
    } else {
        full_text = QString(
                            "<span style='font-size:30px; font-weight:bold;'>%1</span>"
                            "<br>" "<br>"
                            "<span style='font-size:20px; color:#a0a0a0;'>Hà Nội, Việt Nam (UTC +7)</span>"
                            ).arg(time2_text);
    }

    ui->labelLocation->setText(full_text);
    ui->labelLocation->setAlignment(Qt::AlignCenter);
    ui->labelLocation->setFont(timeFont);
}
void MainWindow::setupConnections()
{

    //connect 2 nút giao diện sáng và tối
    connect(ui->darkButton, &QRadioButton::clicked, this, [=]() {
        this->setStyleSheet(m_darkStyle);
    });
    connect(ui->lightButton, &QRadioButton::clicked, this, [=]() {
        this->setStyleSheet(m_lightStyle);
    });

    //connect 2 nút chuyển trang
    connect(ui->settingButton, &QToolButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->settingButton2, &QToolButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    //set QCompleter gợi ý từ điền trong lineEdit
    QCompleter *completer = new QCompleter(m_wardsData.keys(), this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEditCity->setCompleter(completer);

    connect(ui->lineEditCity, &QLineEdit::textChanged, this, [=](const QString &text) {
        ui->comboBox->clear();

        //Completer đọc trực tiếp các tên thành phố trong file json
        if (m_wardsData.contains(text)) {
            const QJsonArray wardsArray = m_wardsData[text].toArray();
            QStringList wardList;
            for (const QJsonValue &val : wardsArray) {
                wardList << val.toString();
            }
            ui->comboBox->addItems(wardList);
        }
    });

    //mở núi tìm sau khi điền thành phố và phường
    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, [=](int index) {
        if (index >= 0) {
            ui->btnSearch->setEnabled(true);
        } else {
            ui->btnSearch->setEnabled(false);
        }
    });

    //connect 2 nút độ C và F
    connect(ui->fButton, &QRadioButton::clicked, this, [=]() {
        ui->btnSearch->click();
    });
    connect(ui->cButton, &QRadioButton::clicked, this, [=]() {
        ui->btnSearch->click();
    });

    //set các thông tin thành phố cơ bản
    connect(ui->btnSearch, &QPushButton::clicked, this,  [=]() {
        QString city = ui->lineEditCity->text();
        QString ward = ui->comboBox->currentText();
        QMap<QString, QString> cityMap = {
            {"Hà Nội", "🇻🇳 Hà Nội - Việt Nam",},
            {"London", "🇬🇧 London - Anh"},
            {"Tokyo", "🇯🇵 Tokyo - Nhật Bản"},
            {"New York", "🇺🇸 New York - Hoa Kỳ"},
            {"Sydney", "🇦🇺 Sydney - Úc"}
        };

        if (cityMap.contains(city)) {
            //hiển thị tên thành phố + tên phường
            ui->labelCity->setText(cityMap[city] + " , " + ward);

            static QMap<QString, int> savedTemps;
            static QMap<QString, int> savedWeathers;

            //random nhiệt độ trong khoảng giữa 10 và 35, thời tiết random 3 trạng thái
            if (!savedTemps.contains(city)) {
                savedTemps[city] = QRandomGenerator::global()->bounded(10, 35);
                savedWeathers[city] = QRandomGenerator::global()->bounded(3);
            }
            int tempC = savedTemps[city];
            int randomWeather = savedWeathers[city];

            //set 3 trạng thái thời tiết
            QStringList weathers = {"☁️ Mây Mù", "☀️ Trời Nắng", "🌧️ Mưa Rào"};
            ui->labelWeather->setText(weathers[randomWeather]);

            //hiển thị emoji lửa khi nhiệt độ hơn hoặc bằng 25, băng khi dưới 25
            QString icon = (tempC >= 25) ? "🔥" : "❄️";

            //quy đổi từ độ C sang độ F
            if (ui->fButton->isChecked()) {
                double tempF = (tempC * 1.8) + 32.0;
                ui->labelTemp->setText(QString("%1 %2°F").arg(icon).arg(tempF, 0, 'f', 1));
            } else {
                ui->labelTemp->setText(QString("%1 %2°C").arg(icon).arg(tempC));
            }
        }
        //Những trường hợp còn lại, hiển thị không hợp lệ
        else {
            ui->labelCity->setText("Hãy nhập tên hợp lệ");
            ui->labelTemp->setText("--");
            ui->labelWeather->setText("--");
        }
    });
}
//Thêm messageBox là miêu tả ứng dụng
void MainWindow::on_aboutButton_clicked()
{
    QMessageBox msgBox(this);
    msgBox.setText(" Tên ứng dụng: Weather"
                   "<br>""<br>"
                   "Mô tả: Một dự án được xây dựng bằng Qt nhằm mục đích học cách sử dụng và phát triển bằng Framework Qt và đồng thời thành thạo C++"
                   "<br>""<br>"
                   "Phiên bản: 1.0.0"
                   "<br>""<br>"
                   "Phát triển bởi: Nguyễn Gia Bảo" "<br>");
    msgBox.exec();
}

