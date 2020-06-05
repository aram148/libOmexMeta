//
// Created by Ciaran on 5/17/2020.
//
#include "gtest/gtest.h"
#include "World.h"
#include "LibrdfSerializer.h"
#include "iostream"

using namespace redland;

class LibrdfSerializerTests : public ::testing::Test {

public:

    LibrdfSerializerTests() = default;

};

TEST_F(LibrdfSerializerTests, TestInstantiateSerializer) {
    LibrdfSerializer serializer1 = LibrdfSerializer("rdfxml");
    ASSERT_NE(serializer1.get(), nullptr);
}

TEST_F(LibrdfSerializerTests, TestMoveConstructor) {
    LibrdfSerializer serializer1 = LibrdfSerializer("rdfxml");
    auto serializer1_int_ptr = reinterpret_cast<std::uintptr_t>(serializer1.get());
    LibrdfSerializer serializer2 = std::move(serializer1);
    auto serializer2_int_ptr = reinterpret_cast<std::uintptr_t>(serializer2.get());
    ASSERT_EQ(serializer1.get(), nullptr);
    ASSERT_EQ(serializer1_int_ptr, serializer2_int_ptr);
}

TEST_F(LibrdfSerializerTests, TestMoveAssignment) {
    LibrdfSerializer serializer1 = LibrdfSerializer("rdfxml");
    auto serializer1_int_ptr = reinterpret_cast<std::uintptr_t>(serializer1.get());
    LibrdfSerializer serializer2 = LibrdfSerializer("rdfxml");
    auto serializer2_int_ptr = reinterpret_cast<std::uintptr_t>(serializer2.get());
    serializer1 = std::move(serializer2);
    ASSERT_NE(serializer1_int_ptr, serializer2_int_ptr);
    ASSERT_EQ(serializer2.get(), nullptr);
}


TEST_F(LibrdfSerializerTests, TestToString) {
    LibrdfStorage storage;
    LibrdfModel model(storage);
    LibrdfStatement statement = LibrdfStatement::fromRawNodePtrs(
            LibrdfNode::fromUriString("https://subject.com"),
            LibrdfNode::fromUriString("https://predicate.com"),
            LibrdfNode::fromUriString("https://resource.com")
    );
    model.addStatement(statement);
    LibrdfSerializer serializer1 = LibrdfSerializer("rdfxml");
//    LibrdfUri uri("base_uri");
    std::string actual = serializer1.toString("base_uri", model);
    std::string expected = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                           "<rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\" xml:base=\"base_uri\">\n"
                           "  <rdf:Description rdf:about=\"https://subject.com\">\n"
                           "    <ns0:predicate.com xmlns:ns0=\"https://\" rdf:resource=\"https://resource.com\"/>\n"
                           "  </rdf:Description>\n"
                           "</rdf:RDF>\n";
    ASSERT_STREQ(expected.c_str(), actual.c_str());
    model.freeModel();
    storage.freeStorage();
}


TEST_F(LibrdfSerializerTests, TestToStringTurtle) {
    LibrdfStorage storage;
    LibrdfModel model(storage);
    LibrdfStatement statement = LibrdfStatement::fromRawNodePtrs(
            LibrdfNode::fromUriString("https://subject.com"),
            LibrdfNode::fromUriString("https://predicate.com"),
            LibrdfNode::fromUriString("https://resource.com")
    );
    model.addStatement(statement);
    LibrdfSerializer serializer1 = LibrdfSerializer("turtle");
    std::string actual = serializer1.toString("base_uri", model);
    std::string expected = "@base <base_uri> .\n"
                           "@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n"
                           "\n"
                           "<https://subject.com>\n"
                           "    <https://predicate.com> <https://resource.com> .\n\n";
    std::cout << actual << std::endl;
    ASSERT_STREQ(expected.c_str(), actual.c_str());
    model.freeModel();
    storage.freeStorage();
}


TEST_F(LibrdfSerializerTests, TestToStringNTriples) {
    LibrdfStorage storage;
    LibrdfModel model(storage);
    LibrdfStatement statement = LibrdfStatement::fromRawNodePtrs(
            LibrdfNode::fromUriString("https://subject.com"),
            LibrdfNode::fromUriString("https://predicate.com"),
            LibrdfNode::fromUriString("https://resource.com")
    );
    model.addStatement(statement);
    LibrdfSerializer serializer1 = LibrdfSerializer("ntriples");
    std::string actual = serializer1.toString("base_uri", model);
    std::string expected = "<https://subject.com> <https://predicate.com> <https://resource.com> .\n";
    std::cout << actual << std::endl;
    ASSERT_STREQ(expected.c_str(), actual.c_str());
    model.freeModel();
    storage.freeStorage();
}















