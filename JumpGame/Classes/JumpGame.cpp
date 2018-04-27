//
//  JumpGame.cpp
//  JumpGame-mobile
//
//  Created by nanbei on 2018/4/18.
//

#include "cocos2d.h"
#include "JumpGame.h"
#include "resource.h"
#include "Obstacle.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;
using namespace std;

Scene* JumpGame::createScene()
{
    auto scene = Scene::create();
    auto layer = JumpGame::create();
    scene->addChild(layer);
    return scene;
}

bool JumpGame::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto winSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //background
    auto bg = Sprite::create(background);
    bg->setPosition(winSize.width*0.5, winSize.height*0.6);
    bg->setScale(winSize.width / bg->getContentSize().width, winSize.height / bg->getContentSize().height);
    this->addChild(bg);
    
    // create and initialize a label
    auto label = Label::createWithTTF(title, "fonts/Marker Felt.ttf", 24);
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + winSize.width/2,
                            origin.y + winSize.height - label->getContentSize().height));
    // add the label as a child to this layer
    this->addChild(label, 1);
    label->setColor(Color3B(0,0,0));
    label->setVisible(true);
    label->setTag(TAG_LABEL);
    
    //score
    auto score = LabelBMFont::create("0", "fonts/futura-48.fnt");
    score->setPosition(winSize.width/2,winSize.height);
    this->addChild(score, 1);
    score->setVisible(false);
    score->setTag(TAG_SCORE);
    score->setScale(1.5f);
    
    //startbutton
    auto startBtn = MenuItemImage::create(start,start,CC_CALLBACK_1(JumpGame::gameStart, this));
    auto menustart = Menu::create(startBtn, NULL);
    menustart->setTag(TAG_START_BTN);
    this->addChild(menustart);
    
    //role
    auto role = Sprite::create(role1);
    role->setPosition(winSize.width/4, winSize.height/2);
    role->setVisible(false);
    role->setTag(TAG_ROLE);
    role->setScale(0.8f);
    this->addChild(role, 1);
    
    //add animation effects
    Animation* an = Animation::create();
    an->addSpriteFrameWithFile(role2);
    an->addSpriteFrameWithFile(role3);
    an->addSpriteFrameWithFile(role4);
    an->addSpriteFrameWithFile(role5);
    an->addSpriteFrameWithFile(role6);
    an->setDelayPerUnit(delayperunit);
    an->setLoops(-1);
    Animate* anim = Animate::create(an);
    role->runAction(anim);
    
    obstacle = new Obstacle;
    this->addChild(obstacle);
    
    scheduleUpdate();
    GAME_STATUS = GAME_STATUS_START;
    
    //gameover
    auto gameover = Sprite::create(over);
    gameover->setPosition(winSize.width/2, winSize.height*0.6);
    gameover->setTag(TAG_OVER);
    gameover->setVisible(false);
    this->addChild(gameover);
    
//    //closebutton
//    auto closeItem = MenuItemImage::create(close,
//                                           close,
//                                           CC_CALLBACK_1(JumpGame::menuCloseCallback, this));
//    closeItem->setPosition(origin.x+winSize.width-closeItem->getContentSize().width/2, origin.y+winSize.height-closeItem->getContentSize().height/2);
//    closeItem->setScale(0.7f);
//    // create menu, it's an autorelease object
//    auto menuclose = Menu::create(closeItem, NULL);
//    menuclose->setPosition(Vec2::ZERO);
//    this->addChild(menuclose, 1);
    
    // touch
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesEnded = CC_CALLBACK_2(JumpGame::onTouchesEnded, this);
    listener->onTouchesBegan = CC_CALLBACK_2(JumpGame::onTouchesBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void JumpGame::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
    #endif
}

void JumpGame::gameStart(cocos2d::Ref* pSender)
{
    this->getChildByTag(TAG_START_BTN)->setVisible(false);
    this->getChildByTag(TAG_LABEL)->setVisible(false);
    this->getChildByTag(TAG_ROLE)->setVisible(true);
    this->getChildByTag(TAG_SCORE)->setVisible(true);
    GAME_STATUS = GAME_STATUS_PLAYING;
    obstacle->GAME_STATUS = GAME_STATUS_PLAYING;
    auto audio = SimpleAudioEngine::getInstance();//背景音乐
    
    // set the background music and continuously play it.
    audio->playBackgroundMusic(music_bg, true);
}

void JumpGame::update(float time)
{
    auto audio = SimpleAudioEngine::getInstance();//背景音乐
    auto winSize = Director::getInstance()->getVisibleSize();
    auto role = this->getChildByTag(TAG_ROLE);
    Rect rRole = ((Sprite*)role)->getBoundingBox();
    switch (GAME_STATUS)
    {
        case GAME_STATUS_PLAYING:
        {
            obstacle->update();//更新障碍物
            //没有触碰边界
            if (role->getPositionY() > 0 && role->getPositionY() < winSize.height)
            {
                velocity -= gravity;
                role->setPositionY(role->getPositionY() + velocity);
            }
            else{
                GAME_STATUS = GAME_STATUS_GAME_OVER;
                // stop background music.
                audio->stopBackgroundMusic();
                // play a sound effect, just once.
                audio->playEffect(music_gameover, false, 1.0f, 1.0f, 1.0f);
                break;
            }
            //碰撞检测
            for (auto obstacleSprite : obstacle->obstacleList)
            {
                if (rRole.intersectsRect(obstacleSprite->getBoundingBox()))
                {
                    GAME_STATUS = GAME_STATUS_GAME_OVER;
                    // stop background music.
                    audio->stopBackgroundMusic();
                    // play a sound effect, just once.
                    audio->playEffect(music_gameover, false, 1.0f, 1.0f, 1.0f);
                    break;
                }else{
                    int oPosX = obstacleSprite->getPositionX() + obstacleSprite->getContentSize().width/2;
                    int roleX = role->getPositionX() - role->getContentSize().width;
                    if (abs(oPosX - roleX)< 3)//可能存在问题
                    {
                        nscore++;
                        auto scoreSprite = (LabelBMFont*)this->getChildByTag(TAG_SCORE);
                        String* s = String::createWithFormat("%d", nscore / 2);
                        scoreSprite->setString(s->getCString());
                        // play a sound effect, just once.
                        audio->playEffect(music_cross, false, 1.0f, 1.0f, 1.0f);
                    }
                }
            }
            break;
        }
        case GAME_STATUS_GAME_OVER:
        {
            this->getChildByTag(TAG_OVER)->setVisible(true);
            // resume all sound effects.
            audio->resumeAllEffects();
            //重置参数
            nscore = 0;
            velocity = -1;
            break;
        }
        case GAME_STATUS_RESTART:
        {
            // resume all sound effects.
            audio->resumeAllEffects();
            //重置分数显示
            auto scoreSprite = (LabelBMFont*)this->getChildByTag(TAG_SCORE);
            String* s = String::createWithFormat("%d", nscore / 2);
            scoreSprite->setString(s->getCString());
            //重置障碍物
            obstacle->removeAllChildren();
            obstacle->obstacleList.clear();
            //重置角色位置
            role->setPosition(winSize.width/4, winSize.height/2);
            role->setVisible(false);
            this->getChildByTag(TAG_START_BTN)->setVisible(true);
            this->getChildByTag(TAG_LABEL)->setVisible(true);
            this->getChildByTag(TAG_OVER)->setVisible(false);
            this->getChildByTag(TAG_SCORE)->setVisible(false);
            break;
        }
    }
}

void JumpGame::onTouchesEnded(const vector<Touch*>& touches, Event* event)
{
    Touch* touch = touches[0];
    Point location = touch->getLocation();
    if (GAME_STATUS == GAME_STATUS_PLAYING)
    {
        isFlying = false;
    }
}

void JumpGame::onTouchesBegan(const vector<Touch*>& touches, Event* event)
{
    Touch* touch = touches[0];
    Point location = touch->getLocation();
    if (GAME_STATUS == GAME_STATUS_PLAYING)
    {
        isFlying = true;
        velocity = value_velocity;
    }
    else if (GAME_STATUS == GAME_STATUS_GAME_OVER)
    {
        GAME_STATUS = GAME_STATUS_RESTART;
    }
}

