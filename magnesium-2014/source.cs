using System;
using System.Collections.Generic;

class Solution
{
    private class Link
    {
        public int node;
        public int attractiveness;        
        public int longestWalkLength;
        public bool visited;
    }

    private class Node
    {
        public List<Link> links;

        public Node()
        {
            links = new List<Link>();    
        }       
    }

    private Node[] nodes;

    public int solution(int N, int[] A, int[] B, int[] C)
    {
        nodes = new Node[N];

        for (int i = 0; i < N; ++i)
        {
            nodes[i] = new Node();
        }
                
        // Build links with their attractiveness level.
        for (int i = 0; i < A.Length; ++i)
        {
            Link link = new Link();
            link.node = B[i];
            link.attractiveness = C[i];
            nodes[A[i]].links.Add(link);

            link = new Link();
            link.node = A[i];
            link.attractiveness = C[i];
            nodes[B[i]].links.Add(link);
        }

        int longestWalk = 0;

        for (int i = 0; i < nodes.Length; ++i)
        {
            int longestWalkForNode = findLongestWalkForNode(i);
            longestWalk = longestWalk < longestWalkForNode ? longestWalkForNode : longestWalk;
        }

        return longestWalk;
    }

    private int findLongestWalkForNode(int node)
    {
        int longestWalkForNode = 0;

        for (int i = 0; i < nodes[node].links.Count; ++i)
        {
            int longestWalk = 0;

            if (!nodes[node].links[i].visited)
            {
                longestWalk = findLongestWalkForLink(nodes[node].links[i].node, nodes[node].links[i].attractiveness);

                nodes[node].links[i].longestWalkLength = longestWalk;
                nodes[node].links[i].visited = true;
                longestWalkForNode = longestWalkForNode < longestWalk ? longestWalk : longestWalkForNode;
            }
        }

        return longestWalkForNode;
    }

    private int findLongestWalkForLink(int link, int attractiveness)
    {
        int largestStack = 0;        

        for (int i = 0; i < nodes[link].links.Count; ++i)
        {
            int stack = 0;

            if (nodes[link].links[i].visited && attractiveness < nodes[link].links[i].attractiveness)
            {
                stack = nodes[link].links[i].longestWalkLength;
                largestStack = largestStack < stack ? stack : largestStack;
            }
            else if (attractiveness < nodes[link].links[i].attractiveness)
            {
                stack = findLongestWalkForLink(nodes[link].links[i].node, nodes[link].links[i].attractiveness);
                largestStack = largestStack < stack ? stack : largestStack;
                nodes[link].links[i].longestWalkLength = stack;
                nodes[link].links[i].visited = true;
            }            
        }

        return largestStack + 1;
    }

}