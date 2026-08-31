#include "BasicShapes.h"

#include <vector>

#include "Mesh.h"

BasicShapes::BasicShapes()
    : icoSphere("Assets/Models/icoSphere.obj"),
      plane([]()
            {
          std::vector<Mesh> meshes;
          meshes.emplace_back(
              std::vector<Vertex>{
                  { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
                  { { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },
                  { {  1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },
                  { {  1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } }
              },
              std::vector<GLuint>{ 0, 3, 1, 1, 3, 2 },
              NO_MATERIAL);
          return meshes; }())
{
}
