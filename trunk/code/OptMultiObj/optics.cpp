#include "optics.h"

OPTICS::OPTICS()
{
}


/* DBSCAN - density-based spatial clustering of applications with noise */

std::vector< std::vector<Individual> > OPTICS::DBSCAN(std::vector<Individual> & setOfIndividuals , float eps, int minPts)
{
    std::vector< std::vector<Individual> > clusters;
    std::vector<bool> clustered;
    std::vector<int> noise;
    std::vector<bool> visited;
    std::vector<int> neighborPts;
    std::vector<int> neighborPts_;
    int c;

    int noKeys = setOfIndividuals.size();

    //init clustered and visited
    for(int k = 0; k < noKeys; k++)
    {
        clustered.push_back(false);
        visited.push_back(false);
    }

    //C =0;
    c = 0;
    clusters.push_back(std::vector<Individual>()); //will stay empty?

    //for each unvisted point P in dataset setOfIndividuals
    for(int i = 0; i < noKeys; i++)
    {
        if(!visited[i])
        {
            //Mark P as visited
            visited[i] = true;
            neighborPts = regionQuery(setOfIndividuals,&setOfIndividuals.at(i),eps);
            if(neighborPts.size() < minPts)
                //Mark P as Noise
                noise.push_back(i);
            else
            {
                clusters.push_back(std::vector<Individual>());
                c++;
                //expand cluster
                // add P to cluster c
                clusters[c].push_back(setOfIndividuals.at(i));
                clustered[i] = true;
                //for each point P' in neighborPts
                for(int j = 0; j < neighborPts.size(); j++)
                {
                    //if P' is not visited
                    if(!visited[neighborPts[j]])
                    {
                        //Mark P' as visited
                        visited[neighborPts[j]] = true;
                        neighborPts_ = regionQuery(setOfIndividuals,&setOfIndividuals.at(neighborPts[j]),eps);
                        if(neighborPts_.size() >= minPts)
                        {
                            neighborPts.insert(neighborPts.end(),neighborPts_.begin(),neighborPts_.end());
                        }
                    }
                    // if P' is not yet a member of any cluster
                    // add P' to cluster c
                    if(!clustered[neighborPts[j]]) {
                        clusters[c].push_back(setOfIndividuals.at(neighborPts[j]));
                        clustered[neighborPts[j]] = true;
                    }
                }
            }

        }
    }
    return clusters;
}

std::vector<int> OPTICS::regionQuery(std::vector<Individual> & setOfIndividuals, Individual *p, float eps)
{
    float dist;
    std::vector<int> retKeys;
    for(int i = 0; i< setOfIndividuals.size(); i++)
    {
        dist = p->distanceTo(setOfIndividuals.at(i));
        if(dist <= eps && dist != 0.0f)
        {
            retKeys.push_back(i);
        }
    }
    return retKeys;
}
