from conan import ConanFile

from conan.errors import ConanInvalidConfiguration

from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import copy, get, patch
from conan.tools.scm import Git, Version

from typing import Dict

required_conan_version = ">=2.0.0"

class PineConan(ConanFile):
    name = "pine"
    version = "0.3.1"
    license = "Apache 2.0"
    author = "Martin Kvisvik Larsen"
    description = "Pine - Library for graphics and network"
    url = "https://github.com/markvilar/pine"
    homepage = "https://github.com/markvilar/pine"

    settings = ["os", "compiler", "build_type", "arch"]
    
    options = {
        "shared" : [True, False], 
        "fPIC" : [True, False],
        "enable_editor" : [True, False],
        "enable_examples" : [True, False],
        "enable_tests" : [True, False],
    }
    
    default_options = {
        "shared" : False, 
        "fPIC" : True,
        "enable_editor" : True,
        "enable_examples" : True,
        "enable_tests" : True,
    }

    exports_sources = [
        "CMakeLists.txt", 
        "cmake/*",
        "example/*", 
        "resources/*", 
        "src/*", 
        "test/*",
        "vendor/*"
    ]

    def config_options(self):
        """ Configure project options. """
        if self.settings.os == "Windows":
            del self.options.fPIC
        if self.options.shared:
            del self.options.fPIC

    def configure(self):
        """ Configure project settings. """
        self.options["glad/*"].gl_profile = "core"
        self.options["glad/*"].gl_version = "4.6"
        self.options["glad/*"].gles1_version = None
        self.options["glad/*"].gles2_version = "3.2"
        self.options["glad/*"].glsc2_version = None
        self.options["glad/*"].no_loader = False
        self.options["glad/*"].shared = False
        self.options["glad/*"].spec = "gl"

    def requirements(self):
        """ Specifies the requirements of the package. """
        self.requires("argparse/[>=2.9]")
        self.requires("asio/[>=1.21.0]")
        self.requires("glad/0.1.36")
        self.requires("glfw/3.3.4")
        self.requires("glm/0.9.9.8")
        self.requires("spdlog/1.9.2")
        self.requires("stb/cci.20210713")

    def build_requirements(self):
        """ Specifies requirements for building the package. """
        self.tool_requires("cmake/[>=3.19]")

    def validate(self):
        """ Validates the project configuration. """
        if self.settings.compiler == "clang":
            if Version(self.settings.compiler.version) < "8":
                raise ConanInvalidConfiguration("Invalid clang compiler \
                    version.")
        if self.settings.compiler == "gcc":
            if Version(self.settings.compiler.version) < "7":
                raise ConanInvalidConfiguration("Invalid gcc compiler \
                    version.")
        if self.settings.compiler == "msvc":
            if Version(self.settings.compiler.version) < "16":
                raise ConanInvalidConfiguration("Invalid Visual Studio \
                    compiler version.")

    def layout(self):
        """ Defines the project layout. """
        self.folders.root = "."
        self.folders.source = "."
        self.folders.build = "build"
        cmake_layout(self)

    def source(self):
        """ """
        pass

    def _get_cmake_variables(self) -> Dict:
        """ Internal methods to get CMake variables based on options. """
        variables = {
            "PINE_EDITOR_ENABLED" : 
                "ON" if self.options.enable_editor else "OFF",
            "PINE_EXAMPLE_ENABLED" : 
                "ON" if self.options.enable_examples else "OFF",
            "PINE_TEST_ENABLED" : 
                "ON" if self.options.enable_tests else "OFF",
        }
        return variables

    def generate(self):
        """ Generates files necessary for build the package. """
        # Create dependency graph
        deps = CMakeDeps(self)
        deps.generate()

        # Set up toolchain
        tc = CMakeToolchain(self)

        # Add variables to toolchain
        variables = self._get_cmake_variables()
        for name, value in variables.items():
            tc.variables[name] = value
        
        # Generate toolchain
        tc.generate()

    def build(self):
        """ Builds the library. """
        cmake = CMake(self)
        variables = self._get_cmake_variables()
        cmake.configure(variables=variables)
        cmake.build()

    def package(self):
        """ Packages the library. """
        copy(self, pattern="LICENSE*", dst="licenses", src=self.source_folder)
        cmake = CMake(self)
        variables = self._get_cmake_variables()
        cmake.configure(variables=variables)
        cmake.install()

    def package_info(self):
        """ Configures the package information. """
        # ImGui component
        imgui_comp = self.cpp_info.components["libimgui"]
        imgui_comp.set_property("cmake_file_name", "imgui-config.cmake")
        imgui_comp.set_property("cmake_target_name", "imgui::imgui")
        imgui_comp.set_property("pkg_config_name", "imgui-config.cmake")

        self.cpp_info.components["libimgui"].libs = ["imgui"]
        self.cpp_info.components["libimgui"].requires = ["glfw::glfw"]
        self.cpp_info.components["libimgui"].defines.append(
            "IMGUI_IMPL_GLFW_OPENGL3")
        self.cpp_info.components["libimgui"].defines.append(
            "IMGUI_IMPL_OPENGL_LOADER_GLAD")

        # Pine component
        pine_comp = self.cpp_info.components["libpine"]
        pine_comp.set_property("cmake_file_name", "pine-config.cmake")
        pine_comp.set_property("cmake_target_name", "pine::pine")
        pine_comp.set_property("pkg_config_name", "pine-config.cmake")

        self.cpp_info.components["libpine"].libs = ["pine"]
        self.cpp_info.components["libpine"].requires = [
            "libimgui",
            "argparse::argparse",
            "asio::asio",
            "glad::glad",
            "glfw::glfw",
            "glm::glm",
            "spdlog::spdlog",
            "stb::stb",
        ]
        self.cpp_info.components["libpine"].resdirs= ["resources"]

        if self.settings.os == "Windows":
            self.cpp_info.components["libpine"].defines.append(
                "PINE_PLATFORM_WINDOWS")
        elif self.settings.os == "Linux":
            self.cpp_info.components["libpine"].defines.append(
                "PINE_PLATFORM_LINUX")

        if self.settings.build_type == "Debug":
            self.cpp_info.components["libpine"].defines.append("PINE_DEBUG")
        elif self.settings.build_type == "Release":
            self.cpp_info.components["libpine"].defines.append("PINE_RELEASE")

    def export(self):
        """ Responsible for capturing the coordinates of the current URL and 
        commit. """
        # TODO: Implement
        pass
