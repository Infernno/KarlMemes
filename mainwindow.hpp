#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QVector>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget * parent = nullptr);
    ~MainWindow();

private slots:
    void initialize();

    void generateButtonClicked();
    void aboutButtonClicked();

private:
    void scan();
    void load();
    void generate();

    void resizeEvent(QResizeEvent * event);

    Ui::MainWindow * ui;

    int mImageIndex;
    int mQuoteIndex;

    QVector<QString> mImagePaths;
    QVector<QString> mQuotes;
};

#endif // MAINWINDOW_HPP
