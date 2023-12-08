#ifndef SCENE_H_
#define SCENE_H_

#include "Vec3D.h"
#include "Sphere.h"
#include <vector>

namespace pr {

// une scene basique, la camera est fixe
// porte un ensemble d'objets, des spheres a ce stade
class Scene {
	// les objets
	std::vector<Sphere> objects;//存储场景中所有球体对象的容器
	// la camera 
	Vec3D camposition;//存储摄像机的位置
	// son angle de vue, vecteur unitaire dans la direction de la vue
	// suppose fixe actuellement
	// Vec3D viewpos;

	// largeur de l'ecran en pixels : controle la resolution
	// pas la largeur du champ
	int width;
	int height;//屏幕的宽度和高度，以像素为单位。
public :
	// les points d'un ecran 3D 
	typedef std::vector<std::vector<Vec3D> > screen_t;//一个二维向量，表示屏幕上的点，用于渲染。
private :
	// l'ecran, calcule a partir de la position et l'angle
	std::vector<std::vector<Vec3D> > screen;
public:
	// une scene, on donne seulement la resolution
	Scene(int width=800, int height=600)//构造函数，初始化场景的分辨率和摄像机位置。
		// on positionne en dur les positions et champ de vue
		:camposition(width/2,height/2,-1000),width(width),height(height),screen(height, std::vector<Vec3D> (width)) {
		double screendepth = 20.0;
		double W = 400;
		double H = 400;
		auto x0 = -W /2;
		auto y0 = -H /2;
		// un peu violent, suppose que la camera est orientee vers axe Z
		for (int x= 0 ;  x < width ; x++) {
			for (int y=0 ;  y < height ; y++) {
				screen[y][x].x = x0 + x * W / width;
				screen[y][x].y = y0 + y * H / height;
				screen[y][x].z = screendepth;
			}
		}
	}
	// ajoute un objet a la scene
	void add (const Sphere & s) {//向场景中添加一个球体对象。
		objects.push_back(s);
	}
	typedef std::vector<Sphere>::const_iterator const_iterator;//提供迭代器，用于遍历场景中的所有球体对象。
	typedef std::vector<Sphere>::iterator iterator;
	// API (const) iterable pour la scene, par delegation
	iterator begin() { return objects.begin() ; }
	iterator end() { return objects.end() ; }
	const_iterator begin() const { return objects.begin() ; }
	const_iterator end() const { return objects.end() ; }

	// les points de l'ecran par lesquels pqssent les rayons
	const screen_t & getScreenPoints() const { return screen; }//获取屏幕点，这些点是光线穿过的位置。
	// resolution en pixels
	int getHeight() const { return height ;}//返回屏幕的高度和宽度（分辨率）。
	int getWidth() const { return width ;}
	// la camera (const)
	const Vec3D & getCameraPos() const { return camposition ; }//获取摄像机的位置。
};

} /* namespace pr */

#endif /* SCENE_H_ */
