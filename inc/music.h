/*
 * music.h
 * music playing API
 */

#pragma once

#include "speaker.h"
#include "delay.h"
#include <stdlib.h>
#include <math.h>

//default pitch
#define MUSIC_FREQ_A4 440
#define MUSIC_REF_OCTAVE 4
//duration of the notes
#define MUSIC_BPM 180
#define MUSIC_dQ 60000/MUSIC_BPM// 1/4
#define MUSIC_dH 2*MUSIC_dQ 	// 1/2
#define MUSIC_dE MUSIC_dQ/2 	// 1/8
#define MUSIC_dS MUSIC_dQ/4 	// 1/16
#define MSUIC_dW 4*MUSIC_dQ 	// 1

typedef enum{
	C = -9,Cs,D,Ds,E,F,Fs,G,Gs,A,As,B,Pause
}Note_t;

typedef struct{
	Note_t note : 8;
	unsigned char octave;
	unsigned short duration; //[ms]
}MusicNote_t;


//sample Song
const MusicNote_t ImperialMarch[];


//Music_Init: initialize necessary peripherals
void Music_Init();

//Music_Play: play song from array of musicnote_t
//parameter: playCount controls how many times will song repeat (0 = infinity)
void Music_Play(const MusicNote_t* song, unsigned playCount);
