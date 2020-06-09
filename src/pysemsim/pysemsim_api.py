from __future__ import annotations

import ctypes as ct
import os
import sys
from typing import List

if sys.platform != 'linux':
    raise NotImplementedError("Currently pysemsim is only supported "
                              "for linux systems. ")

_WORKING_DIRECTORY = os.path.dirname(os.path.realpath(__file__))


class Util:

    @staticmethod
    def load_lib() -> ct.CDLL:
        lib_path = os.path.join(_WORKING_DIRECTORY, "libsemsim.so")
        lib = ct.CDLL(lib_path)
        if not lib:
            raise ValueError("libsemsim.so not found")
        return lib

    @staticmethod
    def load_func(funcname: str, argtypes: List, restype) -> ct.CDLL._FuncPtr:
        func = Util.load_lib().__getattr__(funcname)
        func.restype = restype
        func.argtypes = argtypes
        return func


libsemsim = Util.load_lib()


class PysemsimAPI:

    # RDF methods
    @staticmethod
    def get_and_free_c_str(c_string_ptr: ct.c_int64) -> str:
        """Uses ctypes to transfer a C string to a python string and free the C string"""
        free_func = Util.load_func("free_c_char_star", [ct.c_void_p], None)
        string = ct.cast(c_string_ptr, ct.c_char_p).value
        decoded_str = string.decode()
        free_func(c_string_ptr)  # free the data
        del c_string_ptr  # free the ptr
        return decoded_str

    @staticmethod
    def crlf_to_lr(string: str) -> str:
        """Helper method for converting line endings. Needed mostly for query. For developers"""
        return string.replace('\r\n', '\n')

    # void free_c_char_star(char *c);
    free_char_star = Util.load_func("free_c_char_star", [ct.c_char_p], None)

    #################################################################
    # RDF methods
    #

    # RDF *RDF_new();
    rdf_new = Util.load_func("RDF_new", [], ct.c_void_p)

    # int RDF_size(RDF *rdf_ptr);
    rdf_size = Util.load_func("RDF_size", [ct.c_int64], ct.c_int64)

    # RDF *RDF_fromString(const char *str, const char *format, const char *baseuri = "./Annotations.rdf");
    rdf_from_string = Util.load_func("RDF_fromString",
                                     [ct.c_char_p, ct.c_char_p, ct.c_char_p], ct.c_int64)

    # void RDF_addFromString(RDF *rdf_ptr, const char *str, const char *format, const char *base_uri);
    rdf_add_from_string = Util.load_func("RDF_addFromString", [ct.c_int64, ct.c_char_p, ct.c_char_p, ct.c_char_p], ct.c_void_p)

    # RDF *RDF_fromUri(const char *uri_string, const char *format);
    rdf_from_uri = Util.load_func("RDF_fromUri", [ct.c_char_p, ct.c_char_p], ct.c_int64)

    # void RDF_addFromUri(RDF *rdf_ptr, const char *uri_string, const char *format);
    rdf_add_from_uri = Util.load_func("RDF_addFromUri", [ct.c_int64, ct.c_char_p, ct.c_char_p], ct.c_void_p)

    # RDF *RDF_fromFile(const char *filename, const char *format);
    rdf_from_file = Util.load_func("RDF_fromFile", [ct.c_char_p, ct.c_char_p], ct.c_int64)

    # void RDF_addFromFile(RDF *rdf_ptr, const char *uri_string, const char *format);
    rdf_add_from_file = Util.load_func("RDF_addFromFile", [ct.c_int64, ct.c_char_p, ct.c_char_p], ct.c_int64)

    # char *RDF_toString(RDF *rdf_ptr, const char *format, const char *base_uri);
    rdf_to_string = Util.load_func("RDF_toString", [ct.c_int64, ct.c_char_p, ct.c_char_p], ct.c_int64)

    # void RDF_delete(RDF *rdf_ptr);
    rdf_delete = Util.load_func("RDF_delete", [ct.c_int64], None)

    # char *RDF_getBaseUri(RDF *rdf_ptr);
    rdf_get_base_uri = Util.load_func("RDF_getBaseUri", [ct.c_int64], ct.c_int64)

    # void RDF_setBaseUri(RDF *rdf_ptr, const char *uri);
    rdf_set_base_uri = Util.load_func("RDF_setBaseUri", [ct.c_int64, ct.c_char_p], None)

    # char *RDF_queryResultsAsStr(RDF *rdf_ptr, const char *query_str, const char *results_format);
    rdf_query_results_as_str = Util.load_func("RDF_queryResultsAsStr", [ct.c_int64, ct.c_char_p, ct.c_char_p],
                                              ct.c_int64)

    # Editor *RDF_toEditor(RDF *rdf_ptr, const char *xml, SemsimXmlType type);
    rdf_to_editor = Util.load_func("RDF_toEditor", [ct.c_int64, ct.c_char_p, ct.c_int], ct.c_int64)

    #################################################################
    # Editor methods
    #

    # void Editor_addNamespace(Editor *editor_ptr, const char *namespace_, const char *prefix);
    editor_add_namespace = Util.load_func("Editor_addNamespace", [ct.c_int64, ct.c_char_p, ct.c_char_p], ct.c_void_p)

    # void Editor_addSingleAnnotation(Editor *editor_ptr, SingularAnnotation *singularAnnotation);
    editor_add_single_annotation = Util.load_func("Editor_addSingleAnnotation",
                                                  [ct.c_int64, ct.c_int64], ct.c_void_p)

    # void Editor_addPhysicalEntity(Editor *editor_ptr, PhysicalEntity *physicalEntity);
    editor_add_physical_entity = Util.load_func("Editor_addPhysicalEntity", [ct.c_int64, ct.c_int64],
                                                ct.c_void_p)

    # void Editor_addPhysicalProcess(Editor *editor_ptr, PhysicalProcess *physicalProcess);
    editor_add_physical_process = Util.load_func("Editor_addPhysicalProcess", [ct.c_int64, ct.c_int64],
                                                 ct.c_void_p)
    # void Editor_addPhysicalForce(Editor *editor_ptr, PhysicalForce *physicalForce);
    editor_add_physical_force = Util.load_func("Editor_addPhysicalForce", [ct.c_int64, ct.c_int64],
                                               ct.c_void_p)
    # void Editor_checkValidMetaid(Editor *editor_ptr, const char *id);
    editor_check_valid_metaid = Util.load_func("Editor_checkValidMetaid", [ct.c_int64, ct.c_char_p],
                                               ct.c_void_p)
    # char *Editor_getMetaId(Editor *editor_ptr, int index);
    editor_get_metaid = Util.load_func("Editor_getMetaId", [ct.c_int64, ct.c_int64], ct.c_int64)

    # int Editor_getNumMetaIds(Editor *editor_ptr);
    editor_get_num_metaids = Util.load_func("Editor_getNumMetaIds", [ct.c_int64], ct.c_int64)

    # void Editor_toRDF(Editor *editor_ptr);
    editor_to_rdf = Util.load_func("Editor_toRDF", [ct.c_int64], ct.c_void_p)

    # char *Editor_getXml(Editor *editor_ptr);
    editor_get_xml = Util.load_func("Editor_getXml", [ct.c_int64], ct.c_int64)

    editor_new_singular_annotation = Util.load_func("SingularAnnotation_new", [ct.c_int64],
                                                    ct.c_int64)
    editor_new_physical_entity = Util.load_func("PhysicalEntity_new", [ct.c_int64], ct.c_int64)
    editor_new_physical_process = Util.load_func("PhysicalProcess_new", [ct.c_int64], ct.c_int64)
    editor_new_physical_force = Util.load_func("PhysicalForce_new", [ct.c_int64], ct.c_int64)
    editor_delete = Util.load_func("Editor_delete", [ct.c_int64], None)

    #################################################################
    # SingularAnnotations methods
    #

    #      SingularAnnotation *SingularAnnotation_setAbout(SingularAnnotation *singular_annotation, const char *about);
    singular_annotation_set_about = Util.load_func("SingularAnnotation_setAbout", [ct.c_int64, ct.c_char_p], ct.c_int64)

    # SingularAnnotation * SingularAnnotation_setPredicate(SingularAnnotation *singular_annotation, const char *namespace_,const char *term);
    singular_annotation_set_predicate = Util.load_func("SingularAnnotation_setPredicate",
                                                       [ct.c_int64, ct.c_char_p, ct.c_char_p], ct.c_int64)

    # SingularAnnotation *SingularAnnotation_setPredicateFromUri(
    #         SingularAnnotation *singular_annotation, const char *uri);
    singular_annotation_set_predicate_uri = Util.load_func("SingularAnnotation_setPredicateFromUri",
                                                           [ct.c_int64, ct.c_char_p], ct.c_int64)

    # SingularAnnotation *SingularAnnotation_setResourceLiteral(
    #         SingularAnnotation *singular_annotation, const char *literal);
    singular_annotation_set_resource_literal = Util.load_func("SingularAnnotation_setResourceLiteral",
                                                              [ct.c_int64, ct.c_char_p], ct.c_int64)
    # SingularAnnotation *
    # SingularAnnotation_setResourceUri(SingularAnnotation *singular_annotation, const char *identifiers_uri);
    singular_annotation_set_resource_uri = Util.load_func("SingularAnnotation_setResourceUri",
                                                          [ct.c_int64, ct.c_char_p],
                                                          ct.c_int64)
    # SingularAnnotation *
    # SingularAnnotation_setResourceBlank(SingularAnnotation *singular_annotation, const char *blank_id);
    singular_annotation_set_resource_blank = Util.load_func("SingularAnnotation_setResourceBlank",
                                                            [ct.c_int64, ct.c_char_p], ct.c_int64)

    # char *SingularAnnotation_getAbout(SingularAnnotation *singular_annotation);
    singular_annotation_get_about = Util.load_func("SingularAnnotation_getAbout", [ct.c_int64], ct.c_int64)

    # char *SingularAnnotation_str(SingularAnnotation *singular_annotation, const char *format, const char *base_uri);
    singular_annotation_str = Util.load_func("SingularAnnotation_str", [ct.c_int64, ct.c_char_p, ct.c_char_p], ct.c_int64)

    # char *SingularAnnotation_getPredicate(SingularAnnotation *singular_annotation);
    singular_annotation_get_predicate = Util.load_func("SingularAnnotation_getPredicate", [ct.c_int64], ct.c_int64)

    # char *SingularAnnotation_getResource(SingularAnnotation *singular_annotation);
    singular_annotation_get_resource = Util.load_func("SingularAnnotation_getResource", [ct.c_int64], ct.c_int64)

    # void SingularAnnotation_delete(SingularAnnotation *singularAnnotation);
    singular_annotation_delete = Util.load_func("SingularAnnotation_delete", [ct.c_int64], None)

    #################################################################
    # PhysicalEntity methods
    #

    # PhysicalEntity *PhysicalEntity_setAbout(PhysicalEntity *physical_entity_ptr, const char *about);
    physical_entity_set_about = Util.load_func("PhysicalEntity_setAbout", [ct.c_int64, ct.c_char_p], ct.c_int64)

    # PhysicalEntity *PhysicalEntity_setPhysicalProperty(
    #         PhysicalEntity *physical_entity_ptr, const char *physical_property);
    physical_entity_set_physical_property = Util.load_func("PhysicalEntity_setPhysicalProperty",
                                                           [ct.c_int64, ct.c_char_p], ct.c_int64)
    # PhysicalEntity *PhysicalEntity_setIdentity(
    #         PhysicalEntity *physical_entity_ptr, const char *identity_resource);
    physical_entity_set_identity = Util.load_func("PhysicalEntity_setIdentity", [ct.c_int64, ct.c_char_p], ct.c_int64)

    # PhysicalEntity *PhysicalEntity_addLocation(
    #         PhysicalEntity *physical_entity_ptr, const char *location_resource);
    physical_entity_add_location = Util.load_func("PhysicalEntity_addLocation", [ct.c_int64, ct.c_char_p], ct.c_int64)

    # char *PhysicalEntity_getAbout(PhysicalEntity *physical_entity_ptr);
    physical_entity_get_about = Util.load_func("PhysicalEntity_getAbout", [ct.c_int64], ct.c_int64)

    # char *PhysicalEntity_getPhysicalProperty(PhysicalEntity *physical_entity_ptr);
    physical_entity_get_physical_property = Util.load_func("PhysicalEntity_getPhysicalProperty", [ct.c_int64],
                                                           ct.c_int64)

    # char *PhysicalEntity_getIdentity(PhysicalEntity *physical_entity_ptr);
    physical_entity_get_identity = Util.load_func("PhysicalEntity_getIdentity", [ct.c_int64], ct.c_int64)

    # int PhysicalEntity_getNumLocations(PhysicalEntity *physicalEntity);
    physical_entity_get_num_locations = Util.load_func("PhysicalEntity_getNumLocations", [ct.c_int64],
                                                       ct.c_int)

    # char *PhysicalEntity_getLocation(PhysicalEntity *physical_entity_ptr, int index);
    physical_entity_get_location = Util.load_func("PhysicalEntity_getLocation", [ct.c_int64, ct.c_int64], ct.c_int64)

    # char *PhysicalEntity_str(PhysicalEntity *physical_entity_ptr, const char *format, const char *base_uri);
    physical_entity_str = Util.load_func("PhysicalEntity_str", [ct.c_int64, ct.c_char_p, ct.c_char_p],
                                         ct.c_int64)
    # void PhysicalEntity_delete(PhysicalEntity *physical_entity_ptr);
    physical_entity_delete = Util.load_func("PhysicalEntity_delete", [ct.c_int64], None)

    # void PhysicalEntity_freeAll(PhysicalEntity *physical_entity_ptr);
    physical_entity_free_all = Util.load_func("PhysicalEntity_freeAll", [ct.c_int64], None)


    #################################################################
    # PhysicalProcess methods
    #

    # PhysicalProcess *PhysicalProcess_setAbout(
    #         PhysicalProcess *physical_process, const char *about);
    physical_process_set_about = Util.load_func("PhysicalProcess_setAbout", [ct.c_int64, ct.c_char_p], ct.c_int64)

    # PhysicalProcess *PhysicalProcess_setPhysicalProperty(
    #         PhysicalProcess *physical_process, const char *physical_property);
    physical_process_set_physical_property = Util.load_func("PhysicalProcess_setPhysicalProperty",
                                                            [ct.c_int64, ct.c_char_p], ct.c_int64)

    # PhysicalProcess *PhysicalProcess_addSource(
    #         PhysicalProcess *physical_process, const char *source_metai, double multiplier,
    #         const char *physical_entity_reference);
    physical_process_add_source = Util.load_func("PhysicalProcess_addSource",
                                                 [ct.c_int64, ct.c_char_p, ct.c_float, ct.c_char_p], ct.c_int64)

    # PhysicalProcess *PhysicalProcess_addSink(
    #         PhysicalProcess *physical_process, const char *sink_metaid, double multiplier,
    #         const char *physical_entity_reference);
    physical_process_add_sink = Util.load_func("PhysicalProcess_addSink",
                                               [ct.c_int64, ct.c_char_p, ct.c_float, ct.c_char_p], ct.c_int64)

    # PhysicalProcess *PhysicalProcess_addMediator(
    #         PhysicalProcess *physical_process, const char *mediator_metaid, double multiplier,
    #         const char *physical_entity_reference);
    physical_process_add_mediator = Util.load_func("PhysicalProcess_addMediator",
                                                   [ct.c_int64, ct.c_char_p, ct.c_float, ct.c_char_p], ct.c_int64)

    # char *PhysicalProcess_str(PhysicalProcess *physical_process_ptr, const char *format, const char *base_uri);
    physical_process_str = Util.load_func("PhysicalProcess_str",
                                          [ct.c_int64, ct.c_char_p, ct.c_char_p], ct.c_int64)

    # char *PhysicalProcess_getAbout(PhysicalProcess *physical_process_ptr);
    physical_process_get_about = Util.load_func("PhysicalProcess_getAbout", [ct.c_int64], ct.c_int64)

    # char *PhysicalProcess_getPhysicalProperty(PhysicalProcess *physical_process_ptr);
    physical_process_get_physical_property = Util.load_func("PhysicalProcess_getPhysicalProperty",
                                                            [ct.c_int64],
                                                            ct.c_int64)

    # void PhysicalProcess_delete(PhysicalProcess *physicalProcess);
    physical_process_delete = Util.load_func("PhysicalProcess_delete", [ct.c_int64], None)

    # void PhysicalProcess_freeAll(PhysicalProcess *physicalProcess);
    physical_process_free_all = Util.load_func("PhysicalProcess_freeAll", [ct.c_int64], None)

    #################################################################
    # PhysicalForce Methods
    #

    # PhysicalForce *PhysicalForce_setAbout(
    #         PhysicalForce *physical_force_ptr, const char *about);
    physical_force_set_about = Util.load_func("PhysicalForce_setAbout", [ct.c_int64, ct.c_char_p],
                                              ct.c_int64)

    # PhysicalForce *PhysicalForce_setPhysicalProperty(
    #         PhysicalForce *physical_force_ptr, const char *physical_property);
    physical_force_set_physical_property = Util.load_func("PhysicalForce_setPhysicalProperty",
                                                          [ct.c_int64, ct.c_char_p], ct.c_int64)

    # PhysicalForce *PhysicalForce_addSource(
    #         PhysicalForce *physical_force_ptr, const char *source_metaid, double multiplier,
    #         const char *physical_entity_reference);
    physical_force_add_source = Util.load_func("PhysicalForce_addSource",
                                               [ct.c_int64, ct.c_char_p, ct.c_float, ct.c_char_p], ct.c_int64)

    # PhysicalForce *PhysicalForce_addSink(
    #         PhysicalForce *physical_force_ptr, const char *sink_metaid, double multiplier,
    #         const char *physical_entity_reference);
    physical_force_add_sink = Util.load_func("PhysicalForce_addSink",
                                             [ct.c_int64, ct.c_char_p, ct.c_float, ct.c_char_p], ct.c_int64)

    # char *PhysicalForce_str(PhysicalForce *physical_force_ptr, const char *format, const char *base_uri);
    physical_force_str = Util.load_func("PhysicalForce_str",
                                        [ct.c_int64, ct.c_char_p, ct.c_char_p], ct.c_int64)

    # char *PhysicalForce_getAbout(PhysicalForce *physical_force_ptr);
    physical_force_get_about = Util.load_func("PhysicalForce_getAbout", [ct.c_int64], ct.c_int64)

    # char *PhysicalForce_getPhysicalProperty(PhysicalForce *physical_force_ptr);
    physical_force_get_physical_property = Util.load_func("PhysicalForce_getPhysicalProperty",
                                                          [ct.c_int64],
                                                          ct.c_int64)

    # void PhysicalForce_delete(PhysicalForce *physicalForce);
    physical_force_delete = Util.load_func("PhysicalForce_delete", [ct.c_int64], None)

    # void PhysicalForce_freeAll(PhysicalForce *physical_force_ptr);
    physical_force_free_all = Util.load_func("PhysicalForce_freeAll", [ct.c_int64], None)

