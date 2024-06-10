#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <QVector>
#include <QString>
#include <QPair>

struct PlayerRecord {
    QString name;
    int score;
    int time;
};

#include <QFile>
#include <QTextStream>

class Leaderboard {
public:
    Leaderboard(const QString &fileName="/home/ziyueyang/ubuntu_code/snake_game/snake_game_1_0/leaderboard.txt");

    void addRecord(const QString& name, int score, int time) ;

    QVector<PlayerRecord>& getRecords() ;

private:
    void load() ;

    void save() ;

    QString fileName_;
    QVector<PlayerRecord> records_;
};


#endif // LEADERBOARD_H
