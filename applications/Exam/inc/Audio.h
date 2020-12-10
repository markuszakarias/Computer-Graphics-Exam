#pragma once

#include <dr_wav.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <vector>




//OpenAL error checking
#define OpenAL_ErrorCheck(message)\
{\
	ALenum error = alGetError();\
	if( error != AL_NO_ERROR)\
	{\
		std::cerr << "OpenAL Error: " << error << " with call for " << #message << std::endl;\
	}\
}

#define alec(FUNCTION_CALL)\
FUNCTION_CALL;\
OpenAL_ErrorCheck(FUNCTION_CALL)

struct audioEngineStruct {
	unsigned int channels = 0;
	unsigned int sampleRate = 0;
	drwav_uint64 totalPCMFrameCount = 0;
	std::vector<uint16_t> pcmData;
	drwav_uint64 getTotalSamples() { return totalPCMFrameCount * channels; }
};

class Audio {
private:
	const ALCchar* defaultDeviceString;
	ALCdevice* device;
	ALCcontext* context;
	ALuint stereoSoundBuffer;
	ALuint stereoSource;
	ALint sourceState;
	drwav_int16* pSampleData;

public:

	audioEngineStruct stereoData;

	Audio();
	int getHostDevice();
	int createContext();
	void createListener();
	int initializeStruct();
	void generateStereoBuffer();
	void initializeSourceState();
	void runSound();
	void freeResources();
	~Audio();

	ALint getSourceState() { return sourceState; }
	ALuint getStereoSource() { return stereoSource; }

};

