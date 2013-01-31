#include "stdafx.h"
#include "AudioEngine.h"



AudioEngine::AudioEngine()
{
	m_DirectSound = 0;
	m_primaryBuffer = 0;
	m_secondaryBufferMain = 0;
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
	
	//Load the main music into the secondaryBufferMain
	result = LoadWaveFile("Audio/Music/MainMusic.wav", &m_secondaryBufferMain);
	if(!result)
		return false;

	//Play the main music file
	result = PlayWaveFile();
	if(!result)
		return false;

	//If everything was succesfull
	return true;
}

void AudioEngine::Shutdown()
{
	//Release secondary buffers
	ShutdownWaveFile(&m_secondaryBufferMain);

	//Shutdown the DirectSound AIP
	ShutdownDS();

	return;
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
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
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

	//If successfull
	return true;
}

void AudioEngine::ShutdownDS()
{
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

bool AudioEngine::LoadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer)
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
	unsigned char *bufferPtr;
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

	//Check that the file is in stereo format
	if(waveFileHeader.numChannels != 2)
	{
		return false;
	}

	//Check that sample rate is 44.1 KHz
	if(waveFileHeader.sampleRate != 44100)
	{
		return false;
	}

	//Check that the file is in 16bit format
	if(waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}

	//Check for the data chunk header
	if((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
	    (waveFileHeader.dataChunkId[0] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		return false;
	}
 
	//Set wave format for secondary buffers that this file will be loaded onto
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	//Set buffer description for secondary buffer
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
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
	count = fread(waveData,1,waveFileHeader.dataSize,filePtr);
	if(count != waveFileHeader.dataSize)
		return false;

	//Close the file
	error = fclose(filePtr);
	if(error != 0)
		return false;

	//Lock the secound buffer to enable data to be written to it
	result = (*secondaryBuffer)->Lock(0,waveFileHeader.dataSize,(void**)&bufferPtr,(DWORD*)&bufferSize,NULL,0,0);
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

	//If everything was successfull
	return true;
}

void AudioEngine::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer)
{
//Release secondary buffer
	if(*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}
	return;
}

bool AudioEngine::PlayWaveFile()
{
	HRESULT result;

	//Set pos at the beginning of the sound buffer
	result = m_secondaryBufferMain->SetCurrentPosition(0);
	if(FAILED(result))
		return false;

	//Set volum to 100%
	result = m_secondaryBufferMain->SetVolume(DSBVOLUME_MAX);
	if(FAILED(result))
		return false;

	//Everything was successfull
	return true;
}