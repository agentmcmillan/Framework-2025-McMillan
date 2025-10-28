#pragma once
#include "scene_common.hpp"

// Assign stable IDs here
enum SceneId : uint8_t {
  SCN_FIREWORKS = 0,
  SCN_RAINBOW   = 1,
  SCN_SPINCAT   = 2,
  SCN_COUNT
};

extern const SceneRef gScenes[SCN_COUNT];