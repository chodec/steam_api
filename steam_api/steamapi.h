#ifndef STEAMAPI_H
#define STEAMAPI_H
#include <QString>
#include <QUrlQuery>
#define URL "http://api.steampowered.com/IPlayerService/GetOwnedGames/v0001/"
#define URLPHOTO "http://api.steampowered.com/ISteamUser/GetPlayerSummaries/v0002/"
#define APIKEY "4C8E0C2FC22A4DA5312994423DA9D4FC"

class SteamApi
{
public:
    SteamApi();
    void addQuery(QString key, QString value, QString key1, QString value1);
    QUrl getUrl(QString neco);
    QUrl getUrlPlayer(QString neco);
    void clear();
    void clearPhoto();

private:
    QUrlQuery query;
    QUrlQuery queryPhoto;
};

#endif // STEAMAPI_H
