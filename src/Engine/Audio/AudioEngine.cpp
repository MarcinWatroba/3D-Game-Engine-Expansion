#include <Engine\Audio\AudioEngine.h>

/* -----Implementation Struct----- */
// constructor
Implementation::Implementation() {
	mpStudioSystem = NULL;
	AudioEngine::ErrorCheck(FMOD::Studio::System::create(&mpStudioSystem));
	AudioEngine::ErrorCheck(mpStudioSystem->initialize(maxChannels, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));

	mpSystem = NULL;
	AudioEngine::ErrorCheck(mpStudioSystem->getLowLevelSystem(&mpSystem));
}
// destructor
Implementation::~Implementation() {
	AudioEngine::ErrorCheck(mpStudioSystem->unloadAll());
	AudioEngine::ErrorCheck(mpStudioSystem->release());
}
void Implementation::Update() {
	vector<ChannelMap::iterator> pStoppedChannels;
	for (auto it = mChannels.begin(), itEnd = mChannels.end(); it != itEnd; ++it) {
		bool bIsPlaying = false;
		it->second->isPlaying(&bIsPlaying);
		if (!bIsPlaying) {
			pStoppedChannels.push_back(it);
		}
	}
	for (auto& it : pStoppedChannels) {
		mChannels.erase(it);
	}
	AudioEngine::ErrorCheck(mpStudioSystem->update());
}

Implementation* sgpImplementation = nullptr;

/* -----Main Class----- */ 
void AudioEngine::Init() {
	sgpImplementation = new Implementation;
}
void AudioEngine::Update() {
	sgpImplementation->Update();
}
void AudioEngine::Shutdown() {
	delete sgpImplementation;
}
int AudioEngine::ErrorCheck(FMOD_RESULT result) {
	if (result != FMOD_OK) {
		cout << "FMOD Error: " << result << endl;
		return 1;
	}
	// cout << "FMOD is ok~" << endl;
	return 0;
}

/* ---Load Functions--- */
void AudioEngine::LoadSound(const string& strSoundName, bool b3d, bool bLooping, bool bStream) {
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt != sgpImplementation->mSounds.end()) {
		return;
	}

	FMOD_MODE eMode = FMOD_DEFAULT;
	eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	eMode |= b3d ? FMOD_3D : FMOD_2D;
	eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

	FMOD::Sound* pSound = nullptr;
	AudioEngine::ErrorCheck(sgpImplementation->mpSystem->createSound(strSoundName.c_str(), eMode, nullptr, &pSound));
	if (pSound) {
		sgpImplementation->mSounds[strSoundName] = pSound;
		cout << "Sound Loaded" << endl;
	}
}
void AudioEngine::UnloadSound(const string& strSoundName) {
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt != sgpImplementation->mSounds.end()) {
		return;
	}

	AudioEngine::ErrorCheck(tFoundIt->second->release());
	sgpImplementation->mSounds.erase(tFoundIt);
	cout << "Sound Unloaded" << endl;
}
void AudioEngine::LoadBank(const string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags) {
	auto tFoundIt = sgpImplementation->mBanks.find(strBankName);
	if (tFoundIt == sgpImplementation->mBanks.end()) {
		return;
	}

	FMOD::Studio::Bank* pBank;
	AudioEngine::ErrorCheck(sgpImplementation->mpStudioSystem->loadBankFile(strBankName.c_str(), flags, &pBank));
	if (pBank) {
		sgpImplementation->mBanks[strBankName] = pBank;
		cout << "Bank Loaded" << endl;
	}
}
void AudioEngine::LoadEvent(const string& strEventName) {
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end()) {
		return;
	}

	FMOD::Studio::EventDescription* pEventDescription = NULL;
	AudioEngine::ErrorCheck(sgpImplementation->mpStudioSystem->getEvent(strEventName.c_str(), &pEventDescription));
	if (pEventDescription) {
		FMOD::Studio::EventInstance* pEventInstance = NULL;
		AudioEngine::ErrorCheck(pEventDescription->createInstance(&pEventInstance));
		if (pEventInstance) {
			sgpImplementation->mEvents[strEventName] = pEventInstance;
			cout << "Event Loaded" << endl;
		}
	}
}

/* ---Play Functions--- */
int AudioEngine::PlaySound(const string& strSoundName, const glm::vec3& vPos, float fVolumeB) {
	int nChannelId = sgpImplementation->mnNextChannelId++;
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt == sgpImplementation->mSounds.end()) {
		LoadSound(strSoundName);
		tFoundIt = sgpImplementation->mSounds.find(strSoundName);
		if (tFoundIt == sgpImplementation->mSounds.end()) {
			return nChannelId;
		}
	}
	FMOD::Channel* pChannel = nullptr;
	AudioEngine::ErrorCheck(sgpImplementation->mpSystem->playSound(tFoundIt->second, nullptr, true, &pChannel));
	if (pChannel) {
		FMOD_MODE currentMode;
		tFoundIt->second->getMode(&currentMode);
		if (currentMode & FMOD_3D) {
			FMOD_VECTOR position = VectorToFMOD(vPos);
			AudioEngine::ErrorCheck(pChannel->set3DAttributes(&position, nullptr));
		}
		AudioEngine::ErrorCheck(pChannel->setVolume(dbToVolume(fVolumeB)));
		AudioEngine::ErrorCheck(pChannel->setPaused(false));
		sgpImplementation->mChannels[nChannelId] = pChannel;
	}
	//cout << "Currently Playing: " << strSoundName << endl;
	return nChannelId;
}
void AudioEngine::PlayEvent(const string& strEventName) {
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end()) {
		LoadEvent(strEventName);
		tFoundIt = sgpImplementation->mEvents.find(strEventName);
		if (tFoundIt == sgpImplementation->mEvents.end()){
			return;
		}
	}
	tFoundIt->second->start();
}

/* ---Stop Functions--- */
void AudioEngine::StopChannel(int nChannelId) {
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end()){
		return;
	}

	AudioEngine::ErrorCheck(tFoundIt->second->stop());
}
void AudioEngine::StopEvent(const string& strEventName, bool bImmediate) {
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end()) {
		return;
	}

	FMOD_STUDIO_STOP_MODE eMode;
	eMode = bImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
	AudioEngine::ErrorCheck(tFoundIt->second->stop(eMode));
}
void AudioEngine::StopAllChannels() {
	for (size_t i = 0; i < maxChannels; i++) {
		FMOD::Channel* pChannel = nullptr;
		FMOD_RESULT res = sgpImplementation->mpSystem->getChannel(i, &pChannel);

		if (res == FMOD_OK && pChannel) {
			AudioEngine::ErrorCheck(pChannel->stop());
		}
	}
}

/* ---Check Functions--- */
bool AudioEngine::isEventPlaying(const string& strEventName) const {
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end()) {
		return false;
	}

	FMOD_STUDIO_PLAYBACK_STATE* state = NULL;
	if (tFoundIt->second->getPlaybackState(state) == FMOD_STUDIO_PLAYBACK_PLAYING) {
		return true;
	}
	return false;
}
bool AudioEngine::IsPlaying(int nChannelID) const {
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelID);
	if (tFoundIt == sgpImplementation->mChannels.end()) {
		return false;
	}

	bool* audioPlaying = false;
	if (tFoundIt->second->isPlaying(audioPlaying)) {
		return true;
	}
	return false;
}

/* ---Other Functions--- */
void AudioEngine::Set3dListenerAndOrientation(const glm::vec3& vPos, const glm::vec3& vOri, float fVolumeB) {
	// Position and Orientation
	auto tFoundPos = sgpImplementation->mpSystem->set3DListenerAttributes(0, &VectorToFMOD(vPos), NULL, &VectorToFMOD(vOri), &VectorToFMOD(upVec));
	AudioEngine::ErrorCheck(tFoundPos);
	// Volume
	auto tFoundVol = sgpImplementation->mpSystem;
	//AudioEngine::ErrorCheck(tFoundVol);
}
void AudioEngine::GetEventParameter(const string& strEventName, const string& strParameterName, float* parameter) {
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end()) {
		return;
	}

	FMOD::Studio::ParameterInstance* pParameter = NULL;
	AudioEngine::ErrorCheck(tFoundIt->second->getParameter(strParameterName.c_str(), &pParameter));
	AudioEngine::ErrorCheck(pParameter->getValue(parameter));
}
void AudioEngine::setEventParameter(const string& strEventName, const string& strParameterName, float fValue) {
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end()) {
		return;
	}

	FMOD::Studio::ParameterInstance* pParameter = NULL;
	AudioEngine::ErrorCheck(tFoundIt->second->getParameter(strParameterName.c_str(), &pParameter));
	AudioEngine::ErrorCheck(pParameter->setValue(fValue));
}
void AudioEngine::SetChannel3dPosition(int channelId, const glm::vec3 vPos) {
	auto tFoundIt = sgpImplementation->mChannels.find(channelId);
	if(tFoundIt == sgpImplementation->mChannels.end()) {
		return;
	}

	FMOD_VECTOR position = VectorToFMOD(vPos);
	AudioEngine::ErrorCheck(tFoundIt->second->set3DAttributes(&position, NULL));
}
void AudioEngine::setChannelVolume(int channelId, float fVolumeB) {
	auto tFoundIt = sgpImplementation->mChannels.find(channelId);
	if (tFoundIt == sgpImplementation->mChannels.end()) {
		return;
	}

	AudioEngine::ErrorCheck(tFoundIt->second->setVolume(dbToVolume(fVolumeB)));
}
float AudioEngine::dbToVolume(float db) {
	return powf(10.0f, 0.05f * db);
}
float AudioEngine::VolumeTodb(float volume) {
	return 20.0f * log10f(volume);
}
FMOD_VECTOR AudioEngine::VectorToFMOD(const glm::vec3& vPos) {
	FMOD_VECTOR fVec;
	fVec.x = vPos.x;
	fVec.y = vPos.y;
	fVec.z = vPos.z;
	return fVec;
}