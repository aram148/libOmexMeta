//
// Created by Ciaran on 4/4/2020.
//

#include "AnnotationSamples.h"
#include "OmexMetaTestUtils.h"
#include "omexmeta/Predicate.h"
#include "omexmeta/Resource.h"
#include "omexmeta/Subject.h"
#include "omexmeta/Triple.h"
#include "gtest/gtest.h"

using namespace omexmeta;

class TripleTests : public ::testing::Test {
public:
    AnnotationSamples samples;
    std::string subject_str = "./NewModel#metaid_0";
    std::string predicate_str = "http://biomodels.net/biology-qualifiers/is";
    std::string resource_namespace = "uniprot";
    std::string resource_id = "P0DP23";

    Subject subject;
    Resource resource;
    BiomodelsBiologyQualifier predicate;


    //todo subject could pass the world_ to the node
    TripleTests() {
        subject = Subject::fromRawPtr(LibrdfNode::fromUriString(subject_str).get());
        resource = Resource::fromRawPtr(LibrdfNode::fromUriString(resource_namespace + "/" + resource_id).get());
        predicate = BiomodelsBiologyQualifier("is");
    }
};

TEST_F(TripleTests, TestInstantiation1) {
    Triple triple(subject.getNode(), predicate.getNode(), resource.getNode());
    ASSERT_TRUE(true);// if we get this far the test has passed
    triple.freeStatement();
}

TEST(TripleTestsNoFixture, TestInstantiationEmptyForBuilder) {
    // when we use builder interface, Triple is created empty and we fill in the bits of information  from user input
    Triple triple;
    ASSERT_TRUE(triple.isEmpty());
    // remember to free the unused resources from test fixture
    triple.freeStatement();
}

TEST_F(TripleTests, TestInstantiation2) {
    Triple triple(subject,
                  std::make_shared<Predicate>(predicate),
                  resource);
    ASSERT_TRUE(true);// if we get this far the test has passed
    triple.freeStatement();
}

TEST_F(TripleTests, TestSubjectString) {
    Triple triple(subject.getNode(), predicate.getNode(), resource.getNode());
    std::string &expected = subject_str;
    librdf_node *node = triple.getSubject();
    librdf_uri *uri = librdf_node_get_uri(node);
    const char *actual = (const char *) librdf_uri_as_string(uri);
    ASSERT_STREQ(expected.c_str(), actual);
    triple.freeStatement();
}


TEST_F(TripleTests, TestSubjectStr2) {
    Triple triple(subject.getNode(), predicate.getNode(), resource.getNode());
    std::string &expected = subject_str;
    ASSERT_STREQ(expected.c_str(), triple.getSubjectStr().c_str());
    triple.freeStatement();
}

TEST_F(TripleTests, TestPredicate1) {
    Triple triple(subject.getNode(), predicate.getNode(), resource.getNode());
    std::string expected = predicate_str;
    ASSERT_STREQ(expected.c_str(), triple.getPredicateStr().c_str());
    triple.freeStatement();
}


TEST_F(TripleTests, TestResource) {
    Triple triple(subject.getNode(), predicate.getNode(), resource.getNode());
    std::string actual = triple.getResourceStr();
    std::string expected = resource_id;
    ASSERT_STREQ(expected.c_str(), resource_id.c_str());
    triple.freeStatement();
}

TEST_F(TripleTests, TestStatementPred) {
    Triple triple(subject.getNode(), predicate.getNode(), resource.getNode());
    librdf_statement *statement = triple.get();
    librdf_node *n = librdf_statement_get_predicate(statement);
    librdf_uri *uri = librdf_node_get_uri(n);
    unsigned char *s = librdf_uri_as_string(uri);
    std::string expected = "http://biomodels.net/biology-qualifiers/is";
    ASSERT_STREQ(expected.c_str(), (const char *) s);
    triple.freeStatement();
}

TEST(TripleTestsNoFixture, TestEquality) {
    Triple triple1(
            LibrdfNode::fromUriString("subject"),
            LibrdfNode::fromUriString("predicate"),
            LibrdfNode::fromUriString("resource"));
    Triple triple2(
            LibrdfNode::fromUriString("subject"),
            LibrdfNode::fromUriString("predicate"),
            LibrdfNode::fromUriString("resource"));
    ASSERT_EQ(triple1, triple2);
    triple2.freeStatement();
    triple1.freeStatement();
}

TEST(TripleTestsNoFixture, TestInequality) {
    Triple triple1(
            LibrdfNode::fromUriString("subject1"),
            LibrdfNode::fromUriString("predicate1"),
            LibrdfNode::fromUriString("resource1"));
    Triple triple2(
            LibrdfNode::fromUriString("subject2"),
            LibrdfNode::fromUriString("predicate2"),
            LibrdfNode::fromUriString("resource2"));
    ASSERT_NE(triple1, triple2);
    triple2.freeStatement();
    triple1.freeStatement();
}

TEST_F(TripleTests, TestStatementResource) {
    Triple triple(subject.getNode(), predicate.getNode(), resource.getNode());
    librdf_statement *statement = triple.get();
    librdf_node *n = librdf_statement_get_object(statement);
    librdf_uri *uri = librdf_node_get_uri(n);
    unsigned char *s = librdf_uri_as_string(uri);
    std::string expected = "https://identifiers.org/uniprot/P0DP23";
    ASSERT_STREQ(expected.c_str(), (const char *) s);
    triple.freeStatement();
}


TEST(TripleTestsNoFixture, TestAboutTwoArguments) {
    Triple triple;
    triple.about("http://omex-library/myomex.omex", "mymodel.xml", "metaid2");
    std::string expected = "http://omex-library/myomex.omex/mymodel.xml#metaid2";
    std::string actual = triple.getAbout();
    ASSERT_STREQ(expected.c_str(), actual.c_str());
    triple.freeStatement();
}

TEST(TripleTestsNoFixture, TestAboutOneArgumentWithSetLocal) {
    Triple triple;
    triple.setModelUri("http://omex-library.org/omex.omex/model.xml");
    triple.about("metaid2");
    std::string expected = "http://omex-library.org/omex.omex/model.xml#metaid2";
    std::string actual = triple.getAbout();
    ASSERT_STREQ(expected.c_str(), actual.c_str());
    triple.freeStatement();
}

TEST(TripleTestsNoFixture, TestAboutAgain2) {
    Triple triple;

    triple.setModelUri("http://omex-library.org/omex.omex/model.xml");
    triple.about("#metaid3");
    std::string expected = "http://omex-library.org/omex.omex/model.xml#metaid3";
    std::string actual = triple.getAbout();
    ASSERT_STREQ(expected.c_str(), actual.c_str());
    triple.freeStatement();
}


TEST(TripleTestsNoFixture, TestSetPredicate) {
    Triple triple;
    triple.setPredicate("bqbiol", "is");
    std::string expected = "http://biomodels.net/biology-qualifiers/is";
    ASSERT_STREQ(expected.c_str(), triple.getPredicateStr().c_str());
    triple.freeStatement();
}


TEST(TripleTestsNoFixture, TestResourceLiteral) {
    Triple triple;
    triple.setResourceLiteral("Annotating");
    std::string expected = "Annotating";
    std::string actual = triple.getResourceStr();
    ASSERT_STREQ(expected.c_str(), actual.c_str());
    triple.freeStatement();
}

TEST(TripleTestsNoFixture, TestResourceUri) {
    Triple triple;
    triple.setResourceUri("AnnotatingUri");
    std::string expected = "AnnotatingUri";
    ASSERT_STREQ(expected.c_str(), triple.getResourceStr().c_str());
    triple.freeStatement();
}

TEST(TripleTestsNoFixture, TestResourceBlank) {
    Triple triple;
    triple.setResourceBlank("AnnotatingBlank");
    std::string expected = "AnnotatingBlank";
    ASSERT_STREQ(expected.c_str(), triple.getResourceStr().c_str());
    triple.freeStatement();
}

TEST_F(TripleTests, TestStatementSubject) {
    Triple triple(subject.getNode(), predicate.getNode(), resource.getNode());
    librdf_statement *statement = triple.get();
    librdf_node *n = librdf_statement_get_subject(statement);
    librdf_uri *uri = librdf_node_get_uri(n);
    unsigned char *s = librdf_uri_as_string(uri);
    std::string expected = "./NewModel#metaid_0";
    ASSERT_STREQ(expected.c_str(), (const char *) s);
    triple.freeStatement();
}

/*
 * does the statement only get built when all three nodes are not null??
 *
 */

TEST_F(TripleTests, TestBuilderPattern1) {
    Triple triple;
    //    triple.setLocalUri("http://omex-library.org/NewOmex.omex/NewModel.rdf");
    triple.setModelUri("http://omex-library.org/NewOmex.omex/NewModel.xml");
    triple.about("#metaid1")
            .setPredicate("bqbiol", "is")
            .setResourceUri("uniprot/PD4034");

    std::string actual = triple.str();
    std::cout << actual << std::endl;
    std::string expected = "@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n"
                           "@prefix bqbiol: <http://biomodels.net/biology-qualifiers/> .\n"
                           "@prefix OMEXlib: <http://omex-library.org/> .\n"
                           "@prefix myOMEX: <http://omex-library.org/NewOmex.omex/> .\n"
                           "@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .\n"
                           "\n"
                           "<http://omex-library.org/NewOmex.omex/NewModel.xml#metaid1>\n"
                           "    bqbiol:is <https://identifiers.org/uniprot/PD4034> .\n"
                           "\n";
    ASSERT_STREQ(expected.c_str(), actual.c_str());
    triple.freeStatement();

    // Aaand free the excess nodes
    predicate.freeNode();
    subject.free();
    resource.free();
}

TEST_F(TripleTests, TestBuilderPattern2) {
    Triple triple;
    triple.setModelUri("http://omex-library.org/omex.omex/model.xml#");
    triple.about("#metaid00001")
            .setPredicate("bqbiol", "is")
            .setResourceBlank("Blank");

    std::string actual = triple.str();
    std::cout << actual << std::endl;
    std::string expected = "@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n"
                           "@prefix bqbiol: <http://biomodels.net/biology-qualifiers/> .\n"
                           "@prefix OMEXlib: <http://omex-library.org/> .\n"
                           "@prefix myOMEX: <http://omex-library.org/NewOmex.omex/> .\n"
                           "@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .\n"
                           "\n"
                           "<http://omex-library.org/omex.omex/model.xml#metaid00001>\n"
                           "    bqbiol:is _:Blank .\n"
                           "\n"
                           "";
    ASSERT_STREQ(expected.c_str(), actual.c_str());
    triple.freeStatement();

    // Aaand free the excess nodes
    predicate.freeNode();
    subject.free();
    resource.free();
}


class TripleTestsVector : public ::testing::Test {

public:
    AnnotationSamples samples;
    std::string subject_str = "./NewModel#metaid_0";
    std::string predicate_str = "http://biomodels.net/biology-qualifiers/is";
    std::string resource_namespace = "uniprot";
    std::string resource_id = "P0DP23";

    Subject subject;
    Resource resource;
    BiomodelsBiologyQualifier predicate;


    //todo subject could pass the world_ to the node
    TripleTestsVector() {
        subject = Subject::fromRawPtr(LibrdfNode::fromUriString(subject_str).get());
        resource = Resource::fromRawPtr(LibrdfNode::fromUriString(resource_namespace + "/" + resource_id).get());
        predicate = BiomodelsBiologyQualifier("is");
    }
};

///*
// * Here we make a copy of triple and put it into vector.
// * Hence, we need to free both to prevent memory leak
// */
//TEST_F(TripleTestsVector, TestTripleVecCopyRaw) {
//    Triple triple(subject.getNode(), predicate.getNode(), resource.getNode());
//    std::vector<Triple> vec;
//    vec.push_back(triple);
//    std::string actual = vec[0].getIsVersionOfValue();
//    std::string expected = "https://identifiers.org/uniprot/P0DP23";
//    ASSERT_STREQ(expected.c_str(), actual.c_str());
//    triple.freeStatement();
//    vec[0].freeStatement();
//}

/*
 * Now we have moved the triple into a vector, we
 * must free the statement from the vector, not the
 * original triple
 */
TEST_F(TripleTestsVector, TestTripleVecMove) {
    Triple triple(subject.getNode(), predicate.getNode(), resource.getNode());
    std::vector<Triple> vec;
    vec.push_back(std::move(triple));
    std::string actual = vec[0].getResourceStr();
    std::string expected = "https://identifiers.org/uniprot/P0DP23";
    ASSERT_STREQ(expected.c_str(), actual.c_str());
    vec[0].freeStatement();
}


class TestTripleTwice : public ::testing::Test {
    /*
     * A collection of test to try and figure out
     * why I cannot create two Triples objects from a
     * PhysicalEntity and free them both. Here I try to
     * reproduce the problem by process of elimination.
     */
    TestTripleTwice(){};
};

/*
 * Test two Triple with raw nodes
 */

TEST(TestTripleTwice, WithRawNodes) {
    Triple triple1(
            LibrdfNode::fromUriString("subject").get(),
            LibrdfNode::fromUriString("predicate").get(),
            LibrdfNode::fromUriString("resource").get());
    Triple triple2(
            LibrdfNode::fromUriString("subject").get(),
            LibrdfNode::fromUriString("predicate").get(),
            LibrdfNode::fromUriString("resource").get());
    triple1.freeStatement();
    triple2.freeStatement();
}

/*
 * Now test two triples with Subject Predicate and Resource
 */
TEST(TestTripleTwice, WithSubjectPredAndRes) {
    Triple triple1(
            Subject(LibrdfNode::fromUriString("subject")).getNode(),
            Predicate("uniprot", "PD1234", "uni").getNode(),
            Resource(LibrdfNode::fromUriString("resource")).getNode());
    Triple triple2(
            Subject(LibrdfNode::fromUriString("subject")).getNode(),
            Predicate("uniprot", "PD1234", "uni").getNode(),
            Resource(LibrdfNode::fromUriString("resource")).getNode());
    triple1.freeStatement();
    triple2.freeStatement();
}

/*
 * Try with different predicate
 */
TEST(TestTripleTwice, WithSubjectBiomodelModelQualifierAndRes) {
    Triple triple1(
            Subject(LibrdfNode::fromUriString("subject")).getNode(),
            BiomodelsBiologyQualifier("is").getNode(),
            Resource(LibrdfNode::fromUriString("resource")).getNode());
    Triple triple2(
            Subject(LibrdfNode::fromUriString("subject")).getNode(),
            Predicate("uniprot", "PD1234", "uni").getNode(),
            Resource(LibrdfNode::fromUriString("resource")).getNode());
    triple1.freeStatement();
    triple2.freeStatement();
}


TEST(TestTripleTwice, WithRawNodesWithMovingTheFirst) {
    Triple triple1(
            LibrdfNode::fromUriString("subject").get(),
            LibrdfNode::fromUriString("predicate").get(),
            LibrdfNode::fromUriString("resource").get());
    Triple triple2 = std::move(triple1);
    Triple triple3(
            LibrdfNode::fromUriString("subject").get(),
            LibrdfNode::fromUriString("predicate").get(),
            LibrdfNode::fromUriString("resource").get());
    triple2.freeStatement();
    triple3.freeStatement();
}

TEST(TestTripleTwice, TestUsageSimple) {
    Triple triple1(
            LibrdfNode::fromUriString("subject").get(),
            LibrdfNode::fromUriString("predicate").get(),
            LibrdfNode::fromUriString("resource").get());

    auto usage = triple1.getUsages();
    ASSERT_EQ(1, usage["statement"]);
    ASSERT_EQ(1, usage["subject"]);
    ASSERT_EQ(1, usage["predicate"]);
    ASSERT_EQ(1, usage["resource"]);
    ASSERT_EQ(1, usage["subject_uri"]);
    ASSERT_EQ(1, usage["predicate_uri"]);
    ASSERT_EQ(1, usage["resource_uri"]);
    //    for (auto &it: usage){
    //        std::cout << it.first << ": " << it.second << std::endl;
    //    }

    triple1.freeTriple();
}

TEST(TestTripleTwice, TestUsageTwoUrisTheSame) {
    Triple triple1(
            LibrdfNode::fromUriString("subject").get(),
            LibrdfNode::fromUriString("predicate").get(),
            LibrdfNode::fromUriString("subject").get());

    auto usage = triple1.getUsages();
    ASSERT_EQ(1, usage["statement"]);
    ASSERT_EQ(1, usage["subject"]);
    ASSERT_EQ(1, usage["predicate"]);
    ASSERT_EQ(1, usage["resource"]);
    ASSERT_EQ(2, usage["subject_uri"]);
    ASSERT_EQ(1, usage["predicate_uri"]);
    ASSERT_EQ(2, usage["resource_uri"]);
    triple1.freeTriple();
}

TEST(TestTripleTwice, TestUsageWhenTripleSimple) {
    Triple triple1(
            LibrdfNode::fromUriString("subject").get(),
            LibrdfNode::fromUriString("predicate").get(),
            LibrdfNode::fromUriString("resource").get());

    Triple triple2 = std::move(triple1);

    auto usage = triple2.getUsages();
    ASSERT_EQ(1, usage["statement"]);
    ASSERT_EQ(1, usage["subject"]);
    ASSERT_EQ(1, usage["predicate"]);
    ASSERT_EQ(1, usage["resource"]);
    ASSERT_EQ(1, usage["subject_uri"]);
    ASSERT_EQ(1, usage["predicate_uri"]);
    ASSERT_EQ(1, usage["resource_uri"]);
    triple2.freeTriple();
}

TEST(TestTripleTwice, TestUsageWhenTripleMoved) {
    Triple triple1(
            LibrdfNode::fromUriString("subject").get(),
            LibrdfNode::fromUriString("predicate").get(),
            LibrdfNode::fromUriString("subject").get());

    Triple triple2 = std::move(triple1);

    auto usage = triple2.getUsages();
    ASSERT_EQ(1, usage["statement"]);
    ASSERT_EQ(1, usage["subject"]);
    ASSERT_EQ(1, usage["predicate"]);
    ASSERT_EQ(1, usage["resource"]);
    ASSERT_EQ(2, usage["subject_uri"]);
    ASSERT_EQ(1, usage["predicate_uri"]);
    ASSERT_EQ(2, usage["resource_uri"]);
    triple2.freeTriple();
}
