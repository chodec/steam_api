#include "steamapi.h"
#include "mainwindow.h"
SteamApi::SteamApi()
{

}

void SteamApi::addQuery(QString key, QString value, QString key1, QString value1)
{
    this->query.addQueryItem(key, value);
    this->queryPhoto.addQueryItem(key1, value1);

}

QUrl SteamApi::getUrl(QString neco)
{
    QUrl url(URL);
    query.addQueryItem("key",APIKEY);
    query.addQueryItem("steamid", neco);
    query.addQueryItem("include_appinfo", "1");
    query.addQueryItem("format", "json");
    url.setQuery(query);
    query.clear();
    return url;
}

QUrl SteamApi::getUrlPlayer(QString neco)
{
    QUrl urlPhoto(URLPHOTO);
    queryPhoto.addQueryItem("key",APIKEY);
    queryPhoto.addQueryItem("steamid", neco);
    urlPhoto.setQuery(queryPhoto);
    queryPhoto.clear();
    return urlPhoto;
    qDebug()<<urlPhoto << " dsadfasf";
}

void SteamApi::clear()
{
    query.clear();
}

void SteamApi::clearPhoto()
{
    queryPhoto.clear();
}
