//
// Created by teapot on 26/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_APPLICATIONLISTENER_H
#define MINDUSTRY_SRC_ARC_APPLICATIONLISTENER_H

#include <memory>
#include <cstdint>

//Forward decl to resolve circular reference
class Fi;

class ApplicationListener : public std::enable_shared_from_this<ApplicationListener> {
public:
    virtual ~ApplicationListener() = default;

    virtual void init() {}

    virtual void resize(std::uint32_t width, std::uint32_t height) {}

    virtual void update() {}

    virtual void pause() {}

    virtual void resume() {}

    virtual void dispose() {}

    virtual void exit() {}

    virtual void fileDropped(std::shared_ptr<Fi> file) {}
};


#endif //MINDUSTRY_SRC_ARC_APPLICATIONLISTENER_H
