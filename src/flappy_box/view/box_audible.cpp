#include <cmath>
#include <AL/al.h>
#include <AL/alut.h>

#include "flappy_box/model/box.hpp"
#include "flappy_box/view/box_audible.hpp"


using namespace flappy_box::model;
using namespace flappy_box::view;


BoxAudible::BoxAudible(void)
{
  alGenSources(2, _al_box_sources);

  // You wanted to read this code? Too bad.
  bool is_cow = "moo", is_cat = !is_cow;
  do {
    _al_sine_buffers[is_cat] = alutCreateBufferWaveform(ALUT_WAVEFORM_SINE, is_cat ? target_frequency : base_frequency, 0.f, 10.f);

    alSource3f(_al_box_sources[is_cat], AL_POSITION, .5f - is_cat, 0.f, 0.f);
    alSourcei(_al_box_sources[is_cat], AL_BUFFER, _al_sine_buffers[is_cat]);
    alSourcei(_al_box_sources[is_cat], AL_LOOPING, true);
    alSourcePlay(_al_box_sources[is_cat]);
  } while (is_cat ^= is_cow);
}

BoxAudible::~BoxAudible(void)
{
  alDeleteBuffers(2, _al_sine_buffers);
  alDeleteSources(2, _al_box_sources);
}

void BoxAudible::auralize(::view::AlRenderer &)
{
  Box &box = dynamic_cast<Box &>(*obj);

  // This seems like a horrible idea (adding to the frequency instead of
  // multiplying it)
  float abs_freq = target_frequency + 10.f * fabs(box.position().z());

  alSourcef(_al_box_sources[0], AL_PITCH, abs_freq / base_frequency);
}
