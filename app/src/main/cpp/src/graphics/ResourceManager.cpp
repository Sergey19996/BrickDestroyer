#include "ResourceManager.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "../../extern/stb/stb_image.h"
#include <android/imagedecoder.h>
#include "../AndroidOut.h"

std::map<std::string, Shader>ResourceManager::Shaders;
std::map<std::string, Texture>ResourceManager::Textures;
AAssetManager* ResourceManager::assetManager = nullptr;


sf_count_t ResourceManager::get_filelen(void* user_data) {
    return AAsset_getLength(reinterpret_cast<AAsset*>(user_data));
}

sf_count_t ResourceManager::seek(sf_count_t offset, int whence, void* user_data) {
    return AAsset_seek(reinterpret_cast<AAsset*>(user_data), offset, whence);
}

sf_count_t ResourceManager::read(void* ptr, sf_count_t count, void* user_data) {
    return AAsset_read(reinterpret_cast<AAsset*>(user_data), ptr, count);
}

sf_count_t ResourceManager::tell(void* user_data) {
    return AAsset_seek(reinterpret_cast<AAsset*>(user_data), 0, SEEK_CUR);
}


Shader ResourceManager::LoadShader(const std::string&  vShaderFile, const std::string&  fShaderFile,  std::string name)
{

    Shader shader(vShaderFile, fShaderFile);
    Shaders[name] = shader;
    return Shaders[name] = shader;
}

Shader& ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture ResourceManager::LoadTexture(const char* path,  std::string name)
{

    return Textures[name] = loadTextureFromFIle(path);
}

Texture& ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

void ResourceManager::clear()
{
    Shaders.clear();
    Textures.clear();
}



Texture ResourceManager::loadTextureFromFIle(const std::string& assetPath)
{
    Texture texture;

    //Reading asset
    AAsset* asset = AAssetManager_open(assetManager, assetPath.c_str(), AASSET_MODE_BUFFER);
    if (!asset) {
        // Обработка ошибки
        aout << "ERROR::LOAD_TEXTURE_FROM_FILE: Failed to open texture" << std::endl;
    }

    // Make a decoder to turn it into a texture
    AImageDecoder* decoder = nullptr;
    if (AImageDecoder_createFromAAsset(asset, &decoder) != ANDROID_IMAGE_DECODER_SUCCESS) {
        // Обработка ошибки
        AAsset_close(asset);

    }
    // make sure we get 8 bits per channel out. RGBA order.
    AImageDecoder_setAndroidBitmapFormat(decoder, ANDROID_BITMAP_FORMAT_RGBA_8888);

    // Get the image header, to help set everything up
    const AImageDecoderHeaderInfo* headerInfo = AImageDecoder_getHeaderInfo(decoder);
    // important metrics for sending to GL
    int32_t width = AImageDecoderHeaderInfo_getWidth(headerInfo);
    int32_t height = AImageDecoderHeaderInfo_getHeight(headerInfo);
    size_t stride = AImageDecoder_getMinimumStride(decoder);

    // Get the bitmap data of the image
    std::vector<uint8_t> imageData(height * stride);

    if (AImageDecoder_decodeImage(decoder, imageData.data(), stride, imageData.size()) != ANDROID_IMAGE_DECODER_SUCCESS) {
        // Обработка ошибки
        AImageDecoder_delete(decoder);
        AAsset_close(asset);
        aout << "ERROR::LOAD_AImageDecoder_decodeImage: Failed to  get image data" << std::endl;
    }



        texture.Generate(imageData.data(), height, width);  // тут создалась текстура


    //and finaly free image data
    AImageDecoder_delete(decoder);
    AAsset_close(asset);

    return texture;
}
