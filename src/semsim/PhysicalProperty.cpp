//
// Created by Ciaran on 4/22/2020.
//

#include "PhysicalProperty.h"

#include <utility>


namespace semsim {

    PhysicalProperty::PhysicalProperty(std::string subject_str, std::string resource_str)
            : subject_(std::move(subject_str)),
              resource_(std::move(resource_str)){
        validate();
    }

    void PhysicalProperty::validate() {
        std::string expected_prefix1 = "https://identifiers.org/OPB";
        std::string expected_prefix2 = "https://identifiers.org/opb";

        // create a temporary resource obj for checking only.
        Resource r(LibrdfNode::fromUriString(resource_));
        std::string uri = r.str();
        r.free();
        if ((uri.rfind(expected_prefix1, 0) != 0) && (uri.rfind(expected_prefix2, 0) != 0)) {
            std::ostringstream err;
            err << __FILE__ << ":" << __LINE__ << ": The uri generated (";
            err << uri << ") is not a OPB resource. Physical properties "
                          "must be OPB resources.";
            throw semsim::InappropriateResourceException(err.str());
        }
    }

    Triple PhysicalProperty::isVersionOfTriple() const {
        return Triple(
                LibrdfNode::fromUriString(subject_).get(),
                BiomodelsBiologyQualifier("isVersionOf").getNode(),
                Resource(LibrdfNode::fromUriString(resource_)).getNode()
        );
    }

    Triple PhysicalProperty::isVersionOfTriple(const Subject &subject_metaid) {
        Triple triple(
                subject_metaid.getNode(),
                BiomodelsBiologyQualifier("isVersionOf").getNode(),
                Resource(LibrdfNode::fromUriString(resource_)).getNode()
        );
        // The node_ object is used up - ownership responsibility
        // is passed on to the Triple.
//        node_ = nullptr;
        return triple;
    }

    Triple PhysicalProperty::isPropertyOfTriple(const std::string& property_metaid) const {
        return Triple(
                LibrdfNode::fromUriString(subject_).get(),
                BiomodelsBiologyQualifier("isPropertyOf").getNode(),
                LibrdfNode::fromUriString(property_metaid).get()
        );
    }

    Triple PhysicalProperty::isPropertyOfTriple(const Subject &subject_metaid, const std::string& property_metaid) {
        return Triple(
                subject_metaid.getNode(),
                BiomodelsBiologyQualifier("isPropertyOf").getNode(),
                LibrdfNode::fromUriString(property_metaid).get()
        );
    }

    Triples PhysicalProperty::toTriples(const std::string& property_metaid) const {
        Triple v = isVersionOfTriple();
        Triple p = isPropertyOfTriple(property_metaid);
        Triples triples;
        triples.move_back(v);
        triples.move_back(p);
        return triples;
    }

    const std::string &PhysicalProperty::getSubjectStr() const {
        return subject_;
    }

    void PhysicalProperty::setSubject(const std::string &subject) {
        subject_ = subject;
    }

    const std::string &PhysicalProperty::getResourceStr() const {
        return resource_;
    }

    void PhysicalProperty::setResource(const std::string &resource) {
        resource_ = resource;
    }

//    Triples PhysicalProperty::toTriples(Subject subject_metaid) {
//        Triple v = isVersionOfTriple(subject_metaid);
//        Triple p = isPropertyOfTriple(subject_metaid);
//
//        /*
//         * Here we have used subject_metaid twice. Both times
//         * the same node is being used but this is happening
//         * outside the scope of librdf functions (i.e. in this function)
//         * The librdf functinos would add 1 to the internal reference
//         * counter of the node.
//         * While a bit of a hack, here we add 1 to the node usage
//         * manually so that we do not get a access violation error
//         * later.
//         * todo: find a better solution to this problem.
//         */
//        subject_metaid.getNode()->usage++;
//
//        Triples triples;
//        triples.move_back(v);
//        triples.move_back(p);
//        return triples;
//    }



}