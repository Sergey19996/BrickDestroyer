//
// Created by Lenovo on 20.04.2025.
//

#ifndef MY_APPLICATION_RESOURCEMANAGER_H
#define MY_APPLICATION_RESOURCEMANAGER_H

#include "rendering/Texture.h"
#include "rendering/Shader.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <sndfile.h>
#include <map>

class ResourceManager
{
public:

    // Инициализация один раз из android_main
    static void Init(AAssetManager* mgr) {
        assetManager = mgr;
    }

    // Открыть AAsset* по пути в assets/
    static AAsset* OpenAsset(const char* path) {
        return AAssetManager_open(assetManager, path, AASSET_MODE_STREAMING);
    }



    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, Texture>Textures;

    static Shader LoadShader(const std::string&  vShaderFile, const std::string&  fShaderFile,  std::string name);
    static Shader& GetShader(std::string name);
    static Texture LoadTexture(const char* file, std::string name);
    static Texture& GetTexture(std::string name);
    //properly de-allocates all loaded resources
    static void clear();

    // SFML‑style виртуальный I/O для sndfile
    static sf_count_t get_filelen(void* user_data);
    static sf_count_t seek(sf_count_t offset, int whence, void* user_data);
    static sf_count_t read(void* ptr, sf_count_t count, void* user_data);
    static sf_count_t tell(void* user_data);
    static AAssetManager* getAssetManeger() { return assetManager; }

private:
    ResourceManager() = delete;
   ~ResourceManager() = delete;
    static Texture loadTextureFromFIle( const std::string& assetPath);
    static AAssetManager* assetManager;
};

#endif //MY_APPLICATION_RESOURCEMANAGER_H
