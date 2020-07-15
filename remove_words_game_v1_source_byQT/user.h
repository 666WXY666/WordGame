#ifndef USER_H
#define USER_H
#include<QString>

class user
{
public:
    user();
    virtual void level_up()=0;
    QString name;
    int experience;
    int level;
    virtual ~user();
};
class player:public user
{
public:
    player(QString name,int level);
    void level_up();
    virtual ~player();
private:
    int passed_num;

};

class creater:public user
{
public:
    creater(QString name,int level);
    void level_up();
    virtual ~creater();
private:
    int create_num;
};

#endif // USER_H
