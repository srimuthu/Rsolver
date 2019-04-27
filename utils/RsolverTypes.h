#pragma once
#include <vector>
#include <tuple>
#include <map>

namespace Rsolver {

	const int g_numFaces = 6;
	const int g_cubiesPerFace = 9;
	const int g_numCornerCubes = 8;
	const int g_numEdgeCubes = 12;
	const std::string g_cubeStatesTxtFile = "CubeStates.txt";

enum class CubeFaces {
	Up,
	Front,
	Right,
	Down,
	Back,
	Left
};

enum class Colors {
	White,
	Green,
	Red,
	Yellow,
	Blue,
	Orange
};

enum class CubieType {
	Edge,
	Corner
};

typedef std::string CubeStateInUFRDBL;
typedef std::vector<Colors> FaceColorVector;

struct CubeFaceInfo {
	CubeFaceInfo()
	{}
	CubeFaceInfo(CubeFaces face)
		:cubeFace(face)
	{}

	bool operator==(const CubeFaceInfo& right)const
	{
		return ((cubeFace == right.cubeFace) &&
			(faceColorVector == right.faceColorVector));
	}

	CubeFaces cubeFace;
	FaceColorVector faceColorVector;
};

typedef std::vector<CubeFaceInfo> CubeStateInColors;

struct CubieInfo {
	int xpos;
	int ypos;
};

struct EdgeCube {
	std::string edgeCubeOrientationId;
	CubieInfo cubie0;
	CubieInfo cubie1;
};

struct CornerCube{
	std::string cornerCubeOrientationId;
	CubieInfo cubie0;
	CubieInfo cubie1;
	CubieInfo cubie2;
};

struct CubeState
{
	std::map<std::string, EdgeCube> edgeMap;
	std::map<std::string, CornerCube> cornerMap;
};

struct LabVal
{
	LabVal()
	:lVal(0)
	, aVal(0)
	, bVal(0)
	{}
	double lVal;
	double aVal;
	double bVal;
};

struct ColorBoundaries
{
	ColorBoundaries()
		:color(Colors::White)
	{}
	ColorBoundaries(Colors color)
		:color(color)
	{}
	Colors color;
	LabVal labVal;

};

enum class Motors {
	BottomGrip,
	RightGrip,
	LeftGrip,
	TopGrip,
	BottomSlide,
	RightSlide,
	LeftSlide,
	TopSlide
};

enum class ControlCommands {
	NoPending,
	BlinkLed,
	SetMotors
};

struct RobotCommand {
	unsigned char command;
	unsigned char arg1;
	unsigned char arg2;
};

}


