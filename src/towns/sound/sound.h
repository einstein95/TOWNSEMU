/* LICENSE>>
Copyright 2020 Soji Yamakawa (CaptainYS, http://www.ysflight.com)

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

<< LICENSE */
#ifndef SOUND_IS_INCLUDED
#define SOUND_IS_INCLUDED
/* { */

#include <vector>
#include <string>

#include "device.h"
#include "townsdef.h"
#include "cpputil.h"
#include "ym2612.h"
#include "rf5c68.h"

class TownsSound : public Device
{
private:
	class FMTowns *townsPtr;
public:
	enum
	{
#if !defined(__linux__) && !defined(__linux)
		FM_PCM_MILLISEC_PER_WAVE=10, // Looks like time resolution of Wave Playback of Direct Sound is 10ms.
		BEEP_MILLISEC_PER_WAVE=10
#else
		FM_PCM_MILLISEC_PER_WAVE=40, // Maybe because I am developing on VirtualBox, I am getting outrageously slow latency of 80ms (40ms*2).
		BEEP_MILLISEC_PER_WAVE=40
#endif
	};

	virtual const char *DeviceName(void) const{return "SOUND";}

	class State
	{
	public:
		YM2612 ym2612;
		unsigned int muteFlag;
		unsigned int addrLatch[2];

		RF5C68 rf5c68;

		void PowerOn(void);
		void Reset(void);
		void ResetVariables(void);
	};

	State state;
	class Outside_World *outside_world=nullptr;

	bool recordFMandPCM=false;
	std::vector <unsigned char> FMPCMrecording;

	std::vector <unsigned char> nextFMPCMWave;

	inline bool IsFMPlaying(void) const
	{
		return 0!=state.ym2612.state.playingCh;
	}
	inline bool IsPCMPlaying(void) const
	{
		return true==state.rf5c68.IsPlaying();
	}

	TownsSound(class FMTowns *townsPtr);
	void SetOutsideWorld(class Outside_World *outside_world);
	void PCMStartPlay(unsigned char chStartPlay);
	void PCMStopPlay(unsigned char chStopPlay);

	virtual void PowerOn(void);
	virtual void Reset(void);

	virtual void IOWriteByte(unsigned int ioport,unsigned int data);

	virtual unsigned int IOReadByte(unsigned int ioport);

	/*! Called from FMTowns::RunFastDevicePolling.
	*/
	void SoundPolling(unsigned long long int townsTime);

	std::vector <std::string> GetStatusText(void) const;

	/*! Call this function periodically to continue sound playback.
	*/
	void ProcessSound(void);

	void StartRecording(void);
	void EndRecording(void);
	void SaveRecording(std::string fName) const;


	void SerializeYM2612(std::vector <unsigned char> &data) const;
	void DeserializeYM2612(const unsigned char *&data,unsigned int version);
	void SerializeRF5C68(std::vector <unsigned char> &data) const;
	void DeserializeRF5C68(const unsigned char *&data);

	virtual uint32_t SerializeVersion(void) const;
	virtual void SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const;
	virtual bool SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version);
};

/* } */
#endif
