#include "api.h"
#include "mainwindow.h"

API::API()
{

}

void API::addQuery(QString key, QString value)
{
    this->query.addQueryItem(key, value);
}

QUrl API::getUrl(QString nazev)
{
    QUrl url("http://api.openweathermap.org/data/2.5/weather?");

    query.addQueryItem("q", nazev);
    query.addQueryItem("appid","6e468b98a321945daad97aea311ff9dd");
     //query.addQueryItem("format", "json");
    url.setQuery(query);
    query.clear();
    return url;
}

void API::clear()
{
    query.clear();
}
