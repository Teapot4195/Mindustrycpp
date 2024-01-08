//
// Created by teapot on 09/12/23.
//

#include "Tmp.h"

#include <arc/math/Mat.h>
#include <arc/math/geom/Circle.h>
#include <arc/math/geom/Point2.h>
#include <arc/math/geom/Rect.h>
#include <arc/graphics/Color.h>

const std::shared_ptr<Vec2> Tmp::v1 = std::make_shared<Vec2>();
const std::shared_ptr<Vec2> Tmp::v2 = std::make_shared<Vec2>();
const std::shared_ptr<Vec2> Tmp::v3 = std::make_shared<Vec2>();
const std::shared_ptr<Vec2> Tmp::v4 = std::make_shared<Vec2>();
const std::shared_ptr<Vec2> Tmp::v5 = std::make_shared<Vec2>();
const std::shared_ptr<Vec2> Tmp::v6 = std::make_shared<Vec2>();

const std::shared_ptr<Vec3> Tmp::v31 = std::make_shared<Vec3>();
const std::shared_ptr<Vec3> Tmp::v32 = std::make_shared<Vec3>();
const std::shared_ptr<Vec3> Tmp::v33 = std::make_shared<Vec3>();
const std::shared_ptr<Vec3> Tmp::v34 = std::make_shared<Vec3>();

const std::shared_ptr<Rect> Tmp::r1 = std::make_shared<Rect>();
const std::shared_ptr<Rect> Tmp::r2 = std::make_shared<Rect>();
const std::shared_ptr<Rect> Tmp::r3 = std::make_shared<Rect>();

const std::shared_ptr<Circle> Tmp::cr1 = std::make_shared<Circle>();
const std::shared_ptr<Circle> Tmp::cr2 = std::make_shared<Circle>();
const std::shared_ptr<Circle> Tmp::cr3 = std::make_shared<Circle>();

const std::shared_ptr<Vec2> Tmp::t1 = std::make_shared<Vec2>();

const std::shared_ptr<Color> Tmp::c1 = std::make_shared<Color>();
const std::shared_ptr<Color> Tmp::c2 = std::make_shared<Color>();
const std::shared_ptr<Color> Tmp::c3 = std::make_shared<Color>();
const std::shared_ptr<Color> Tmp::c4 = std::make_shared<Color>();

const std::shared_ptr<Point2> Tmp::p1 = std::make_shared<Point2>();
const std::shared_ptr<Point2> Tmp::p2 = std::make_shared<Point2>();
const std::shared_ptr<Point2> Tmp::p3 = std::make_shared<Point2>();

const std::shared_ptr<TextureRegion> Tmp::tr1 = std::make_shared<TextureRegion>();
const std::shared_ptr<TextureRegion> Tmp::tr2 = std::make_shared<TextureRegion>();
const std::shared_ptr<Mat> Tmp::m1 = std::make_shared<Mat>();
const std::shared_ptr<Mat> Tmp::m2 = std::make_shared<Mat>();
const std::shared_ptr<Mat> Tmp::m3 = std::make_shared<Mat>();

const std::shared_ptr<Bezier<Vec2>> Tmp::bz2 = std::make_shared<Bezier<Vec2>>();
const std::shared_ptr<Bezier<Vec3>> Tmp::bz3 = std::make_shared<Bezier<Vec3>>();
