#include "stdafx.h"
#include "AudioEngine.h"



AudioEngine::AudioEngine()
{
	m_DirectSound = 0;
	m_primaryBuffer = 0;
	m_listener = 0;
	m_secondaryBufferMain = 0;
	m_secondary3DBufferMain = 0;
}
AudioEngine::AudioEngine(const AudioEngine& other)
{
}

AudioEngine::~AudioEngine()
{

}

bool AudioEngine::Initialize(HWND hwnd)
{
	bool result;

	//Init DirectSound and the primary buffer
	result = InitializeDS(hwnd);
	if(!result)
		return false;
	
	result = LoadFiles();
	if(!result)
		return false;

	//If everything was succesfull
	return true;
}

bool AudioEngine::LoadFiles()
{
	bool result;

	//result = LoadWaveFile("Content/Audio/Music/MainMusicStereo.wav", &m_secondaryBufferMusic, NULL, 2);
	//if(!result)
	//	return false;

	////Play the main music file
	//result = PlayWaveFile2D(m_secondaryBufferMusic);
	//if(!result)
	//	return false;
	//Load the main music into the secondaryBufferMusic
	result = LoadWaveFile("Content/Audio/Music/MainMusicMono.wav", &m_secondaryBufferMusic, &m_secondary3DBufferMusic, 1);
	if(!result)
		return false;

	//Play the main music file
	result = PlayWaveFile3D(D3DXVECTOR3(230, 0, 140), m_secondaryBufferMusic, m_secondary3DBufferMusic);
	if(!result)
		return false;
}

void AudioEngine::Shutdown()
{
	//Release secondary buffers
	ShutdownWaveFile(&m_secondaryBufferMain,&m_secondary3DBufferMain);

	//Shutdown the DirectSound AIP
	ShutdownDS();

	return;
}

void AudioEngine::UpdateListener(D3DXVECTOR3 position,D3DXVECTOR3 orientation)
{
	m_listener->SetPosition(position.x,position.y,position.z,DS3D_IMMEDIATE);
	//m_listener->SetOrientation(orientation.x,orientation.y,orientation.z,0,0,0,DS3D_IMMEDIATE);
}

bool AudioEngine::InitializeDS(HWND hwnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	//Init the dsound pointer for the default sound device
	result = DirectSoundCreate8(NULL,&m_DirectSound,NULL);
	if(FAILED(result))
	{
	return false;
	}

	//Set cooperative level to priority so the formet of the primary buffer can be modified
	result = m_DirectSound->SetCooperativeLevel(hwnd,DSSCL_PRIORITY);
	if(FAILED(result))
	{
	return false;
	}

	//Setup primary buffer description
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	//Get control of the pribuffer on the default sound device
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc,&m_primaryBuffer,NULL);
	if(FAILED(result))
	{
	return false;
	}

	//Setup for the format of the primary buffer
	//Wav file
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	//Set pribnuffer to the specified format
	result = m_primaryBuffer->SetFormat(&waveFormat);
	if(FAILED(result))
	{
	return false;
	}

	// Obtain listener interface
	result = m_primaryBuffer->QueryInterface(IID_IDirectSound3DListener8, (LPVOID*)&m_listener);
	if(FAILED(result))
		return false;

	//Init Listener
	InitListener();

	//If successfull
	return true;
}

void AudioEngine::InitListener()
{
	//Set the listeners initial location to the middle of the screen
	m_listener->SetPosition(0.0f,0.0f,0.0f,DS3D_IMMEDIATE);
	//Set values for controlling audio distance and roll off
	m_listener->SetDistanceFactor(10.0f,DS3D_IMMEDIATE);
	m_listener->SetRolloffFactor(0.06f,DS3D_IMMEDIATE);
}

void AudioEngine::ShutdownDS()
{
	//Release listener
	if(m_listener)
	{
		m_listener->Release();
		m_listener = 0;
	}
	
	//Release primary sound buffer
	if(m_primaryBuffer)
	{
	m_primaryBuffer->Release();
	m_primaryBuffer = 0;
	}
	//Release dsound interface pointer
	if(m_DirectSound)
	{
		m_DirectSound->Release();
		m_DirectSound = 0;
	}
	return;
}

void AudioEngine::MuteSound()
{
	m_primaryBuffer->SetVolume(0);
	m_secondaryBufferMusic->SetVolume(0);
}

bool AudioEngine::LoadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer, IDirectSound3DBuffer8** secondary3DBuffer, int channels)
{
	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	//Open wave file in binary and ensure that it is not corrupt
	error = fopen_s(&filePtr,filename,"rb");
	if(error != 0)
		return false;
	
	//Read wave file header
	count = fread(&waveFileHeader, sizeof(waveFileHeader),1,filePtr);
	if(count!=1)
		return false;

	//Check chunk id in RIFF format
	if((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		return false;
	}

	//Make sure that the file is in WAVE format
	if((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		return false;
	}

	//Check if sub chunk id is in fmt format
	if((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		return false;
	}

	//Check that the audio format is WAVE_FORMAT_PCM
	if(waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}
	//Check that the file is in mono format
	if(waveFileHeader.numChannels != channels) //1=mono;2=stereo (mono for 3D sounds)
	{
		return false;
	}

	//Check that sample rate is 44.1 KHz
	if(waveFileHeader.sampleRate != 44100)
	{
		return false;
	}

	//Check that the file is in 16bit format
	if(waveFileHeader.bitsPerSample != 16)
	{
		return false;
	}

	//Check for the data chunk header
	if((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
	    (waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		return false;
	}
 
	//Set wave format for secondary buffers that this file will be loaded onto
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = channels;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	//Set buffer description for secondary buffer
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	//Check if the file is got 1 or 2 channels to determin if it is in 3D
	if(channels == 1)
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	else if(channels == 2)
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	//
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	//Create temporary sound buffer using the specified buffer settings
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer,NULL);
	if(FAILED(result))
		return false;

	//Test the buffer format against the DS8 interface and crate secondary buffers
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8,(void**)&*secondaryBuffer);
	if(FAILED(result))
		return false;

	//Relase the temp buffer
	tempBuffer->Release();
	tempBuffer = 0;

	//Move to the beginning of the wave data
	fseek(filePtr, sizeof(WaveHeaderType),SEEK_SET);
	
	//Create a temp buffer for the wave data
	waveData = new unsigned char[waveFileHeader.dataSize];
	if(!waveData)
		return false;

	//Read from wave file into the temp buffer
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if(count != waveFileHeader.dataSize)
		return false;

	//Close the file
	error = fclose(filePtr);
	if(error != 0)
		return false;

	//Lock the secound buffer to enable data to be written to it
	result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if(FAILED(result))
		return false;

	//Copy the wave data into the buffer
	memcpy(bufferPtr,waveData,waveFileHeader.dataSize);

	//Unlock the secondary buffer
	result = (*secondaryBuffer)->Unlock((void*)bufferPtr,bufferSize,NULL,0);
	if(FAILED(result))
		return false;

	//Release the wave data since is is copied to the buffer
	delete [] waveData;
	waveData = 0;

	//Get the 3d interface to the 2ndry buffer
	if(secondary3DBuffer != NULL)
	{
	result = (*secondaryBuffer)->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&*secondary3DBuffer);
	if(FAILED(result))
		return false;
	}

	//If everything was successfull
	return true;
}

void AudioEngine::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer, IDirectSound3DBuffer8** secondary3DBuffer)
{
	//Release 3D buffer
	if(*secondary3DBuffer)
	{
		(*secondary3DBuffer)->Release();
		*secondary3DBuffer = 0;
	}
	//Release secondary buffer
	if(*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}
	return;
}

void AudioEngine::PlaySound(char file[])
{

}

bool AudioEngine::PlayWaveFile2D(IDirectSoundBuffer8* secondBuffer)
{
	HRESULT result;
 
 
	// Set position at the beginning of the sound buffer.
	result = secondBuffer->SetCurrentPosition(0);
	if(FAILED(result))
	{
		return false;
	}
 
	// Set volume of the buffer to 100%.
	result = secondBuffer->SetVolume(DSBVOLUME_MAX);
	if(FAILED(result))
	{
		return false;
	}
 
	// Play the contents of the secondary sound buffer.
	result = secondBuffer->Play(0, 0, 0);
	if(FAILED(result))
	{
		return false;
	}
 
	return true;
}

bool AudioEngine::PlayWaveFile3D(D3DXVECTOR3 position,IDirectSoundBuffer8* secondBuffer, IDirectSound3DBuffer8* second3DBuffer)
{
	HRESULT result;

	float posx,posy,posz;
	//Set the location of where the music shall appear
	posx = position.x;
	posy = position.y;
	posz = position.z;

	//Set pos at the beginning of the sound buffer
	result = secondBuffer->SetCurrentPosition(0);
	if(FAILED(result))
		return false;

	//Set volum to 100%
	result = secondBuffer->SetVolume(DSBVOLUME_MAX);
	if(FAILED(result))
		return false;

	//Set location of the sound
	second3DBuffer->SetPosition(posx,posy,posz, DS3D_IMMEDIATE);

	//Play the file
	result = secondBuffer->Play(0, 0, 0);
	if(FAILED(result))
	{
		return false;
	}

	//Everything was successfull
	return true;
}
