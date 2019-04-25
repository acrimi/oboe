# About This Fork
The purpose of this fork is to add support for output latency calculation using OpenSL ES on Android 7.0+. Usage is the same as the core Oboe library, with the addition that you should provide implementations of [JNI_OnLoad/JNI_OnUnload](https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/invocation.html#JNJI_OnLoad) and pass the associated JavaVM instance to Oboe:

```
#include "oboe/Jni.h"

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM * jvm, void * /*reserved*/) {
  oboe::jniLoad(jvm);
  return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT jint JNICALL JNI_OnUnload(JavaVM * jvm, void * /*reserved*/) {
  oboe::jniUnload();
}
```
Oboe will cache the JavaVM internally and automatically use it to query timestamps via JNI for OpenSL ES output streams, adding support for [getTimestamp()](https://google.github.io/oboe/reference/classoboe_1_1_audio_stream.html#a1d7cf4e43fb9d7b31857a90d9eceee6d) and [calculateLatencyMillis()](https://google.github.io/oboe/reference/classoboe_1_1_audio_stream.html#ae023cb001f3261d064f423101798d6be) in AudioOutputStreamOpenSLES. **NOTE**: No logic is done internally to attach/detach threads to the JavaVM, so these methods should only be called on threads that are already [attached](https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/invocation.html#attaching_to_the_vm).

Querying timestamps/latency is still not supported for input streams.

# Oboe [![Build Status](https://travis-ci.org/google/oboe.svg?branch=master)](https://travis-ci.org/google/oboe)

[![Introduction to Oboe video](docs/images/getting-started-video.jpg)](https://www.youtube.com/watch?v=csfHAbr5ilI&list=PLWz5rJ2EKKc_duWv9IPNvx9YBudNMmLSa)

Oboe is a C++ library which makes it easy to build high-performance audio apps on Android. It was created primarily to allow developers to target a simplified API that works across multiple API levels back to API level 16 (Jelly Bean).

## Features
- Compatible with API 16 onwards - runs on 99% of Android devices
- Chooses the audio API (OpenSL ES on API 16+ or AAudio on API 27+) which will give the best audio performance on the target Android device
- Automatic latency tuning
- Modern C++ allowing you to write clean, elegant code
- [Used by popular apps and frameworks](docs/AppsUsingOboe.md)

## Requirements
To build Oboe you will need the Android NDK r17 or above. It can be installed using Android Studio's SDK manager, or via [direct download](https://developer.android.com/ndk/downloads/).

## Documentation
- [Getting Started Guide](docs/GettingStarted.md)
- [Full Guide to Oboe](docs/FullGuide.md)
- [API reference](https://google.github.io/oboe/reference)
- [History of Audio features/bugs by Android version](docs/AndroidAudioHistory.md)
- [Frequently Asked Questions](docs/FAQ.md) (FAQ)

## Sample code
Sample apps can be found in the [samples directory](samples). Also check out the [Rhythm Game codelab](https://codelabs.developers.google.com/codelabs/musicalgame-using-oboe/index.html#0).

### Third party sample code
- [Ableton Link integration demo](https://github.com/jbloit/AndroidLinkAudio) (author: jbloit)

## Contributing
We would love to receive your pull requests. Before we can though, please read the [contributing](CONTRIBUTING.md) guidelines.

## Version history
View the [releases page](../../releases).

## License
[LICENSE](LICENSE)

