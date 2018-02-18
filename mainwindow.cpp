#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QTextStream>
#include <QFileInfo>

#include <time.h>

MainWindow::MainWindow(QWidget * parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initialize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialize()
{
    mImageIndex = mQuoteIndex = -1;

    srand(time(nullptr));

    scan();
    generate();
}

void MainWindow::generateButtonClicked()
{
    generate();
}

void MainWindow::aboutButtonClicked()
{
    QMessageBox::information(this, "About", "Hello world!");
}

void MainWindow::scan()
{
    mImagePaths.clear();
    mQuotes.clear();

    // Load images
    QDir imageDirectory(QDir::currentPath() + QDir::separator() + "Memes");
    QStringList images = imageDirectory.entryList(QStringList() << "*.*", QDir::Files);

    if(images.size() > 0)
    {
        for(const auto & image : images)
            mImagePaths.push_back(imageDirectory.absolutePath() + QDir::separator() + image);

        // Load facts
        QFile inputFile("KarlFacts.txt");

        if (inputFile.open(QIODevice::ReadOnly))
        {
            QTextStream in(&inputFile);

            while (!in.atEnd())
            {
                mQuotes.push_back(in.readLine());
            }

            inputFile.close();
        }

        qDebug() << "Image base folder: " << imageDirectory.absolutePath() << '\n'
                 << "Images count:" << mImagePaths.size() << '\n'
                 << "Quotes count:" << mQuotes.size();
    }
}

void MainWindow::load()
{
    int width  = std::max(ui->ImageLabel->width(), 300);
    int height = std::max(ui->ImageLabel->height(), 282);

    QString quote = mQuotes[mQuoteIndex];

    QPixmap image = mImagePaths[mImageIndex];
    QPixmap scaledImage = image.scaled(QSize(width, height), Qt::KeepAspectRatio);

    ui->ImageLabel->setPixmap(scaledImage);
    ui->QuoteLabel->setText(quote);
}

void MainWindow::generate()
{
    if(mImagePaths.size() > 0 && mQuotes.size() > 0)
    {
        mImageIndex = rand() % mImagePaths.size();
        mQuoteIndex = rand() % mQuotes.size();

        QString imagePath  = mImagePaths[mImageIndex];
        qDebug() << "Selected image" << imagePath.section("/", -1);

        if(!QFileInfo::exists(imagePath))
        {
            qDebug() << "File not found" << imagePath;

            QMessageBox::warning(this, "File is moved or deleted", tr("File '%1' is moved or deleted!").arg(imagePath));
            scan();
        }
        else
        {
            load();
        }
    }
    else
    {
        qDebug() << "Containers are empty!";

        QMessageBox::warning(this, "Warning", "No images & quotes found!");
        scan();
    }
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    if(mImageIndex != -1 && mQuoteIndex != -1)
        load();
}
