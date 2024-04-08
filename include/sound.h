#ifndef SOUND_H
#define SOUND_H

#include "gba.h"

// Note structure definition
typedef struct
{
    int frequency; // Frequency of the note
    int duration;  // Duration of the note
} Note;

#define NOTE_A2 110
#define NOTE_B2 123

// Octave 3
#define NOTE_C3 131
#define NOTE_D3 147
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_G3 196
#define NOTE_A3 220
#define NOTE_B3 247

// Octave 4
#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494

// Octave 5
#define NOTE_C5 523
#define NOTE_D5 587
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_G5 784
#define NOTE_A5 880
#define NOTE_B5 988

// Octave 6
#define NOTE_C6 1047
#define NOTE_D6 1175
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_G6 1568
#define NOTE_A6 1760
#define NOTE_B6 1976

// Sound control bits

#define REG_SOUNDCNT_X *(volatile u16 *)0x04000084
#define REG_SOUNDCNT_L *(volatile u16 *)0x04000080
#define REG_SOUNDCNT_H *(volatile u16 *)0x04000082

#define REG_SOUND1CNT_L *(volatile u16 *)0x04000060
#define REG_SOUND1CNT_H *(volatile u16 *)0x04000062
#define REG_SOUND1CNT_X *(volatile u16 *)0x04000064

#define SOUND1_ENABLE (1 << 12)
#define DSOUND_A_RIGHT_CHANNEL (1 << 8)
#define DSOUND_A_LEFT_CHANNEL (1 << 9)
#define DSOUND_A_FIFO_RESET (1 << 11)

// Function prototypes
void play_note(Note note);
void play_theme(Note *theme);
void sound_init(void);
void play_sound(void);

#endif