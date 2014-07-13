//
//  AudioManager.cpp
//  DragonGeneral
//
//  Created by taoming li
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "AudioManager.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

#define KEY_AUDIO_BACKGROUND_ENABLED "Audio_Background_Enabled"
#define KEY_AUDIO_EFFECT_ENABLED "Audio_Effect_Enabled"

#define KEY_GROUND_AUDIO_V "ground_audio_volume"

const char * g_EffectTable[] = {
    "mirco/audio/CHIBI.mp3",             //city01 background music
    "mirco/audio/CITY_CHANGAN.mp3",      //city02 background music
    "mirco/audio/CITY_SUANZAODAYING.mp3",//blockpoint background music
    "mirco/audio/HANZHONGGUMU.mp3",      //fuben 01
    "mirco/audio/BEIFAXIONGNU.mp3",      //fuben02
    
    "mirco/audio/ZHANDOU_JINGJICHANG.mp3",//battle music
	
    "mirco/audio/ZHANDOU_BOSS.mp3",
    "mirco/audio/attack4.mp3",
    "mirco/audio/JINGYIWANZHUAN.mp3",
   // "mirco/audio/fireEffect.mp3",
};


using namespace CocosDenshion;
using namespace cocos2d;

static AudioManager * ms_pInstance = NULL;

AudioManager::AudioManager():
m_bAudioBackgroudEnabled(true),
m_bAudioEffectEnabled(true),
m_groundVolume(0.5)
{
    
}

AudioManager::~AudioManager()
{
    
}

AudioManager * AudioManager::instance()
{
    if (ms_pInstance == NULL) {
        ms_pInstance = new AudioManager();
        ms_pInstance->loadConfig();
    }
    
    return ms_pInstance;
}

void AudioManager::playEffect(AudioEffect effect)
{
    if (!m_bAudioEffectEnabled) {
        return;
    }
    
    const char * pszFile = getEffectFilePath(effect);
    SimpleAudioEngine::sharedEngine()->preloadEffect(pszFile);
    SimpleAudioEngine::sharedEngine()->playEffect(pszFile);
}

void AudioManager::playGroundMusic(AudioEffect effect){
    if (!m_bAudioBackgroudEnabled) {
        return ;
    }
    
    if (SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()) {
        SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    }
    
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(g_EffectTable[effect]);
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(g_EffectTable[effect], true);
}

void AudioManager::updateGroundMusicVolume(float v){
    m_groundVolume = v;
    
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(m_groundVolume);
   
}

float AudioManager::getGroundVolume(){
    return m_groundVolume;
}

void AudioManager::setGroundVolume(float pVol){
    
    m_groundVolume = pVol;
}

void AudioManager::loadConfig()
{
    m_groundVolume = CCUserDefault::sharedUserDefault()->getFloatForKey(KEY_GROUND_AUDIO_V,-1);
    
    if (m_groundVolume<=-1) {
        m_groundVolume = 0.5;
    }
    
    CCLog("AudioManager-loadconfig-m_groundVolume == %f",m_groundVolume);
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(m_groundVolume);
    
    //m_bAudioBackgroudEnabled = CCUserDefault::sharedUserDefault()->getBoolForKey(KEY_AUDIO_BACKGROUND_ENABLED, m_bAudioBackgroudEnabled);
    //  m_bAudioEffectEnabled = CCUserDefault::sharedUserDefault()->getBoolForKey(KEY_AUDIO_EFFECT_ENABLED, m_bAudioEffectEnabled);
}

void AudioManager::saveConfig()
{
    
      CCLog("AudioManager-saveconfig-m_groundVolume == %f",m_groundVolume);
    CCUserDefault::sharedUserDefault()->setFloatForKey(KEY_GROUND_AUDIO_V, m_groundVolume);
    
    CCUserDefault::sharedUserDefault()->flush();
    
   // CCUserDefault::sharedUserDefault()->setBoolForKey(KEY_AUDIO_BACKGROUND_ENABLED, m_bAudioBackgroudEnabled);
   // CCUserDefault::sharedUserDefault()->setBoolForKey(KEY_AUDIO_EFFECT_ENABLED, m_bAudioEffectEnabled);
}



///////////////no use////////////////

void AudioManager::stop()
{
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    
    SimpleAudioEngine::sharedEngine()->end();
}

void AudioManager::pause()
{
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    
    saveConfig();
}

void AudioManager::resume()
{
    if (m_bAudioBackgroudEnabled) {
          SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    }
  
    if (m_bAudioEffectEnabled) {
          SimpleAudioEngine::sharedEngine()->resumeAllEffects();
    }
}

void AudioManager::turningBackground()
{
    if (m_bAudioBackgroudEnabled) {
        SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    }else {
        playBackground();
    }
    
    m_bAudioBackgroudEnabled = !m_bAudioBackgroudEnabled;
    
    saveConfig();
}

void AudioManager::turningEffect()
{
    if (m_bAudioEffectEnabled) {
        SimpleAudioEngine::sharedEngine()->stopAllEffects();
    }
    
    m_bAudioEffectEnabled = !m_bAudioEffectEnabled;
    
    saveConfig();
}





const char * AudioManager::getEffectFilePath(AudioEffect effect) const
{
    const char * pszName = g_EffectTable[effect];
    char szBuf[32] = {0};
    
   // sprintf(szBuf, "%s/%s.%s", AUDIO_FOLDER, pszName, AUDIO_FORMAT);
 
    pszName = CCFileUtils::sharedFileUtils()->fullPathForFilename(pszName).c_str();
    
   // pszName = CCFileUtils::fullPathFromRelativePath(szBuf);
    return pszName;
}

void AudioManager::playBackground()
{
  
}