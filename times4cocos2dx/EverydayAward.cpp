//
//  EverydayAward.cpp
//
//  Created by qiong on 14-3-13.
//
//

#include "EverydayAward.h"
#include "Variables.h"
#include <sstream>
#include "Timer.h"
EverydayAward::EverydayAward()
{
}
EverydayAward::~EverydayAward(){
    delete []_awardsArr;
}

void EverydayAward::onTick(system_clock::time_point&& current_time)
{
    onTick(current_time);
}
void EverydayAward::onTick(system_clock::time_point& current_time)
{
    if (isOnlineYesterday(current_time)) {//昨天上线了
        _nthOnline+=1;
        _hasAcceptAward = false;
        if (_nthOnline == _daysNum) {   //
            _nthOnline = 0;
        }
    }else if(!isOnlineToday(current_time)){//昨天没上线, 今儿也没上线
        _nthOnline = 0;
        _hasAcceptAward = false;
    }
    _lastOnlineTime = current_time;
    TempVar* localData = VARIABLES_LOCAL("times_awards.json");
    localData->setInt("dayAfterDay_nthOnline", _nthOnline);
    std::stringstream ss;
    ss<<system_clock::to_time_t(_lastOnlineTime);
    std::string value = ss.str();
    localData->setString("dayAfterDay_lastOnline", value);
    localData->setBool("dayAfterDay_hasAcceptAwd", _hasAcceptAward);
    VARIABLES.persistLocal("times_awards.json");
}

void EverydayAward::acceptAward(std::function<bool(bool,int, int)>&& callback)
{
    acceptAward(callback);
}
void EverydayAward::acceptAward(std::function<bool(bool,int, int)>& callback)
{
    if (!_hasAcceptAward && callback(true, _awardsArr[_nthOnline], _itemType)) {
        _hasAcceptAward = true;
        TempVar* localData = VARIABLES_LOCAL("times_awards.json");
        localData->setBool("dayAfterDay_hasAcceptAwd", _hasAcceptAward);
        VARIABLES.persistLocal("times_awards.json");
    }
}

void EverydayAward::restartCount()
{
    //
}

void EverydayAward::loadAward(ValueMap&& map)
{
    loadAward(map);
}
void EverydayAward::loadAward(ValueMap& map)
{
    _itemType = map["itemType"].asInt();
    ValueVector vv = map["awardsNums"].asValueVector();
    _daysNum = vv.size();
    _awardsArr = new int[_daysNum];
    std::transform(vv.begin(), vv.end(), _awardsArr, [](Value& v)->int{return v.asInt();});
    
    TempVar* localData = VARIABLES_LOCAL("times_awards.json");
    _nthOnline = localData->getInt("dayAfterDay_nthOnline");
    _hasAcceptAward = localData->getBool("dayAfterDay_hasAcceptAwd");
    std::stringstream ss(localData->getString("dayAfterDay_lastOnline"));
    time_t tt = 0;
    ss>>tt;
    if(tt == 0) _lastOnlineTime = MyTimer::getInstance().getCurrentTime();
    else _lastOnlineTime = system_clock::from_time_t(tt);
}

int EverydayAward::getNthOnline()
{
    return _nthOnline;
}

bool EverydayAward::isOnlineYesterday(system_clock::time_point& current_time)
{
    system_clock::time_point yesterday_time = current_time - days(1);
    return isOnlineToday(yesterday_time);
}

bool EverydayAward::isOnlineToday(system_clock::time_point& current_time)
{
    time_t t =system_clock::to_time_t(_lastOnlineTime);
    struct tm* lct = localtime(&t);
    char buf[100];
    strftime(buf,100,"%F",lct);         //eg. 2014-03-15
    std::string date1(buf);
    t = system_clock::to_time_t(current_time);
    lct = localtime(&t);
    strftime(buf,100,"%F",lct);
    std::string date2(buf);
    if (date1 == date2) {
        return true;
    }else
        return false;
}

#include <iostream>
void EverydayAward::debug()
{
    std::cout<<"******EverydayAward*********"<<std::endl;
    std::cout<<"_daysNum:"<<_daysNum<<std::endl;
    std::copy(_awardsArr, _awardsArr+_daysNum, std::ostream_iterator<int>(std::cout,", "));
    std::cout<<"_itemType:"<<_itemType<<std::endl;
    std::cout<<"_nthOnline:"<<_nthOnline<<std::endl;
    std::cout<<"_lastOnlineTime:"<<system_clock::to_time_t(_lastOnlineTime)<<std::endl;
    std::cout<<"_hasAcceptAward:"<<std::boolalpha<<_hasAcceptAward<<std::endl;
    std::cout<<"****************************"<<std::endl;
}