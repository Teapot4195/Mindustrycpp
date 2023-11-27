//
// Created by teapot on 26/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_APPLICATION_H
#define MINDUSTRY_SRC_ARC_APPLICATION_H

#include <memory>
#include <optional>

#include "struct.h"
#include "util/Disposable.h"

// Forward Decl to avoid circular reference
class ApplicationListener;

enum class ApplicationType {
    android,
    desktop,
    headless,
    web,
    iOS
};

class Application : public Disposable {
public:
    // This is what became of the dispose function!
    ~Application() override;

    virtual std::pair<Seq<std::shared_ptr<ApplicationListener>>, std::mutex> getListeners() = 0;

    virtual void addListener(std::shared_ptr<ApplicationListener> listener);

    virtual void removeListener(std::shared_ptr<ApplicationListener> listener);

    virtual void defaultUpdate();

    virtual ApplicationType getType() = 0;

    virtual bool isDesktop();

    virtual bool isHeadless();

    virtual bool isAndroid();

    virtual bool isIOS();

    virtual bool isMobile();

    virtual bool isWeb();

    virtual int getVersion();

    virtual long long getJavaHeap();

    virtual long long getNAtiveHeap();

    virtual std::optional<std::string> getClipboardText() = 0;

    virtual void setClipboardText(std::string text) = 0;

    virtual bool openFolder(std::string file);

    virtual bool openURI(std::string URI);

    virtual void post(std::shared_ptr<Runnable> runnable) = 0;

    virtual void exit() = 0;
};


#endif //MINDUSTRY_SRC_ARC_APPLICATION_H
