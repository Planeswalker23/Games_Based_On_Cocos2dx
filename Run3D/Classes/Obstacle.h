//
//  Obstacle.hpp
//  Run3D-mobile
//
//  Created by nanbei on 2018/5/9.
//


#pragma once
#include "cocos2d.h"
#include <vector>
using namespace cocos2d;
class  Obstacle :public Node
{
public:
    Vector<Sprite3D*> obstacleList1, obstacleList2;//场景中金币或怪物
    Obstacle();
    void add(Node * renderNode);//用来向节点中添加障碍物
    int collision(Node * renderNode, Sprite3D* player);//其中的renderNode是为了确定是哪个跑道的金币。返回值是分数值（碰撞一次得分1分，同时金币消失）
    void update(Node * renderNode);
    
};
