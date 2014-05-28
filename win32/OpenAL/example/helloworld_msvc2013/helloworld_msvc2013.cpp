// helloworld_msvc2013.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include "AL/alut.h"


/**
 * Report ALUT error.
 * Terminates the program.
 *
 * @param location The location description where the error occured
 */
void reportALUTError(const char *location) {
    fprintf(stderr, "ALUT error (%s): %s\nHit any key to continue...",
        location, alutGetErrorString(alutGetError()));
    getchar();
    fprintf(stderr, "\n");
    exit(-1);
}


/**
 * Report OpenAL error.
 * Terminates the program.
 *
 * @param location The location description where the error occured
 * @param code The OpenAL error code
 */
void reportALError(const char *location, ALenum code) {
    fprintf(stderr, "OpenAL error (%s): %d - %s\nHit any key to continue...",
        location, code, "");
    getchar();
    fprintf(stderr, "\n");
    exit(-1);
}


/**
 * The application main entry function
 *
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 *
 * @return Application return value
 */
int _tmain(int argc, _TCHAR* argv[]) {
    ALuint helloBuffer, helloSource;
    ALenum errCode;

    // initializes ALUT (initializes AL as side effect)
    if (!alutInit(&argc, argv)) reportALUTError("glutInit");

    // Generate buffer with "hello world"
    helloBuffer = alutCreateBufferHelloWorld();
    if (helloBuffer == AL_NONE) reportALUTError("alutCreateBufferHelloWorld");

    // reset al error code
    alGetError();
    // generate al source
    alGenSources(1, &helloSource);
    if ((errCode = ::alGetError()) != AL_NO_ERROR) reportALError("alGenSources", errCode);
    // connect source an buffer
    alSourcei(helloSource, AL_BUFFER, helloBuffer);
    if ((errCode = ::alGetError()) != AL_NO_ERROR) reportALError("alSourcei", errCode);

    // play source
    alSourcePlay(helloSource);
    if ((errCode = ::alGetError()) != AL_NO_ERROR) reportALError("alSourcePlay", errCode);

    // wait for source playback
    if (!alutSleep(1)) reportALUTError("alutSleep");

    // exit alut layer
    if (!alutExit()) reportALUTError("alutExit");
    
    return 0;
}

