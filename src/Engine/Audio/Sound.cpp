#include <Engine\Audio\Sound.h>

Sound::Sound(string strAudioName) {
	Audio = new AudioEngine();
	Audio->Init();
	AudioName = "assets/audio/" + strAudioName;
}

//Sound::Sound(string strAudioName, bool b3d, bool bloop, bool bstream, AudioEngine* engine_used)
//{
//	engine_used = new AudioEngine();
//	Audio->Init();
//	AudioName = strAudioName;
//	Audio->LoadSound(AudioName, b3d, bloop, bstream);
//}

void Sound::Load(bool b3d, bool bloop, bool bstream) {
	Audio->LoadSound(AudioName, b3d, bloop, bstream);
}

void Sound::Play() {
	channelID = Audio->PlaySound(AudioName);
}

//void Sound::Play(glm::vec3 position, float volume_In) {
//	channelID = Audio->PlaySound(AudioName, position, volume_In);
//}

void Sound::Stop(int channelID) {

	Audio->StopChannel(channelID);
}

void Sound::setParent(GameObject_3D* new_parent) {
	parent = new_parent;
}

void Sound::SetPosition(glm::vec3 position_In) {
	Audio->SetChannel3dPosition(getChannelID(AudioName), position_In);
}

void Sound::SetVolume(float volume_In) {
	Audio->setChannelVolume(getChannelID(AudioName), volume_In);
}

int Sound::getChannelID(string strAudioName) {
	return channelID;
}