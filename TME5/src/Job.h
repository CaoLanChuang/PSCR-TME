#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include "Color.h"
#include "Sphere.h"
#include "Rayon.h"
#include "Scene.h"

using namespace std::chrono_literals;
//这个代码段是为一个光线追踪渲染器设计的，光线追踪是一种生成高度逼真图像的技术，通过模拟光线与物体的相互作用来计算像素颜色。
//通过多线程处理，这个程序能够更快地渲染复杂场景，特别是在有大量像素需要计算时。

namespace pr {
	//用于找到与光线相交的最近物体（球体）。
    extern int findClosestInter(const Scene & scene, const Rayon & ray);
	//计算光线与物体相交处的颜色。
    extern Color computeColor(const Sphere & obj, const Rayon & ray, const Vec3D & camera, std::vector<Vec3D> & lights);


    class Job {
    public:
        virtual void run () = 0;//虚构的运行函数
        virtual ~Job() {};
    };

// Concrete Job: example
//    class SleepJob : public Job {
//        int calcul (int v) {
//            std::cout << "Computing for arg =" << v << std::endl;
//            // Processing a heavy computation
//            std::this_thread::sleep_for(1s);
//            int ret = v % 255;
//            std::cout << "Obtained for arg =" << arg <<  " result " << ret << std::endl;
//            return ret;
//        }
//        int arg;
//        int * ret;
//    public :
//        SleepJob(int arg, int * ret) : arg(arg), ret(ret) {}
//        void run () {
//            *ret = calcul(arg);
//        }
//        ~SleepJob(){}
//    };



        class PixelJob : public Job {//计算场景中单个像素的颜色
        private:
            int x, y;
            Color* pixel;
            Scene* scene;
            std::vector<Vec3D>* lights;

        public:

            PixelJob(int x, int y, Color* pixel, Scene* scene, std::vector<Vec3D>* lights)
                    : x(x), y(y), pixel(pixel), scene(scene), lights(lights) {}

            void run() override {//run函数计算出指定像素的颜色，并将结果存储在pixel中。

                auto &screenPoint = scene->getScreenPoints()[y][x];
                Rayon ray(scene->getCameraPos(), screenPoint);
                int targetSphere = findClosestInter(*scene, ray);

                if (targetSphere != -1) {
                    const Sphere &obj = *((*scene).begin() + targetSphere);
                    *pixel = computeColor(obj, ray, scene->getCameraPos(), *lights);
                }
            }
        };

    class RowJob : public Job {//用于渲染场景中的一行像素。
    private:
        int y;
        Color* rowPixels;
        Scene* scene;
        std::vector<Vec3D>* lights;

    public:
        RowJob(int y, Color* rowPixels, Scene* scene, std::vector<Vec3D>* lights)
                : y(y), rowPixels(rowPixels), scene(scene), lights(lights) {}

        void run() override {//函数遍历一行中的所有像素，为每个像素调用类似于PixelJob的逻辑。
            for (int x = 0; x < scene->getWidth(); x++) {
                Color* pixel = &rowPixels[x];
                auto &screenPoint = scene->getScreenPoints()[y][x];
                Rayon ray(scene->getCameraPos(), screenPoint);
                int targetSphere = findClosestInter(*scene, ray);

                if (targetSphere != -1) {
                    const Sphere &obj = *((*scene).begin() + targetSphere);
                    *pixel = computeColor(obj, ray, scene->getCameraPos(), *lights);
                }
            }
        }
    };
    }