//
//  JumpGame.h
//  JumpGame
//
//  Created by nanbei on 2018/4/18.
//

#include "cocos2d.h"
#include "Obstacle.h"
#include "Obstacle.h"
using namespace std;

class JumpGame :public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(JumpGame);
    void gameStart(cocos2d::Ref* pSender);
    Obstacle* obstacle;
    void update(float time);
    int GAME_STATUS;//游戏状态
    float velocity = -1;//速度
    float gravity = 0.2;//加速度
    void onTouchesEnded(const vector<Touch*>& touches, Event* event);
    void onTouchesBegan(const vector<Touch*>& touches, Event* event);
    bool isFlying = false;
    int nscore = 0;
    float value_velocity = 2;//每次点击后的速度
};
