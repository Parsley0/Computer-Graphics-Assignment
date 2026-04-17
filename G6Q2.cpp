// GROUP 6

// Members
// Parsley Njoroge - Sct211 - 0007 / 2023
// Patrick Leon – Sct211 - 0003 / 2023

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

// 2D Cross Product
float cross2D(glm::vec2 a, glm::vec2 b) {
    return (a.x * b.y) - (a.y * b.x);
}

// Find where a ray hits a line segment
// Return true if there is a hit, and output the exact hitPoint
bool getIntersection(glm::vec2 rayOrigin, glm::vec2 rayDir, glm::vec2 edgeStart, glm::vec2 edgeEnd, glm::vec2& hitPoint) {
    glm::vec2 edgeDir = edgeEnd - edgeStart;

    float crossDir = cross2D(rayDir, edgeDir);

    if (std::abs(crossDir) < 0.0001f) {
        return false;
    }

    glm::vec2 originDiff = edgeStart - rayOrigin;

    
    float t = cross2D(originDiff, edgeDir) / crossDir;
    float u = cross2D(originDiff, rayDir) / crossDir;


    if (t > 0.0001f && u >= 0.0f && u <= 1.0f) {
        hitPoint = rayOrigin + (rayDir * t);
        return true;
    }
    return false;
}

// The Splitting Function
void splitPolygon(const std::vector<glm::vec2>& poly) {
    int n = poly.size();
    if (n < 3) return;

    for (int k = 0; k < n; ++k) {
        int index_k = k;
        int index_k1 = (k + 1) % n;
        int index_k2 = (k + 2) % n;

        glm::vec2 Vk = poly[index_k];
        glm::vec2 Vk1 = poly[index_k1]; 
        glm::vec2 Vk2 = poly[index_k2];

        glm::vec2 edge1 = Vk1 - Vk;
        glm::vec2 edge2 = Vk2 - Vk1;

        
        if (cross2D(edge1, edge2) < -0.0001f) {
            std::cout << "Concave Found at index " << index_k1 << " \n";

            glm::vec2 rayOrigin = Vk1;
            glm::vec2 rayDir = Vk1 - Vk; 

            glm::vec2 hitPoint;
            int intersectedEdgeIndex = -1;

            
            for (int i = 0; i < n; ++i) {
                
                if (i == index_k || i == index_k1) continue;

                glm::vec2 edgeStart = poly[i];
                glm::vec2 edgeEnd = poly[(i + 1) % n];

                if (getIntersection(rayOrigin, rayDir, edgeStart, edgeEnd, hitPoint)) {
                    intersectedEdgeIndex = i;
                    break;
                }
            }

            if (intersectedEdgeIndex != -1) {
                std::cout << "Ray hit edge starting at index " << intersectedEdgeIndex << " at point (" << hitPoint.x << ", " << hitPoint.y << ")\n";

                
                std::vector<glm::vec2> polyA;
                polyA.push_back(Vk1);
                for (int i = index_k2; i != (intersectedEdgeIndex + 1) % n; i = (i + 1) % n) {
                    polyA.push_back(poly[i]);
                }
                polyA.push_back(hitPoint);
                
                std::vector<glm::vec2> polyB;
                polyB.push_back(hitPoint);
                for (int i = (intersectedEdgeIndex + 1) % n; i != index_k2; i = (i + 1) % n) {
                    polyB.push_back(poly[i]);
                }
                
                std::cout << "\nPolygon A Vertices:\n";
                for (auto p : polyA) std::cout << "  (" << p.x << ", " << p.y << ")\n";

                std::cout << "\nPolygon B Vertices:\n";
                for (auto p : polyB) std::cout << "  (" << p.x << ", " << p.y << ")\n";

                return; 
            }
        }
    }
    std::cout << "Polygon is fully convex!\n";
}

int main() {
    
    std::vector<glm::vec2> starPerimeter = {
         glm::vec2(0.0f,  0.5f),  
         glm::vec2(-0.15f, 0.15f), 
         glm::vec2(-0.5f,  0.0f),  
         glm::vec2(-0.15f,-0.15f), 
         glm::vec2(0.0f, -0.5f),  
         glm::vec2(0.15f,-0.15f), 
         glm::vec2(0.5f,  0.0f),  
         glm::vec2(0.15f, 0.15f)  
    };

    std::cout << "Running Rotational Split on 4-Pointed Star...\n";
    splitPolygon(starPerimeter); 

    return 0;
}