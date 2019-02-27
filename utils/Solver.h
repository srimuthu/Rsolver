#pragma once
namespace Rsolver 
{
namespace Thistlethwaite 
{

	using namespace std;

	typedef vector<int> vi;

	vi ApplyMove(int move, vi state);
	int Inverse(int move);
	vi IdentifyState(vi state);
	string SolveCubeFromGivenState(vector<string> scrambledState, vector<string> solutionState);
}

}