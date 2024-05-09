#include "Gizmos.hpp"

void Gizmos::draw_area(vec2f pos, float radius, Atlas* atlas, vec3f color)
{
#if F_ENABLE_DEBUG
    for(int i = 0 ; i < radius ; ++i){
        if(i == 0){
            for(int j = 0 ; j < radius ; ++j){
                atlas->draw_pixel(pos.x + j, pos.y+i, color.x, color.y, color.z, 255);

                if(j == radius - 1){
                    for(int k = 0 ; k < radius ; ++k){
                        atlas->draw_pixel(pos.x + radius, pos.y+k, color.x, color.y, color.z, 255);
                    }
                }
            }
        }

        if(i == radius - 1){
            for(int j = 0 ; j < radius ; ++j){
                atlas->draw_pixel(pos.x + j, pos.y+i, color.x, color.y, color.z, 255);
            }
        }

        atlas->draw_pixel(pos.x, pos.y+i, color.x, color.y, color.z, 255);
    }
#endif
}