#include "music.h"

const MusicNote_t ImperialMarch[] = {
		{A,3,MUSIC_dQ},{A,3,MUSIC_dQ},{A,3,MUSIC_dQ},{F,3,MUSIC_dE+MUSIC_dS},{C,4,MUSIC_dS},
		{A,3,MUSIC_dQ},{F,3,MUSIC_dS+MUSIC_dE},{C,4,MUSIC_dS},{A,3,MUSIC_dH},
		{E,4,MUSIC_dQ},{E,4,MUSIC_dQ},{E,4,MUSIC_dQ},{F,4,MUSIC_dE+MUSIC_dS},{C,4,MUSIC_dS},
		{Gs,3,MUSIC_dQ},{F,3,MUSIC_dE+MUSIC_dS},{C,4,MUSIC_dS},{A,3,MUSIC_dH},
		{A,4,MUSIC_dQ},{A,3,MUSIC_dE+MUSIC_dS},{A,3,MUSIC_dS},{A,4,MUSIC_dQ},{Gs,4,MUSIC_dE+MUSIC_dS},{G,4,MUSIC_dS},
		{Fs,4,MUSIC_dS},{E,4,MUSIC_dS},{F,4,MUSIC_dE},{Pause,0,MUSIC_dE},{As,3,MUSIC_dE},{Ds,4,MUSIC_dQ},{D,4,MUSIC_dE+MUSIC_dS},{Cs,4,MUSIC_dS},
		{C,4,MUSIC_dS},{B,3,MUSIC_dS},{C,4,MUSIC_dE},{E,4,MUSIC_dQ},{Pause,0,MUSIC_dE},{F,3,MUSIC_dE},{Gs,3,MUSIC_dQ},{F,4,MUSIC_dE+MUSIC_dS},{E,4,MUSIC_dQ},{A,3,MUSIC_dS},
		{C,4,MUSIC_dQ},{A,3,MUSIC_dE+MUSIC_dS},{C,4,MUSIC_dS},{E,4,MUSIC_dH},
		{A,4,MUSIC_dQ},{A,3,MUSIC_dE+MUSIC_dS},{A,3,MUSIC_dS},{A,4,MUSIC_dQ},{Gs,4,MUSIC_dE+MUSIC_dS},{G,4,MUSIC_dS},
		{Fs,4,MUSIC_dS},{E,4,MUSIC_dS},{F,4,MUSIC_dE},{Pause,0,MUSIC_dE},{As,3,MUSIC_dE},{Ds,4,MUSIC_dQ},{D,4,MUSIC_dE+MUSIC_dS},{Cs,4,MUSIC_dS},
		{C,4,MUSIC_dS},{B,3,MUSIC_dS},{C,4,MUSIC_dE},{Pause,0,MUSIC_dE},{F,3,MUSIC_dE},{Gs,3,MUSIC_dQ},{F,3,MUSIC_dE+MUSIC_dS},{C,4,MUSIC_dS},
		{A,3,MUSIC_dQ},{F,3,MUSIC_dE+MUSIC_dS},{C,4,MUSIC_dS},{A,3,MUSIC_dH},{Pause,0,MUSIC_dH+MUSIC_dH},
		{0,0,0}
};


void Music_Init()
{
	//just forward call
	Speaker_Init();
}

void Music_Play(const MusicNote_t* song, unsigned playCount)
{
	do
	{
		const MusicNote_t* curNote = song;
		//while not song end
		while(curNote->duration)
		{
			unsigned freq;
			//
			if (curNote->note != Pause){
				//calculate note frequency
				int octshift = curNote->octave - MUSIC_REF_OCTAVE;
				freq = 1 << abs(octshift);
				freq = (octshift >= 0) ? (freq*MUSIC_FREQ_A4) : (MUSIC_FREQ_A4/freq);
				freq *= pow(2, curNote->note/12.0);
				//play the note
				Speaker_Tone(freq);
				System_DelayMs(curNote->duration);
			}
			Speaker_Mute();
			//wait the same duration after done playing the tone
			System_DelayMs(curNote->duration);
			//next note
			curNote++;
		}
	}while(--playCount);
}
