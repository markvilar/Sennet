#pragma once

#include "assert.hpp"
#include "common.hpp"
#include "platform.hpp"
#include "types.hpp"

// Core
#include "../../src/core/application.hpp"
#include "../../src/core/input.hpp"
#include "../../src/core/key_codes.hpp"
#include "../../src/core/layer.hpp"
#include "../../src/core/log.hpp"
#include "../../src/core/mouse_codes.hpp"
#include "../../src/core/timestep.hpp"
#include "../../src/core/window.hpp"

// Graphical user interface
#include "../../src/gui/common.hpp"
#include "../../src/gui/manager.hpp"
#include "../../src/gui/style.hpp"
#include "../../src/gui/widgets.hpp"
#include "../../src/gui/window.hpp"

// Network
#include "../../src/network/client.hpp"
#include "../../src/network/connection.hpp"
#include "../../src/network/server.hpp"
#include "../../src/network/types.hpp"

// Renderer
#include "../../src/renderer/buffer.hpp"
#include "../../src/renderer/camera.hpp"
#include "../../src/renderer/framebuffer.hpp"
#include "../../src/renderer/image.hpp"
#include "../../src/renderer/quad_renderer.hpp"
#include "../../src/renderer/render_command.hpp"
#include "../../src/renderer/renderer.hpp"
#include "../../src/renderer/shader.hpp"
#include "../../src/renderer/texture.hpp"
#include "../../src/renderer/types.hpp"

// Utils
#include "../../src/utils/argparse.hpp"
#include "../../src/utils/filesystem.hpp"
#include "../../src/utils/fonts.hpp"
#include "../../src/utils/locked_queue.hpp"
#include "../../src/utils/math.hpp"
#include "../../src/utils/uuid.hpp"
