#pragma once
namespace Rsolver 
{
namespace Thistlethwaite 
{

typedef std::vector<int> vi;

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
	int phase;

};

}

}