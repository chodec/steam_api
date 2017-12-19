#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QUrl url_photo;

private slots:
    void download(QNetworkReply *reply);
    void download_summaries(QNetworkReply *reply2);
    void download_img(QNetworkReply *reply);

    void on_pushButton_2_clicked();

    void on_pushButtonSave_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager manager;
    QNetworkAccessManager manager2;
    QNetworkAccessManager manager3;

};

#endif // MAINWINDOW_H
