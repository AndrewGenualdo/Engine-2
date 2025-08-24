//
// Created by cobble on 1/3/2025.
//

#ifndef ENGINE_2_GAMECAMERA_H
#define ENGINE_2_GAMECAMERA_H

namespace cobb {
    class GameCamera {

    public:
        float x, y, w, h; //based on bottom left
        float minX, minY, maxX, maxY;

        GameCamera() {
            x = -2;
            y = -2;
            w = -2;
            h = -2;
            minX = 0;
            minY = 0;
            maxX = Window::GAME_WIDTH;
            maxY = Window::GAME_HEIGHT;
        }

        void copy(float *xPos, float *yPos, float *width, float *height) const {
            *xPos = x == -2 ? maxX / 2 : x;
            *yPos = y == -2 ? maxY / 2 : y;
            *width = w;
            *height = h;
        }

        void set(float xPos, float yPos, float width, float height) {
            x = xPos;
            y = yPos;
            w = width;
            h = height;
        }

        void reset() {
            set(-1, -1, -1, -1);
        }

        void expandToInclude(float newX, float newY) {
            float nx = newX < minX ? minX : (newX + w > maxX ? maxX - newX : newX);
            float ny = newY < minY ? minY : (newY + h > maxY ? maxY - newY : newY);
            if(x < 0 && y < 0 && w < 0 && h < 0) { //first point
                x = nx;
                y = ny;
                w = 0;
                h = 0;
                return;
            }
            if(x > nx) {
                w += x - nx;
                x = nx;
            } else if(nx > x + w) {
                w += nx - (x + w);
            }
            if(y > ny) {
                h += y - ny;
                y = ny;
            } else if(ny > y + h) {
                h += ny - (y + h);
            }
        }

        void setMinWidth(float minWidth) {
            if(w < minWidth) {
                float extraWidth = minWidth - w;
                x -= extraWidth / 2;
                w += extraWidth;
            }
            if(x < minX) x = minX;
            if(x + w > maxX) w = maxX - x;
            if(y < minY) y = minY;
            if(y + h > maxY) h = maxY - y;
        }

        void adjustToAspectRatio(float goalAspectRatio) {
            float aspectRatio = w / h;
            if(aspectRatio < goalAspectRatio) { //taller than goal aspect ratio
                //keep height the same
                float newWidth = h * goalAspectRatio;
                float extraWidth = newWidth - w;
                x -= extraWidth / 2;
                w += extraWidth;
            } else if(aspectRatio > goalAspectRatio) { //wider than goal aspect ratio
                //keep width the same
                float newHeight = w / goalAspectRatio;
                float extraHeight = newHeight - h;
                y -= extraHeight / 2;
                h += extraHeight;
            }
        }
    };
}


#endif //ENGINE_2_GAMECAMERA_H
