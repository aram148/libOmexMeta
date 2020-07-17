//
// Created by Ciaran on 4/22/2020.
//

#include "omexmeta/PhysicalForce.h"

#include <utility>

#include <utility>


namespace omexmeta {

    PhysicalForce::PhysicalForce(librdf_model *model, std::string local_uri, PhysicalProperty physicalProperty,
                                 Sources sources,
                                 Sinks sinks)
            : PhysicalPhenomenon(model, local_uri, std::move(physicalProperty), PHYSICAL_PROCESS),
              sources_(std::move(sources)), sinks_(std::move(sinks)) {

    }

    const std::vector<SourceParticipant> &PhysicalForce::getSources() const {
        return sources_;
    }

    const std::vector<SinkParticipant> &PhysicalForce::getSinks() const {
        return sinks_;
    }

    std::string PhysicalForce::createMetaId() const {
        return generateMetaId("PhysicalForce");
    }

    Triples PhysicalForce::toTriples() {
        if (getAbout().empty()) {
            throw AnnotationBuilderException(
                    "PhysicalForce::toTriples(): Cannot create"
                    " triples because the \"about\" information is not set. "
                    "Use the setAbout() method."
            );
        }
        if (getPhysicalProperty().getResourceStr().empty()) {
            throw AnnotationBuilderException(
                    "PhysicalForce::toTriples(): Cannot create"
                    " triples because the \"physical_property resource\" information is not set. "
                    "Use the setPhysicalProperty() method."
            );
        }

        if (physical_property_id_.empty()) {
            physical_property_id_ = OmexMetaUtils::generateUniqueMetaid(
                    model_, "PhysicalForce",
                    std::vector<std::string>());

        }

        Triples triples = physical_property_.toTriples(physical_property_id_);

        // now we add the local uri on to the metaid - If it already
        // properly formatted it will be left alone
        physical_property_id_ = OmexMetaUtils::addLocalPrefixToMetaid(physical_property_id_, getLocalUri());

        for (auto &source : sources_) {
            for (auto &triple : source.toTriples(physical_property_id_)) {
                triples.move_back(triple);
            }
        }
        for (auto &sink : sinks_) {
            for (auto &triple : sink.toTriples(physical_property_id_)) {
                triples.move_back(triple);
            }
        }
        return triples;
    }

//    PhysicalForce &PhysicalForce::setAbout(std::string metaid) {
//        physical_property_.setSubject(metaid);
//        return (*this);
//    }

    PhysicalForce &PhysicalForce::setPhysicalProperty(PhysicalProperty physicalProperty) {
        physical_property_ = std::move(physicalProperty);
        return (*this);
    }

    PhysicalForce &PhysicalForce::setPhysicalProperty(std::string subject_metaid, std::string physical_property) {
        subject_metaid = OmexMetaUtils::addLocalPrefixToMetaid(subject_metaid, getLocalUri());
        physical_property_ = PhysicalProperty(std::move(subject_metaid), std::move(physical_property), getLocalUri());
        return *this;
    }

    PhysicalForce &PhysicalForce::addSource(double multiplier, const std::string &physical_entity_reference) {
        sources_.push_back(
                std::move(SourceParticipant(
                        model_, multiplier, physical_entity_reference, getLocalUri()
                ))
        );
        return (*this);
    }

    PhysicalForce &PhysicalForce::addSink(double multiplier, const std::string &physical_entity_reference) {
        sinks_.push_back(
                SinkParticipant(model_, multiplier, physical_entity_reference, getLocalUri())
        );

        return (*this);
    }

    PhysicalForce::PhysicalForce(librdf_model *model)
            : PhysicalPhenomenon(model) {}

    PhysicalForce::PhysicalForce(librdf_model *model, const std::string& local_uri)
            : PhysicalPhenomenon(model, local_uri) {}

    int PhysicalForce::getNumSources() {
        return sources_.size();
    }

    int PhysicalForce::getNumSinks() {
        return sinks_.size();
    }

    void PhysicalForce::free() {

        for (auto &i : sources_) {
            i.free();
        }
        for (auto &i : sinks_) {
            i.free();
        }
    }

    bool PhysicalForce::operator==(const PhysicalForce &rhs) const {
        return static_cast<const omexmeta::PhysicalPhenomenon &>(*this) ==
               static_cast<const omexmeta::PhysicalPhenomenon &>(rhs) &&
               sources_ == rhs.sources_ &&
               sinks_ == rhs.sinks_;
    }

    bool PhysicalForce::operator!=(const PhysicalForce &rhs) const {
        return !(rhs == *this);
    }

}