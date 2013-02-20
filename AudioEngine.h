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

	bool Initialize(HWND);
	void InitListener();
	void Shutdown();
	void MuteSound();
	void UpdateListener(D3DXVECTOR3 position, D3DXVECTOR3 orientation);

	void PlaySound(std::string);
	void PlaySoundAtPos(std::string, D3DXVECTOR3 position);
	void PlaySoundAtPosP(std::string, D3DXVECTOR3* position);

private: //DS = DirectSound

	bool InitializeDS(HWND);
	void ShutdownDS();
	bool LoadFiles();



	bool PlayWaveFile2D(IDirectSoundBuffer8* secondBuffer);
	bool PlayWaveFile3D(D3DXVECTOR3 position,IDirectSoundBuffer8* secondBuffer, IDirectSound3DBuffer8* second3DBuffer);

	bool LoadWaveFile(char*, IDirectSoundBuffer8**,IDirectSound3DBuffer8**,int channels);
	void ShutdownWaveFile(IDirectSoundBuffer8**,IDirectSound3DBuffer8**);

private:
	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;//Mixes the sounds from the secondary buffers and plays them.
	
	IDirectSound3DListener8* m_listener; //Listener used by pacman to simulate 3D sounds
	IDirectSoundBuffer8* m_secondaryBufferSound;
	IDirectSoundBuffer8* m_secondaryBufferMusic;//Used for looping music
	IDirectSound3DBuffer8* m_secondary3DBufferSound;
	IDirectSound3DBuffer8* m_secondary3DBufferMusic;
	//Pacman
	IDirectSoundBuffer8* m_secondaryBufferPacmanSounds;
	//Consumables
	IDirectSoundBuffer8* m_secondaryBufferConsumableSounds;
	IDirectSoundBuffer8* m_secondaryBufferConsumableMusic;
	IDirectSound3DBuffer8* m_secondary3DBufferConsumableMusic;
	//Ghosts
	IDirectSoundBuffer8* m_secondaryBufferGhostMusic;
	IDirectSoundBuffer8* m_secondaryBufferGhostSounds;
	IDirectSound3DBuffer8* m_secondary3DBufferGhostMusic;
	IDirectSound3DBuffer8* m_secondary3DBufferGhostSounds;
};

#endif
