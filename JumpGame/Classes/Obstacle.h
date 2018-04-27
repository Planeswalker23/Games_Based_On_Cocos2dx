//
//  Obstacle.h
//  JumpGame
//
//  Created by nanbei on 2018/4/19.
//

#include "cocos2d.h"
#include <vector>
using namespace cocos2d;
#pragma once
class  Obstacle :public Node
{
public:
    Vector<Sprite*> obstacleList;//场景中的障碍物
    Obstacle();
    void update();
    void addOne(int offsetX);
    int addCount = 0;
    int GAME_STATUS = 0;
    
    float dis_obstacles = 45;//每45帧增加一个障碍物
    float dis_bewteenobstacles = 120;//上下障碍物间距
    float movingspeed = 5;//画面移动速度，越大越快
};
