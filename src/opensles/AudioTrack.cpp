//
// Created by Alex Crimi on 2/15/19.
//

#include "AudioTrack.h"

using namespace oboe;

AudioTrack::AudioTrack(const JavaVM &javaVM, jobject audioTrack) {
    mJavaVM = javaVM;
    mAudioTrack = audioTrack;
}

AudioTrack::~AudioTrack() {
    JNIEnv* env = nullptr;
    mJavaVM.GetEnv((void **)&env, JNI_VERSION_1_6);
    if (env != nullptr) {
        env->DeleteGlobalRef(mAudioTimestampClass);
        mAudioTimestampClass = nullptr;
        env->DeleteGlobalRef(mAudioTimestamp);
        mAudioTimestamp = nullptr;
        env->DeleteGlobalRef(mAudioTrackClass);
        mAudioTrackClass = nullptr;
    }
}

ResultWithValue<AudioTimestamp> AudioTrack::getTimestamp() {
    JNIEnv* env;
    int envStat = mJavaVM.GetEnv((void **)&env, JNI_VERSION_1_6);
    if (envStat == JNI_EDETACHED) {
        // Cannot get JNIEnv on a thread that is not attached to the JVM
        return oboe::Result::ErrorInvalidState;
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
        return ResultWithValue(Result::ErrorUnavailable);
    }

    auto framePosition = (int64_t) env->GetLongField(mAudioTimestamp, mFramePositionField);
    auto nanoTime = (int64_t) env->GetLongField(mAudioTimestamp, mNanoTimeField);

    return ResultWithValue(new AudioTimestamp(framePosition, nanoTime));
}
