//
//  scenes.h
//  RayTracing
//
//  Created by moranzcw on 2018/12/28.
//  Copyright © 2018 moranzcw. All rights reserved.
//

#ifndef SCENES_H
#define SCENES_H

#include <vector>
#include "sphere.h"
#include "cube.h"
#include "material.h"
#include "bvh.h"
#include "scene.h"
#include "skybox.h"
#include "resource.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

// 生成场景
Scene scene1() {
    std::vector<Object*> l;
    // 地面
    Texture *checker = new CheckerTexture(new ConstantTexture(Vec3(0.2,0.3, 0.1)), new ConstantTexture(Vec3(0.9, 0.9, 0.9)));
    l.push_back(new Square(Vec3(100,0,100), Vec3(-100,0,100), Vec3(-100,0,-100), Vec3(100,0,-100), new Lambertian(checker)));

    // 在-11<x<11，-11<z<11的区域内生成n个小球
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = drand48();
            // z坐标和x坐标加一些随机偏移，y坐标为0.2，即球心离地0.2，刚好等于半径
            Vec3 center(a+0.9*drand48(),0.2,b+0.9*drand48());
            // 避开大球坐标
            if ((center-Vec3(3, 0.2, 0)).length() > 0.9
                && (center-Vec3(0, 0.2, 0)).length() > 0.9
                && (center-Vec3(-3, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.6) {  // 0.6概率的漫反射材质
                    l.push_back(new Sphere(center, 0.2, new Lambertian(new ConstantTexture(Vec3(0.9*drand48(), 0.3, 0.2)))));
                }
                else if (choose_mat < 0.9) { // 0.3概率的金属材质
                    l.push_back(new Sphere(center, 0.2,
                                new Metal(Vec3(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.3*(1 + drand48())),  0.2*drand48())));
                }
                else {  // 0.1概率的玻璃材质
                    l.push_back(new Sphere(center, 0.2, new Dielectric(1.5)));
                }
            }
        }
    }
    
    // 三个大球
    l.push_back(new Sphere(Vec3(0, 1, 0), 1.0, new Metal(Vec3(0.7, 0.6, 0.5), 0.0)));
    l.push_back(new Sphere(Vec3(-3, 1, 0), 1.0, new Lambertian(new ConstantTexture(Vec3(0.4, 0.2, 0.1)))));
    l.push_back(new Sphere(Vec3(3, 1, 0), 1.0, new Dielectric(1.5)));
    
    Camera cam(Vec3(0,1,15), Vec3(0,1,0), Vec3(0,1,0), 30, 16.0/9.0, 0.0, 15);

    int nx, ny, nn; 
    unsigned char *tex_data = stbi_load(SKYBOX_TEXTURE_DAYLIGHT_2, &nx, &ny, &nn, 0);
    // std::cout<<nx<<" "<<ny<<" "<<nn<<" "<<int(tex_data[nx*ny-1])<<std::endl;
    SkyBox *skb = new DaylightSkyBox(new ImageTexture(tex_data, nx, ny));
    return Scene(l, cam, skb);
}

// 生成场景
Scene skybox_scene() {
    // skybox
    std::vector<Object*> l;
    l.push_back(new Sphere(Vec3(0, 1, 0), 1.0, new Dielectric(1.5)));
    l.push_back(new Sphere(Vec3(0, 1, 0), 0.8, new Dielectric(1.5)));

    Camera cam(Vec3(0,1,2.5), Vec3(0,1,0), Vec3(0,1,0), 110, 16.0/9.0, 0.01, 2.5);

    int nx, ny, nn; 
    unsigned char *tex_data = stbi_load(SKYBOX_TEXTURE_DAYLIGHT_2, &nx, &ny, &nn, 0);
    SkyBox *skb = new DaylightSkyBox(new ImageTexture(tex_data, nx, ny));
    return Scene(l, cam, skb);
}

Scene two_Spheres() {
    std::vector<Object*> l;

    Texture *checker = new CheckerTexture(new ConstantTexture(Vec3(0.2,0.3, 0.1)), new ConstantTexture(Vec3(0.9, 0.9, 0.9)));
    l.push_back(new Sphere(Vec3(0,-10, 0), 10, new Lambertian(checker)));
    l.push_back(new Sphere(Vec3(0, 10, 0), 10, new Lambertian(checker)));

    Camera cam(Vec3(0,1,15), Vec3(0,1,0), Vec3(0,1,0), 30, 16.0/9.0, 0.2, 15); 
    SkyBox *skb = new BlueSkyBox();
    return Scene(l, cam, skb);
}

Scene perlin_Spheres() {
    std::vector<Object*> l;

    Texture *pertext = new NoiseTexture(4);
    l.push_back(new Square(Vec3(100,0,100), Vec3(-100,0,100), Vec3(-100,0,-100), Vec3(100,0,-100),new Lambertian( pertext )));
    l.push_back(new Sphere(Vec3(0, 2, 0), 2, new Lambertian( pertext )));
    
    Camera cam(Vec3(0,1,15), Vec3(0,1,0), Vec3(0,1,0), 30, 16.0/9.0, 0.2, 15); 
    SkyBox *skb = new BlueSkyBox();
    return Scene(l, cam, skb);
}

Scene image_tex() {
    std::vector<Object*> l;

    Texture *checker = new CheckerTexture(new ConstantTexture(Vec3(0.2,0.3, 0.1)), new ConstantTexture(Vec3(0.9, 0.9, 0.9)));
    l.push_back(new Square(Vec3(100,0,100), Vec3(-100,0,100), Vec3(-100,0,-100), Vec3(100,0,-100),new Lambertian( checker )));
    
    int nx, ny, nn;
    unsigned char *tex_data = stbi_load(TEXTURE_MARS_MAP, &nx, &ny, &nn, 0);
    Material *mat =  new Lambertian(new ImageTexture(tex_data, nx, ny));
    l.push_back(new Sphere(Vec3(-2.5, 1, 0), 1, mat));

    unsigned char *tex_data2 = stbi_load(TEXTURE_JUPITER_MAP, &nx, &ny, &nn, 0);
    Material *mat2 =  new Lambertian(new ImageTexture(tex_data2, nx, ny));
    l.push_back(new Sphere(Vec3(0, 1, 0), 1, mat2));

    unsigned char *tex_data3 = stbi_load(TEXTURE_MARBLE_MAP, &nx, &ny, &nn, 0);
    Material *mat3 =  new Lambertian(new ImageTexture(tex_data3, nx, ny));
    l.push_back(new Cube(Vec3(2.5, 1, 0), 1.5, mat3));

    Camera cam(Vec3(0,1,15), Vec3(0,1,0), Vec3(0,1,0), 30, 16.0/9.0, 0.2, 15); 
    SkyBox *skb = new BlueSkyBox();
    return Scene(l, cam, skb);
}

Scene simple_light() {
    std::vector<Object*> l;

    Texture *checker = new CheckerTexture(new ConstantTexture(Vec3(0.2,0.3, 0.1)), new ConstantTexture(Vec3(0.9, 0.9, 0.9)));
    l.push_back(new Square(Vec3(100,0,100), Vec3(-100,0,100), Vec3(-100,0,-100), Vec3(100,0,-100),
                new Lambertian( checker )));
    
    Texture *pertext = new NoiseTexture(4);
    l.push_back(new Sphere(Vec3(0, 1, 0), 1, new Lambertian( pertext )));

    l.push_back(new Square(Vec3(2,2,-1), Vec3(2,2,1), Vec3(2,0,1), Vec3(2,0,-1), 
                new DiffuseLight(new ConstantTexture(Vec3(4,4,4)))));
    Camera cam(Vec3(0,1,15), Vec3(0,1,0), Vec3(0,1,0), 30, 16.0/9.0, 0.2, 15); 
    SkyBox *skb = new BlueSkyBox();
    return Scene(l, cam, skb);
}

// 生成场景
Scene dark1() {
    std::vector<Object*> l;

    int nx, ny, nn; 
    unsigned char *ground_tex = stbi_load(TEXTURE_MARBLE_MAP, &nx, &ny, &nn, 0);
    Material *ground =  new Lambertian(new ImageTexture(ground_tex, nx, ny));
    l.push_back(new Square(Vec3(10,0,10), Vec3(-10,0,10), Vec3(-10,0,-10), Vec3(10,0,-10), ground));
    l.push_back(new Square(Vec3(100,0,95), Vec3(0,0,-5), Vec3(0,100,-5), Vec3(100,100,95), 
                new Metal(Vec3(0.7, 0.6, 0.5), 0.0)));
    l.push_back(new Square(Vec3(-100,0,95), Vec3(-100,100,95), Vec3(0,100,-5), Vec3(0,0,-5), 
                new Metal(Vec3(0.7, 0.6, 0.5), 0.0)));

    // 
    Texture *checker = new CheckerTexture(new ConstantTexture(Vec3(0.2,0.3, 0.1)), new ConstantTexture(Vec3(0.9, 0.9, 0.9)));
    l.push_back(new Cube(Vec3(-0.5,0.75,-1.5), 1.5, new Lambertian(checker)));


    l.push_back(new Sphere(Vec3(-1.5,1,1.5), 1, new Metal(Vec3(0.8, 0.6, 0.2), 0.0)));

    unsigned char *tex_data2 = stbi_load(TEXTURE_JUPITER_MAP, &nx, &ny, &nn, 0);
    Material *mat2 =  new Lambertian(new ImageTexture(tex_data2, nx, ny));
    l.push_back(new Sphere(Vec3(2,1,0), 1.0, mat2));
    l.push_back(new Sphere(Vec3(-0.5,2.5,-1.5), 1.0, new Dielectric(1.5)));

    Material *light = new DiffuseLight( new ConstantTexture(Vec3(7, 7, 7)) );
    l.push_back(new Cube(Vec3(0.4,6,1.0), 2.5, light));

    Camera cam(Vec3(4,6,15), Vec3(0,1,0), Vec3(0,1,0), 20, 16.0/9.0, 0.0, 15); 
    SkyBox *skb = new BlackSkyBox();
    return Scene(l, cam, skb);
}


Scene dark2() {
    std::vector<Object*> l;

    Material *light = new DiffuseLight( new ConstantTexture(Vec3(7, 7, 7)) );
    l.push_back(new Square(Vec3(-150,550,412), Vec3(150,550,412), Vec3(150,550,147), Vec3(-150,550,147), light));

    int nx, ny, nn; 
    unsigned char *ground_tex = stbi_load(TEXTURE_MARBLE_MAP, &nx, &ny, &nn, 0);
    Material *ground =  new Lambertian(new ImageTexture(ground_tex, nx, ny));
    int nb = 20;
    for (int i = 0; i < nb; i++) {
        for (int j = 0; j < nb; j++) {
            float w = 100;
            l.push_back(new Cube(Vec3(-1000+i*w+w/2, w*(drand48()+0.01)/2, -1000+j*w+w/2),100, ground));
        }
    }

    unsigned char *tex_data = stbi_load(TEXTURE_MARS_MAP, &nx, &ny, &nn, 0);
    Material *mat =  new Lambertian(new ImageTexture(tex_data, nx, ny));
    l.push_back(new Sphere(Vec3(-50, 400, 70), 70, mat));

    unsigned char *tex_data2 = stbi_load(TEXTURE_JUPITER_MAP, &nx, &ny, &nn, 0);
    Material *mat2 =  new Lambertian(new ImageTexture(tex_data2, nx, ny));
    l.push_back(new Sphere(Vec3(210, 270, 230), 80, mat2));


    Texture *pertext = new NoiseTexture(0.1);
    l.push_back(new Sphere(Vec3(130, 450, 100), 80, new Lambertian(pertext)));
    
    l.push_back(new Sphere(Vec3(50, 195, 220), 100, new Dielectric(1.5)));
    l.push_back(new Sphere(Vec3(-190, 250, 145), 80, new Metal(Vec3(0.8, 0.8, 0.9), 0.1)));

    l.push_back(new Square(Vec3(310,500,0), Vec3(310,500,600), Vec3(310,140,600), Vec3(310,140,0), new Metal(Vec3(0.7, 0.6, 0.5), 0.0)));

    Camera cam(Vec3(-200, 280, 1200), Vec3(170,320,0), Vec3(0,1,0), 33, 16.0/9.0, 0.0, 15); 
    SkyBox *skb = new BlackSkyBox();
    return Scene(l, cam, skb);
}

#endif /* SCENES_H */