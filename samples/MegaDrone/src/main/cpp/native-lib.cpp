/*
 * Copyright 2018 The Android Open Source Project
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

#include <jni.h>
#include <string>
#include <vector>

#include "MegaDroneEngine.h"


std::vector<int> convertJavaArrayToVector(JNIEnv *env, jintArray intArray){

    std::vector<int> v;
    jsize length = env->GetArrayLength(intArray);

    if (length > 0) {
        jboolean isCopy;
        jint *elements = env->GetIntArrayElements(intArray, &isCopy);
        for (int i = 0; i < length; i++) {
            v.push_back(elements[i]);
        }
    }
    return v;
}
extern "C" {
/**
 * Start the audio engine
 *
 * @param env
 * @param instance
 * @param jCpuIds - CPU core IDs which the audio process should affine to
 * @return a pointer to the audio engine. This should be passed to other methods
 */
JNIEXPORT jlong JNICALL
Java_com_example_oboe_megadrone_MainActivity_startEngine(JNIEnv *env, jobject /*unused*/,
                                                         jintArray jCpuIds) {
    std::vector<int> cpuIds = convertJavaArrayToVector(env, jCpuIds);
    LOGD("cpu ids size: %d", static_cast<int>(cpuIds.size()));
    MegaDroneEngine  *engine = new MegaDroneEngine(std::move(cpuIds));
    LOGD("Engine Started");
    return reinterpret_cast<jlong>(engine);
}

JNIEXPORT void JNICALL
Java_com_example_oboe_megadrone_MainActivity_stopEngine(JNIEnv *env, jobject instance,
        jlong jEngineHandle) {
    auto engine = reinterpret_cast<MegaDroneEngine*>(jEngineHandle);
    if (engine) {
        delete engine;
    } else {
        LOGD("Engine invalid, call startEngine() to create");
    }
}


JNIEXPORT void JNICALL
Java_com_example_oboe_megadrone_MainActivity_tap(JNIEnv *env, jobject instance,
        jlong jEngineHandle, jboolean isDown) {

    auto *engine = reinterpret_cast<MegaDroneEngine*>(jEngineHandle);
    if (engine) {
        engine->tap(isDown);
    } else {
        LOGE("Engine handle is invalid, call createEngine() to create a new one");
    }
}

JNIEXPORT void JNICALL
Java_com_example_oboe_megadrone_MainActivity_native_1setDefaultStreamValues(JNIEnv *env,
                                                                            jclass type,
                                                                            jint sampleRate,
                                                                            jint framesPerBurst) {
    oboe::DefaultStreamValues::SampleRate = (int32_t) sampleRate;
    oboe::DefaultStreamValues::FramesPerBurst = (int32_t) framesPerBurst;
}

} // extern "C"