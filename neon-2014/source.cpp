#include <vector>

using std::vector;
using std::max;

struct Cluster
{
	int start;			// Start position.
	int end;			// End position.
	int ropeLength;		// Longest rope in the cluster.

	Cluster()
	{
	}

	Cluster(int _start, int _end, int _ropeLength) :
		start(_start),
		end(_end),
		ropeLength(_ropeLength)
	{
	}

	int size()
	{
		return end - start;
	}

};

void checkCollisions(vector<Cluster>& clusters, Cluster newCluster, int M)
{
	// No clusters, add new cluster as is.
	if (clusters.size() == 0)
	{
		clusters.push_back(newCluster);
		return;
	}
	// Enough space to add new cluster.
	else if (clusters.back().end < newCluster.start)
	{
		clusters.push_back(newCluster);
		return;
	}
	// Clusters next to each other, merge.
	else if (clusters.back().end == newCluster.start)
	{
		clusters.back().end = newCluster.end;
		clusters.back().ropeLength = max(clusters.back().ropeLength, newCluster.ropeLength);
		return;
	}

	// Calculate optimal movement to left.
	int overlap = clusters.back().end - newCluster.start;
	int maxRope = overlap + newCluster.ropeLength;	
	int movement = (maxRope + 1 - clusters.back().ropeLength) / 2;
	movement = movement > overlap ? overlap : movement;				// Movement can not be more than overlap.
	movement = max(0, movement);									// Movement to left can not be negative.

	// Check left border.
	if (clusters.back().start - movement < 0)
	{
		movement = clusters.back().start;
		clusters.back().start = 0;
		clusters.back().end += newCluster.size() - movement;
		clusters.back().ropeLength = max(clusters.back().ropeLength + movement, maxRope - movement);
		return;
	}
	// Check right border.
	else if (clusters.back().end - movement + newCluster.size() > M)
	{
		movement = clusters.back().end + newCluster.size() - M;
		int rope = max(clusters.back().ropeLength + movement, newCluster.ropeLength);
		Cluster cluster(clusters.back().start - movement, M, rope);
		clusters.pop_back();
		checkCollisions(clusters, cluster, M);
	}
	// Merge and move.
	else
	{
		int rope = max(clusters.back().ropeLength + movement, maxRope - movement);
		Cluster cluster(clusters.back().start - movement, clusters.back().end + newCluster.size() - movement, rope);
		clusters.pop_back();
		checkCollisions(clusters, cluster, M);
	}
}

int solution(vector<int> &R, int X, int M)
{
	if (2 * X * static_cast<int>(R.size()) > M)
	{
		return -1;
	}

	vector<Cluster> clusters;

	for (unsigned int i = 0; i < R.size(); ++i)
	{
		Cluster newBoat;

		// Create new boat as a new cluster.
		if (R[i] - X < 0)
		{
			newBoat = Cluster(0, 2*X, X - R[i]);
		}
		else if (R[i] + X > M)
		{
			newBoat = Cluster(M - 2*X, M, R[i] + X - M);
		}
		else
		{
			newBoat = Cluster(R[i] - X, R[i] + X, 0);
		}

		checkCollisions(clusters, newBoat, M);
	}

	int max = 0;

	for (unsigned int i = 0; i < clusters.size(); ++i)
	{
		if (clusters[i].ropeLength > max)
		{
			max = clusters[i].ropeLength;
		}
	}

	return max;
}