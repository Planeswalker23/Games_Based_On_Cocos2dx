//
//  Obstacle.cpp
//  Run3D-mobile
//
//  Created by nanbei on 2018/5/9.
//

#include "Obstacle.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;
Obstacle::Obstacle(void)
{
    srand(time(NULL));//为了每次运行时能随机
}

void Obstacle::add(Node * renderNode)
{
    float posx = 0;
    int len = renderNode->getContentSize().width;//跑道长度
    int num = 20;//金币数量
    float start = renderNode->getPositionX();//跑道位置
    for (int i = 0; i < num; i++)
    {
        posx = start - len / 2 + rand() % len;//金币位置随机
        auto obstacle = Sprite3D::create("model/coin.c3b");
        obstacle->setPosition3D(Vec3(posx, 6.0f, -30.0f + rand() % 60));
        obstacle->setRotation3D(Vec3(90.0f, 90.0f, 180.0f));//需要旋转
        renderNode->addChild(obstacle, 10, "coin");
        if (renderNode->getName() == "scene1")//根据命名确定跑道
            obstacleList1.pushBack(obstacle);
        else
            obstacleList2.pushBack(obstacle);
    }
    
    //添加怪物并设置动画
    num = 5;
    for (int i = 0; i < num; i++)
    {
        posx = start - len / 2 + rand() % len;
        auto obstacle = Sprite3D::create("model/orc.c3b");
        auto animation = Animation3D::create("model/orc.c3b");
        auto animate = Animate3D::create(animation);
        obstacle->runAction(RepeatForever::create(animate));
        obstacle->setPosition3D(Vec3(posx, 6.0f, -30.0f + rand() % 60));
        obstacle->setRotation3D(Vec3(0.0f, -90.0f, 0.0f));
        obstacle->setScale(2.0f);
        renderNode->addChild(obstacle, 10, "orc");
        if (renderNode->getName() == "scene1")
            obstacleList1.pushBack(obstacle);
        else
            obstacleList2.pushBack(obstacle);
    }
    
}

int Obstacle::collision(Node * renderNode, Sprite3D* player)
{
    int score = 0;
    if (renderNode->getName() == "scene1")
    {
        for (auto obstacle : obstacleList1)
        {
            if (!obstacle->isVisible())
                continue;
            bool res = obstacle->getAABB().intersects(player->getAABB());
            if (res)
            {
                if(obstacle->getName()=="coin"){
                    score++;
                    auto audio = SimpleAudioEngine::getInstance();
                    audio->playEffect("sound/coin.wav", false, 1.0f, 0.0f, 1.0f);
                }else if(obstacle->getName()=="orc"){
                    score--;
                    auto audio = SimpleAudioEngine::getInstance();
                    audio->playEffect("sound/orc.wav", false, 1.0f, 0.0f, 1.0f);
                }
                obstacle->setVisible(false);
            }
        }
    }
    else
    {
        for (auto obstacle : obstacleList2)
        {
            if (!obstacle->isVisible())
                continue;
            bool res = obstacle->getAABB().intersects(player->getAABB());
            if (res)
            {
                if(obstacle->getName()=="coin"){
                    score++;
                    auto audio = SimpleAudioEngine::getInstance();
                    audio->playEffect("sound/coin.wav", false, 1.0f, 0.0f, 1.0f);
                }else if(obstacle->getName()=="orc"){
                    score--;
                    auto audio = SimpleAudioEngine::getInstance();
                    audio->playEffect("sound/orc.wav", false, 1.0f, 0.0f, 1.0f);
                }
                obstacle->setVisible(false);
            }
        }
    }
    return score;
}

void Obstacle::update(Node * renderNode)
{
    float posx = 0;
    int len = renderNode->getContentSize().width;
    float start = renderNode->getPositionX();
    if (renderNode->getName() == "scene1")
    {
        for (auto obstacle : obstacleList1)
        {
            posx = start - len / 2 + rand() % len;
            obstacle->setPosition3D(Vec3(posx, 6.0f, -30.0f + rand() % 60));
            obstacle->setVisible(true);
        }
    }
    else
    {
        for (auto obstacle : obstacleList2)
        {
            posx = start - len / 2 + rand() % len;
            obstacle->setPosition3D(Vec3(posx, 6.0f, -30.0f + rand() % 60));
            obstacle->setVisible(true);
        }
    }
}
