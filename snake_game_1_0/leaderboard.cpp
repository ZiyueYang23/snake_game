#include "leaderboard.h"

Leaderboard::Leaderboard(const QString& fileName) : fileName_(fileName) {
//    fileName_="/home/ziyueyang/ubuntu_code/snake_game/snake_game_1_0/leaderboard.txt";
    load();
}

void Leaderboard::addRecord(const QString& name, int score, int time) {
    records_.append({name, score, time});
    std::sort(records_.begin(), records_.end(), [](const PlayerRecord &a, const PlayerRecord &b) {
        return a.score > b.score; // 按分数从大到小排序
    });

    if (records_.size() > 10) {
        records_.removeLast(); // 只保留前10名
    }

    save();
}

QVector<PlayerRecord>& Leaderboard::getRecords() {
    return records_;
}

void Leaderboard::load() {
    QFile file(fileName_);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString name;
            int score;
            int time;
            in >> name >> score >> time;
            records_.append({name, score, time});
        }
        file.close();
    }
}

void Leaderboard::save() {
    QFile file(fileName_);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const PlayerRecord& record : records_) {
            out << record.name << " " << record.score << " " << record.time << "\n";
        }
        file.close();
    }
}

