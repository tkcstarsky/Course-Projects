#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(400, 247);
    this->setMinimumSize(450, 278);
    this->setMaximumSize(450, 278);
    this->setWindowIcon(QIcon("hflog.ico"));

    button_open1 = new QPushButton(QString::fromLocal8Bit("open file"), this);
    button_open1->setGeometry(20, 40, 80, 37);
    path1 = new QLineEdit(this);
    path1->setReadOnly(true);
    path1->setGeometry(110, 40, 200, 37);
    button_compress = new QPushButton(QString::fromLocal8Bit("Dijkstra"), this);
    button_compress->setGeometry(350, 40, 100, 37);

    QObject::connect(button_compress, &QPushButton::clicked, this, &MainWindow::Dijkstra);

    QObject::connect(button_open1, &QPushButton::clicked, this, &MainWindow::open_compressed_file);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Dijkstra()
{
    if(dijkstra((compressed_filename->toStdString()).c_str()) == true)
        QMessageBox::information(this, tr("Information"), QString::fromLocal8Bit("Operation is done!"));
    else
        QMessageBox::information(this, tr("Information"), QString::fromLocal8Bit("Operation is failed!"));
}

void MainWindow::open_compressed_file()
{
    compressed_filename = new QString(
            QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("打开要压缩的文件"), "", tr("Allfile(*.*)"))
        );
    path1->setText(*compressed_filename);
}

