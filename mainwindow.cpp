    #include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
//tạo icon ( chưa test vì trên macos không hiển thị được icon)
    this->setWindowIcon(QIcon(":/icon.png"));
//link nút tìm và thanh search
    connect(ui->btnSearch, &QPushButton::clicked, this,  [=]() {
        QString city = ui->lineEditCity->text();
//lệnh if else để set các thành phố + nhiệt độ + thời tiết + thay đổi giữa độ C và độ F
        if (city == "Hanoi" || city == "hanoi" || city == "haNoi" || city == "ha noi" || city == "Ha Noi") {
            ui->labelCity->setText("Hà Nội - Việt Nam");
            if (ui->fButton->isChecked()) {
                ui->labelTemp->setText("🔥 86°F");
            } else {
                ui->labelTemp->setText("🔥 30°C");
            }
            ui->labelWeather->setText("☀️ Trời Nắng");
        }
        else if (city == "London" || city == "Lon don" || city == "london") {
            ui->labelCity->setText("London - Anh");
            if (ui->fButton->isChecked()) {
                ui->labelTemp->setText("❄️ 59°F");
            } else {
                ui->labelTemp->setText("❄️ 15°C");
            }
            ui->labelWeather->setText("🌧️ Mưa Rào");
        }
        else if (city == "Tokyo" || city == "tokyo") {
            ui->labelCity->setText("Tokyo - Nhật Bản");
            if (ui->fButton->isChecked()) {
                ui->labelTemp->setText("❄️ 64,4°F");
            } else {
                ui->labelTemp->setText("❄️ 18°C");
            }
            ui->labelWeather->setText("☁️ Mây Mù");
        }
        else if (city == "New York" || city == "ny" || city == "newyork" || city == "NewYork" || city == "new york") {
            ui->labelCity->setText("New York - Hoa Kỳ");
            if (ui->fButton->isChecked()) {
                ui->labelTemp->setText("🔥 80.6 °F");
            } else {
                ui->labelTemp->setText("🔥 27°C");
            }
            ui->labelWeather->setText("☀️ Trời Nắng");
        }
        else if ( city == "Sydney" || city == "sydney") {
            ui->labelCity->setText("Sydney -  Úc");
            if (ui->fButton->isChecked()) {
                ui->labelTemp->setText("🔥 71.6 °F");
            } else {
                ui->labelTemp->setText("🔥 22°C");
            }
            ui->labelWeather->setText("☁️ Mây Mù");
        }
        else {
            ui->labelCity->setText("Hãy nhập tên hợp lệ");
            ui->labelTemp->setText("--");
            ui->labelWeather->setText("--");
        }
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

//Tự động search lại thành phố sau khi đổi độ
    connect(ui->fButton, &QRadioButton::clicked, this, [=]() {
        ui->btnSearch->click();
    });

    connect(ui->cButton, &QRadioButton::clicked, this, [=]() {
        ui->btnSearch->click();
    });
//connect nút lineEdit với nút enter/return
connect(ui->lineEditCity, &QLineEdit::returnPressed, this, [=](){ ui->btnSearch->click(); });
    ui->labelIcon->setScaledContents(true);
//load ảnh trong qrc
    QPixmap pix(":/weather2.png");
    ui->labelIcon->setPixmap(pix);
    }

MainWindow::~MainWindow()
{
    delete ui;
}
