#pragma once

#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(Location, Color) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, Color, true);
#define DRAW_SPHERE_FIVE_SECONDS(Location, Color) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 10.f, 12, Color, false, 5.f);
#define DRAW_SPHERE_SingleFrame(Location, Color) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, Color, false);
#define DRAW_LINE(StartLocation, EndLocation, Color) if(GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, Color, true, -1.f, 0, 1.f);
#define DRAW_LINE_SingleFrame(StartLocation, EndLocation, Color) if(GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, Color, false, -1.f, 0, 1.f);
#define DRAW_POINT(Position, Color) if(GetWorld()) DrawDebugPoint(GetWorld(), Position, 15.f, Color, true);
#define DRAW_POINT_SingleFrame(Position, Color) if(GetWorld()) DrawDebugPoint(GetWorld(), Position, 15.f, Color, false);
#define DRAW_VECTOR(StartLocation, EndLocation, LineColor, PointColor) if(GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, LineColor, true, -1.f, 0, 1.f); \
		DrawDebugPoint(GetWorld(), EndLocation, 15.f, PointColor, true); \
	}
#define DRAW_VECTOR_SingleFrame(StartLocation, EndLocation, LineColor, PointColor) if(GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, LineColor, false, -1.f, 0, 1.f); \
		DrawDebugPoint(GetWorld(), EndLocation, 15.f, PointColor, false); \
	}