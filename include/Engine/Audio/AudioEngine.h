#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

// fmod related 
#include "FMOD\lowlevel\fmod.hpp"
#include "FMOD\Studio\fmod_studio.hpp"

// general includes
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>

#include<glm\glm.hpp>

using namespace std;

const glm::vec3 upVec = {0, 1, 0};

const int maxChannels = 32;

struct Implementation {
	Implementation();
	~Implementation();

	void Update();

	FMOD::Studio::System* mpStudioSystem;
	FMOD::System* mpSystem;

	int mnNextChannelId;

	typedef map<string, FMOD::Sound*> SoundMap;
	typedef map<int, FMOD::Channel*> ChannelMap;
	typedef map<string, FMOD::Studio::EventInstance*> EventMap;
	typedef map<string, FMOD::Studio::Bank*> BankMap;

	BankMap mBanks;	// contains data for each Event, sounds and information
	EventMap mEvents;	// instance that triggers the sound to be played
	ChannelMap mChannels;	//	handles all interaction for the sound after loading, such as volume, loop
	SoundMap mSounds;	// audio to be played
};

class AudioEngine {
public:
	AudioEngine() {};
	static void Init();
	static void Update();
	static void Shutdown();
	static int ErrorCheck(FMOD_RESULT result);

	// load functions
	void LoadBank(const string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
	void LoadEvent(const string& strEventName);
	void LoadSound(const string& strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false);
	void UnloadSound(const string& strSoundName);
	// play functions
	int PlaySound(const string& strSoundName, const glm::vec3& vPos = glm::vec3{0,0,0}, float fVolumeB = 0.0f);	// return a channel ID
	void PlayEvent(const string& strEventName);
	// stop functions
	void StopChannel(int nChannelID);
	void StopEvent(const string& strEventName, bool bImmediate = false);
	void StopAllChannels();
	// check functions
	bool IsPlaying(int nChannelID) const;
	bool isEventPlaying(const string& strEventName) const;
	// other functions
	void Set3dListenerAndOrientation(const glm::vec3& vPos = glm::vec3{ 0,0,0 }, const glm::vec3& vOri = glm::vec3{0, 0, 0}, float fVolumeB = 0.0f);
	void GetEventParameter(const string& strEventName, const string& strEventParameter, float* parameter);
	void setEventParameter(const string& strEventName, const string& strParameterName, float fValue);
	void SetChannel3dPosition(int nChannelID, const glm::vec3 vPos);
	void setChannelVolume(int nChannelId, float fVolumeB);
	float dbToVolume(float db);
	float VolumeTodb(float volume);
	FMOD_VECTOR VectorToFMOD(const glm::vec3& vPos);
};

#endif // !AUDIOENGINE_H
