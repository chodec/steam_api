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
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDate>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&manager,SIGNAL(finished(QNetworkReply*)), SLOT(download(QNetworkReply*)));
    connect(&manager2,SIGNAL(finished(QNetworkReply*)), SLOT(download_summaries(QNetworkReply*)));
    connect(&manager3,SIGNAL(finished(QNetworkReply*)), SLOT(download_img(QNetworkReply*)));

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../apihistory.db");
    db.open();
    if(!db.open())
        qDebug()<< "databaze konci";

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
            qDebug()<< "Array cannot be loaded.";
        QJsonObject objectGameCount = doc.object();
        QJsonValue valueGameCount = objectGameCount.value(QString("response"));
        QJsonObject itemGameCount = valueGameCount.toObject();
        QJsonValue vgc = itemGameCount["game_count"];
        int gameCount =vgc.toInt();

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
        qDebug() << "Player library is out.";
        return;
    }
}

void MainWindow::download_summaries(QNetworkReply *reply2)
{
    QSqlQuery dotaz;
    QString id = ui->lineEdit->text();
    QString date = QDate::currentDate().toString();

    if(!reply2->error())
    {
        QString nickname_string;
        QString url_string;
        QString img_string;
        QImage img;

        QString strReply = (QString)reply2->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonObject doc_obj = doc.object();
        QJsonObject response = doc_obj["response"].toObject();
        QJsonArray jsonArray = response["players"].toArray();
        if(jsonArray.isEmpty())
            qDebug()<< "Array cannot be loaded." ;

        foreach (const QJsonValue & value, jsonArray)
        {
            QJsonObject obj = value.toObject();
            nickname_string.append(obj["personaname"].toString());
            url_string.append(obj["profileurl"].toString());
            img_string.append(obj["avatarmedium"].toString());

        }

        url_photo = img_string;
        QNetworkRequest request3;
        request3.setUrl(url_photo);
        manager3.get(request3);

        ui->label_name->setText(nickname_string);
        ui->label_url->setText(url_string);

        dotaz.prepare("INSERT INTO browse_history(id, nickname, date) VALUES(:id, :nickname, :date)");
        dotaz.bindValue(":id",id);
        dotaz.bindValue(":nickname",nickname_string);
        dotaz.bindValue(":date",date);
        dotaz.exec();
    }

    else
    {
        qDebug()<< "Player summaries are out.";
        return;
    }

}

void MainWindow::download_img(QNetworkReply *reply)
{
    if(!reply->error())
    {
        QByteArray arr = reply->readAll();
        QImage img = QImage::fromData(arr);
        //img = img.scaledToWidth(500);
        QPixmap map;
        map.convertFromImage(img);
        ui->label_photo->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->label_photo->setPixmap(map);

    }
    else
    {
        qDebug()<<"download img err";
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    SteamApi s;
    QNetworkRequest request;
    QNetworkRequest request2;


    QString neco = ui->lineEdit->text().toLocal8Bit();

    request.setUrl(s.getUrl(neco));
    manager.get(request);

    QUrl url("http://api.steampowered.com/ISteamUser/GetPlayerSummaries/v0002/?key=4C8E0C2FC22A4DA5312994423DA9D4FC&steamids=76561198074929562");
    request2.setUrl(url);
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
