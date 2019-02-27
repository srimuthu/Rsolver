#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <regex>
#include "RsolverHelpers.h"

namespace Rsolver {
	namespace Helpers
	{
		Colors GetDefaultColorForCubeFace(CubeFaces cubeFace)
		{
			switch (cubeFace)
			{
			case CubeFaces::Up: return Colors::White;
			case CubeFaces::Front: return Colors::Green;
			case CubeFaces::Right: return Colors::Red;
			case CubeFaces::Down: return Colors::Yellow;
			case CubeFaces::Back: return Colors::Blue;
			case CubeFaces::Left: return Colors::Orange;
			default:
				throw std::runtime_error("Invalid CubeFace!");
			}
		}

		CubeFaces GetDefaultCubeFaceForColor(Colors color)
		{
			switch (color)
			{
			case Colors::White: return CubeFaces::Up;
			case Colors::Green: return CubeFaces::Front;
			case Colors::Red: return CubeFaces::Right;
			case Colors::Yellow: return CubeFaces::Down;
			case Colors::Blue: return CubeFaces::Back;
			case Colors::Orange: return CubeFaces::Left;
			default:
				throw std::runtime_error("Invalid color!");
			}
		}

		std::string GetLetterForCubeFace(CubeFaces cubeFace)
		{
			switch (cubeFace)
			{
			case CubeFaces::Up: return "U";
			case CubeFaces::Front: return "F";
			case CubeFaces::Right: return "R";
			case CubeFaces::Down: return "D";
			case CubeFaces::Back: return "B";
			case CubeFaces::Left: return "L";
			default:
				throw std::runtime_error("Invalid CubeFace!");
			}
		}

		FaceColorVector GetDefaultFaceColorVectorForColor(Colors color)
		{
			FaceColorVector faceColorVector;
			for (auto i = 0; i < g_cubiesPerFace; ++i)
			{
				faceColorVector.emplace_back(color);
			}
			return faceColorVector;
		}

		CubeStateInColors GetDefaultSolvedCubeStateInColors()
		{
			CubeStateInColors defaultStateInColors;

			for (int face = 0; face < g_numFaces; face++)
			{
				CubeFaceInfo cubeFaceInfo;
				auto defaultFaceColor = static_cast<Colors>(face);
				cubeFaceInfo.cubeFace = GetDefaultCubeFaceForColor(defaultFaceColor);
				cubeFaceInfo.faceColorVector = GetDefaultFaceColorVectorForColor(defaultFaceColor);
				defaultStateInColors.emplace_back(cubeFaceInfo);
			}
			return defaultStateInColors;
		}

		CubeState ParseCubeStatesTxtFile(const std::string & filename)
		{
			std::string line;
			std::ifstream cubeStatesFile(filename);
			CubeState cubeState;
			if (cubeStatesFile.is_open())
			{
				while (std::getline(cubeStatesFile, line))
				{
					auto tokens = SplitStringBySpace(line);
					if (tokens[0] == "edge")
					{
						std::string edgeCubeKey = tokens[1];
						EdgeCube edgeCube;
						edgeCube.edgeCubeOrientationId = tokens[2];
						edgeCube.cubie0.xpos = std::stoi(tokens[3]);
						edgeCube.cubie0.ypos = std::stoi(tokens[4]);
						edgeCube.cubie1.xpos = std::stoi(tokens[5]);
						edgeCube.cubie1.ypos = std::stoi(tokens[6]);
						cubeState.edgeMap.insert(std::pair<std::string, EdgeCube>(edgeCubeKey, edgeCube));
					}
					else if (tokens[0] == "corner")
					{
						std::string cornerCubeKey = tokens[1];
						CornerCube cornerCube;
						cornerCube.cornerCubeOrientationId = tokens[2];
						cornerCube.cubie0.xpos = std::stoi(tokens[3]);
						cornerCube.cubie0.ypos = std::stoi(tokens[4]);
						cornerCube.cubie1.xpos = std::stoi(tokens[5]);
						cornerCube.cubie1.ypos = std::stoi(tokens[6]);
						cornerCube.cubie2.xpos = std::stoi(tokens[7]);
						cornerCube.cubie2.ypos = std::stoi(tokens[8]);
						cubeState.cornerMap.insert(std::pair<std::string, CornerCube>(cornerCubeKey, cornerCube));
					}
				}
			}
			else
			{
				throw std::runtime_error("Cannot open CubeStates txt file!");
			}
			return cubeState;
		}

		std::string GetOrientationForKey(const CubeState & cubeState, const std::string& key, CubeType type)
		{
			switch (type)
			{
			case CubeType::Edge: return cubeState.edgeMap.find(key)->second.edgeCubeOrientationId;
			case CubeType::Corner:return cubeState.cornerMap.find(key)->second.cornerCubeOrientationId;
			default: throw std::runtime_error("Invalid cube type!");
			}
		}

		std::string GetKeyForOrientation(const CubeState & cubeState, const std::string& orientation, CubeType type)
		{
			switch (type)
			{
			case CubeType::Edge: 
			{
				auto edgeMap = cubeState.edgeMap;
				for (auto const& entry : edgeMap)
				{
					if (entry.second.edgeCubeOrientationId == orientation)
					{
						return entry.first;
					}
				}
				throw std::runtime_error("orientation not found! "+ orientation);
			}
			case CubeType::Corner: 
			{
				auto cornerMap = cubeState.cornerMap;
				for (auto const& entry : cornerMap)
				{
					if (entry.second.cornerCubeOrientationId == orientation)
					{
						return entry.first;
					}
				}
				throw std::runtime_error("orientation not found! " + orientation);
			}
			default: throw std::runtime_error("Invalid cube type!");
			}
		}

		CubeStateInUFRDBL GetCubeStateInUFRDBL(const CubeStateInColors & cubeStateInColors,
			const CubeStateInUFRDBL & solutionState,
			const CubeState & defaultStateMap)
		{
			CubeStateInUFRDBL cubeStateInUfrdbl = "";
			auto solutionStateTokens = SplitStringBySpace(solutionState);
			for (auto const& token : solutionStateTokens)
			{
				if (token.length() == 2) // edge
				{
					// Get cubie indices from default state map
					auto edgeCube = defaultStateMap.edgeMap.find(token)->second;
					auto actualOrientationId = GetEdgeOrientationFromCubeStateInColors(cubeStateInColors, edgeCube);
					// search for the actual orientation id in default map
					auto actualKey = GetKeyForOrientation(defaultStateMap, actualOrientationId, CubeType::Edge);
					cubeStateInUfrdbl = cubeStateInUfrdbl + actualKey + " ";
				}
				else if (token.length() == 3) // corner
				{
					{
						// Get cubie indices from default state map
						auto cornerCube = defaultStateMap.cornerMap.find(token)->second;
						auto actualOrientationId = GetCornerOrientationFromCubeStateInColors(cubeStateInColors, cornerCube);
						// search for the actual orientation id in default map
						auto actualKey = GetKeyForOrientation(defaultStateMap, actualOrientationId, CubeType::Corner);
						cubeStateInUfrdbl = cubeStateInUfrdbl + actualKey + " ";
					}
				}
			}
			return trim(cubeStateInUfrdbl);
		}

		std::string GetEdgeOrientationFromCubeStateInColors(const CubeStateInColors & cubeStateInColors, const EdgeCube & edgeCube)
		{
			Colors cubie0color = cubeStateInColors[edgeCube.cubie0.xpos].faceColorVector[edgeCube.cubie0.ypos];
			Colors cubie1color = cubeStateInColors[edgeCube.cubie1.xpos].faceColorVector[edgeCube.cubie1.ypos];
			std::string orientation = std::to_string(static_cast<int>(cubie0color)) + std::to_string(static_cast<int>(cubie1color));
			return orientation;
		}

		std::string GetCornerOrientationFromCubeStateInColors(const CubeStateInColors & cubeStateInColors, const CornerCube & cornerCube)
		{
			Colors cubie0color = cubeStateInColors[cornerCube.cubie0.xpos].faceColorVector[cornerCube.cubie0.ypos];
			Colors cubie1color = cubeStateInColors[cornerCube.cubie1.xpos].faceColorVector[cornerCube.cubie1.ypos];
			Colors cubie2color = cubeStateInColors[cornerCube.cubie2.xpos].faceColorVector[cornerCube.cubie2.ypos];
			std::string orientation = std::to_string(static_cast<int>(cubie0color)) + std::to_string(static_cast<int>(cubie1color)) + std::to_string(static_cast<int>(cubie2color));
			return orientation;
		}

		std::vector<std::string> SplitStringBySpace(const std::string &str)
		{
			std::istringstream buf(str);
			std::istream_iterator<std::string> beg(buf), end;
			std::vector<std::string> tokens(beg, end);
			return tokens;
		}

		std::string ltrim(const std::string& s) {
			return std::regex_replace(s, std::regex("^\\s+"), std::string(""));
		}

		std::string rtrim(const std::string& s) {
			return std::regex_replace(s, std::regex("\\s+$"), std::string(""));
		}

		std::string trim(const std::string& s) {
			return ltrim(rtrim(s));
		}

	}
}