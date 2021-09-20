from conans import ConanFile, CMake, tools

class Sennet(ConanFile):
    name = "Sennet"
    version = "0.1"
    requires = (
        "asio/[>=1.18.0]",
        "glad/[>=0.1.29]",
        "glfw/[>=3.3]",
        "glm/[>=0.9.9]",
        "spdlog/[>=1.9.2]",
        "stb/cci.20210713",
    )
    generators = "cmake", "gcc", "txt", "cmake_find_package"
    settings = "os", "compiler", "build_type", "arch"

    def source(self):
        """  Defines the source code origin. """
        pass

    def configure(self):
        """ Configure dependencies. """
        self.options["glad"].shared        = False
        self.options["glad"].fPIC          = True
        self.options["glad"].no_loader     = False
        self.options["glad"].spec          = "gl"
        self.options["glad"].gl_profile    = "core"
        self.options["glad"].gl_version    = "4.6"
        self.options["glad"].gles1_version = "None"
        self.options["glad"].gles2_version = "None"
        self.options["glad"].glsc2_version = "None"

    def build(self):
        """ Wrapper for the build system. """
        pass

    def package(self):
        """ Defines the contents that will be in the package. """
        pass

    def package_info(self):
        """ Defines the information of the package. """
        self.cpp_info.libs = ["Sennet"]
