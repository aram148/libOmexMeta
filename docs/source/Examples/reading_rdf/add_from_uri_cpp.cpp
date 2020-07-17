#include "omexmeta/RDF.h" // omexmeta::RDF
#include <iostream>     // std::cout, std::endl

using namespace omexmeta;

int main(){

    std::string rdf_str = "@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n"
                          "@prefix bqbiol: <http://biomodels.net/biology-qualifiers/> .\n"
                          "@prefix myOMEXlib: <http://omex-library.org/> .\n"
                          "@prefix myOMEX: <http://omex-library.org/NewOmex.omex> .\n"
                          "@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .\n"
                          "\n"
                          "local:OmexMetaId0000\n"
                          "   bqbiol:is <https://identifiers.org/uniprot/PD12345> .\n";

    // collect the link from "https://www.ebi.ac.uk/biomodels/BIOMD0000000308#Files"
    std::string tyson2003 = "https://www.ebi.ac.uk/biomodels/model/download/BIOMD0000000308.2?filename=BIOMD0000000308_url.xml";

    // first create a RDF graph, by any means available - here we choose from_string
    RDF rdf = RDF::fromString(rdf_str, format="turtle");

    // And now add to it from a uri
    rdf.add_from_uri(tyson2003, "rdfxml");

    std::cout << "Number of annotations in graph: " << rdf.size() << std::endl;
    return 0;
}