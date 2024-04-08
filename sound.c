#include "sound.h"

/**
 * The function `play_note` plays a note with a specific frequency on a sound channel of a device.
 *
 * @param note The `play_note` function takes a parameter `note` of type `Note`. The `Note` struct
 * likely contains information about a musical note, such as its frequency. The function checks if the
 * frequency of the note is greater than 0, calculates a frequency value based on the note's frequency
 */
void play_note(Note note)
{
    if (note.frequency > 0)
    {
        int freq = 2048 - (131072 / note.frequency);
        REG_SOUND1CNT_L = 0x0070;
        REG_SOUND1CNT_H = 0xF780;
        REG_SOUND1CNT_X = freq | 0x8000;
    }
    else
    {
        REG_SOUND1CNT_X = 0;
    }
}

/**
 * The function `play_theme` plays a sequence of notes stored in an array with specified durations.
 *
 * @param theme The `theme` parameter is a pointer to an array of `Note` structures. Each `Note`
 * structure likely contains information about a musical note, such as its frequency and duration.
 */
void play_theme(Note *theme)
{
    static int currentNote = 0;
    static int noteTimer = 0;

    if (noteTimer <= 0)
    {
        play_note(theme[currentNote]);

        noteTimer = theme[currentNote].duration;

        currentNote++;

        if (theme[currentNote].frequency == 0)
        {
            currentNote = 0;
        }
    }
    else
    {
        noteTimer--;
    }
}

/**
 * The function `sound_init` initializes the sound registers for a C program.
 */
void sound_init(void)
{
    REG_SOUNDCNT_X = 0x80;
    REG_SOUNDCNT_L = 0x1177;
    REG_SOUNDCNT_H = 0x0;
}

/**
 * The function `play_sound` sets the frequency, duty cycle, volume, and other parameters to play a
 * sound on a specific channel.
 */
void play_sound(void)
{
    // Set the frequency (2048 - (131072 / frequency))
    // For Middle C (C4), the frequency is about 261.63 Hz
    int freq = 2048 - (131072 / 262);

    // Set the duty cycle (50%), volume (max), and attack/decay/sustain
    REG_SOUND1CNT_L = 0x0070;        // Duty cycle = 50%
    REG_SOUND1CNT_H = 0xF780;        // Volume = max, no envelope
    REG_SOUND1CNT_X = freq | 0x8000; // Set frequency and enable the channel
}



/* The `Note theme[]` array is storing a sequence of musical notes along with their durations. Each
element in the array represents a musical note and its duration. For example, `{NOTE_A2, 20}`
indicates that the note A2 should be played for a duration of 20 units. */
Note theme[] = {{NOTE_A2, 20}, {NOTE_B2, 20}, {NOTE_C3, 20}, {NOTE_D3, 20}, {NOTE_E3, 20}, {NOTE_F3, 20}, {NOTE_G3, 20}, {NOTE_A3, 20}, {NOTE_B3, 20}, {NOTE_C4, 20}, {NOTE_D4, 20}, {NOTE_E4, 20}, {NOTE_F4, 20}, {NOTE_G4, 20}, {NOTE_A4, 20}, {NOTE_B4, 20}, {NOTE_C5, 20}, {NOTE_D5, 20}, {NOTE_E5, 20}, {NOTE_F5, 20}, {NOTE_G5, 20}, {NOTE_A5, 20}, {NOTE_B5, 20}, {NOTE_C6, 20}, {NOTE_D6, 20}, {NOTE_E6, 20}, {NOTE_F6, 20}, {NOTE_G6, 20}, {NOTE_A6, 20}, {NOTE_B6, 20}};
