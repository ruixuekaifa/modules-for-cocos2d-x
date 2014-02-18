#ifndef _FONT_TEST_H_
#define _FONT_TEST_H_

#include "cocos2d.h"
#include "../testBasic.h"

class TestSceneExample : public TestScene
{
    ADDTEST(TestSceneExample)
public:
    virtual void runThisTest();
};

class TestLayerExample : public Layer
{
public:
    virtual bool init();
    CREATE_FUNC(TestLayerExample);
};


#endif // _FONT_TEST_H_