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
	void UpdateListener(D3DXVECTOR3 position, D3DXVECTOR3 orientation);//Updates the position of the listener
	void UpdateGhostMusic(D3DXVECTOR3 position, int track);//Used in classic mode
	void UpdateGhostMusic(D3DXVECTOR3 position[], int track[]);//Used in 1st person mode
	void ChangeGhostMusic(int tracknumber);//Used for changing the track that is played when ghosts are not eatable

	void PlaySound(int index,bool loop);
	void PlaySoundGhost(int index, int track);//0 for nomal and 1 for super candy
	void PlaySoundAtPos(int index, D3DXVECTOR3 position,bool loop);

private: //DS = DirectSound

	bool InitializeDS(HWND);
	void ShutdownDS();
	bool LoadFiles();

	bool PlayWaveFile(D3DXVECTOR3 position,IDirectSoundBuffer8* secondBuffer, IDirectSound3DBuffer8* second3DBuffer, bool loop);

	bool LoadWaveFile(char*, IDirectSoundBuffer8**,IDirectSound3DBuffer8**,int channels);
	void ShutdownWaveFile(IDirectSoundBuffer8**,IDirectSound3DBuffer8**);

private:
	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;//Mixes the sounds from the secondary buffers and plays them.
	
	IDirectSound3DListener8* m_listener; //Listener used by pacman to simulate 3D sounds

	//Arrays used for holding the audio buffers
	IDirectSoundBuffer8* m_secondaryBuffers[5];
	IDirectSound3DBuffer8* m_secondary3DBuffers[5];

	//Audio buffers used by the ghosts
	IDirectSoundBuffer8* m_secondaryBuffersGhostMusic[4][2];
	IDirectSound3DBuffer8* m_secondary3DBuffersGhostMusic[4][2];
	int m_currentTrack[4];
	bool m_singleGhostSource;
};

#endif
