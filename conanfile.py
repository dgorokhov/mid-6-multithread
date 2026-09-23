from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps

class PriorityTaskDispatcherRecipe(ConanFile):
    name = "priority_task_dispatcher"
    version = "1.0.0"
    package_type = "application"

    # Настройки целевой платформы (ОС, компилятор, архитектура, тип сборки)
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        # Добавляем Google Test (версию можно зафиксировать посвежее)
        self.requires("gtest/1.15.0")

    def layout(self):
        # Автоматически настраивает папки сборки (build/ для Conan и CMake)
        cmake_layout(self)

    def generate(self):
        # Генерирует файлы для линковки библиотек (напр., GTest::gtest_main)
        deps = CMakeDeps(self)
        deps.generate()
        
        # Генерирует conan_toolchain.cmake для передачи настроек компилятора в CMake
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        # Команда сборки (опционально, если будешь собирать через conan build)
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
