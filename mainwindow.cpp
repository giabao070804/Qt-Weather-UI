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
//hiển thi thời gian thực trên label
    QTime time = QTime::currentTime();
    QString time_text = time.toString("hh : mm : ss");
    ui->labelLocation->setText(time_text);

    ui->labelLocation->setAlignment(Qt::AlignCenter);
    QFont timeFont("Arial", 20, QFont::Bold);
//add địa điểm vào dòng bên dưới thời gian
    QString full_text = QString(
                            "<span style='font-size:24px; font-weight:bold;'>%1</span>"
                            "<br>"
                            "<br>"
                            "<span style='font-size:23px; color:#a0a0a0;'>Hà Nội, Việt Nam</span>"
                            ).arg(time_text);

// Set text vào label
    ui->labelLocation->setText(full_text);
    ui->labelLocation->setFont(timeFont);

//set default khi chạy app sẽ hiện trang đầu
    ui->stackedWidget->setCurrentIndex(0);
//connect nút setting để mở phần cài đặt(trang 2)
    connect(ui->settingButton, &QToolButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);
        });
 //connect nút back để quay lại giao diện chính(trang 1)
    connect(ui->settingButton2, &QToolButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
    });
//link nút tìm và thanh search
    connect(ui->btnSearch, &QPushButton::clicked, this,  [=]() {
        QString city = ui->lineEditCity->text();
        QMap<QString, QString> cityMap = {
            {"Hà Nội", "🇻🇳 Hà Nội - Việt Nam"},
            {"London", "🏴󠁧󠁢󠁥󠁮󠁧󠁿 London - Anh"},
            {"Tokyo", "🇯🇵 Tokyo - Nhật Bản"},
            {"New York", "🇺🇸 New York - Hoa Kỳ"},
            {"Sydney", "🇦🇺 Sydney - Úc"}
        };

        if (cityMap.contains(city)) {
            // Set tên thành phố
            ui->labelCity->setText(cityMap[city]);

            static QMap<QString, int> savedTemps;
            static QMap<QString, int> savedWeathers;

            // Nếu thành phố này chưa được random bao giờ -> Random và lưu vào sổ
            if (!savedTemps.contains(city)) {
                savedTemps[city] = QRandomGenerator::global()->bounded(10, 35);
                savedWeathers[city] = QRandomGenerator::global()->bounded(3); // 0, 1, 2
            }

            int tempC = savedTemps[city];
            int randomWeather = savedWeathers[city];
            // ----------------------------------------------------

            // Set thời tiết
            QStringList weathers = {"☁️ Mây Mù", "☀️ Trời Nắng", "🌧️ Mưa Rào"};
            ui->labelWeather->setText(weathers[randomWeather]);

            // Thêm icon lửa hoặc tuyết(trên 25 độ thì nóng)
            QString icon = (tempC >= 25) ? "🔥" : "❄️";

            // Xử lý C sang F
            if (ui->fButton->isChecked()) {
                double tempF = (tempC * 1.8) + 32.0;
                ui->labelTemp->setText(QString("%1 %2°F").arg(icon).arg(tempF, 0, 'f', 1));
            } else {
                ui->labelTemp->setText(QString("%1 %2°C").arg(icon).arg(tempC));
            }
        }
        else {
            // Xử lý khi nhập sai tên thành phố
            ui->labelCity->setText("Hãy nhập tên hợp lệ");
            ui->labelTemp->setText("--");
            ui->labelWeather->setText("--");


        };
    });

//đặt style tối
    QString darkStyle =
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


//đặt style sáng
    QString lightStyle =
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

//kết nối 2 style với 2 button
    connect(ui->darkButton, &QRadioButton::clicked, this, [=]() {
        this->setStyleSheet(darkStyle);
    });

    connect(ui->lightButton, &QRadioButton::clicked, this, [=]() {
        this->setStyleSheet(lightStyle);
    });

//đặt default là darkmode khi run app
    ui->darkButton->setChecked(true);

    qApp->setStyleSheet(darkStyle);

//Đặt ComboBox về vị trí mặc định
    ui->comboBox->setCurrentIndex(-1);

//Tự động search lại thành phố sau khi đổi độ
    connect(ui->fButton, &QRadioButton::clicked, this, [=]() {
        ui->btnSearch->click();
    });

    connect(ui->cButton, &QRadioButton::clicked, this, [=]() {
        ui->btnSearch->click();
    });

//Đọc file json
    QJsonObject wardsData;
    QFile file(":/data.json");
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        wardsData = doc.object().value("wards").toObject();
        file.close();
    }

 //QCompleter lấy tên thành phố trong file json
    QCompleter *completer = new QCompleter(wardsData.keys(), this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEditCity->setCompleter(completer);

//connect lineEdit với combobox
    connect(ui->lineEditCity, &QLineEdit::textChanged, this, [=](const QString &text) {
        ui->comboBox->clear();

// Nếu JSON có chứa tên thành phố vừa nhập
        if (wardsData.contains(text)) {
           const QJsonArray wardsArray = wardsData[text].toArray();


            QStringList wardList;
            for (const QJsonValue &val : wardsArray) {
                wardList << val.toString();
            }
            ui->comboBox->addItems(wardList);
        }
    });

//Khóa nút tìm kiếm khi vừa mở app
    ui->btnSearch->setEnabled(false);

//Kiểm tra tương tác với ComboBox
    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, [=](int index) {
//Mở và khoá lại nút tìm
        if (index >= 0) {
            ui->btnSearch->setEnabled(true);
        } else {

            ui->btnSearch->setEnabled(false);
        }
    });

//load ảnh trong qrc
    QPixmap pix(":/weather2.png");
    ui->labelIcon->setPixmap(pix);
}

MainWindow::~MainWindow()
{
    delete ui;
}
