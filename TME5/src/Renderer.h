#pragma once

#include "Scene.h"
#include "Image.h"
#include "Ray.h"
#include <vector>
#include <thread>
namespace pr {

// Classe pour rendre une scène dans une image
class Renderer {
public:


    // Rend la scène dans l'image
    void render(const Scene& scene, Image& img) {
        // les points de l'ecran, en coordonnées 3D, au sein de la Scene.
        // on tire un rayon de l'observateur vers chacun de ces points
        const Scene::screen_t& screen = scene.getScreenPoints();
        
        // pour chaque pixel, calculer sa couleur
        for (int x = 0; x < scene.getWidth(); x++) {
            for (int y = 0; y < scene.getHeight(); y++) {
                // le point de l'ecran par lequel passe ce rayon
                auto& screenPoint = screen[y][x];
                // le rayon a inspecter
                Ray ray(scene.getCameraPos(), screenPoint);

                int targetSphere = scene.findClosestInter(ray);

                if (targetSphere == -1) {
                    // keep background color
                    continue;
                } else {
                    const Sphere& obj = scene.getObject(targetSphere);
                    // pixel prend la couleur de l'objet
                    Color finalcolor = scene.computeColor(obj, ray);
                    // mettre a jour la couleur du pixel dans l'image finale.
                    img.pixel(x, y) = finalcolor;
                }
            }
        }
    }

    void renderThreadPerPixel(const Scene& scene, Image& img) {
        // les points de l'ecran, en coordonnées 3D, au sein de la Scene.
        // on tire un rayon de l'observateur vers chacun de ces points
        const Scene::screen_t& screen = scene.getScreenPoints();
        
        std::vector <std::thread> threads;

        
        // pour chaque pixel, calculer sa couleur
        for (int x = 0; x < scene.getWidth(); x++) {
            for (int y = 0; y < scene.getHeight(); y++) {
                threads.emplace_back([&, x, y](){

                    // le point de l'ecran par lequel passe ce rayon
                    auto& screenPoint = screen[y][x];
                    // le rayon a inspecter
                    Ray ray(scene.getCameraPos(), screenPoint);

                    int targetSphere = scene.findClosestInter(ray);

                    if (targetSphere == -1) {
                        // keep background color
                        return;
                    } else {
                        const Sphere& obj = scene.getObject(targetSphere);
                        // pixel prend la couleur de l'objet
                        Color finalcolor = scene.computeColor(obj, ray);
                        // mettre a jour la couleur du pixel dans l'image finale.
                        img.pixel(x, y) = finalcolor;
                        
                    }
                });
            }
        }
        for ( auto &t : threads) {t.join();}
    }    
    void renderThreadPerRow(const Scene& scene, Image& img) {
        // les points de l'ecran, en coordonnées 3D, au sein de la Scene.
        // on tire un rayon de l'observateur vers chacun de ces points
        const Scene::screen_t& screen = scene.getScreenPoints();
        
        std::vector <std::thread> threads;

        for (int x = 0; x < scene.getWidth(); x++) {
    
        // pour chaque pixel, calculer sa couleur
        
                threads.emplace_back([&, x](){
                        for (int y = 0; y < scene.getHeight(); y++) {                
                        // le point de l'ecran par lequel passe ce rayon
                        auto& screenPoint = screen[y][x];
                        // le rayon a inspecter
                        Ray ray(scene.getCameraPos(), screenPoint);

                        int targetSphere = scene.findClosestInter(ray);

                        if (targetSphere == -1) {
                            // keep background color
                            continue;
                        } else {
                            const Sphere& obj = scene.getObject(targetSphere);
                            // pixel prend la couleur de l'objet
                            Color finalcolor = scene.computeColor(obj, ray);
                            // mettre a jour la couleur du pixel dans l'image finale.
                            img.pixel(x, y) = finalcolor;
                            
                        }
                    }
                    });
            
        }
        for ( auto &t : threads) {t.join();}
    } 

     void renderThreadManual(const Scene& scene, Image& img, int nbthread) {
        // les points de l'ecran, en coordonnées 3D, au sein de la Scene.
        // on tire un rayon de l'observateur vers chacun de ces points
        const Scene::screen_t& screen = scene.getScreenPoints();
        
        std::vector <std::thread> threads;

        threads.reserve(nbthread);

        const int base = scene.getHeight() / nbthread;           
        int reste = scene.getHeight() % nbthread;          

        int yStart = 0;
        for (int t = 0; t < nbthread; ++t) {

            int bloc = base + (reste > 0 ? 1 : 0); 
            if (reste > 0) --reste;

            int yEnd = yStart + bloc;              
            threads.emplace_back([&, yStart, yEnd]() {
                for (int y = yStart; y < yEnd; ++y) {
                    for (int x = 0; x < scene.getWidth(); ++x) {
                        // le point de l'ecran par lequel passe ce rayon
                        auto& screenPoint = screen[y][x];
                        // le rayon a inspecter
                        Ray ray(scene.getCameraPos(), screenPoint);

                        int targetSphere = scene.findClosestInter(ray);

                        if (targetSphere == -1) {
                            // keep background color
                            continue;
                        } else {
                            const Sphere& obj = scene.getObject(targetSphere);
                            // pixel prend la couleur de l'objet
                            Color finalcolor = scene.computeColor(obj, ray);
                            // mettre a jour la couleur du pixel dans l'image finale.
                            img.pixel(x, y) = finalcolor;
                            
                    }
            });
            yStart = yEnd;
        };

        
    }
        for ( auto &t : threads) {t.join();}
    } 
};

} // namespace pr