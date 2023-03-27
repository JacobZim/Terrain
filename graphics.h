#pragma once

void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3);
void DrawMap();
double getTerrainHeight(double x, double y);
extern double gWaterHeight;
extern double gFloatHeight;
enum viewtype { top_view, perspective_view, rat_view };

extern viewtype current_view ;


