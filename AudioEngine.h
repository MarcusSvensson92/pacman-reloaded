#ifndef _AUDIOENGINE_H_
#define _AUDIOENGINE_H_

//Linking
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
//Include
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

class AudioEngine
{
private://audio file header types
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

public:
	AudioEngine();
	AudioEngine(const AudioEngine&);
	~AudioEngine();
private: //DS = DirectSound
	bool Initialize(HWND);
	void Shutdown();

	bool InitializeDS(HWND);
	void ShutdownDS();

	bool LoadWaveFile(char*, IDirectSoundBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**);

	bool PlayWaveFile();

	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;//Mixes the sounds from the secondary buffers and plays them.
	IDirectSoundBuffer8* m_secondaryBufferMain;//Used for looping the main music
	//Add more secondary buffers in order to play more files
};

#endif
