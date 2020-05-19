//
// Created by Ciaran on 5/8/2020.
//

#include "semsim/SemsimCApi.h"
#include <cstring>
#include "semsim/SemsimUtils.h"

/*
 * Todo: Put checks in for nullptr in all functions
 */

namespace semsim {
    void free_c_char_star(char *c) {
        if (!c)
            return;
        free(c);
    }

    void free_c_char_star_star(char **c, int size) {
        if (!c)
            return;
        std::cout << size<<std::endl;
        for (int i = 0; i < size; i++) {
            std::cout << i << std::endl;
            std::cout << __FILE__<<":"<<__LINE__<<": "<<c << std::endl;
            free(c);
            c++;
        }
        free(c);
    }

    char *_func_that_returns_dynamic_alloc_str() {
        std::string str = "ADynamicallyAllocatedStringForTesting";
        char *cstr = (char *) malloc((str.size() + 1) * sizeof(char *));
        strcpy(cstr, str.c_str());
        return cstr;
    }

    char **_func_that_returns_dynamic_alloc_char_star_star() {
        std::vector<std::string> vec = {"A", "Dynamically", "Allocated", "List", "of", "Strings"};
        char **arr = (char **) malloc((vec.size() + 1) * sizeof(char *));

        for (int i = 0; i < vec.size(); i++) {
            arr[i] = (char *) malloc(strlen(vec[i].c_str()) + sizeof(char));
            strcpy(arr[i], vec[i].c_str());
        }
        return arr;
    }


    semsim::RDF *RDF_new() {
        return new semsim::RDF();
    }

    void RDF_delete(semsim::RDF *rdf_ptr) {
        if (!rdf_ptr)
            return;

        delete rdf_ptr;
    }

    void RDF_fromString(semsim::RDF *rdf_ptr, const char *str, const char *format) {
        if (!rdf_ptr) {
            throw std::invalid_argument("RDF_fromString(): \"rdf_ptr\" is nullptr");
        }
        semsim::RDF rdf = semsim::RDF::fromString(str, format);
        *rdf_ptr = rdf;
    }

    char * RDF_toString(semsim::RDF *rdf_ptr, const char *format, const char *base_uri) {
        // do not return a temporary object:
        static std::string s = rdf_ptr->toString(format, base_uri);
        char* cstr = (char*)malloc(s.size()+sizeof(char*));
        strcpy(cstr, s.c_str());
        return cstr;
    }

    char *RDF_getBaseUri(semsim::RDF *rdf_ptr) {
        std::string str = rdf_ptr->getBaseUriAsString();
        char *cstr = (char *) malloc(str.size() + sizeof(char));
        strcpy(cstr, str.c_str());
        return cstr;
    }

    void RDF_setBaseUri(semsim::RDF *rdf_ptr, const char *uri) {
        rdf_ptr->setBaseUri(uri);
    }

    const char *RDF_queryResultsAsStr(semsim::RDF *rdf_ptr, const char *query_str, const char *results_format) {
        static std::string query_results = rdf_ptr->queryResultsAsStr(query_str, results_format);
        return query_results.c_str();
    }

    int RDF_size(semsim::RDF *rdf_ptr) {
        return rdf_ptr->size();
    }

    Editor *RDF_toEditor(semsim::RDF *rdf_ptr, const char *xml, semsim::XmlAssistantType type) {
        return rdf_ptr->toEditorPtr(xml, type);
    }

/*********************************************************************
 * Editor class methods
 */

    void Editor_addNamespace(Editor *editor_ptr, const char *namespace_, const char *prefix) {
        editor_ptr->addNamespace(namespace_, prefix);
    }

    void Editor_addSingleAnnotation(Editor *editor_ptr, SingularAnnotation *singularAnnotation) {
        editor_ptr->addSingleAnnotation(*singularAnnotation);
    }

    void Editor_addPhysicalEntity(Editor *editor_ptr, PhysicalEntity *physicalEntity) {
        editor_ptr->addPhysicalEntity(*physicalEntity);
    }

    void Editor_addPhysicalProcess(Editor *editor_ptr, PhysicalProcess *physicalProcess) {
        editor_ptr->addPhysicalProcess(*physicalProcess);
    }

    void Editor_addPhysicalForce(Editor *editor_ptr, PhysicalForce *physicalForce) {
        editor_ptr->addPhysicalForce(*physicalForce);
    }

    void Editor_checkValidMetaid(Editor *editor_ptr, const char *id) {
        editor_ptr->checkValidMetaid(id);
    }

    void Editor_toRDF(Editor *editor_ptr) {
        editor_ptr->toRDF();
    }

    void Editor_delete(Editor *editor_ptr) {
        if (!editor_ptr)
            return;
        delete editor_ptr;
    }


/*********************************************************************
 * SingularAnnotation class methods
 */
    SingularAnnotation *SingularAnnotation_new(Editor *editor_ptr) {
        return new SingularAnnotation(editor_ptr->getWorld());
    }

    void SingularAnnotation_delete(SingularAnnotation *singularAnnotation) {
        if (!singularAnnotation)
            return;
        delete singularAnnotation;
    }

    void free_singular_annotation(SingularAnnotation *singularAnnotationPtr) {
        free(singularAnnotationPtr);
    }

    SingularAnnotation *SingularAnnotation_setAbout(SingularAnnotation *singular_annotation, const char *about) {
        singular_annotation->setAbout(about);
        return singular_annotation;
    };


    SingularAnnotation *SingularAnnotation_setPredicate(
            SingularAnnotation *singular_annotation, const char *namespace_, const char *term) {
        singular_annotation->setPredicate(namespace_, term);
        return singular_annotation;
    }

    SingularAnnotation *SingularAnnotation_setPredicateNew(
            SingularAnnotation *singular_annotation, const char *namespace_,
            const char *term, const char *prefix) {
        singular_annotation->setPredicateNew(namespace_, term, prefix);
        return singular_annotation;
    }

    SingularAnnotation *
    SingularAnnotation_setResourceLiteral(SingularAnnotation *singular_annotation, const char *literal) {
        singular_annotation->setResourceLiteral(literal);
        return singular_annotation;
    }

    SingularAnnotation *
    SingularAnnotation_setResourceUri(SingularAnnotation *singular_annotation, const char *identifiers_uri) {
        singular_annotation->setResourceUri(identifiers_uri);
        return singular_annotation;
    }

    SingularAnnotation *
    SingularAnnotation_setResourceBlank(SingularAnnotation *singular_annotation, const char *blank_id) {
        singular_annotation->setResourceBlank(blank_id);
        return singular_annotation;
    }

    char *SingularAnnotation_getAbout(SingularAnnotation *singular_annotation) {
        std::string about = singular_annotation->getAbout();
        char *cstr = (char *) malloc(about.size() + sizeof(char));
        strcpy(cstr, about.c_str());
        return cstr;
    }

    char *SingularAnnotation_getPredicate(SingularAnnotation *singular_annotation) {
        std::string predicate_str = singular_annotation->getPredicatePtr()->str();
        char *cstr = (char *) malloc(predicate_str.size() + sizeof(char));
        strcpy(cstr, predicate_str.c_str());
        return cstr;
    }

    char *SingularAnnotation_getResource(SingularAnnotation *singular_annotation) {
        std::string resource = singular_annotation->getResource().str();
        char *cstr = (char *) malloc(resource.size() + sizeof(char));
        strcpy(cstr, resource.c_str());
        return cstr;
    }

    char *SingularAnnotation_str(
            SingularAnnotation *singular_annotation, const char *format, const char *base_uri
    ) {
        std::string str = singular_annotation->str(format, base_uri);
        char *cstr = (char *) malloc(str.size() + sizeof(char));
        strcpy(cstr, str.c_str());
        return cstr;
    }

/*********************************************************************
 * PhysicalEntity class methods
 */
    PhysicalEntity *PhysicalEntity_new(Editor *editor_ptr) {
        return new PhysicalEntity(editor_ptr->getWorld(), editor_ptr->getModel());
    }

    void PhysicalEntity_delete(PhysicalEntity *physical_entity_ptr) {
        if (!physical_entity_ptr)
            return;
        delete physical_entity_ptr;
    }

    PhysicalEntity *PhysicalEntity_setAbout(PhysicalEntity *physical_entity_ptr, const char *about) {
        physical_entity_ptr->setAbout(about);
        return physical_entity_ptr;
    }

    PhysicalEntity *PhysicalEntity_setPhysicalProperty(
            PhysicalEntity *physical_entity_ptr, const char *physical_property) {
        physical_entity_ptr->setPhysicalProperty(physical_property);
        return physical_entity_ptr;
    }

    PhysicalEntity *PhysicalEntity_setIdentity(PhysicalEntity *physical_entity_ptr, const char *identity_resource) {
        physical_entity_ptr->setIdentity(identity_resource);
        return physical_entity_ptr;
    }

    PhysicalEntity *PhysicalEntity_addLocation(PhysicalEntity *physical_entity_ptr, const char *location_resource) {
        physical_entity_ptr->addLocation(location_resource);
        return physical_entity_ptr;
    }

    char *PhysicalEntity_getAbout(PhysicalEntity *physical_entity_ptr) {
        std::string about = physical_entity_ptr->getAbout().str();
        char *cstr = (char *) malloc((about.size() + 1) * sizeof(char));
        strcpy(cstr, about.c_str());
        return cstr;
    }

    char *PhysicalEntity_getPhysicalProperty(PhysicalEntity *physical_entity_ptr) {
        std::string physical_property = physical_entity_ptr->getPhysicalProperty().str();
        char *cstr = (char *) malloc(physical_property.size() + sizeof(char));
        strcpy(cstr, physical_property.c_str());
        return cstr;
    }

    char *PhysicalEntity_getIdentity(PhysicalEntity *physical_entity_ptr) {
        std::string identity = physical_entity_ptr->getIdentityResource().str();
        char *cstr = (char *) malloc(identity.size() + sizeof(char));
        strcpy(cstr, identity.c_str());
        return cstr;

    }

    int PhysicalEntity_getNumLocations(PhysicalEntity *physicalEntity) {
        return physicalEntity->getNumLocations();
    }

    char **PhysicalEntity_getLocations(PhysicalEntity *physical_entity_ptr) {
        Resources locations = physical_entity_ptr->getLocationResources();
        char **arr = (char **) malloc((locations.size() + 1) * sizeof(char **));

        for (int i = 0; i < locations.size(); i++) {
            arr[i] = (char *) malloc(strlen(locations[i].str().c_str()) + sizeof(char*));
            strcpy(arr[i], locations[i].str().c_str());
        }

        return arr;
    }

    char *PhysicalEntity_str(PhysicalEntity *physical_entity_ptr, const char *format, const char *base_uri) {
        std::string str = physical_entity_ptr->toTriples().str(format, base_uri);
        char *cstr = (char *) malloc(str.size() + sizeof(char));
        strcpy(cstr, str.c_str());
        return cstr;
    }


/*********************************************************************
 * PhysicalProcess class methods
 */
    PhysicalProcess *PhysicalProcess_new(Editor *editor_ptr) {
        return new PhysicalProcess(editor_ptr->getWorld(), editor_ptr->getModel());
    }

    void PhysicalProcess_delete(PhysicalProcess *physicalProcess) {
        if (!physicalProcess)
            return;
        delete physicalProcess;
    }

    PhysicalProcess *PhysicalProcess_setAbout(PhysicalProcess *physical_process, const char *about) {
        physical_process->setAbout(about);
        return physical_process;
    }

    PhysicalProcess *
    PhysicalProcess_setPhysicalProperty(PhysicalProcess *physical_process, const char *physical_property) {
        physical_process->setPhysicalProperty(physical_property);
        return physical_process;

    }

    PhysicalProcess *
    PhysicalProcess_addSource(PhysicalProcess *physical_process, const char *source_metaid, double multiplier,
                              const char *physical_entity_reference) {
        physical_process->addSource(source_metaid, multiplier, physical_entity_reference);
        return physical_process;

    }

    PhysicalProcess *
    PhysicalProcess_addSink(PhysicalProcess *physical_process, const char *sink_metaid, double multiplier,
                            const char *physical_entity_reference) {
        physical_process->addSink(sink_metaid, multiplier, physical_entity_reference);
        return physical_process;

    }

    PhysicalProcess *
    PhysicalProcess_addMediator(PhysicalProcess *physical_process, const char *mediator_metaid, double multiplier,
                                const char *physical_entity_reference) {
        physical_process->addMediator(mediator_metaid, multiplier, physical_entity_reference);
        return physical_process;

    }

    char *PhysicalProcess_str(PhysicalProcess *physical_process_ptr, const char *format, const char *base_uri) {
        std::string str = physical_process_ptr->toTriples().str(format, base_uri);
        char *cstr = (char *) malloc(str.size() + sizeof(char));
        strcpy(cstr, str.c_str());
        return cstr;
    }

    char *PhysicalProcess_getAbout(PhysicalProcess *physical_process_ptr) {
        std::string about = physical_process_ptr->getAbout().str();
        char *cstr = (char *) malloc(about.size() + sizeof(char));
        strcpy(cstr, about.c_str());
        return cstr;
    }

    char *PhysicalProcess_getPhysicalProperty(PhysicalProcess *physical_process_ptr) {
        std::string pp = physical_process_ptr->getPhysicalProperty().str();
        char *cstr = (char *) malloc(pp.size() + sizeof(char));
        strcpy(cstr, pp.c_str());
        return cstr;
    }

    int PhysicalProcess_getNumSources(PhysicalProcess *physicalProcess) {
        return physicalProcess->getNumSources();
    }

    int PhysicalProcess_getNumSinks(PhysicalProcess *physicalProcess) {
        return physicalProcess->getNumSinks();
    }

    int PhysicalProcess_getNumMediators(PhysicalProcess *physicalProcess) {
        return physicalProcess->getNumMediators();
    }


/*********************************************************************
 * PhysicalForce class methods
 */
    PhysicalForce *PhysicalForce_new(Editor *editor_ptr) {
        return new PhysicalForce(editor_ptr->getWorld(), editor_ptr->getModel());
    }

    void PhysicalForce_delete(PhysicalForce *physicalForce) {
        if (!physicalForce)
            return;
        delete physicalForce;
    }


    PhysicalForce *PhysicalForce_setAbout(PhysicalForce *physical_force_ptr, const char *about) {
        physical_force_ptr->setAbout(about);
        return physical_force_ptr;
    }

    PhysicalForce *PhysicalForce_addSource(
            PhysicalForce *physical_force_ptr, const char *source_metaid,
            double multiplier,
            const char *physical_entity_reference) {
        physical_force_ptr->addSource(source_metaid, multiplier, physical_entity_reference);
        return physical_force_ptr;

    }

    PhysicalForce *PhysicalForce_addSink(PhysicalForce *physical_force_ptr, const char *sink_metaid,
                                         double multiplier,
                                         const char *physical_entity_reference) {
        physical_force_ptr->addSink(sink_metaid, multiplier, physical_entity_reference);
        return physical_force_ptr;
    }

    PhysicalForce *PhysicalForce_setPhysicalProperty(PhysicalForce *physical_force_ptr, const char *physical_property) {
        physical_force_ptr->setPhysicalProperty(physical_property);
        return physical_force_ptr;
    }

    int PhysicalForce_getNumSources(PhysicalForce *physicalForce) {
        return physicalForce->getNumSources();
    }

    int PhysicalForce_getNumSinks(PhysicalForce *physicalForce) {
        return physicalForce->getNumSinks();
    }

    char *PhysicalForce_str(PhysicalForce *physical_force_ptr, const char *format, const char *base_uri) {
        std::string str = physical_force_ptr->toTriples().str(format, base_uri);
        char *cstr = (char *) malloc(str.size() + sizeof(char));
        strcpy(cstr, str.c_str());
        return cstr;
    }


    char *PhysicalForce_getAbout(PhysicalForce *physical_force_ptr) {
        std::string about = physical_force_ptr->getAbout().str();
        char *cstr = (char *) malloc(about.size() + sizeof(char));
        strcpy(cstr, about.c_str());
        return cstr;
    }


    char *PhysicalForce_getPhysicalProperty(PhysicalForce *physical_force_ptr) {
        std::string pp = physical_force_ptr->getPhysicalProperty().str();
        char *cstr = (char *) malloc(pp.size() + sizeof(char));
        strcpy(cstr, pp.c_str());
        return cstr;
    }


}



























