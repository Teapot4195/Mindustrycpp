//
// Created by teapot on 21/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_CORE_H
#define MINDUSTRY_SRC_ARC_CORE_H

#include <memory>
#include <optional>

class Application;
class Graphics;
class Audio;
class Input;
class Files;
class Settings;
//class KeyBinds;
//class I18NBundle;
class Camera;
class Batch;
class Scene;
class AssetManager;
class TextureAtlas;
class GL20;
class GL30;

class Core {
public:
    static std::optional<std::shared_ptr<Application>> app;
    static std::optional<std::shared_ptr<Graphics>> graphics;
    static std::optional<std::shared_ptr<Audio>> audio;
    static std::optional<std::shared_ptr<Input>> input;
    static std::optional<std::shared_ptr<Files>> files;
    static std::optional<std::shared_ptr<Settings>> settings;
//    static std::optional<std::shared_ptr<KeyBinds>> keybinds = new KeyBinds();

//    static std::optional<std::shared_ptr<I18NBundle>> bundle = I18NBundle.createEmptyBundle();
    static std::optional<std::shared_ptr<Camera>> camera;
    static std::optional<std::shared_ptr<Batch>> batch;
    static std::optional<std::shared_ptr<Scene>> scene;
    static std::optional<std::shared_ptr<AssetManager>> assets;
    static std::optional<std::shared_ptr<TextureAtlas>> atlas;

    static std::optional<std::shared_ptr<GL20>> gl;
    static std::optional<std::shared_ptr<GL20>> gl20;
    static std::optional<std::shared_ptr<GL30>> gl30;
};

#endif //MINDUSTRY_SRC_ARC_CORE_H
