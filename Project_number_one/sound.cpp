#include "Sound.h"
#include <thread>
#include <chrono>

void SoundManager::playNow(const std::string& path) {
    PlaySoundA(path.c_str(), NULL, SND_FILENAME | SND_ASYNC);
}

void SoundManager::playWithDelay(const std::string& path, int delayMs) {
    std::thread([path, delayMs]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        PlaySoundA(path.c_str(), NULL, SND_FILENAME | SND_ASYNC);
        }).detach();
}

void SoundManager::playLoop(const std::string& path) {
    PlaySoundA(path.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void SoundManager::playLoopWithDelay(const std::string& path, int delayMs) {
    std::thread([path, delayMs]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        PlaySoundA(path.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        }).detach();
}

void SoundManager::stopAll() {
    PlaySoundA(NULL, 0, 0);
}