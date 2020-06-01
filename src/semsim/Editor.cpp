//
// Created by Ciaran on 4/15/2020.
//

#include "semsim/Editor.h"

namespace semsim {
    Editor::Editor(const std::string &xml, SemsimXmlType type,
                           const LibrdfModel& model, NamespaceMap &nsmap)
            : model_(model), namespaces_(nsmap) {
        XmlAssistantPtr xmlAssistantPtr = XmlAssistantFactory::generate(xml, type);
        std::pair<std::string, std::vector<std::string>> xml_and_metaids = xmlAssistantPtr->addMetaIds();
        xml_ = xml_and_metaids.first;
        //todo create a structure mapping metaids to more useful information for the user.
        metaids_ = xml_and_metaids.second;
    }

    const std::string &Editor::getXml() const {
        return xml_;
    }

    const std::vector<std::string> &Editor::getMetaids() const {
        return metaids_;
    }

    const std::unordered_map<std::string, std::string> &Editor::getNamespaces() const {
        return namespaces_;
    }

    void Editor::setNamespaces(const std::unordered_map<std::string, std::string> &namespaces) {
        namespaces_ = namespaces;
    }


    void Editor::checkValidMetaid(const std::string &metaid) {
        if (std::find(metaids_.begin(), metaids_.end(), metaid) == metaids_.end()) {
            std::ostringstream err;
            err << __FILE__ << ":" << __LINE__ << ": metaid \"" << metaid << "\" not found. ";
            err << "These are your available metaids: ";
            for (auto &it: metaids_) {
                err << it << ", ";
            }
            throw std::invalid_argument(err.str());
        }
    }


    const NestedTriples &Editor::getTripleList() const {
        return triple_list_;
    }

//    void Editor::extractNamespacesFromTriplesVector(Triples triples) {
//        for (auto &triple: triples) {
//            namespaces_[triple->getPredicate()->getNamespace()] = triple.getPredicatePtr()->getPrefix();
//        }
//    }
//
//
//    void Editor::toRDF() {
//        for (auto &annot : triple_list_) {
//            for (auto &triple : annot) {
//                LibrdfStatement stmt = triple.toStatement();
//                //todo add get namespace to triple
//                librdf_model_add_statement(*model_.getModel(), *stmt.getStatement());
//            }
//        }
//    }


    void Editor::addNamespace(std::string ns, std::string prefix) {
        namespaces_[ns] = prefix;
    }


//    void Editor::addSingleAnnotation(
//            Subject subject, PredicatePtr predicate_ptr,
//            Resource resource) {
//        if (!predicate_ptr) {
//            std::ostringstream err;
//            err << __FILE__ << ":" << __LINE__ << ":PredicatePtr argument is null" << std::endl;
//            throw NullPointerException(err.str());
//        }
//        checkValidMetaid(subject.str());
//        Triple triple(std::move(subject), predicate_ptr, std::move(resource));
//        Triples vec = {std::move(triple)};
//        triple_list_.push_back(vec);
//        for (auto &it : namespaces_) {
//            std::cout << "ns: " << it.first << ": " << it.second << std::endl;
//        }
//        namespaces_[predicate_ptr->getNamespace()] = predicate_ptr->getPrefix();
//    }

//    void Editor::addSingleAnnotation(SingularAnnotation singularAnnotation) {
//        Triples vec = {std::move(singularAnnotation)};
//        triple_list_.push_back(vec);
////        namespaces_[singularAnnotation.getPredicate()->getNamespace()] = singularAnnotation.getPredicate()->getPrefix();
//
//    }

//    void Editor::addAnnotationFromNestedTriples(NestedTriples tripleList) {
//        for (auto &inner_triple_vec: tripleList) {
//            extractNamespacesFromTriplesVector(inner_triple_vec);
//            triple_list_.push_back(inner_triple_vec);
//        }
//    }
//
//    void Editor::addAnnotationFromTriples(Triples triples) {
//        extractNamespacesFromTriplesVector(triples);
//        triple_list_.push_back(triples);
//
//    }


//    void Editor::addCompositeAnnotation(PhysicalPhenomenonPtr phenomenonPtr) {
//        Triples triples = phenomenonPtr->toTriples();
//        extractNamespacesFromTriplesVector(triples);
//        for (auto &triple : triples) {
//            model_.addStatement(triple);
//        }
//    }

//    void Editor::addPhysicalEntity(PhysicalEntity physicalEntity) {
//        addCompositeAnnotation(
//                std::make_shared<PhysicalEntity>(physicalEntity)
//        );
//    }
//
//    void Editor::addPhysicalProcess(PhysicalProcess physicalProcess) {
//        addCompositeAnnotation(
//                std::make_shared<PhysicalProcess>(physicalProcess)
//        );
//
//    }
//
//    void Editor::addPhysicalForce(PhysicalForce physicalForce) {
//        addCompositeAnnotation(
//                std::make_shared<PhysicalForce>(physicalForce)
//        );
//
//    }
//
//    void Editor::removeAnnotation(std::string metaid) {
//
//    }


}









