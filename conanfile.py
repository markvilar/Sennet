from conans import ConanFile, CMake, tools

class Sennet(ConanFile):
    name = "sennet"
    version = "0.1"
    generators = "cmake_find_package"
    settings = "os", "compiler", "build_type", "arch"
    license = "Apache 2.0"
    author = "Martin Kvisvik Larsen"

    _source_dir = ""

    def source(self):
        """  Defines the source code origin. """
        pass

    def configure(self):
        """ Configure dependencies. """
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
        self.requires("asio/[>=1.18.0]")
        self.requires("entt/[>=3.8.1]")
        self.requires("glad/[>=0.1.29]")
        self.requires("glfw/[>=3.3]")
        self.requires("glm/[>=0.9.9]")
        self.requires("imgui/1.84.2")
        self.requires("spdlog/[>=1.9.2]")

    def imports(self):
        """ Imports source code from the dependencies. """
        pass

    def validate(self):
        """ """
        pass

    def build(self):
        """ """
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()

    def package(self):
        """ """
        pass

    def package_info(self):
        """"""
        pass
