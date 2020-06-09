//
// Created by Ciaran on 5/12/2020.
//

#include "gtest/gtest.h"


#include "gtest/gtest.h"
#include "AnnotationSamples.h"
#include "semsim/SemsimCApi.h"
#include "semsim/RDF.h"
#include "SBMLFactory.h"
#include "semsim/SemsimXmlAssistant.h"
#include <experimental/filesystem>
#include <fstream>

using namespace semsim;

class CAPITests : public ::testing::Test {
public:

    AnnotationSamples samples = AnnotationSamples();

    CAPITests() = default;

    void TearDown() override {
    };
};


TEST_F(CAPITests, RDFNew) {
    RDF *rdf_ptr = RDF_new();
    // we access the default base uri to check we have a RDF object
    std::string uri = rdf_ptr->base_uri_;
    std::string expected = "file://./Annotations.rdf";
    ASSERT_STREQ(expected.c_str(), uri.c_str());
    RDF_delete(rdf_ptr);

}

TEST_F(CAPITests, RDFSize) {
    RDF *rdf_ptr = RDF_fromString(samples.singular_annotation1.c_str(), "rdfxml", "LannotationsBase.rdf");
    int actual = RDF_size(rdf_ptr);
    int expected = 1;
    ASSERT_EQ(expected, actual);
    RDF_delete(rdf_ptr);

}

TEST_F(CAPITests, FreeCharStar) {
// validated with valgrind. Not sure how to built into gtest
    char *cstring = _func_that_returns_dynamic_alloc_str();
    free_c_char_star(cstring);
}

TEST_F(CAPITests, RDFToString) {
    RDF *rdf_ptr = RDF_fromString(samples.singular_annotation1.c_str(), "rdfxml");
    std::string actual = rdf_ptr->toString("rdfxml-abbrev", "annotation.rdf");

    std::cout << actual << std::endl;
    std::string expected = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                           "<rdf:RDF xmlns:bqbiol=\"http://biomodels.net/biology-qualifiers/\"\n"
                           "   xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
                           "   xml:base=\"file://annotation.rdf\">\n"
                           "  <rdf:Description rdf:about=\"file://./metaid_1\">\n"
                           "    <bqbiol:is rdf:resource=\"https://identifiers.org/uniprot/P0DP23\"/>\n"
                           "  </rdf:Description>\n"
                           "</rdf:RDF>\n";
    ASSERT_STREQ(expected.c_str(), actual.c_str());
    RDF_delete(rdf_ptr);
}

TEST_F(CAPITests, RDFgetBaseUri) {
    RDF *rdf_ptr = RDF_fromString(samples.singular_annotation1.c_str(), "rdfxml");
    char *actual = RDF_getBaseUri(rdf_ptr);
    std::cout << actual << std::endl;
    const char *expected = "file://./Annotations.rdf";
    ASSERT_STREQ(expected, actual);

    free(actual);
    RDF_delete(rdf_ptr);
}

TEST_F(CAPITests, RDFsetBaseUri) {
    RDF *rdf_ptr = RDF_fromString(samples.singular_annotation1.c_str(), "rdfxml");
    RDF_setBaseUri(rdf_ptr, "ANewBaseUri.rdf");
    char *actual = RDF_getBaseUri(rdf_ptr);
    std::cout << actual << std::endl;
    const char *expected = "file://ANewBaseUri.rdf";
    ASSERT_STREQ(expected, actual);
    free(actual); // necessary because we allocated on heap.
    RDF_delete(rdf_ptr);

}


TEST_F(CAPITests, RDFqueryResultsAsStr) {
    RDF *rdf_ptr = RDF_fromString(samples.composite_annotation_pe.c_str(), "rdfxml");

    const char *query = "SELECT ?x ?y ?z \n"
                        "WHERE { ?x ?y ?z }";
    char *results = RDF_queryResultsAsStr(rdf_ptr, query, "csv");
    std::string expected = "x,y,z\r\n"
                           "file://./VLV,http://biomodels.net/biology-qualifiers/isVersionOf,http://identifiers.org/opb/OPB_00154\r\n"
                           "file://./VLV,http://biomodels.net/biology-qualifiers/isPropertyOf,file://./MyModel.rdf#entity_0\r\n"
                           "file://./MyModel.rdf#entity_0,http://biomodels.net/biology-qualifiers/is,http://identifiers.org/fma/FMA:9670\r\n"
                           "file://./MyModel.rdf#entity_0,http://biomodels.net/biology-qualifiers/isPartOf,http://identifiers.org/fma/FMA:18228\r\n";
    ASSERT_STREQ(expected.c_str(), results);
    RDF_delete(rdf_ptr);
    free_c_char_star(results);
}

TEST_F(CAPITests, TestCheckValidMetaid) {
    RDF *rdf_ptr = RDF_new();

    Editor *editor_ptr = rdf_ptr->toEditorPtr(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML
    );
    Editor_checkValidMetaid(editor_ptr, "SemsimMetaid0000");

    Editor_delete(editor_ptr);
    RDF_delete(rdf_ptr);
}

TEST_F(CAPITests, TestGetMetaID) {
    RDF *rdf_ptr = RDF_new();

    Editor *editor_ptr = rdf_ptr->toEditorPtr(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML
    );
    char *actual = Editor_getMetaId(editor_ptr, 0);
    std::cout << actual << std::endl;
    const char *expected = "SemsimMetaid0000";
    ASSERT_STREQ(expected, actual);

    free(actual);
    Editor_delete(editor_ptr);
    RDF_delete(rdf_ptr);
}

TEST_F(CAPITests, TestGetNumMetaIDs) {
    RDF *rdf_ptr = RDF_new();

    Editor *editor_ptr = rdf_ptr->toEditorPtr(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML
    );
    int actual = Editor_getNumMetaIds(editor_ptr);
    std::cout << actual << std::endl;
    int expected = 13;
    ASSERT_EQ(expected, actual);
    Editor_delete(editor_ptr);
    RDF_delete(rdf_ptr);
}

TEST_F(CAPITests, TestEditorGetXml) {
    RDF *rdf_ptr = RDF_new();

    Editor *editor_ptr = rdf_ptr->toEditorPtr(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML
    );

    char *actual = Editor_getXml(editor_ptr);
    const char *expected = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                           "<sbml xmlns=\"http://www.sbml.org/sbml/level3/version2/core\" level=\"3\" version=\"2\">\n"
                           "  <model id=\"TestModelNotAnnotated\" metaid=\"SemsimMetaid0000\">\n"
                           "    <listOfUnitDefinitions>\n"
                           "      <unitDefinition id=\"molar\">\n"
                           "        <listOfUnits>\n"
                           "          <unit kind=\"mole\" exponent=\"1\" scale=\"1\" multiplier=\"1\" metaid=\"SemsimMetaid0001\"/>\n"
                           "          <unit kind=\"litre\" exponent=\"-1\" scale=\"1\" multiplier=\"1\" metaid=\"SemsimMetaid0002\"/>\n"
                           "        </listOfUnits>\n"
                           "      </unitDefinition>\n"
                           "    </listOfUnitDefinitions>\n"
                           "    <listOfCompartments>\n"
                           "      <compartment metaid=\"cytosol\" id=\"cytosol\" size=\"1\" constant=\"true\"/>\n"
                           "    </listOfCompartments>\n"
                           "    <listOfSpecies>\n"
                           "      <species metaid=\"Meta00001\" id=\"X\" compartment=\"cytosol\" initialConcentration=\"10\" substanceUnits=\"molar\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\"/>\n"
                           "      <species id=\"Y\" compartment=\"cytosol\" initialConcentration=\"20\" substanceUnits=\"molar\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\" metaid=\"SemsimMetaid0003\"/>\n"
                           "      <species id=\"Y\" compartment=\"cytosol\" initialConcentration=\"15\" substanceUnits=\"molar\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\" metaid=\"SemsimMetaid0004\"/>\n"
                           "    </listOfSpecies>\n"
                           "    <listOfReactions>\n"
                           "      <reaction id=\"X2Y\" reversible=\"false\" metaid=\"SemsimMetaid0005\">\n"
                           "        <listOfProducts>\n"
                           "          <speciesReference species=\"Y\" constant=\"false\"/>\n"
                           "        </listOfProducts>\n"
                           "        <kineticLaw metaid=\"SemsimMetaid0006\">\n"
                           "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                           "            <apply>\n"
                           "              <times/>\n"
                           "              <ci> x </ci>\n"
                           "              <ci> kx2y </ci>\n"
                           "            </apply>\n"
                           "          </math>\n"
                           "          <listOfLocalParameters>\n"
                           "            <localParameter id=\"kx2y\" value=\"1\" metaid=\"SemsimMetaid0007\"/>\n"
                           "            <localParameter id=\"ky2z\" value=\"1\" metaid=\"SemsimMetaid0008\"/>\n"
                           "          </listOfLocalParameters>\n"
                           "        </kineticLaw>\n"
                           "      </reaction>\n"
                           "      <reaction id=\"y2z\" reversible=\"false\" metaid=\"SemsimMetaid0009\">\n"
                           "        <listOfProducts>\n"
                           "          <speciesReference species=\"Z\" constant=\"false\"/>\n"
                           "        </listOfProducts>\n"
                           "        <kineticLaw metaid=\"SemsimMetaid0010\">\n"
                           "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                           "            <apply>\n"
                           "              <times/>\n"
                           "              <ci> y </ci>\n"
                           "              <ci> ky2z </ci>\n"
                           "            </apply>\n"
                           "          </math>\n"
                           "        </kineticLaw>\n"
                           "      </reaction>\n"
                           "    </listOfReactions>\n"
                           "  </model>\n"
                           "</sbml>\n";
    std::cout << actual << std::endl;
    ASSERT_STREQ(expected, actual);
    free(actual);
    Editor_delete(editor_ptr);
    RDF_delete(rdf_ptr);
}

TEST_F(CAPITests, TestSingularAnnotationSetAbout) {
    RDF *rdf_ptr = RDF_new();

    Editor *editor_ptr = rdf_ptr->toEditorPtr(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML
    );

    SingularAnnotation *singularAnnotation = SingularAnnotation_new(editor_ptr);
    SingularAnnotation_setAbout(singularAnnotation,
                                "metaid6");
    char *actual = SingularAnnotation_getAbout(singularAnnotation);
    const char *expected = "metaid6";
    ASSERT_STREQ(expected, actual);


    Editor_delete(editor_ptr);
    SingularAnnotation_delete(singularAnnotation);
    free_c_char_star(actual);
    RDF_delete(rdf_ptr);
}

TEST_F(CAPITests, TestSingularAnnotationSetPredicate) {
    RDF *rdf_ptr = RDF_new();

    Editor *editor_ptr = rdf_ptr->toEditorPtr(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML
    );

    SingularAnnotation *singularAnnotation = SingularAnnotation_new(editor_ptr);
    SingularAnnotation_setPredicate(singularAnnotation,
                                    "bqb", "is");
    char *actual = SingularAnnotation_getPredicate(singularAnnotation);
    const char *expected = "http://biomodels.net/biology-qualifiers/is";
    ASSERT_STREQ(expected, actual);


    Editor_delete(editor_ptr);
    SingularAnnotation_delete(singularAnnotation);
    free_c_char_star(actual);
    RDF_delete(rdf_ptr);
}

TEST_F(CAPITests, TestSingularAnnotationSetPredicateUri) {
    RDF *rdf_ptr = RDF_new();

    Editor *editor_ptr = rdf_ptr->toEditorPtr(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML
    );

    SingularAnnotation *singularAnnotation = SingularAnnotation_new(editor_ptr);
    SingularAnnotation_setPredicateFromUri(singularAnnotation, "http://predicate.com/from/uri");
    char *actual = SingularAnnotation_getPredicate(singularAnnotation);
    const char *expected = "http://predicate.com/from/uri";
    ASSERT_STREQ(expected, actual);


    Editor_delete(editor_ptr);
    SingularAnnotation_delete(singularAnnotation);
    free_c_char_star(actual);
    RDF_delete(rdf_ptr);
}

TEST_F(CAPITests, TestEditorPtrMem) {
// verified with valgrind, not sure how to gtest
    RDF *rdf_ptr = RDF_new();

    Editor *editor_ptr = rdf_ptr->toEditorPtr(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML
    );

    RDF_delete(rdf_ptr);
    delete editor_ptr;
}


TEST_F(CAPITests, TestSingularAnnotationSetResourceLiteral) {
    RDF *rdf_ptr = RDF_new();

    Editor *editor_ptr = rdf_ptr->toEditorPtr(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML
    );

    SingularAnnotation *singularAnnotation = SingularAnnotation_new(editor_ptr);
    SingularAnnotation_setResourceLiteral(singularAnnotation,
                                          "LiterallyAString");
    char *actual = SingularAnnotation_getResource(singularAnnotation);
    const char *expected = "LiterallyAString";
    ASSERT_STREQ(expected, actual);


    Editor_delete(editor_ptr);
    SingularAnnotation_delete(singularAnnotation);
    free_c_char_star(actual);
    RDF_delete(rdf_ptr);
}

TEST_F(CAPITests, TestSingularAnnotationSetResourceUri) {
    RDF *rdf_ptr = RDF_new();

    Editor *editor_ptr = rdf_ptr->toEditorPtr(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML
    );

    SingularAnnotation *singularAnnotation = SingularAnnotation_new(editor_ptr);
    SingularAnnotation_setResourceUri(singularAnnotation,
                                      "uniprot:PD98723");
    char *actual = SingularAnnotation_getResource(singularAnnotation);
    const char *expected = "https://identifiers.org/uniprot/PD98723";
    ASSERT_STREQ(expected, actual);

    Editor_delete(editor_ptr);
    SingularAnnotation_delete(singularAnnotation);
    free_c_char_star(actual);
    RDF_delete(rdf_ptr);
}

TEST_F(CAPITests, TestSingularAnnotationSetResourceBlank) {
    RDF *rdf_ptr = RDF_new();

    Editor *editor_ptr = rdf_ptr->toEditorPtr(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML
    );

    SingularAnnotation *singularAnnotation = SingularAnnotation_new(editor_ptr);
    SingularAnnotation_setResourceBlank(singularAnnotation, "Nothing");
    char *actual = SingularAnnotation_getResource(singularAnnotation);
    const char *expected = "Nothing";
    ASSERT_STREQ(expected, actual);


    Editor_delete(editor_ptr);
    SingularAnnotation_delete(singularAnnotation);
    free_c_char_star(actual);
    RDF_delete(rdf_ptr);
}


TEST_F(CAPITests, TestPhysicalEntity) {
    RDF *rdf_ptr = RDF_new();

    Editor *editor_ptr = rdf_ptr->toEditorPtr(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML
    );

    PhysicalEntity *physical_entity_ptr = PhysicalEntity_new(editor_ptr);
    physical_entity_ptr = PhysicalEntity_setAbout(physical_entity_ptr, "metaid87");
    physical_entity_ptr = PhysicalEntity_setPhysicalProperty(physical_entity_ptr, "opb/OPB007");
    physical_entity_ptr = PhysicalEntity_setIdentity(physical_entity_ptr, "uniprot:PD58736");
    physical_entity_ptr = PhysicalEntity_addLocation(physical_entity_ptr, "fma:FMA_8764");

    char *actual = PhysicalEntity_str(physical_entity_ptr, "rdfxml-abbrev", "./annotations.rdf");
    const char *expected = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                           "<rdf:RDF xmlns:bqbiol=\"http://biomodels.net/biology-qualifiers/\"\n"
                           "   xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
                           "   xml:base=\"file://./annotations.rdf\">\n"
                           "  <rdf:Description rdf:about=\"PhysicalEntity0000\">\n"
                           "    <bqbiol:is rdf:resource=\"https://identifiers.org/uniprot/PD58736\"/>\n"
                           "    <bqbiol:isPartOf rdf:resource=\"https://identifiers.org/fma/FMA_8764\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"metaid87\">\n"
                           "    <bqbiol:isPropertyOf rdf:resource=\"PhysicalEntity0000\"/>\n"
                           "    <bqbiol:isVersionOf rdf:resource=\"https://identifiers.org/opb/OPB007\"/>\n"
                           "  </rdf:Description>\n"
                           "</rdf:RDF>\n";
    std::cout << actual <<
              std::endl;
    ASSERT_STREQ(expected, actual);


    Editor_delete(editor_ptr);
    PhysicalEntity_delete(physical_entity_ptr);
    free_c_char_star(actual);
    RDF_delete(rdf_ptr);
}

TEST_F(CAPITests, TestPhysicalEntityAbout) {
    RDF *rdf_ptr = RDF_new();
    Editor *editor_ptr = rdf_ptr->toEditorPtr(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML
    );
    PhysicalEntity *physical_entity_ptr = PhysicalEntity_new(editor_ptr);
    physical_entity_ptr = PhysicalEntity_setAbout(physical_entity_ptr, "metaid87");
    char *actual = PhysicalEntity_getAbout(physical_entity_ptr);
    const char *expected = "metaid87";
    std::cout << actual << std::endl;
    ASSERT_STREQ(expected, actual);

    Editor_delete(editor_ptr);
    free_c_char_star(actual);
    // we need to free the node inside get about manually here because
    // we are not using the str() method
    PhysicalEntity_freeAll(physical_entity_ptr);
    RDF_delete(rdf_ptr);
}


TEST_F(CAPITests, TestPhysicalEntityPhysicalProperty) {
    RDF *rdf_ptr = RDF_new();
    Editor *editor_ptr = rdf_ptr->toEditorPtr(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML
    );
    PhysicalEntity *physical_entity_ptr = PhysicalEntity_new(editor_ptr);
    physical_entity_ptr = PhysicalEntity_setPhysicalProperty(physical_entity_ptr, "opb/opb_465");
    char *actual = PhysicalEntity_getPhysicalProperty(physical_entity_ptr);
    const char *expected = "https://identifiers.org/opb/opb_465";
    std::cout << actual <<
              std::endl;
    ASSERT_STREQ(expected, actual);

    Editor_delete(editor_ptr);
    // we must free this node manually here. Cannot be incorporated into
    // delete function because it'll clash with the main use case (i.e. str method)
    PhysicalEntity_freeAll(physical_entity_ptr);
    free_c_char_star(actual);
    RDF_delete(rdf_ptr);
}

TEST_F(CAPITests, TestPhysicalEntityGetIdentity) {
    RDF *rdf_ptr = RDF_new();
    Editor *editor_ptr = rdf_ptr->toEditorPtr(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML
    );
    PhysicalEntity *physical_entity_ptr = PhysicalEntity_new(editor_ptr);
    physical_entity_ptr = PhysicalEntity_setIdentity(physical_entity_ptr, "uniprot/PD7363");
    char *actual = PhysicalEntity_getIdentity(physical_entity_ptr);
    const char *expected = "https://identifiers.org/uniprot/PD7363";
    std::cout << actual <<
              std::endl;
    ASSERT_STREQ(expected, actual);


    Editor_delete(editor_ptr);
    // as opposed to PhysicalEntity_delete which would leave behind un-freed nodes.
    PhysicalEntity_freeAll(physical_entity_ptr);
    free_c_char_star(actual);
    RDF_delete(rdf_ptr);
}

TEST_F(CAPITests, TestPhysicalEntityLocations) {
    RDF *rdf_ptr = RDF_new();
    Editor *editor_ptr = rdf_ptr->toEditorPtr(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML
    );
    PhysicalEntity *physical_entity_ptr = PhysicalEntity_new(editor_ptr);
    physical_entity_ptr = PhysicalEntity_addLocation(physical_entity_ptr, "FMA:fma:8376");
    physical_entity_ptr = PhysicalEntity_addLocation(physical_entity_ptr, "FMA:fma:8377");
    physical_entity_ptr = PhysicalEntity_addLocation(physical_entity_ptr, "FMA:fma:8378");
    int num_locations = PhysicalEntity_getNumLocations(physical_entity_ptr);
    const char *expected = "https://identifiers.org/FMA/fma:8376\n"
                           "https://identifiers.org/FMA/fma:8377\n"
                           "https://identifiers.org/FMA/fma:8378\n";
    std::ostringstream os;
    for (int i = 0; i < num_locations; i++) {
        char *string = PhysicalEntity_getLocation(physical_entity_ptr, i);
        os << string << '\n';
        free_c_char_star(string);
    }
    ASSERT_STREQ(expected, os.str().c_str());
    PhysicalEntity_freeAll(physical_entity_ptr);
    Editor_delete(editor_ptr);
    RDF_delete(rdf_ptr);

}


TEST_F(CAPITests, TestPhysicalEntityNumLocations) {
    RDF *rdf_ptr = RDF_new();
    Editor *editor_ptr = rdf_ptr->toEditorPtr(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML
    );
    PhysicalEntity *physical_entity_ptr = PhysicalEntity_new(editor_ptr);
    physical_entity_ptr = PhysicalEntity_addLocation(physical_entity_ptr, "FMA:fma:8376");
    physical_entity_ptr = PhysicalEntity_addLocation(physical_entity_ptr, "FMA:fma:8377");
    physical_entity_ptr = PhysicalEntity_addLocation(physical_entity_ptr, "FMA:fma:8378");
    int expected = 3;
    int actual = PhysicalEntity_getNumLocations(physical_entity_ptr);
    ASSERT_EQ(expected, actual);

    Editor_delete(editor_ptr);
    PhysicalEntity_freeAll(physical_entity_ptr);
    RDF_delete(rdf_ptr);
}


TEST_F(CAPITests, TestPhysicalEntityStr) {
    RDF *rdf_ptr = RDF_new();
    Editor *editor_ptr = rdf_ptr->toEditorPtr(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML
    );
    PhysicalEntity *physical_entity_ptr = PhysicalEntity_new(editor_ptr);
    physical_entity_ptr = PhysicalEntity_setPhysicalProperty(physical_entity_ptr, "opb/opb_465");
    physical_entity_ptr = PhysicalEntity_setAbout(physical_entity_ptr, "metaid87");
    physical_entity_ptr = PhysicalEntity_setIdentity(physical_entity_ptr, "uniprot/PD7363");
    physical_entity_ptr = PhysicalEntity_addLocation(physical_entity_ptr, "FMA:fma:8376");
    physical_entity_ptr = PhysicalEntity_addLocation(physical_entity_ptr, "FMA:fma:8377");
    physical_entity_ptr = PhysicalEntity_addLocation(physical_entity_ptr, "FMA:fma:8378");
    std::string expected = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                           "<rdf:RDF xmlns:bqbiol=\"http://biomodels.net/biology-qualifiers/\"\n"
                           "   xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
                           "   xml:base=\"file://./Annot.rdf\">\n"
                           "  <rdf:Description rdf:about=\"PhysicalEntity0000\">\n"
                           "    <bqbiol:is rdf:resource=\"https://identifiers.org/uniprot/PD7363\"/>\n"
                           "    <bqbiol:isPartOf rdf:resource=\"https://identifiers.org/FMA/fma:8376\"/>\n"
                           "    <bqbiol:isPartOf rdf:resource=\"https://identifiers.org/FMA/fma:8377\"/>\n"
                           "    <bqbiol:isPartOf rdf:resource=\"https://identifiers.org/FMA/fma:8378\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"metaid87\">\n"
                           "    <bqbiol:isPropertyOf rdf:resource=\"PhysicalEntity0000\"/>\n"
                           "    <bqbiol:isVersionOf rdf:resource=\"https://identifiers.org/opb/opb_465\"/>\n"
                           "  </rdf:Description>\n"
                           "</rdf:RDF>\n";

    char *actual = PhysicalEntity_str(physical_entity_ptr, "rdfxml-abbrev", "./Annot.rdf");
    std::cout << actual << std::endl;
    ASSERT_STREQ(expected.c_str(), actual);

    Editor_delete(editor_ptr);
    PhysicalEntity_delete(physical_entity_ptr);
    free_c_char_star(actual);
    RDF_delete(rdf_ptr);
}

TEST_F(CAPITests, TestPhysicalProcess) {
    RDF *rdf_ptr = RDF_new();
    Editor *editor_ptr = rdf_ptr->toEditorPtr(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML
    );
    PhysicalProcess *physical_process_ptr = PhysicalProcess_new(editor_ptr);

    physical_process_ptr = PhysicalProcess_setAbout(physical_process_ptr, "Metaid0937");
    physical_process_ptr = PhysicalProcess_setPhysicalProperty(physical_process_ptr, "opb/opb93864");
    physical_process_ptr = PhysicalProcess_addSink(
            physical_process_ptr, "Sink9", 1.0, "Entity8");
    physical_process_ptr = PhysicalProcess_addSource(
            physical_process_ptr, "Source1", 1.0, "Entity8");
    physical_process_ptr = PhysicalProcess_addMediator(
            physical_process_ptr, "Mod4", 1.0, "Entity8");

    char *actual = PhysicalProcess_str(physical_process_ptr, "rdfxml-abbrev", "./annotations.rdf");
    std::string expected = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                           "<rdf:RDF xmlns:bqbiol=\"http://biomodels.net/biology-qualifiers/\"\n"
                           "   xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
                           "   xmlns:semsim=\"http://www.bhi.washington.edu/semsim#\"\n"
                           "   xml:base=\"file://./annotations.rdf\">\n"
                           "  <rdf:Description rdf:about=\"Metaid0937\">\n"
                           "    <bqbiol:isPropertyOf rdf:resource=\"PhysicalProcess0000\"/>\n"
                           "    <bqbiol:isVersionOf rdf:resource=\"https://identifiers.org/opb/opb93864\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"Mod4\">\n"
                           "    <semsim:hasPhysicalEntityReference rdf:resource=\"Entity8\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"PhysicalProcess0000\">\n"
                           "    <semsim:hasMediatorParticipant rdf:resource=\"Mod4\"/>\n"
                           "    <semsim:hasSinkParticipant rdf:resource=\"Sink9\"/>\n"
                           "    <semsim:hasSourceParticipant rdf:resource=\"Source1\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"Sink9\">\n"
                           "    <semsim:hasMultiplier rdf:datatype=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#http://www.w3.org/2001/XMLSchema#double\">1</semsim:hasMultiplier>\n"
                           "    <semsim:hasPhysicalEntityReference rdf:resource=\"Entity8\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"Source1\">\n"
                           "    <semsim:hasMultiplier rdf:datatype=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#http://www.w3.org/2001/XMLSchema#double\">1</semsim:hasMultiplier>\n"
                           "    <semsim:hasPhysicalEntityReference rdf:resource=\"Entity8\"/>\n"
                           "  </rdf:Description>\n"
                           "</rdf:RDF>\n";
    std::cout << actual << std::endl;
    ASSERT_STREQ(expected.c_str(), actual);

    Editor_delete(editor_ptr);
    PhysicalProcess_delete(physical_process_ptr);
    free_c_char_star(actual);
    RDF_delete(rdf_ptr);
}

TEST_F(CAPITests, TestPhysicalForce) {
    RDF *rdf_ptr = RDF_new();
    Editor *editor_ptr = rdf_ptr->toEditorPtr(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML
    );
    PhysicalForce *physical_force_ptr = PhysicalForce_new(editor_ptr);

    physical_force_ptr = PhysicalForce_setAbout(physical_force_ptr, "Metaid0937");
    physical_force_ptr = PhysicalForce_setPhysicalProperty(physical_force_ptr, "opb/opb93864");
    physical_force_ptr = PhysicalForce_addSink(
            physical_force_ptr, "Sink9", 1.0, "Entity8");
    physical_force_ptr = PhysicalForce_addSource(
            physical_force_ptr, "Source1", 1.0, "Entity9");

    char *actual = PhysicalForce_str(physical_force_ptr, "rdfxml-abbrev", "./Annot.rdf");
    std::string expected = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                           "<rdf:RDF xmlns:bqbiol=\"http://biomodels.net/biology-qualifiers/\"\n"
                           "   xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
                           "   xmlns:semsim=\"http://www.bhi.washington.edu/semsim#\"\n"
                           "   xml:base=\"file://./Annot.rdf\">\n"
                           "  <rdf:Description rdf:about=\"Metaid0937\">\n"
                           "    <bqbiol:isPropertyOf rdf:resource=\"PhysicalForce0000\"/>\n"
                           "    <bqbiol:isVersionOf rdf:resource=\"https://identifiers.org/opb/opb93864\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"PhysicalForce0000\">\n"
                           "    <semsim:hasSinkParticipant rdf:resource=\"Sink9\"/>\n"
                           "    <semsim:hasSourceParticipant rdf:resource=\"Source1\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"Sink9\">\n"
                           "    <semsim:hasMultiplier rdf:datatype=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#http://www.w3.org/2001/XMLSchema#double\">1</semsim:hasMultiplier>\n"
                           "    <semsim:hasPhysicalEntityReference rdf:resource=\"Entity8\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"Source1\">\n"
                           "    <semsim:hasMultiplier rdf:datatype=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#http://www.w3.org/2001/XMLSchema#double\">1</semsim:hasMultiplier>\n"
                           "    <semsim:hasPhysicalEntityReference rdf:resource=\"Entity9\"/>\n"
                           "  </rdf:Description>\n"
                           "</rdf:RDF>\n"
                           "";
    std::cout << actual << std::endl;
    ASSERT_STREQ(expected.c_str(), actual);


    RDF_delete(rdf_ptr);
    Editor_delete(editor_ptr);
    PhysicalForce_delete(physical_force_ptr);
    free_c_char_star(actual);
}

TEST_F(CAPITests, TestEditorToRDF) {
    RDF *rdf_ptr = RDF_new();
    Editor *editor_ptr = rdf_ptr->toEditorPtr(
            SBMLFactory::getSBMLString(SBML_NOT_ANNOTATED),
            SEMSIM_TYPE_SBML
    );
    PhysicalProcess *physical_process_ptr = PhysicalProcess_new(editor_ptr);

    physical_process_ptr = PhysicalProcess_setAbout(physical_process_ptr, "SemsimMetaid0006");
    physical_process_ptr = PhysicalProcess_setPhysicalProperty(physical_process_ptr, "opb/opb93864");
    physical_process_ptr = PhysicalProcess_addSink(
            physical_process_ptr, "Sink9", 1.0, "Entity8");
    physical_process_ptr = PhysicalProcess_addSource(
            physical_process_ptr, "Source1", 1.0, "Entity8");
    physical_process_ptr = PhysicalProcess_addMediator(
            physical_process_ptr, "Mod4", 1.0, "Entity8");

    PhysicalEntity *physical_entity_ptr = PhysicalEntity_new(editor_ptr);
    physical_entity_ptr = PhysicalEntity_setPhysicalProperty(physical_entity_ptr, "opb/opb_465");
    physical_entity_ptr = PhysicalEntity_setAbout(physical_entity_ptr, "SemsimMetaid0007");
    physical_entity_ptr = PhysicalEntity_setIdentity(physical_entity_ptr, "uniprot/PD7363");
    physical_entity_ptr = PhysicalEntity_addLocation(physical_entity_ptr, "FMA:fma:8376");
    physical_entity_ptr = PhysicalEntity_addLocation(physical_entity_ptr, "FMA:fma:8377");
    physical_entity_ptr = PhysicalEntity_addLocation(physical_entity_ptr, "FMA:fma:8378");

    PhysicalForce *physical_force_ptr = PhysicalForce_new(editor_ptr);

    physical_force_ptr = PhysicalForce_setAbout(physical_force_ptr, "SemsimMetaid0008");
    physical_force_ptr = PhysicalForce_setPhysicalProperty(physical_force_ptr, "opb/opb93864");
    physical_force_ptr = PhysicalForce_addSink(
            physical_force_ptr, "Sink9", 1.0, "Entity8");
    physical_force_ptr = PhysicalForce_addSource(
            physical_force_ptr, "Source1", 1.0, "Entity9");


    Editor_addPhysicalProcess(editor_ptr, physical_process_ptr
    );
    Editor_addPhysicalEntity(editor_ptr, physical_entity_ptr
    );
    Editor_addPhysicalForce(editor_ptr, physical_force_ptr
    );
    Editor_toRDF(editor_ptr);

    std::string expected = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                           "<rdf:RDF xmlns:bqbiol=\"http://biomodels.net/biology-qualifiers/\"\n"
                           "   xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
                           "   xmlns:semsim=\"http://www.bhi.washington.edu/semsim#\"\n"
                           "   xml:base=\"file://./Annot.rdf\">\n"
                           "  <rdf:Description rdf:about=\"SemsimMetaid0006\">\n"
                           "    <bqbiol:isPropertyOf rdf:resource=\"PhysicalProcess0000\"/>\n"
                           "    <bqbiol:isVersionOf rdf:resource=\"https://identifiers.org/opb/opb93864\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"SemsimMetaid0007\">\n"
                           "    <bqbiol:isPropertyOf rdf:resource=\"PhysicalForce0000\"/>\n"
                           "    <bqbiol:isVersionOf rdf:resource=\"https://identifiers.org/opb/opb93864\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"Mod4\">\n"
                           "    <semsim:hasPhysicalEntityReference rdf:resource=\"Entity8\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"PhysicalEntity0000\">\n"
                           "    <bqbiol:is rdf:resource=\"https://identifiers.org/uniprot/PD7363\"/>\n"
                           "    <bqbiol:isPartOf rdf:resource=\"https://identifiers.org/FMA/fma:8376\"/>\n"
                           "    <bqbiol:isPartOf rdf:resource=\"https://identifiers.org/FMA/fma:8377\"/>\n"
                           "    <bqbiol:isPartOf rdf:resource=\"https://identifiers.org/FMA/fma:8378\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"PhysicalForce0000\">\n"
                           "    <semsim:hasSinkParticipant rdf:resource=\"Sink9\"/>\n"
                           "    <semsim:hasSourceParticipant rdf:resource=\"Source1\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"PhysicalProcess0000\">\n"
                           "    <semsim:hasMediatorParticipant rdf:resource=\"Mod4\"/>\n"
                           "    <semsim:hasSinkParticipant rdf:resource=\"Sink9\"/>\n"
                           "    <semsim:hasSourceParticipant rdf:resource=\"Source1\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"Sink9\">\n"
                           "    <semsim:hasMultiplier rdf:datatype=\"http://www.w3.org/2001/XMLSchema#double\">1</semsim:hasMultiplier>\n"
                           "    <semsim:hasPhysicalEntityReference rdf:resource=\"Entity8\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"Source1\">\n"
                           "    <semsim:hasMultiplier rdf:datatype=\"http://www.w3.org/2001/XMLSchema#double\">1</semsim:hasMultiplier>\n"
                           "    <semsim:hasPhysicalEntityReference rdf:resource=\"Entity8\"/>\n"
                           "    <semsim:hasPhysicalEntityReference rdf:resource=\"Entity9\"/>\n"
                           "  </rdf:Description>\n"
                           "  <rdf:Description rdf:about=\"SemsimMetaid0008\">\n"
                           "    <bqbiol:isPropertyOf rdf:resource=\"PhysicalEntity0000\"/>\n"
                           "    <bqbiol:isVersionOf rdf:resource=\"https://identifiers.org/opb/opb_465\"/>\n"
                           "  </rdf:Description>\n"
                           "</rdf:RDF>\n";
    char *actual = RDF_toString(rdf_ptr, "rdfxml-abbrev", "./Annot.rdf");
    std::cout << actual << std::endl;


    Editor_delete(editor_ptr);
    PhysicalEntity_delete(physical_entity_ptr);
    PhysicalProcess_delete(physical_process_ptr);
    PhysicalForce_delete(physical_force_ptr);
    free_c_char_star(actual);
    RDF_delete(rdf_ptr);

}

/*
 * todo support for equality operators
 */

TEST_F(CAPITests, TestRDFTwice1) {
    RDF *rdf_ptr1 = RDF_new();
    RDF *rdf_ptr2 = RDF_new();
    ASSERT_NE(rdf_ptr1, rdf_ptr2);
    RDF_delete(rdf_ptr1);
    RDF_delete(rdf_ptr2);
}

TEST_F(CAPITests, TestRDFTwice2) {
    RDF *rdf_ptr1 = RDF_new();
    RDF *rdf_ptr2 = RDF_new();
    ASSERT_NE(rdf_ptr1->getModel(), rdf_ptr2->getModel());
    RDF_delete(rdf_ptr1);
    RDF_delete(rdf_ptr2);
}

TEST_F(CAPITests, TestRDFTwice3) {
    RDF *rdf_ptr1 = RDF_new();
    RDF *rdf_ptr2 = RDF_new();
    ASSERT_NE(rdf_ptr1->getStorage(), rdf_ptr2->getStorage());
    RDF_delete(rdf_ptr1);
    RDF_delete(rdf_ptr2);
}


TEST_F(CAPITests, RDF_fromString2) {
    RDF *rdf_ptr = RDF_fromString(samples.composite_annotation_pf.c_str(), "rdfxml", "RDF_fromStringTest.rdf");
    int expected = 6;
    int actual = RDF_size(rdf_ptr);
    ASSERT_EQ(expected, actual);
    RDF_delete(rdf_ptr);
}

TEST_F(CAPITests, RDF_addFromString) {
    RDF *rdf_ptr = RDF_new();
    RDF_addFromString(rdf_ptr, samples.singular_annotation1.c_str(), "rdfxml", "RDF_addFromStringTest.rdf");
    int expected = 1;
    int actual = RDF_size(rdf_ptr);
    ASSERT_EQ(expected, actual);
    RDF_delete(rdf_ptr);
}

TEST_F(CAPITests, RDF_fromUri) {
    RDF *rdf_ptr = RDF_fromUri(samples.sbml_url1.c_str(), "rdfxml");
    int expected = 277;
    int actual = RDF_size(rdf_ptr);
    ASSERT_EQ(expected, actual);
    RDF_delete(rdf_ptr);
}

TEST_F(CAPITests, RDF_addFromUri) {
    RDF *rdf_ptr = RDF_new();
    RDF_addFromUri(rdf_ptr, samples.sbml_url1.c_str(), "rdfxml");
    int expected = 277;
    int actual = RDF_size(rdf_ptr);
    ASSERT_EQ(expected, actual);
    RDF_delete(rdf_ptr);
}

TEST_F(CAPITests, RDF_fromFile) {
    // we can cheat and use C++ to write the file we need - who's counting
    std::string fname = std::experimental::filesystem::current_path().string() + "/TestParseFromFile.rdf";
    std::cout << fname << std::endl;
    std::ofstream f(fname);
    if (f.is_open()) {
        f << samples.composite_annotation_pe << std::endl;
        f.flush();
        f.close();
    } else {
        throw std::logic_error("No file was opened for test");
    }

    RDF *rdf_ptr = RDF_fromFile(fname.c_str(), "rdfxml");
    int expected = 4;
    int actual = RDF_size(rdf_ptr);
    ASSERT_EQ(expected, actual);
    std::remove(fname.c_str());
    RDF_delete(rdf_ptr);

}

TEST_F(CAPITests, RDF_addFromFile) {
    // we can cheat and use C++ to write the file we need - who's counting
    std::string fname = std::experimental::filesystem::current_path().string() + "/TestParseFromFile.rdf";
    std::cout << fname << std::endl;
    std::ofstream f(fname);
    if (f.is_open()) {
        f << samples.composite_annotation_pe << std::endl;
        f.flush();
        f.close();
    } else {
        throw std::logic_error("No file was opened for test");
    }

    RDF *rdf_ptr = RDF_new();
    RDF_addFromFile(rdf_ptr, fname.c_str(), "rdfxml");
    int expected = 4;
    int actual = RDF_size(rdf_ptr);
    ASSERT_EQ(expected, actual);
    std::remove(fname.c_str());
    RDF_delete(rdf_ptr);
}






































