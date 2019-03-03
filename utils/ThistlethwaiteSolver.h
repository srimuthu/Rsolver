#pragma once
namespace Rsolver 
{
namespace Thistlethwaite 
{

typedef std::vector<int> vi;

const int applicableMoves[] = { 0, 262143, 259263, 74943, 74898 };

const int affectedCubies[][8] = {
	{  0,  1,  2,  3,  0,  1,  2,  3 },   // U
	{  4,  7,  6,  5,  4,  5,  6,  7 },   // D
	{  0,  9,  4,  8,  0,  3,  5,  4 },   // F
	{  2, 10,  6, 11,  2,  1,  7,  6 },   // B
	{  3, 11,  7,  9,  3,  2,  6,  5 },   // L
	{  1,  8,  5, 10,  1,  0,  4,  7 },   // R
};

class ThistlethwaiteSolver
{
public:
	ThistlethwaiteSolver();
	~ThistlethwaiteSolver();
	vi ApplyMove(int move, vi state);
	int Inverse(int move);
	vi IdentifyState(vi state);
	std::string SolveCubeFromGivenState(std::vector<std::string> scrambledState, std::vector<std::string> solutionState);
private:
	int m_phase;

};

}

}