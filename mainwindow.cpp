#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//tạo icon
    this->setWindowIcon(QIcon(":/icon.png"));
//link nút tìm và thanh search
    connect(ui->btnSearch, &QPushButton::clicked, this, [=]() {
        QString city = ui->lineEditCity->text();
//lệnh if else để set các thành phố + nhiệt độ + thời tiết
        if (city == "Hanoi" || city == "hanoi" || city == "haNoi" || city == "ha noi" || city == "Ha Noi") {
            ui->labelCity->setText("Hà Nội - Việt Nam");
            ui->labelTemp->setText("🔥 30°C");
            ui->labelWeather->setText("☀️ Trời Nắng");
        }
        else if (city == "London" || city == "Lon don" || city == "london") {
            ui->labelCity->setText("London - Anh");
            ui->labelTemp->setText("❄️ 15°C");
            ui->labelWeather->setText("🌧️ Mưa Rào");
        }
        else if (city == "Tokyo" || city == "tokyo") {
            ui->labelCity->setText("Tokyo - Nhật Bản");
            ui->labelTemp->setText("❄️ 18°C");
            ui->labelWeather->setText("☁️ Mây Mù");
        }
        else if (city == "New York" || city == "ny" || city == "newyork" || city == "NewYork" || city == "new york") {
            ui->labelCity->setText("New York - Hoa Kỳ");
            ui->labelTemp->setText("🔥 27°C");
            ui->labelWeather->setText("☀️ Trời Nắng");
        }
        else if ( city == "Sydney" || city == "sydney") {
            ui->labelCity->setText("Sydney -  Úc");
            ui->labelTemp->setText("🔥 22°C");
            ui->labelWeather->setText("☁️ Mây Mù");
        }
        else {
            ui->labelCity->setText("Hãy nhập tên hợp lệ, tắt VietKey");
            ui->labelTemp->setText("--");
            ui->labelWeather->setText("--");
        }
   });

    ui->labelIcon->setScaledContents(true);
//load ảnh trong qrc
    QPixmap pix(":/weather2.png");
    ui->labelIcon->setPixmap(pix);
    }
MainWindow::~MainWindow()
{
    delete ui;
}
