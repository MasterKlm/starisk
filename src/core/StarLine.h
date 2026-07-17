#ifndef STARLINE_H
#define STARLINE_H
#include "StarVec2D.h"
#include "StarVertex.h"
#include <vector>
#include "StarBatchManager.h"
#include "StarUtils.h"



class StarLine{
public:
    StarVec2D p1, p2;
    SBM_DATA sbm_data;


    StarLine(){}

    StarLine(StarVec2D point1, StarVec2D point2) : p1(point1), p2(point2){}


    void updatePoints(StarVec2D point1, StarVec2D point2, StarBatchManager* sbm){
        
        p1 = point1;
        p2 = point2;

        // Convert raw screen coordinates to NDC (-1.0f to 1.0f)
        float nx1 = (p1.x / StariskSettings::WINDOW_WIDTH) * 2.0f - 1.0f;
        float ny1 = 1.0f - (p1.y / StariskSettings::WINDOW_HEIGHT) * 2.0f;
        float nx2 = (p2.x / StariskSettings::WINDOW_WIDTH) * 2.0f - 1.0f;
        float ny2 = 1.0f - (p2.y / StariskSettings::WINDOW_HEIGHT) * 2.0f;

        std::vector<LineVertex> verts = { {nx1, ny1}, {nx2, ny2} };
        
        sbm->move(verts, sbm_data);
        
    }
private:
};



#endif