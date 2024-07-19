from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout

class TambuaRecipe(ConanFile):
    name = "tambua"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        self.requires("abseil/20230125.3")
        self.requires("tensorflow-lite/2.10.0")
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self, generator='Ninja')
        tc.preprocessor_definitions["TFLITE_PROFILING_ENABLED"] = "ON"
        tc.user_presets_path = False
        tc.generate()
