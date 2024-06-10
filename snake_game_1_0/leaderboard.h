#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <QVector>
#include <QString>
#include <QPair>

struct PlayerRecord
{
    QString name;
    int score;
    int time;
};

#include <QFile>
#include <QTextStream>

class Leaderboard
{
public:
    Leaderboard(const QString &file_name = "/home/ziyueyang/ubuntu_code/snake_game/snake_game_1_0/leaderboard.txt");

    void AddRecord(const QString &name, int score, int time);

    QVector<PlayerRecord> &GetRecords();

private:
    void Load();

    void Save();

    QString file_name_;
    QVector<PlayerRecord> records_;
};

#endif // LEADERBOARD_H
