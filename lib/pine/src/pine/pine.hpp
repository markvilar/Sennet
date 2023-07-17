#pragma once

// Defines
#include "pine/defines/assert.hpp"
#include "pine/defines/platform.hpp"

// Core
#include "pine/core/engine.hpp"
#include "pine/core/input.hpp"
#include "pine/core/key_codes.hpp"
#include "pine/core/log.hpp"
#include "pine/core/mouse_codes.hpp"
#include "pine/core/timestep.hpp"
#include "pine/core/window.hpp"

// Graphical user interface
#include "pine/gui/flags.hpp"
#include "pine/gui/manager.hpp"
#include "pine/gui/style.hpp"
#include "pine/gui/widgets.hpp"
#include "pine/gui/window.hpp"

// Networkpine
#include "pine/network/client.hpp"
#include "pine/network/connection.hpp"
#include "pine/network/server.hpp"
#include "pine/network/types.hpp"

// Renderer
#include "pine/renderer/buffer.hpp"
#include "pine/renderer/camera.hpp"
#include "pine/renderer/framebuffer.hpp"
#include "pine/renderer/image.hpp"
#include "pine/renderer/quad_renderer.hpp"
#include "pine/renderer/render_command.hpp"
#include "pine/renderer/renderer.hpp"
#include "pine/renderer/shader.hpp"
#include "pine/renderer/texture.hpp"
#include "pine/renderer/types.hpp"

// Utils
#include "pine/utils/argparse.hpp"
#include "pine/utils/filesystem.hpp"
#include "pine/utils/fonts.hpp"
#include "pine/utils/locked_queue.hpp"
#include "pine/utils/math.hpp"
#include "pine/utils/uuid.hpp"
