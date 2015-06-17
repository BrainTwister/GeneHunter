#ifndef TAXONOMY_H_
#define TAXONOMY_H_

#include "CreateDataClass.h"
#include "Organism.h"
#include <cstdlib>
#include <mysql.h>
#include <string>

namespace GeneHunter {

class Taxonomy
{
public:

	CREATE_DATA_CLASS( Settings,\
		(( uint8_t, verbosity, 0 ))\
	)

	Taxonomy( Settings const& settings = Settings() );

    ~Taxonomy();

	size_t getTaxIDByNucGI( size_t geneID ) const;

	std::string getScientificName( size_t taxID, std::string const& rank = "" ) const;

	std::string getScientificNameOfSpecies( size_t taxID ) const;

	size_t getLowestTaxIDWithRank( size_t taxID ) const;

	std::tuple<size_t,std::string,bool> getTaxInfo( size_t taxID ) const;

	std::string getLineage( size_t taxID, bool withRank = false, bool abbreviated = true ) const;

	std::string getOrganismName( size_t taxID ) const;

	Organism getOrganism( size_t geneID ) const;

private:

	void updateLineageTrace( size_t taxID ) const;

	std::string getScientificNameByTaxID( size_t taxID ) const;

	// scientific name / rank / taxID / hiddenFlag
	mutable std::vector< std::tuple< std::string, std::string, size_t, bool > > currentTrace_;

	mutable size_t currentTraceTaxID_ = 0;

	MYSQL *myConnection_;

	Settings settings_;

};

} // namespace GeneHunter

#endif /* TAXONOMY_H_ */
