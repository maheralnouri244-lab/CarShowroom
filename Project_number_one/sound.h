#ifndef SOUND_H
#define SOUND_H

#include <windows.h>
#include <mmsystem.h>
#include <string>

class SoundManager {
public:
    // لتشغيل صوت فوراً
    static void playNow(const std::string& path);

    // لتشغيل صوت واحد بعد تأخير
    static void playWithDelay(const std::string& path, int delayMs);

    // لتشغيل صوت متكرر فوراً
    static void playLoop(const std::string& path);

    // لتشغيل صوت متكرر بعد تأخير
    static void playLoopWithDelay(const std::string& path, int delayMs);

    // لإيقاف كل الأصوات
    static void stopAll();
};

#endif