//
// Created by Alex Crimi on 4/3/19.
//

#include <oboe/Jni.h>

namespace oboe {

static JavaVM * cachedJavaVM;

void jniLoad(JavaVM * javaVM) {
    cachedJavaVM = javaVM;
}

void jniUnload() {
    cachedJavaVM = nullptr;
}

JavaVM * getJavaVM() {
    return cachedJavaVM;
}

}