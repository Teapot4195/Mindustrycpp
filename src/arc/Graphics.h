//
// Created by teapot on 04/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_H

#include <memory>
#include <vector>

#include <arc/Utilities.h>
#include <arc/util/Disposable.h>

// Forward decl, to avoid circular reference
class Color;
class Cursor;
class GLVersion;
class GL20;
class GL30;
class Pixmap;

class BufferFormat {
public:
    const int r, g, b, a;
    const int depth, stencil;
    const int samples;
    const bool coverageSampling;

    BufferFormat(int r, int g, int b, int a, int depth, int stencil, int samples, bool coverageSamppling);

    [[nodiscard]] std::string toString() const;
};

class Cursor : public Disposable, virtual public java::NamedTree {
public:
    Cursor();
};

class SystemCursor : public Cursor, virtual public java::NamedTree {
public:
    static const std::shared_ptr<SystemCursor> arrow;
    static const std::shared_ptr<SystemCursor> ibeam;
    static const std::shared_ptr<SystemCursor> crosshair;
    static const std::shared_ptr<SystemCursor> hand;
    static const std::shared_ptr<SystemCursor> horizontalResize;
    static const std::shared_ptr<SystemCursor> verticalResize;

    std::shared_ptr<Cursor> cursor;

    SystemCursor();

    void set(std::shared_ptr<Cursor> curs);

    void dispose() override;
};

const std::shared_ptr<SystemCursor> SystemCursor::arrow = std::make_shared<SystemCursor>();
const std::shared_ptr<SystemCursor> SystemCursor::ibeam = std::make_shared<SystemCursor>();
const std::shared_ptr<SystemCursor> SystemCursor::crosshair = std::make_shared<SystemCursor>();
const std::shared_ptr<SystemCursor> SystemCursor::hand = std::make_shared<SystemCursor>();
const std::shared_ptr<SystemCursor> SystemCursor::horizontalResize = std::make_shared<SystemCursor>();
const std::shared_ptr<SystemCursor> SystemCursor::verticalResize = std::make_shared<SystemCursor>();

class Graphics : public Disposable {
private:
    std::shared_ptr<Cursor> lastCursor;

public:
    virtual bool isGL30Available();

    virtual std::shared_ptr<GL20> getGL20() = 0;

    virtual void setGL20(std::shared_ptr<GL20> gl20) = 0;

    virtual std::shared_ptr<GL30> getGL30() = 0;

    virtual void setGL30(std::shared_ptr<GL30> gl30) = 0;

    virtual void clear(float r, float g, float b, float a);

    virtual void clear(std::shared_ptr<Color> color) = 0;

    virtual bool isPortrait();

    virtual int getWidth() = 0;

    virtual int getHeight() = 0;

    virtual float getAspect();

    virtual bool isHidden();

    virtual int getBackBufferWidth() = 0;

    virtual int getBackBufferHeight() = 0;

    virtual std::vector<int> getSaveInsets();

    virtual long getFrameId() = 0;

    virtual float getDeltaTime() = 0;

    virtual int getFramesPerSecond() = 0;

    virtual std::shared_ptr<GLVersion> getGLVersion() = 0;

    virtual float getPpiX() = 0;

    virtual float getPpiY() = 0;

    virtual float getPpcX() = 0;

    virtual float getPpcY() = 0;

    virtual float getDensity() = 0;

    virtual bool setFullscreen();

    virtual bool setWindowedMode(int width, int height) = 0;

    virtual void setTitle(std::string title) = 0;

    virtual void setBorderless(bool undecorated) = 0;

    virtual void setResizable(bool resizable) = 0;

    virtual void setVSync(bool vsync) = 0;

    virtual std::shared_ptr<BufferFormat> getBufferFormat() = 0;

    virtual bool supportsExtension(std::string extension) = 0;

    virtual bool isContinuousRendering() = 0;

    virtual void setContinuousRendering(bool isContinuous) = 0;

    virtual void requestRendering() = 0;

    virtual bool isFullscreen() = 0;

    virtual std::shared_ptr<Cursor> newCursor(std::shared_ptr<Pixmap> pixmap, int xHotspot, int yHotspot) = 0;

    std::shared_ptr<Cursor> newCursor(const std::shared_ptr<Pixmap>& pixmap, int scaling, std::shared_ptr<Color> outlineColor, int outlineThickness);

    std::shared_ptr<Cursor> newCursor(const std::string& filename, int scale);

    std::shared_ptr<Cursor> newCursor(const std::string& filename);

    std::shared_ptr<Cursor> newCursor(const std::string& filename, int scaling, std::shared_ptr<Color> outlineColor, int outlineScaling);

    void restoreCursor();

    void cursor(const std::shared_ptr<Cursor>& cursor);

protected:
    virtual void setCursor(std::shared_ptr<Cursor> cursor) = 0;

    virtual void setSystemCursor(std::shared_ptr<SystemCursor> systemCursor) = 0;

public:
    void dispose() override;
};


#endif //MINDUSTRY_SRC_ARC_GRAPHICS_H
