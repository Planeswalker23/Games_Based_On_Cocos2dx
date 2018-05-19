//
//  GameScene.cpp
//  Run3D-mobile
//
//  Created by nanbei on 2018/5/9.
//

#include "cocos2d.h"
#include "GameScene.h"
#include "Obstacle.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;
using namespace std;

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    
    //创建相机
    auto winSize = Director::getInstance()->getWinSize();
    auto camera1 = Camera::createOrthographic(winSize.width, winSize.height, 0, 1000);
    camera1->setCameraFlag(CameraFlag::USER1);
    scene->addChild(camera1);
    //创建背景层
    auto bg_layer = Layer::create();
    scene->addChild(bg_layer);
    auto the_background = Sprite::create("model/back_ground.jpg");
    the_background->setAnchorPoint(Vec2(0, 0));
    bg_layer->addChild(the_background, -100);
    bg_layer->setCameraMask((unsigned short)CameraFlag::USER1);//在上述相机中可见
    
    //自适应屏幕大小
    double ratio = winSize.height / 600;
    the_background->setScale(ratio);
    
    //游戏场景层
    auto layer = GameScene::create();
    scene->addChild(layer, 100);

    //背景音乐
    auto audio = SimpleAudioEngine::getInstance();
    audio->playBackgroundMusic("sound/background.mp3", true);
    
    
    
    return scene;
}

bool GameScene::init()
{
    //游戏层场景的初始化
    //初始化跑道
    scene1 = cocos2d::Sprite3D::create("model/scene.c3b");
    scene1->setScale(0.2f);
    scene1->setPosition3D(Vec3(0, 0, 0));
    this->addChild(scene1, 10, "scene1");//命名为scene1
    
    auto winSize = Director::getInstance()->getWinSize();
    
    //创建一个角色3D精灵
    double ratio = winSize.height / 600;
    player = Sprite3D::create("model/zhanshi_pao.c3b");
    player->setScale(ratio);
    player->setPosition3D(Vec3(0, 0, 0));//人物处在原点
    this->addChild(player, 10);

    player->setRotation3D(Vec3(0, -90, 0));//旋转角度

    //添加动画
    auto aniamtion = Animation3D::create("model/zhanshi_pao.c3b");
    auto animate = Animate3D::create(aniamtion);
    player->runAction(RepeatForever::create(animate));
    
    //添加精灵监听
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    Device::setAccelerometerEnabled(true);//加速度传感器需要开启
    auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(GameScene::onAcceleration, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    //场景从原点向相机方向移动
    auto moveTo = MoveTo::create(5, Vec3(400, 0, 0));
    scene1->runAction(moveTo);
    
    //场景移动 实现循环
    scene2 = cocos2d::Sprite3D::create("model/scene.c3b");
    scene2->setScale(0.2f);
    scene2->setPosition3D(Vec3(-400, 0, 0));
    moveTo = MoveTo::create(10, Vec3(400, 0, 0));
    scene2->runAction(moveTo);
    this->addChild(scene2, 10, "scene2");//命名为scene2
    curScene = scene1;
    
    //调用update
    schedule(schedule_selector(GameScene::upDateScene));
    
    obstacle = new Obstacle;
    obstacle->add(scene1);
    obstacle->add(scene2);
    
    //创建游戏层相机
    auto camera = Camera::createPerspective(60, winSize.width / winSize.height, 1, 200);
    camera->setCameraFlag(CameraFlag::USER2);
    camera->setPosition3D(Vec3(0, 0, 200));
    camera->lookAt(Vec3(0, 0, 0), Vec3(0, 1, 0));
    this->addChild(camera);
    this->setCameraMask((unsigned short)CameraFlag::USER2);
    
    //修改观察位置（从x轴右侧向左侧看）
    camera->setPosition3D(Vec3(20, 20, 0));
    camera->lookAt(Vec3(-20, 0, 0), Vec3(0, 1, 0));
    
    auto s = LabelBMFont::create("0", "fonts/futura-48.fnt");
    s->setPosition(Vec2(winSize.width / 2, winSize.height*0.9f));
    this->addChild(s, 100, "score");//命名
    s->setCameraMask((unsigned short)CameraFlag::USER1);//背景层的相机
    
    
    if (!Layer::init())
    {
        return false;
    }
    return true;
}

void GameScene::upDateScene(float dt)
{
    //obstacle->collision(curScene, player);
    score += obstacle->collision(curScene, player);
    char str[100];
    sprintf(str, "%d", score);
    auto s = (LabelBMFont*)getChildByName("score");
    s->setString(str);
    if (curScene->getPositionX() >= 400)
    {
        if (curScene == scene1)
        {
            curScene = scene2;
            scene1->setPositionX(-400);
            obstacle->update(scene1);//增加语句
            auto moveTo = MoveTo::create(10, Vec3(400, 0, 0));
            scene1->runAction(moveTo);
        }
        else
        {
            curScene = scene1;
            scene2->setPositionX(-400);
            obstacle->update(scene2);//增加语句
            auto moveTo = MoveTo::create(10, Vec3(400, 0, 0));
            scene2->runAction(moveTo);
        }
    }

    velocity -= gravity;
    float pos = player->getPositionY() + velocity;
    if (pos < 0)
    {
        isflying = false;
        pos = 0;
    }
    player->setPositionY(pos);
}

bool GameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    Point location = touch->getLocation();
    if (isflying == false)//控制只能跳一次
    {
        isflying = true;
        velocity = 2.5f;
    }
    return true;
}

void GameScene::onTouchEnded(Touch *touch, Event *unused_event)//暂时未用
{
    Point location = touch->getLocation();
}

void GameScene::onTouchMoved(Touch *touch, Event *unused_event)
{
    Point location = touch->getLocation();
    Point temp = touch->getDelta();
    float pos = player->getPositionZ() - temp.x/50.0f;
    if (pos < -12.0f)
        pos = -12.0f;
    if (pos > 12.0f)
        pos = 12.0f;
    player->setPositionZ(pos);
}

void GameScene::onAcceleration(Acceleration* acc, Event* event)
{
    float pos = player->getPositionZ() - acc->x;
    if (pos < -12.0f)
        pos = -12.0f;
    if (pos > 12.0f)
        pos = 12.0f;
    player->setPositionZ(pos);
}
