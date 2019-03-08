//
// Created by Alex Crimi on 2/15/19.
//

#ifndef OBOE_AUDIOTRACK_H
#define OBOE_AUDIOTRACK_H

#include "oboe/Oboe.h"
#include <jni.h>

namespace oboe {

class AudioTrack {
public:
    AudioTrack(JavaVM *javaVM, jobject audioTrack);
    ~AudioTrack();

    ResultWithValue<FrameTimestamp> getTimestamp();

private:
    // Cached JVM instance for retrieving JNIEnv
    JavaVM *mJavaVM = nullptr;

    // Cached AudioTrack.java objects
    jclass mAudioTrackClass = nullptr;
    jmethodID mGetTimestampID;
    jobject mAudioTrack;

    // Cached AudioTimestamp.java objects
    jclass mAudioTimestampClass = nullptr;
    jfieldID mFramePositionField;
    jfieldID mNanoTimeField;
    jobject mAudioTimestamp;
};

} // namespace oboe

#endif // OBOE_AUDIOTRACK_H
