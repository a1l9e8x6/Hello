//
//  AudioManager.h
// 
//
//  Created by taoming li
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef DragonGeneral_AudioManager_h
#define DragonGeneral_AudioManager_h

typedef enum{
    
    kAudio_city01 = 0,
    kAudio_city02,
    kAudio_blockPoint,
    
    kAudio_copy01,
    kAudio_copy02,
    
    kAudio_battle,
    
    kAudio_monster,
    
    //effect
    kAudio_effect_attack,
    
    kAudio_jingyiwanzhuan,
    //kAudio_effect_hitban,

    
}AudioEffect;


class AudioManager 
{
public:
    ~AudioManager();
    
    void start();
    void pause();
    void resume();
    void stop();
    
    void turningBackground();
    void turningEffect();
    
    void playEffect(AudioEffect effect);
    void playRandomEffect();
    
    void playGroundMusic(AudioEffect effect);
    void updateGroundMusicVolume(float v);
    
    bool isAudioBackgroundEnabled() const { return m_bAudioBackgroudEnabled; }
    bool isAudioEffectEnabled() const { return m_bAudioEffectEnabled; }
    
    static AudioManager * instance();
    

    float getGroundVolume();
    void setGroundVolume(float pVol);
        
 
    void loadConfig();
    void saveConfig();
protected:
    void playBackground();
    
    const char * getEffectFilePath(AudioEffect effect) const;

private:
    AudioManager();
    
    float m_groundVolume;

    
    bool m_bAudioBackgroudEnabled;
    bool m_bAudioEffectEnabled;
};


#endif
