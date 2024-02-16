#pragma once
#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 50.0f, 12, FColor::Red, true, -1.0f, 0, 1);
#define DRAW_SPHERE_single_frame(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 50.0f, 12, FColor::Red, false, -1.0f, 0, 1);
#define DRAW_LINE(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.0f, 0, 1);
#define DRAW_LINE_single_frame(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.0f, 0, 1);
#define DRAW_POINT(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.0f, FColor::Red, true, -1.0f, 0);
#define DRAW_POINT_single_frame(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.0f, FColor::Red, false, -1.0f, 0);
#define DRAW_VECTOR(StartLocation, EndLocation) if (GetWorld())\
	{\
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.0f, 0, 1.0f );\
		DrawDebugPoint(GetWorld(), EndLocation, 15.0f, FColor::Red, true, -1.0f, 0);\
	}
#define DRAW_VECTOR_single_frame(StartLocation, EndLocation) if (GetWorld())\
	{\
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.0f, 0, 1.0f );\
		DrawDebugPoint(GetWorld(), EndLocation, 15.0f, FColor::Red, false, -1.0f, 0);\
	}