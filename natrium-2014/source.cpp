#include <vector>
#include <algorithm>

using std::vector;
using std::min;
using std::max;

int solution(vector<int> &A) 
{
	int size = static_cast<int>(A.size());
	vector<int> leftMin;
	vector<int> rightMax;
	leftMin.resize(size);
	rightMax.resize(size);

	leftMin[0] = A[0];
	rightMax[size-1] = A[size-1];

	for (int i = 1; i < size; ++i)
	{
		leftMin[i] = min(A[i], leftMin[i-1]);
	}

	for (int i = size-2; i >= 0; --i)
	{
		rightMax[i] = max(A[i], rightMax[i+1]);
	}

	int i = 0;
	int j = 0;
	int maxDist = 0;

	while (i < size && j < size)
	{
		if (leftMin[i] <= rightMax[j])
		{
			maxDist = max(maxDist, j-i);
			++j;
		}
		else
		{
			++i;
		}
	}

	return maxDist;
}