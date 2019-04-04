/*
 * Copyright 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AUDIO_OUTPUT_STREAM_OPENSL_ES_H_
#define AUDIO_OUTPUT_STREAM_OPENSL_ES_H_


#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include "oboe/Oboe.h"
#include "AudioStreamOpenSLES.h"
#include "AudioTrack.h"

namespace oboe {

/**
 * INTERNAL USE ONLY
 */
class AudioOutputStreamOpenSLES : public AudioStreamOpenSLES {
public:
    AudioOutputStreamOpenSLES();
    explicit AudioOutputStreamOpenSLES(const AudioStreamBuilder &builder);

    virtual ~AudioOutputStreamOpenSLES();

    Result open() override;
    Result close() override;

    Result requestStart() override;
    Result requestPause() override;
    Result requestFlush() override;
    Result requestStop() override;

    Result
    getTimestamp(clockid_t clockId, int64_t *framePosition, int64_t *timeNanoseconds) override;

    ResultWithValue<FrameTimestamp> getTimestamp(clockid_t clockId) override;

    ResultWithValue<double> calculateLatencyMillis() override;

protected:

    void setFramesRead(int64_t framesRead);

    Result updateServiceFrameCounter() override;

    void updateFramesRead() override;

private:

    SLuint32 channelCountToChannelMask(int chanCount);

    Result onAfterDestroy() override;

    Result requestFlush_l();

    /**
     * Set OpenSL ES PLAYSTATE.
     *
     * @param newState SL_PLAYSTATE_PAUSED, SL_PLAYSTATE_PLAYING, SL_PLAYSTATE_STOPPED
     * @return
     */
    Result setPlayState_l(SLuint32 newState);

    bool canQueryTimestamp();

    SLPlayItf      mPlayInterface = nullptr;

    AudioTrack *mAudioTrack = nullptr;
    int64_t mLastTimestampQuery = -1;
    double mLastKnownLatency = -1;
};

} // namespace oboe

#endif //AUDIO_OUTPUT_STREAM_OPENSL_ES_H_
