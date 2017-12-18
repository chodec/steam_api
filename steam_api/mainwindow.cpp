#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "steamapi.h"
#include <QUrl>
#include <QUrlQuery>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTextCodec>
#include <QFile>
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::download(QNetworkReply *reply)
{


    if(!reply->error())
    {

        QString gameName;
        QString strReply = (QString)reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonObject doc_obj = doc.object();
        QJsonObject response= doc_obj["response"].toObject();
        QJsonArray jsonArray =response["games"].toArray();
        if(jsonArray.isEmpty())
            qDebug()<< "array is empty";
        QJsonObject objectGameCount = doc.object();
        QJsonValue valueGameCount = objectGameCount.value(QString("response"));
        QJsonObject itemGameCount = valueGameCount.toObject();
        QJsonValue vgc = itemGameCount["game_count"];
        int gameCount =vgc.toInt();
        qDebug() << gameCount;

        foreach (const QJsonValue & value, jsonArray)
        {
            QJsonObject obj = value.toObject();
            gameName.append(obj["name"].toString());
            gameName.append("\t\n\n");
        }

        ui->textBrowser->setText("Game count of his library is: " + QString::number(gameCount) + "\n\n"
                                 + "Which contains " + "\n\n" + gameName);

        ui->textBrowser_2->setText(strReply);

    }
    else
    {
        qDebug() << "rip";
    }
}

void MainWindow::download_summaries(QNetworkReply *reply2)
{
    if(!reply2->error())
    {
        QString strReply = (QString)reply2->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonObject doc_obj = doc.object();
        QJsonObject response = doc_obj["response"].toObject();
        QJsonArray jsonArray =response["players"].toArray();
        if(jsonArray.isEmpty())
            qDebug()<< "array is empty";

        QJsonObject nickname = doc.object();
        QJsonValue nickname_value = nickname.value(QString("response"));
        QJsonObject nickname_value_item = nickname_value.toObject();
        QJsonValue nvi = nickname_value_item["personaname"];
        QString nickname_string = nvi.toString();

        QJsonObject playerUrl = doc.object();
        QJsonValue url_value = playerUrl.value(QString("response"));
        QJsonObject url_value_item = url_value.toObject();
        QJsonValue uvi = url_value_item["profileurl"];
        QString url_string = uvi.toString();

        ui->label_name->setText(nickname_string);
        ui->label_url->setText(url_string);
    }
    else
    {
        qDebug()<< "rip zase";
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    connect(&manager,SIGNAL(finished(QNetworkReply*)), SLOT(download(QNetworkReply*)));
    connect(&manager2,SIGNAL(finished(QNetworkReply*)), SLOT(download_summaries(QNetworkReply*)));
    SteamApi s;
    SteamApi a;
    QNetworkRequest request;
    QNetworkRequest request2;
    QString neco = ui->lineEdit->text().toLocal8Bit();

    request.setUrl(s.getUrl(neco));
    manager.get(request);

    request2.setUrl(a.getUrlPlayer(neco));
    manager2.get(request2);
}

void MainWindow::on_pushButtonSave_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Save Json"),tr("Set file name: "),
                                         QLineEdit::Normal,tr("fileName.json"),&ok);
    QFile file(text);
    if (ok && !text.isEmpty())
    if (!file.open(QIODevice::WriteOnly))
        return;
    file.write(ui->textBrowser->toHtml().toLatin1());
    file.close();
    ui->textBrowser->clear();
    ui->textBrowser->setText("File was succesfully saved! cg");
}
