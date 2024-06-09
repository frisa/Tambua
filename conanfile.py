from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout

class TambuaRecipe(ConanFile):
    name = "tambua"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"
    types = "Release", "Debug"

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        self.requires("tensorflow-lite/2.12.0")
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()