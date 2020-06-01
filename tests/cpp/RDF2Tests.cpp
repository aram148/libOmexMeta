//
// Created by Ciaran on 5/31/2020.
//

#include "gtest/gtest.h"
#include "semsim/RDF2.h"
#include "AnnotationSamples.h"

using namespace semsim;

class RDFTests : public ::testing::Test {

public:

    AnnotationSamples samples;

    RDFTests() = default;

    void TearDown() override {
        samples.removeAllFilesIfTheyExist();
    }
};

TEST_F(RDFTests, TestCreateEmptyRDF) {
    RDF2 rdf;
    ASSERT_EQ(0, rdf.size());
    ASSERT_TRUE(rdf.empty());
}

TEST_F(RDFTests, TestFromStringSingularAnnotation) {
    RDF2 rdf = RDF2::fromString(samples.singular_annotation1);
    int expected = 1;
    int actual = rdf.size();
    ASSERT_EQ(expected, actual);
}

TEST_F(RDFTests, TestFromStringTurtleBag) {
    RDF2 rdf = RDF2::fromString(samples.rdf_turtle_bag_example, "turtle");
    int expected = 7;
    int actual = rdf.size();
    ASSERT_EQ(expected, actual);
}

TEST_F(RDFTests, TestToString) {
    RDF2 rdf = RDF2::fromString(samples.rdf_xml_example7, "rdfxml");
    std::string expected = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                           "<rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\">\n"
                           "  <rdf:Description rdf:about=\"http://www.w3.org/TR/rdf-syntax-grammar\">\n"
                           "    <ns1:editor xmlns:ns1=\"http://example.org/stuff/1.0/\">\n"
                           "      <rdf:Description>\n"
                           "        <ns1:fullName>Dave Beckett</ns1:fullName>\n"
                           "        <ns1:homePage rdf:resource=\"http://purl.org/net/dajobe/\"/>\n"
                           "      </rdf:Description>\n"
                           "    </ns1:editor>\n"
                           "    <ns2:title xmlns:ns2=\"http://purl.org/dc/elements/1.1/\">RDF1.1 XML Syntax</ns2:title>\n"
                           "  </rdf:Description>\n"
                           "</rdf:RDF>\n";
    std::string actual = rdf.toString();
    std::cout << actual << std::endl;
    ASSERT_STREQ(expected.c_str(), actual.c_str());
}






























