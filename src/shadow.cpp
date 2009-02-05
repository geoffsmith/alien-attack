#include "shadow.h"
#include <OpenGL/gl.h>

void renderShadow(Renderable *renderable) {
    // We don't need lighting for this part
    glDisable(GL_LIGHTING);

    // Write-only depth and color masks so our rendering doesn't change the 
    // write and depth buffers
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    // Enable face culling
    glEnable(GL_CULL_FACE);

    // and of course enable the stencil test
    glEnable(GL_STENCIL_TEST);

    // This is to fight some depth buffer issues i.e. z-fighting
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(0, 100);

    // First we do a pass culling the front face
    glCullFace(GL_BACK);

    // Set up the stencil
    glStencilFunc(GL_ALWAYS, 0x0, 0xff);
    glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);

    // Render the shadow volume onto the stencil buffer
    renderable->renderShadow();

    // Now we do the second pass for the back face
    glCullFace(GL_FRONT);
    glStencilFunc(GL_ALWAYS, 0x0, 0xff);
    glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
    renderable->renderShadow();

    // Re-enable the color and depth buffers etc
    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_CULL_FACE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);

    // Update the stencil to allow drawing
    glStencilFunc(GL_NOTEQUAL, 0x0, 0xff);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

    drawShadow();

    glDisable(GL_STENCIL_TEST);
    glEnable(GL_LIGHTING);
}

void drawShadow() {
    // All we're doing here is drawing a semi-transparent polygon over the pixels
    // to simulate a shadow.
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 1, 1, 0, 0, 1);
    glDisable(GL_DEPTH_TEST);
    glColor4f(1, 0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(0, 1);
    glVertex2f(1, 1);
    glVertex2f(1, 0);
    glEnd();

    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
