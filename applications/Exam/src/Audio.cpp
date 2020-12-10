#include "Audio.h"
#include <iostream>

Audio::Audio() {}

int Audio::getHostDevice() {
	defaultDeviceString = alcGetString(/*device*/nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
	device = alcOpenDevice(defaultDeviceString);
	if (!device) {
		std::cerr << "Could not get a default OpenAL device" << "\n";
		return -1;
	}
	std::cout << "Selected OpenAL Device is --> " << alcGetString(device, ALC_DEVICE_SPECIFIER) << "\n";
}

int Audio::createContext() {
	context = alcCreateContext(device, nullptr); // Create audio context with the device.
	//OpenAL_ErrorCheck(context);

	if (!alcMakeContextCurrent(context)) { // Activate context such that we can manipulate the state.
		std::cerr << "failed to make the OpenAL context the current context" << std::endl;
		return -1;
	}
	//OpenAL_ErrorCheck("Make context current");
	/*std::cout << "OpenAL context has been created!" << "\n";*/
}

void Audio::createListener() {
	// Construction of a 3d listener.
	alec(alListener3f(AL_POSITION, 0.f, 0.f, 0.f));
	alec(alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f));
	ALfloat forwardAndUpVectors[] = {
		  1.f, 0.f, 0.f, // forward sound
		  0.f, 1.f, 0.f // upwards sound
	};
	alec(alListenerfv(AL_ORIENTATION, forwardAndUpVectors));
	/*std::cout << "OpenAL listener has been created!" << "\n";*/
}

int Audio::initializeStruct() {

	pSampleData = drwav_open_file_and_read_pcm_frames_s16("assets/sounds/meadowlark_daniel-simion.wav", &stereoData.channels, &stereoData.sampleRate, &stereoData.totalPCMFrameCount, nullptr);
	if (pSampleData == NULL) {
		std::cerr << "failed to load audio file" << std::endl;
		return -1;
	}
	if (stereoData.getTotalSamples() > drwav_uint64(std::numeric_limits<size_t>::max())) {
		std::cerr << "You have too much data in the file for a 64bit vector" << std::endl;
		return -1;
	}
	stereoData.pcmData.resize(size_t(stereoData.getTotalSamples()));
	std::memcpy(stereoData.pcmData.data(), pSampleData, stereoData.pcmData.size() * /*twobytes_in_s15*/2);
	drwav_free(pSampleData, nullptr);

	/*std::cout << "Struct initalized!" << "\n";*/

}

void Audio::generateStereoBuffer() {

	alec(alGenBuffers(1, &stereoSoundBuffer));
	alec(alBufferData(stereoSoundBuffer, stereoData.channels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
		stereoData.pcmData.data(),
		stereoData.pcmData.size() * 2 /*two bytes per sample*/,
		stereoData.sampleRate));

	// No reason to actually use any veloctiy or position. 
	// The sound file is stereo, this means that the sounds is coming from everywhere at the same 3D spacial location.
	alec(alGenSources(1, &stereoSource));

	alec(alSourcef(stereoSource, AL_PITCH, 1.f));
	alec(alSourcef(stereoSource, AL_GAIN, 1.f));
	alec(alSourcei(stereoSource, AL_LOOPING, AL_FALSE));
	alec(alSourcei(stereoSource, AL_BUFFER, stereoSoundBuffer));

	alec(alSourcePlay(stereoSource));
	alec(alGetSourcei(stereoSource, AL_SOURCE_STATE, &sourceState));
}

void Audio::initializeSourceState() {
	alec(alSourcePlay(stereoSource));
	alec(alGetSourcei(stereoSource, AL_SOURCE_STATE, &sourceState));
}

void Audio::runSound() {
	alec(alGetSourcei(stereoSource, AL_SOURCE_STATE, &sourceState));
}

void Audio::freeResources() {
	alec(alDeleteSources(1, &stereoSource));
	alec(alDeleteBuffers(1, &stereoSoundBuffer));
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

Audio::~Audio() { freeResources(); }


