//
// Created by Ciaran on 4/15/2020.
//

#include "omexmeta/Editor.h"

#include <utility>
#include "filesystem"

namespace omexmeta {
    Editor::Editor(const std::string &xml, SemsimXmlType type, bool create_ids,
                   const LibrdfModel &model, NamespaceMap &nsmap)
            : model_(model), namespaces_(nsmap), create_ids_(create_ids) {
        XmlAssistantPtr xmlAssistantPtr = SemsimXmlAssistantFactory::generate(xml, type);
        std::pair<std::string, std::vector<std::string>> xml_and_metaids = xmlAssistantPtr->addMetaIds();
        xml_ = xml_and_metaids.first;
        //todo create a structure mapping metaids to more useful information for the user.
        metaids_ = xml_and_metaids.second;
    }

    int Editor::size() const {
        return model_.size();
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

    librdf_model *Editor::getModel() const {
        return model_.get();
    }

    void Editor::checkValidMetaid(const std::string &metaid) {
        if (std::find(metaids_.begin(), metaids_.end(), metaid) == metaids_.end()) {
            std::ostringstream err;
            err << "std::invalid_argument(): metaid \"" << metaid << "\" not found. ";
            err << "These are your available metaids: ";
            for (auto &it: metaids_) {
                err << it << ", ";
            }
            throw std::invalid_argument(err.str());
        }
    }

    void Editor::extractNamespacesFromTriplesVector(PhysicalPhenomenon *pp) {
        // here we create our own localized Triples object
        // and deplete it during the while loop. This
        // is preferable to passing in a Triples object
        // as argument because that would take copies and
        // mess with cleaning up the triples later.
        Triples triples = pp->toTriples();
        while (!triples.isEmpty()) {
            Triple triple = triples.pop_front();
            addNamespaceFromAnnotation(triple.getPredicateStr());
            triple.freeStatement();
        }
    }

    void Editor::addNamespace(const std::string &ns, std::string prefix) {
        namespaces_[ns] = std::move(prefix);
    }

    void Editor::addSingleAnnotation(
            Subject subject, const PredicatePtr &predicate_ptr,
            const Resource &resource) {
        if (!predicate_ptr) {
            std::ostringstream err;
            err << __FILE__ << ":" << __LINE__ << ":PredicatePtr argument is null" << std::endl;
            throw NullPointerException(err.str());
        }
        checkValidMetaid(subject.str());
        Triple triple(subject, predicate_ptr, resource);
        model_.addStatement(triple);
        triple.freeStatement();
        namespaces_[predicate_ptr->getNamespace()] = predicate_ptr->getPrefix();
    }

    void Editor::addSingleAnnotation(SingularAnnotation &singularAnnotation) {
        checkValidMetaid(singularAnnotation.getSubjectStr());
        addNamespaceFromAnnotation(singularAnnotation.getPredicateStr());
        model_.addStatement(singularAnnotation);
    }

    void Editor::addSingleAnnotationNoValidation(SingularAnnotation &singularAnnotation) {
        addNamespaceFromAnnotation(singularAnnotation.getPredicateStr());
        model_.addStatement(singularAnnotation);
    }

    void Editor::addNamespaceFromAnnotation(const std::string &predicate_string) {
        // store namespaces for later
        std::string ns = SemsimUtils::getNamespaceFromUri(predicate_string);
        if (Predicate::namespaceKnown(ns)) {
            namespaces_[ns] = Predicate::namespaceMap()[ns];
        };
    }

    void Editor::addCompositeAnnotation(PhysicalPhenomenon *phenomenonPtr) {
        Triples triples = phenomenonPtr->toTriples();
        while (!triples.isEmpty()) {
            // remove a Triple off the front of triples
            Triple triple = triples.pop_front();
            // collect the namespace from the triple
            addNamespaceFromAnnotation(triple.getPredicateStr());
            // add to the model
            model_.addStatement(triple.getStatement());
            // remember to free it.
            triple.freeStatement();
        }
    }

    void Editor::addCompositeAnnotation2(PhysicalPhenomenon *phenomenonPtr) {
        Triples triples = phenomenonPtr->toTriples();
        for (auto &triple: triples) {
            // collect the namespace from the triple
            addNamespaceFromAnnotation(triple.getPredicateStr());
            addSingleAnnotationNoValidation(triple);
        }
        triples.freeTriples();
    }

    void Editor::addPhysicalEntity(PhysicalEntity &physicalEntity) {
        if (physicalEntity.getAbout().empty()) {
            throw NullPointerException(
                    "NullPointerException: Editor::addPhysicalEntity() physicalEntity::subject_ (i.e. about) node is empty");
        }
        /*
         * look at result of commenting out the check stateent?
         */
        checkValidMetaid(physicalEntity.getAbout());
        addCompositeAnnotation((PhysicalPhenomenon *) &physicalEntity);
    }

    void Editor::addTriples(Triples &triples) {
        for (auto &triple: triples) {
            // collect the namespace from the triple
            addNamespaceFromAnnotation(triple.getPredicateStr());
            addSingleAnnotationNoValidation(triple);
        }
    }

    void Editor::addPhysicalProcess(const PhysicalProcess &physicalProcess) {
        if (physicalProcess.getAbout().empty()) {
            throw NullPointerException(
                    "NullPointerException: Editor::addPhysicalProcess() PhysicalProcess::subject_ (i.e. about) node is empty");
        }
        checkValidMetaid(physicalProcess.getAbout());
        /*
         * object slicing???
         */
        addCompositeAnnotation((PhysicalPhenomenon *) &physicalProcess);

    }

    void Editor::addPhysicalForce(const PhysicalForce &physicalForce) {
        if (physicalForce.getAbout().empty()) {
            throw NullPointerException(
                    "NullPointerException: Editor::addPhysicalForce() PhysicalForce::subject_ (i.e. about) node is empty");
        }
        checkValidMetaid(physicalForce.getAbout());
        addCompositeAnnotation((PhysicalPhenomenon *) &physicalForce);

    }

    void Editor::removeSingleAnnotation(const SingularAnnotation &singularAnnotation) const {
        librdf_statement *stmt = singularAnnotation.getStatement();
        model_.removeStatement(stmt);
    }

    void Editor::removePhysicalPhenomenon(PhysicalPhenomenon *physicalPhenomenon) const {
        Triples triples = physicalPhenomenon->toTriples();
        while (!triples.isEmpty()) {
            Triple triple = triples.pop();
            model_.removeStatement(triple.getStatement());
            triple.freeTriple();
        }
    }

    void Editor::removePhysicalEntity(PhysicalEntity &physicalEntity) const {
        removePhysicalPhenomenon(&physicalEntity);
    }


    void Editor::removePhysicalForce(PhysicalForce &physicalForce) const {
        removePhysicalPhenomenon(&physicalForce);
    }

    void Editor::removePhysicalProcess(PhysicalProcess &physicalProcess) const {
        removePhysicalPhenomenon(&physicalProcess);
    }

    PhysicalEntity Editor::createPhysicalEntity() {
        return PhysicalEntity(model_.get());
    }

    PhysicalForce Editor::createPhysicalForce() {
        return PhysicalForce(model_.get());
    }

    PhysicalProcess Editor::createPhysicalProcess() {
        return PhysicalProcess(model_.get());
    }

    void Editor::addAuthor(const std::string &orcid_id) {
        // Note, this mechanism of retrieving the model level metaid
        // is very likely to be brittle since it assumes its the first
        // element in the metaid list.
        Triple triple(
                LibrdfNode::fromUriString(getMetaids()[0]).get(),
                PredicateFactory("dc", "creator")->getNode(),
                LibrdfNode::fromUriString(orcid_id).get()
        );
        model_.addStatement(triple);
    }

    void Editor::addCurator(const std::string &orcid_id) {
        // Note, this mechanism of retrieving the model level metaid
        // is very likely to be brittle since it assumes its the first
        // element in the metaid list.
        Triple triple(
                LibrdfNode::fromUriString(getMetaids()[0]).get(),
                PredicateFactory("dc", "creator")->getNode(),
                LibrdfNode::fromUriString(orcid_id).get()
        );
        model_.addStatement(triple);
    }

    std::string Editor::getLocalName() const {
        return local_name_;
    }

    std::string Editor::getModelName() const {
        return model_name_;
    }

    std::string Editor::getArchiveName() const {
        return archive_name_;
    }

    std::string Editor::getOmexRepository() const {
        return repository_name_;
    }

    void Editor::setArchiveName(std::string archive_name) {
        // archives end in .omex
        if (!SemsimUtils::hasEnding(archive_name, ".omex")) {
            archive_name = archive_name + ".omex";
        }
        // Check if model_name is already a valid uri
        if (SemsimUtils::isFormattedUri(archive_name)) {
            // if so, also check that its relative to
            // the repository name, otherwise it does not
            // make sense
            if (archive_name.rfind(getOmexRepository(), 0) != 0) {
                std::ostringstream os;
                os << "std::invalid_argument: Editor::setArchiveName: "
                   << "A full uri has been given as the archive_name "
                      "attribute (\"" + archive_name + "\") but this does not "
                   << "match the uri given for the repository name : \"" + getOmexRepository() + "\"";
                throw std::invalid_argument(os.str());
            }
            archive_name_ = std::move(archive_name);
        } else {
            archive_name_ = getOmexRepository() + archive_name;
        }
    }

    void Editor::setModelName(std::string model_name) {
        // Check if model_name is already a valid uri
        if (SemsimUtils::isFormattedUri(model_name)) {
            // if so, also check that its relative to
            // the archive name, otherwise it does not
            // make sense
            if (model_name.rfind(getArchiveName(), 0) != 0) {
                std::ostringstream os;
                os << "std::invalid_argument: Editor::setModelName: "
                   << "A full uri has been given as the model_name "
                      "attribute (\"" + model_name + "\") but this does not "
                   << "match the uri given for the archive name : \"" + getModelName() + "\"";
                throw std::invalid_argument(os.str());
            }
            model_name_ = std::move(model_name);
        } else {
            if (getArchiveName().empty()) {
                throw std::logic_error("std::logic_error: Editor::setModelName: "
                                       "Trying to set a model name without an archive name. Please"
                                       " first use setArchiveName. ");
            }
            model_name_ = getArchiveName() + "/" + model_name;
        }
    }

    void Editor::setLocalName(std::string local_name) {
        // local names have the .rdf suffix
        if (!SemsimUtils::hasEnding(local_name, ".rdf")) {
            local_name = local_name + ".rdf";
        }
        // Check if model_name is already a valid uri
        if (SemsimUtils::isFormattedUri(local_name)) {
            // if so, also check that its relative to
            // the archive name, otherwise it does not
            // make sense
            if (local_name.rfind(getArchiveName(), 0) != 0) {
                std::ostringstream os;
                os << "std::invalid_argument: Editor::setLocalName: "
                   << "A full uri has been given as the local_name "
                      "attribute (\"" + local_name + "\") but this does not "
                   << "match the uri given for the archive name : \"" + getModelName() + "\"";
                throw std::invalid_argument(os.str());
            }
            local_name_ = std::move(local_name);
        } else {
            if (getArchiveName().empty()) {
                throw std::logic_error("std::logic_error: Editor::setModelName: "
                                       "Trying to set a model name without an archive name. Please"
                                       " first use setArchiveName. ");
            }
            local_name_ = getArchiveName() + "/" + local_name;
        }
    }

    void Editor::setOmexRepository(std::string repository_name) {
        if (SemsimUtils::isFormattedUri(repository_name)) {
            repository_name_ = std::move(repository_name);
        } else {
            throw std::invalid_argument("std::invalid_argument: Editor::setOmexRepository "
                                        "repository_name"
                                        " argument \"" + repository_name + "\" is not formatted like a "
                                                                           "proper url. ");
        }
    }


}








