#pragma once

/**
 * A renderable class
 */
class Renderable {
    public:
        virtual void render() = 0;
        virtual void renderShadow() = 0;
        virtual ~Renderable() {};
};
