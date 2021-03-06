//
// Created by Alex Crimi on 2/15/19.
//

#include <common/OboeDebug.h>
#include "AudioTrack.h"

using namespace oboe;

AudioTrack::AudioTrack(JavaVM *javaVM, jobject audioTrack) {
    mJavaVM = javaVM;
    mAudioTrack = audioTrack;
}

AudioTrack::~AudioTrack() {
    JNIEnv* env = nullptr;
    mJavaVM->GetEnv((void **)&env, JNI_VERSION_1_6);
    if (env != nullptr) {
        env->DeleteGlobalRef(mAudioTimestampClass);
        mAudioTimestampClass = nullptr;
        env->DeleteGlobalRef(mAudioTimestamp);
        mAudioTimestamp = nullptr;
        env->DeleteGlobalRef(mAudioTrackClass);
        mAudioTrackClass = nullptr;
    }
}

ResultWithValue<FrameTimestamp> AudioTrack::getTimestamp() {
    if (mJavaVM == nullptr) {
        return ResultWithValue<FrameTimestamp>(Result::ErrorNull);
    }

    JNIEnv* env;
    int envStat = mJavaVM->GetEnv((void **)&env, JNI_VERSION_1_6);
    if (envStat == JNI_EDETACHED) {
        // Cannot get JNIEnv on a thread that is not attached to the JVM
        // Only call getTimestamp from threads that are already attached
        return ResultWithValue<FrameTimestamp>(Result::ErrorInvalidState);
    }

    if (mAudioTimestampClass == nullptr) {
        // cache AudioTimestamp.java class and fields
        mAudioTimestampClass = (jclass) env->NewGlobalRef(env->FindClass("android/media/AudioTimestamp"));
        mFramePositionField = env->GetFieldID(mAudioTimestampClass, "framePosition", "J");
        mNanoTimeField = env->GetFieldID(mAudioTimestampClass, "nanoTime", "J");

        // create reusable AudioTimestamp.java instance
        jmethodID newAudioTimestamp = env->GetMethodID(mAudioTimestampClass, "<init>", "()V");
        mAudioTimestamp = (jobject) env->NewGlobalRef(env->NewObject(mAudioTimestampClass, newAudioTimestamp));
    }

    if (mAudioTrackClass == nullptr) {
        // cache AudioTrack.java class and getTimestamp method
        mAudioTrackClass = (jclass) env->NewGlobalRef(env->FindClass("android/media/AudioTrack"));
        mGetTimestampID = env->GetMethodID(mAudioTrackClass, "getTimestamp", "(Landroid/media/AudioTimestamp;)Z");
    }

    jboolean timestampAvailable = env->CallBooleanMethod(mAudioTrack, mGetTimestampID, mAudioTimestamp);

    if (timestampAvailable == JNI_FALSE) {
        return ResultWithValue<FrameTimestamp>(Result::ErrorUnavailable);
    }

    FrameTimestamp frame;
    frame.position = (int64_t) env->GetLongField(mAudioTimestamp, mFramePositionField);
    frame.timestamp = (int64_t) env->GetLongField(mAudioTimestamp, mNanoTimeField);

    return ResultWithValue<FrameTimestamp>(frame);
}
