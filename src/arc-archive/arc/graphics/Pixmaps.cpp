//
// Created by teapot on 28/11/23.
//

#include "Pixmaps.h"

std::shared_ptr<Pixmap> Pixmaps::noise(int w, int h) {
    std::shared_ptr<Pixmap> out = std::make_shared<Pixmap>(w, h);
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            out->set(x, y, Tmp)
        }
    }
}
