//
//  texture.h
//  RayTracing
//
//  Created by moranzcw on 2018/12/30.
//  Copyright © 2018 moranzcw. All rights reserved.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include "perlin.h"
#include "vec3.h"

class Texture  {
public:
    virtual Vec3 value(float u, float v, const Vec3& p) const = 0;
};

// 常量纹理
class ConstantTexture : public Texture {
public:
    ConstantTexture() { }
    ConstantTexture(Vec3 c) : color(c) { }
    virtual Vec3 value(float u, float v, const Vec3& p) const {
        return color;
    }
    Vec3 color;
};

// 测试纹理
class CheckerTexture : public Texture {
public:
    CheckerTexture() { }
    CheckerTexture(Texture *t0, Texture *t1): even(t0), odd(t1) { }
    virtual Vec3 value(float u, float v, const Vec3& p) const {
        float s1, s2, s3;
        s1 = sin(10*p.x());
        s2 = sin(10*p.y());
        s3 = sin(10*p.z());
        if(s1 == 0) s1 = 1;
        if(s2 == 0) s2 = 1;
        if(s3 == 0) s3 = 1;
        if (s1*s2*s3 < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }
    Texture *odd;
    Texture *even;
};


class NoiseTexture : public Texture {
    public:
        NoiseTexture() {}
        NoiseTexture(float sc) : scale(sc) {}
        virtual Vec3 value(float u, float v, const Vec3& p) const {
            // return Vec3(1,1,1)*0.5*(1 + noise.turb(scale * p));
            // return Vec3(1,1,1)*noise.turb(scale * p);
            return Vec3(1,1,1)*0.5*(1 + sin(scale*p.x() + 5*noise.turb(scale*p))) ;
        }
        Perlin noise;
        float scale;
};

class ImageTexture : public Texture {
public:
    ImageTexture() {}
    ImageTexture(unsigned char *pixels, int A, int B) : data(pixels), nx(A), ny(B) {}
    virtual Vec3 value(float u, float v, const Vec3& p) const;

    unsigned char *data;
    int nx, ny;
};

Vec3 ImageTexture::value(float u, float v, const Vec3& p) const {
    int i = (  u)*nx;
    int j = (1-v)*ny-0.001;
    if (i < 0) i = 0;
    if (j < 0) j = 0;
    if (i > nx-1) i = nx-1;
    if (j > ny-1) j = ny-1;
    float r = int(data[3*i + 3*nx*j]  ) / 255.0;
    float g = int(data[3*i + 3*nx*j+1]) / 255.0;
    float b = int(data[3*i + 3*nx*j+2]) / 255.0;
    return Vec3(r, g, b);
}

#endif /* TEXTURE_H */
