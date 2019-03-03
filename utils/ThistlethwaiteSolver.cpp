#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <sstream>

#include "ThistlethwaiteSolver.h"

/**********************************************************************
 * 
 * A cube 'state' is a vector<int> with 40 entries, the first 20
 * are a permutation of {0,...,19} and describe which cubie is at
 * a certain position (regarding the input ordering). The first
 * twelve are for edges, the last eight for corners.
 * 
 * The last 20 entries are for the orientations, each describing
 * how often the cubie at a certain position has been turned
 * counterclockwise away from the correct orientation. Again the
 * first twelve are edges, the last eight are corners. The values
 * are 0 or 1 for edges and 0, 1 or 2 for corners.
 * 
 **********************************************************************/
namespace Rsolver
{
namespace Thistlethwaite
{

ThistlethwaiteSolver::ThistlethwaiteSolver()
	:m_phase(0)
{}

ThistlethwaiteSolver::~ThistlethwaiteSolver()
{
	m_phase = 0;
}

vi ThistlethwaiteSolver::ApplyMove(int move, vi state)
{
	int turns = move % 3 + 1;
	int face = move / 3;
	while (turns--) {
		vi oldState = state;
		for (int i = 0; i < 8; i++) {
			int isCorner = i > 3;
			int target = affectedCubies[face][i] + isCorner * 12;
			int killer = affectedCubies[face][(i & 3) == 3 ? i - 3 : i + 1] + isCorner * 12;;
			int orientationDelta = (i < 4) ? (face > 1 && face < 4) : (face < 2) ? 0 : 2 - (i & 1);
			state[target] = oldState[killer];
			state[target + 20] = oldState[killer + 20] + orientationDelta;
			if (!turns)
				state[target + 20] %= 2 + isCorner;
		}
	}
	return state;
}

int ThistlethwaiteSolver::Inverse(int move)
{
	return move + 2 - 2 * (move % 3);
}

vi ThistlethwaiteSolver::IdentifyState(vi state)
{
	//--- Phase 1: Edge orientations.
	if (m_phase < 2)
		return vi(state.begin() + 20, state.begin() + 32);

	//-- Phase 2: Corner orientations, E slice edges.
	if (m_phase < 3) {
		vi result(state.begin() + 31, state.begin() + 40);
		for (int e = 0; e < 12; e++)
			result[0] |= (state[e] / 8) << e;
		return result;
	}

	//--- Phase 3: Edge slices M and S, corner tetrads, overall parity.
	if (m_phase < 4) {
		vi result(3);
		for (int e = 0; e < 12; e++)
			result[0] |= ((state[e] > 7) ? 2 : (state[e] & 1)) << (2 * e);
		for (int c = 0; c < 8; c++)
			result[1] |= ((state[c + 12] - 12) & 5) << (3 * c);
		for (int i = 12; i < 20; i++)
			for (int j = i + 1; j < 20; j++)
				result[2] ^= state[i] > state[j];
		return result;
	}

	//--- Phase 4: The rest.
	return state;
}

std::string ThistlethwaiteSolver::SolveCubeFromGivenState(std::vector<std::string> scrambledState, std::vector<std::string> solutionState)
{
	std::ostringstream os;
	//--- Convert goal state to array for pointer arithmetic
	std::string goal[20];
	for (int i = 0; i < 20; i++)
	{
		goal[i] = solutionState[i];
	}
	//--- Prepare current (start) and goal state.
	vi currentState(40), goalState(40);
	for (int i = 0; i < 20; i++) {

		//--- Goal state.
		goalState[i] = i;

		//--- Current (start) state.
		std::string cubie = scrambledState[i];
		while ((currentState[i] = find(goal, goal + 20, cubie) - goal) == 20) {
			cubie = cubie.substr(1) + cubie[0];
			currentState[i + 20]++;
		}
	}

	//--- Dance the funky Thistlethwaite...
	while (++m_phase < 5) {

		//--- Compute ids for current and goal state, skip phase if equal.
		vi currentId = IdentifyState(currentState), goalId = IdentifyState(goalState);
		if (currentId == goalId)
			continue;

		//--- Initialize the BFS queue.
		std::queue<vi> q;
		q.push(currentState);
		q.push(goalState);

		//--- Initialize the BFS tables.
		std::map<vi, vi> predecessor;
		std::map<vi, int> direction, lastMove;
		direction[currentId] = 1;
		direction[goalId] = 2;

		//--- Dance the funky bidirectional BFS...
		while (1) {

			//--- Get state from queue, compute its ID and get its direction.
			vi oldState = q.front();
			q.pop();
			vi oldId = IdentifyState(oldState);
			int& oldDir = direction[oldId];

			//--- Apply all applicable moves to it and handle the new state.
			for (int move = 0; move < 18; move++) {
				if (applicableMoves[m_phase] & (1 << move)) {

					//--- Apply the move.
					vi newState = ApplyMove(move, oldState);
					vi newId = IdentifyState(newState);
					int& newDir = direction[newId];

					//--- Have we seen this state (id) from the other direction already?
					//--- I.e. have we found a connection?
					if (newDir  &&  newDir != oldDir) {

						//--- Make oldId represent the forwards and newId the backwards search state.
						if (oldDir > 1) {
							swap(newId, oldId);
							move = Inverse(move);
						}

						//--- Reconstruct the connecting algorithm.
						vi algorithm(1, move);
						while (oldId != currentId) {
							algorithm.insert(algorithm.begin(), lastMove[oldId]);
							oldId = predecessor[oldId];
						}
						while (newId != goalId) {
							algorithm.push_back(Inverse(lastMove[newId]));
							newId = predecessor[newId];
						}

						//--- Print and apply the algorithm.
						for (int i = 0; i < (int)algorithm.size(); i++) {
							os << "UDFBLR"[algorithm[i] / 3] << algorithm[i] % 3 + 1 << " ";
							currentState = ApplyMove(algorithm[i], currentState);
						}

						//--- Jump to the next phase.
						goto nextPhasePlease;
					}

					//--- If we've never seen this state (id) before, visit it.
					if (!newDir) {
						q.push(newState);
						newDir = oldDir;
						lastMove[newId] = move;
						predecessor[newId] = oldId;
					}
				}
			}
		}
	nextPhasePlease:
		;
	}
	return os.str();
}

} // Thistlethwaite
} // Rsolver
