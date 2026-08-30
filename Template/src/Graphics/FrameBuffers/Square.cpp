#include "Square.h"

#include "Globals.h"

// Square::Square()
//     : squareVAO(),
//       squareVBO(square, sizeof(square), GL_STATIC_DRAW),
//       squareEBO(squareIndices, sizeof(squareIndices))
// {
//     squareVAO.Bind();
//     squareVBO.Bind();
//     squareVAO.LinkAttrib(squareVBO, 0, 3, GL_FLOAT, 5 * sizeof(GLfloat), (void*)0);
//     squareVAO.LinkAttrib(squareVBO, 1, 2, GL_FLOAT, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
// }

// void Square::BindSquare()
// {
//     squareVAO.Bind();
//     squareVBO.Bind();
//     squareEBO.Bind();
// }
// void Square::UnbindSquare()
// {
//     squareVAO.Unbind();
//     squareVBO.Unbind();
//     squareEBO.Unbind();
// }
// void Square::DrawSquare()
// {
//     BindSquare();
//     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//     UnbindSquare();
// }