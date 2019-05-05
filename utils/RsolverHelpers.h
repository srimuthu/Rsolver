#pragma once
#include "RsolverTypes.h"
#include <string>
#include <map>

namespace Rsolver {
	namespace Helpers
	{
		Colors GetDefaultColorForCubeFace(CubeFaces cubeFace);
		CubeFaces GetDefaultCubeFaceForColor(Colors color);

		std::string GetLetterForCubeFace(CubeFaces cubeFace);
		CubeFaces GetCubeFaceForLetter(const char& letter);
		std::string ColorToString(Colors color);

		void PrintCubeStateInColorsToConsole(CubeStateInColors cubeStateInColors);

		FaceColorVector GetDefaultFaceColorVectorForColor(Colors color);
		CubeStateInColors GetDefaultSolvedCubeStateInColors();

		CubeState ParseCubeStatesTxtFile(const std::string& filename);
		std::string GetOrientationForKey(const CubeState& cubeState, const std::string& key, CubieType type);
		std::string GetKeyForOrientation(const CubeState& cubeState, const std::string& orientation, CubieType type);

		bool IsTokenAnEdge(const std::string& token);

		std::string GetEdgeOrientationFromCubeStateInColors(const CubeStateInColors& cubeStateInColors, const EdgeCube& edgeCube);
		std::string GetCornerOrientationFromCubeStateInColors(const CubeStateInColors& cubeStateInColors, const CornerCube& cornerCube);

		std::vector<std::string> SplitStringBySpace(const std::string& str);

		std::string ltrim(const std::string& s);
		std::string rtrim(const std::string& s);
		std::string trim(const std::string& s);

		// Robot helpers
		int ConvertControlCommand(ControlCommands command);
		Motors GetSliderForCubeFace(CubeFaces face);
		Motors GetGripperForCubeFace(CubeFaces face);

	}
}

