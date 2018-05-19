//
//  GameScene.hpp
//  Run3D-mobile
//
//  Created by nanbei on 2018/5/9.
//

#include "cocos2d.h"
#include "Obstacle.h"
using namespace std;
USING_NS_CC;
class GameScene:public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameScene);
    Sprite3D* scene1;
    Sprite3D* player;
    Sprite3D* curScene;
    Sprite3D* scene2;
    void upDateScene(float dt);
    Obstacle* obstacle;
    //添加onTouchMoved成员函数响应
    bool onTouchBegan(Touch *touch, Event *unused_event);
    void onTouchEnded(Touch *touch, Event *unused_event);
    void onTouchMoved(Touch *touch, Event *unused_event);
    void onAcceleration(Acceleration* acc, Event* event);
    float gravity = 0.15f;
    float velocity = 0.0f;
    bool isflying = false;
    int score = 0;
    
};
