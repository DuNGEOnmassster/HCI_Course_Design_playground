#ifndef STONE_H
#define STONE_H

#include <QRect>
#include <QPainter>

class Stone
{
public:
    Stone();
    ~Stone();

    enum TYPE{CHE, MA, PAO, BING, JIANG, SHI, XIANG};

    void init(int id);

    int _row;
    int _col;
    TYPE _type;
    bool _dead;
    bool _red;
    int _id;

    QString name();

    void rotate();
};

#endif // STONE_H
