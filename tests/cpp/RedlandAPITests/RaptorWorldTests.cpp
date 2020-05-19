#include "gtest/gtest.h"
#include "semsim/SemsimUtils.h"
#include "semsim/RelandAPIWrappers/RaptorWorld.h"
#include "semsim/RelandAPIWrappers/LibrdfWorld.h"

class RaptorWorldTests : public ::testing::Test {

public:

    RaptorWorldTests() = default;

};
//
//TEST_F(RaptorWorldTests, TestCopyConstructor) {
//    HERE();
//    semsim::LibrdfWorld librdfWorld;
//    HERE();
//    semsim::RaptorWorld raptorWorld1 = librdfWorld.getRaptor();
//    HERE();
//    semsim::RaptorWorld raptorWorld2 = raptorWorld1;
//    HERE();
//    ASSERT_EQ(raptorWorld1, raptorWorld2);
//}
//
//TEST_F(RaptorWorldTests, TestMoveConstructor) {
//    semsim::LibrdfWorld librdfWorld;
//    semsim::RaptorWorld raptorWorld1 = librdfWorld.getRaptor();
//    semsim::RaptorWorld raptorWorld2 = std::move(raptorWorld1);
//    ASSERT_NE(raptorWorld1, raptorWorld2);
//}
//
//TEST_F(RaptorWorldTests, TestCopyAssignment) {
//    semsim::LibrdfWorld librdfWorld;
//    semsim::RaptorWorld raptorWorld1 = librdfWorld.getRaptor();
//    semsim::RaptorWorld raptorWorld2 = librdfWorld.getRaptor();
//    raptorWorld2 = raptorWorld1;
//    ASSERT_EQ(raptorWorld1.getRaptorWorld(), raptorWorld2.getRaptorWorld());
//    ASSERT_EQ(raptorWorld1, raptorWorld2);
//}
//
//TEST_F(RaptorWorldTests, TestMoveAssignment) {
//    semsim::LibrdfWorld librdfWorld;
//    semsim::RaptorWorld raptorWorld1 = librdfWorld.getRaptor();
//    semsim::RaptorWorld raptorWorld2 = librdfWorld.getRaptor();
//    raptorWorld2 = std::move(raptorWorld1);
//    ASSERT_NE(raptorWorld1, raptorWorld2);
//}



