from conans import ConanFile, CMake, tools

required_conan_version = ">=1.39.0"

class PineConan(ConanFile):
    name = "pine"
    version = "0.3.0"
    license = "Apache 2.0"
    author = "Martin Kvisvik Larsen"
    description = "Pine - Library for graphics and network"
    url = "https://github.com/markvilar/pine"
    homepage = "https://github.com/markvilar/pine"

    settings = ["os", "compiler", "build_type", "arch"]
    
    options = {
        "shared" : [True, False], 
        "fPIC" : [True, False]
    }
    
    default_options = {
        "shared" : False, 
        "fPIC" : True
    }

    exports_sources = [
        "CMakeLists.txt", 
        "cmake/*",
        "examples/*", 
        "resources/*", 
        "src/*", 
        "test/*",
        "vendor/*"
    ]
    
    generators = ["cmake", "cmake_find_package", "cmake_find_package_multi"]

    @property
    def _source_subfolder(self):
        return "source_subfolder"

    @property
    def _build_subfolder(self):
        return "build_subfolder"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        """ Configure project settings. """
        if self.options.shared:
            del self.options.fPIC
        self.options["glad"].shared        = False
        self.options["glad"].no_loader     = False
        self.options["glad"].spec          = "gl"
        self.options["glad"].gl_profile    = "core"
        self.options["glad"].gl_version    = "4.6"
        self.options["glad"].gles1_version = "None"
        self.options["glad"].gles2_version = "None"
        self.options["glad"].glsc2_version = "None"
        self.options["spdlog"].header_only = True

    def requirements(self):
        """ Specifies the requirements of the package. """
        self.requires("asio/1.21.0")
        self.requires("glad/0.1.34")
        self.requires("glfw/3.3.4")
        self.requires("glm/0.9.9.8")
        self.requires("spdlog/1.9.2")
        self.requires("stb/cci.20210713")

    def validate(self):
        """ Validates the project configuration. """
        if self.settings.compiler == "clang":
            if tools.Version(self.settings.compiler.version) < "8":
                raise ConanInvalidConfiguration("Invalid clang compiler \
                    version.")
        if self.settings.compiler == "gcc":
            if tools.Version(self.settings.compiler.version) < "7":
                raise ConanInvalidConfiguration("Invalid gcc compiler \
                    version.")
        if self.settings.compiler == "Visual Studio":
            if tools.Version(self.settings.compiler.version) < "16":
                raise ConanInvalidConfiguration("Invalid Visual Studio \
                    compiler version.")

    def _configure_cmake(self):
        """ """
        cmake = CMake(self)
        cmake.definitions["PINE_BUILD_SHARED"] = self.options["shared"]
        cmake.definitions["PINE_BUILD_WARNING"] = True
        cmake.definitions["PINE_BUILD_EDITOR"] = False
        cmake.definitions["PINE_BUILD_EXAMPLES"] = False
        cmake.definitions["PINE_BUILD_TESTS"] = False
        cmake.configure(build_folder=self._build_subfolder)        
        return cmake

    def build(self):
        """ Builds the library. """
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        """ Packages the library. """
        self.copy("LICENSE*", dst="licenses", src=self._source_subfolder)
        cmake = self._configure_cmake()
        cmake.install()

    def package_info(self):
        """ Configures the package information. """
        # ImGui component
        self.cpp_info.components["libimgui"].libs = ["imgui"]
        self.cpp_info.components["libimgui"].requires = ["glfw::glfw"]
        self.cpp_info.components["libimgui"].defines.append(
            "IMGUI_IMPL_GLFW_OPENGL3")
        self.cpp_info.components["libimgui"].defines.append(
            "IMGUI_IMPL_OPENGL_LOADER_GLAD")

        # Pine component
        self.cpp_info.components["libpine"].libs = ["pine"]
        self.cpp_info.components["libpine"].requires = [
            "libimgui",
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
