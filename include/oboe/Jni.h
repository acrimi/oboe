//
// Created by Alex Crimi on 4/3/19.
//

#ifndef OBOE_JNI_H
#define OBOE_JNI_H

#include <jni.h>

namespace oboe {

void jniLoad(JavaVM * javaVM);
void jniUnload();
JavaVM * getJavaVM();

}

#endif //OBOE_JNI_H
