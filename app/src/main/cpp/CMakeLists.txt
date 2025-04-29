# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.22.1)

project("myapplication")

#set(CMAKE_CXX_FLAGS_DEBUG "-g")
# Для Release сборки оптимизация
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Добавляем путь к GLM (предполагая, что он в extern/glm)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/glm/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/libsndfile/include)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/freetype/include/freetype2)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/freetype/include/freetype2)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/OpenAL/include)
add_library(openal SHARED IMPORTED)
set_target_properties(openal PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/extern/OpenAL/lib/${ANDROID_ABI}/libopenal.so"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/extern/OpenAL/include"
)

#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/OpenAL/lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/libsndfile/lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/freetype/lib)

# Creates your game shared library. The name must be the same as the
# one used for loading in your Kotlin/Java or AndroidManifest.txt files.
# Добавление всех .cpp файлов из подкаталогов
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        src/*.cpp
        src/graphics/*.cpp
        src/graphics/algorithms/*.cpp
        src/graphics/Effects/*.cpp
        src/graphics/Level/*.cpp
        src/graphics/models/*.cpp
        src/graphics/rendering/*.cpp
        src/graphics/Text/*.cpp
        src/audio/*.cpp
)

# Создаем библиотеку или исполняемый файл
add_library(myapplication SHARED ${SOURCES})

# Searches for a package provided by the game activity dependency
find_package(game-activity REQUIRED CONFIG)


include(FetchContent)

# Configure libraries CMake uses to link your target library.
target_link_libraries(myapplication
        PRIVATE
        # The game activity
        game-activity::game-activity   # Позволяет использовать GameActivity API: lifecycle, input, surface и т.д.
        # EGL and other dependent libraries required for drawing
        # and interacting with Android system
        EGL     # Интерфейс между OpenGL и Android Surface. Без него нельзя создать OpenGL контекст
        GLESv3  # OpenGL ES 3.x — основной графический API для отрисовки
        jnigraphics  #	Позволяет работать с Android Bitmap через JNI
        android  # Базовая Android NDK библиотека. Доступ к AAssetManager, NativeActivity, и т.д.
        log   # Для __android_log_print — логирование в logcat
        openal
        sndfile
        freetype
        z

)
