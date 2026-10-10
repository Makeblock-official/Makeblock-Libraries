#include <MeMCore.h>
#include <MeAudioPlayer.h>

MeAudioPlayer AudioPlayer(4);

void setup() {
    //Give it a bit
    delay(100);

    // init AudioPlayer
    AudioPlayer.PlayerInit();
    delay(100);
    AudioPlayer.stopRecording();
    delay(100);
    AudioPlayer.stopMusic();
    delay(100);
    AudioPlayer.setMusicPlayMode(0);
    delay(100);
    // set volume to 80%
    AudioPlayer.setMusicVolume(80);
    delay(100);
    // play the first file on the micro-SD card
    AudioPlayer.playMusicFileIndex(1);
    delay(10000);
    
    AudioPlayer.stopMusic();
}

void loop() {
}
