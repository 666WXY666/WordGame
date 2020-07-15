#include "user.h"

user::user()
{

}
user::~user()
{

}
player::player(QString name,int level)
{
    this->name=name;
    this->level=level;
}
void player::level_up()
{
    for(;experience>=level;)
    {
        experience=experience-level;
        level++;
    }
}
player::~player()
{

}
creater::creater(QString name,int level)
{
    this->name=name;
    this->level=level;
}
void creater::level_up()
{
    for(;experience>=level;)
    {
        experience=experience-level;
        level++;
    }
}
creater::~creater()
{

}
