#ifndef OPTICS_H
#define OPTICS_H

#include <individual.h>

class OPTICS
{
public:
    OPTICS();
    std::vector< std::vector<Individual> > DBSCAN(std::vector<Individual> & setOfIndividuals, float eps, int minPts) ;
    std::vector<int> regionQuery(std::vector<Individual>  & Individuals, Individual *Individual, float eps) ;

private:

    double epsilon;
    int minpts;
};

#endif // OPTICS_H
