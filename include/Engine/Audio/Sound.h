#pragma once
#include <Engine\Audio\AudioEngine.h>
#include <Engine\Game_Objects\GameObject_3D.h>
#include <string>

using namespace std;

class AudioEngine;

class Sound
{
public:
	Sound() {};
	Sound(string strAudioName);	// load in via XML
	//Sound(string strAudioName, bool b3d, bool bloop, bool bstream, AudioEngine* engine_used);	// load in via game scene
	~Sound() {};
	void Load(bool b3d, bool bloop, bool bstream);
	void Play();	// XML
	//void Play(glm::vec3 position_In, float volume_In);	// game scene
	void Stop(int channelID);

	void SetPosition(glm::vec3 position_In);
	void SetVolume(float volume_In);
	void setParent(GameObject_3D* new_parent);

	int getChannelID(string strAudioName);
private:
	AudioEngine *Audio;
	string AudioName;
	int channelID;
	GameObject_3D* parent;
};