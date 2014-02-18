#include "testBasic.h"
#include "VisibleRect.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"

TestScene::TestScene(bool bPortrait, bool physics/* = false*/)
{
    if (physics)
    {
#if CC_USE_PHYSICS
        TestScene::initWithPhysics();
#else
        Scene::init();
#endif
    }
    else
    {
        Scene::init();
    }
}

void testScene_callback( Object *sender )
{
    Director::getInstance()->popScene();
}

void TestScene::onEnter()
{
    Scene::onEnter();

    //add the menu item for back to main menu
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
//    auto label = LabelBMFont::create("MainMenu",  "fonts/arial16.fnt");
//#else
    auto label = LabelTTF::create("MainMenu", "Arial", 40);
//#endif
    auto menuItem = MenuItemLabel::create(label, testScene_callback );
    auto menu = Menu::create(menuItem, NULL);

    menu->setPosition( Point::ZERO );
    menuItem->setPosition( Point( VisibleRect::right().x - 100, VisibleRect::bottom().y + 25) );

    addChild(menu, 1);
}