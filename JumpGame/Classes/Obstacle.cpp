//
//  Obstacle.cpp
//  JumpGame-mobile
//
//  Created by nanbei on 2018/4/19.
//

#include "Obstacle.h"
#include "resource.h"
Obstacle::Obstacle(void)
{
    
}

void Obstacle::update()
{
    if (GAME_STATUS != GAME_STATUS_PLAYING)
        return;
    addCount++;//计数器
    if (addCount == dis_obstacles)//每50帧增加一个障碍物
    {
        addOne(0);
        addCount = 0;
    }
    for (auto it = obstacleList.begin(); it != obstacleList.end();)
    {
        //更新每个障碍物的位置，如果到达最左边则移除
        Sprite* s = *it;
        s->setPositionX(s->getPositionX() - movingspeed);//可控制画面移动速度
        if (s->getPositionX() <-s-> getContentSize().width / 2)
        {
            it = obstacleList.erase(it);
            this->removeChild(s);
        }
        else
            it++;
    }
}

void Obstacle::addOne(int offsetX)
{
    Size size = Director::getInstance()->getWinSize();
    auto sprite = Sprite::create(obstacle);//上方障碍物
    Size spriteSize = sprite->getContentSize();
    obstacleList.pushBack(sprite);
    this->addChild(sprite);
    auto sprite2 = Sprite::create(obstacle);//下方障碍物
    Size spriteSize2 = sprite->getContentSize();
    obstacleList.pushBack(sprite2);
    this->addChild(sprite2);
    // 设置位置，x值相同
    int maxUpY = size.height + spriteSize.height / 4;
    int minUpY = size.height - spriteSize.height / 4;
    int y1 = CCRANDOM_0_1()*(maxUpY - minUpY) + minUpY;
    int maxDownY = spriteSize.height / 4;
    int minDownY = -spriteSize.height / 4;
    int y2 = CCRANDOM_0_1()*(maxDownY - minDownY) + minDownY;
    if (y1 - y2 - spriteSize.height < dis_bewteenobstacles)
    {
        y2 = y1 - spriteSize.height - dis_bewteenobstacles;
    }
    sprite->setPosition(size.width + spriteSize.width / 2 + offsetX, y1);
    sprite2->setPosition(size.width + spriteSize2.width / 2 + offsetX, y2);
}
