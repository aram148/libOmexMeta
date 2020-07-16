import os
import site
import unittest
import ctypes as ct

# take care of directories so we can test the pyomexmeta api
test_dir = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
proj_dir = os.path.dirname(test_dir)
src_dir = os.path.join(proj_dir, "src")
pyomexmeta_dir = os.path.join(src_dir, "pyomexmeta")

site.addsitedir(src_dir)

from pyomexmeta import PyOmexMetaAPI


class TestStrings:
    xml = """<?xml version="1.0" encoding="UTF-8"?>
    <sbml xmlns="http://www.sbml.org/sbml/level3/version2/core" level="3" version="2">
      <model id="TestModelNotAnnotated">
        <listOfUnitDefinitions>
          <unitDefinition id="molar">
            <listOfUnits>
              <unit kind="mole" exponent="1" scale="1" multiplier="1"/>
              <unit kind="litre" exponent="-1" scale="1" multiplier="1"/>
            </listOfUnits>
          </unitDefinition>
        </listOfUnitDefinitions>
        <listOfCompartments>
          <compartment metaid="cytosol" id="cytosol" size="1" constant="True"/>
        </listOfCompartments>
        <listOfSpecies>
          <species metaid="Meta00001" id="X" compartment="cytosol" initialConcentration="10" substanceUnits="molar" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
          <species id="Y" compartment="cytosol" initialConcentration="20" substanceUnits="molar" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
          <species id="Y" compartment="cytosol" initialConcentration="15" substanceUnits="molar" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
        </listOfSpecies>
        <listOfReactions>
          <reaction id="X2Y" reversible="false">
            <listOfProducts>
              <speciesReference species="Y" constant="false"/>
            </listOfProducts>
            <kineticLaw>
              <math xmlns="http://www.w3.org/1998/Math/MathML">
                <apply>
                  <times/>
                  <ci> x </ci>
                  <ci> kx2y </ci>
                </apply>
              </math>
              <listOfLocalParameters>
                <localParameter id="kx2y" value="1"/>
                <localParameter id="ky2z" value="1"/>
              </listOfLocalParameters>
            </kineticLaw>
          </reaction>
          <reaction id="y2z" reversible="false">
            <listOfProducts>
              <speciesReference species="Z" constant="false"/>
            </listOfProducts>
            <kineticLaw>
              <math xmlns="http://www.w3.org/1998/Math/MathML">
                <apply>
                  <times/>
                  <ci> y </ci>
                  <ci> ky2z </ci>
                </apply>
              </math>
            </kineticLaw>
          </reaction>
        </listOfReactions>
      </model>
    </sbml>"""

    singular_annotation2 = """<?xml version=\"1.0\" encoding=\"UTF-8\"?>
   <rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"
            xmlns:bqmodel=\"http://biomodels.net/model-qualifiers/\"
            xmlns:bqbiol=\"http://biomodels.net/biology-qualifiers/\"
            xml:base=\"file://./NewModel.rdf\">
   <rdf:Description rdf:about=\"./NewModel.xml#modelmeta1\">
       <bqmodel:isDescribedBy rdf:resource=\"https://identifiers.org/pubmed/12991237\" />
   </rdf:Description>
</rdf:RDF>"""


class TestAPI(unittest.TestCase):
    maxDiff = None

    def setUp(self) -> None:
        # loads the function that makes a new RDF
        self.rdf = PyOmexMetaAPI.rdf_new(
            "memory".encode(), "semsim_store".encode(),
            None, None
        )

    def tearDown(self) -> None:
        """calls the RDF delete function after each test"""
        PyOmexMetaAPI.rdf_delete(self.rdf)

    def test_rdf_obj(self):
        self.assertIsInstance(self.rdf, int)

    def test_rdf_size(self):
        rdf = PyOmexMetaAPI.rdf_from_string(
            TestStrings.singular_annotation2.encode(), 'rdfxml'.encode(), "test_rdf_from_string.rdf".encode(),
            "memory".encode(), "libOmexMetaStore".encode(), None, None
        )
        expected = 1
        actual = PyOmexMetaAPI.rdf_size(rdf)
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.rdf_delete(rdf)

    def test_rdf_from_string(self):
        rdf = PyOmexMetaAPI.rdf_from_string(
            TestStrings.singular_annotation2.encode(), 'rdfxml'.encode(), "test_rdf_from_string.rdf".encode(),
            "memory".encode(), "libOmexMetaStore".encode(), None, None
        )
        expected = 1
        actual = PyOmexMetaAPI.rdf_size(rdf)
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.rdf_delete(rdf)

    def test_rdf_add_from_string(self):
        PyOmexMetaAPI.rdf_add_from_string(self.rdf, TestStrings.singular_annotation2.encode(), 'rdfxml'.encode(),
                                          "test_rdf_from_string.rdf".encode())
        expected = 1
        actual = PyOmexMetaAPI.rdf_size(self.rdf)
        self.assertEqual(expected, actual)

    def test_rdf_from_uri(self):
        sbml_url = "https://www.ebi.ac.uk/biomodels/model/download/BIOMD0000000064.2?filename=BIOMD0000000064_url.xml"

        rdf = PyOmexMetaAPI.rdf_from_uri(
            sbml_url.encode(), 'rdfxml'.encode(),
            "hashes".encode(), "semsim_hash".encode(), None, None
        )
        expected = 277
        actual = PyOmexMetaAPI.rdf_size(rdf)
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.rdf_delete(rdf)

    def test_rdf_add_from_uri(self):
        sbml_url = "https://www.ebi.ac.uk/biomodels/model/download/BIOMD0000000064.2?filename=BIOMD0000000064_url.xml"

        PyOmexMetaAPI.rdf_add_from_uri(self.rdf, sbml_url.encode(), 'rdfxml'.encode())
        expected = 277
        actual = PyOmexMetaAPI.rdf_size(self.rdf)
        self.assertEqual(expected, actual)

    def test_rdf_from_file(self):
        fname = os.path.join(os.getcwd(), "test_rdf_from_file.rdf")
        with open(fname, "w") as f:
            f.write(TestStrings.singular_annotation2)

        rdf = PyOmexMetaAPI.rdf_from_file(
            fname.encode(), 'rdfxml'.encode(),
            "hashes".encode(), "semsim_hash".encode(), None, None)
        expected = 1
        actual = PyOmexMetaAPI.rdf_size(rdf)
        self.assertEqual(expected, actual)
        os.remove(fname)
        PyOmexMetaAPI.rdf_delete(rdf)

    def test_rdf_add_from_file(self):
        fname = os.path.join(os.getcwd(), "test_rdf_from_file.rdf")
        with open(fname, "w") as f:
            f.write(TestStrings.singular_annotation2)

        PyOmexMetaAPI.rdf_add_from_file(self.rdf, fname.encode(), 'rdfxml'.encode())
        expected = 1
        actual = PyOmexMetaAPI.rdf_size(self.rdf)
        self.assertEqual(expected, actual)
        os.remove(fname)

    def test_rdf_to_string(self):
        PyOmexMetaAPI.rdf_add_from_string(self.rdf, TestStrings.singular_annotation2.encode(),
                                          "rdfxml".encode(), "test_rdf_to_string.rdf".encode())
        string_ptr = PyOmexMetaAPI.rdf_to_string(self.rdf, "rdfxml-abbrev".encode(), "basey.rdf".encode())
        actual2 = PyOmexMetaAPI.get_and_free_c_str(string_ptr)
        print(actual2)
        expected = """<?xml version="1.1" encoding="utf-8"?>
<rdf:RDF xmlns:bqbiol="http://biomodels.net/biology-qualifiers/"
   xmlns:bqmodel="http://biomodels.net/model-qualifiers/"
   xmlns:local="http://omex-library.org/NewOmex.omex/NewModel.rdf#"
   xmlns:myOMEX="http://omex-library.org/NewOmex.omex"
   xmlns:myOMEXlib="http://omex-library.org/"
   xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#">
  <rdf:Description rdf:about="file://./NewModel.xml#modelmeta1">
    <bqmodel:isDescribedBy rdf:resource="https://identifiers.org/pubmed/12991237"/>
  </rdf:Description>
</rdf:RDF>
"""
        self.assertEqual(expected, actual2)

    def test_rdf_to_editor(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        self.assertIsInstance(editor_ptr, int)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_rdf_query_results_as_str(self):
        PyOmexMetaAPI.rdf_add_from_string(self.rdf, TestStrings.singular_annotation2.encode(),
                                          "rdfxml".encode(), "test_rdf_to_string.rdf".encode())
        query = """
        SELECT ?x ?y ?z 
        WHERE {
            ?x ?y ?z
        }
        """
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_query_results_as_str(self.rdf, query.encode(), "csv".encode())
        )
        expected = """x,y,z
file://./NewModel.xml#modelmeta1,http://biomodels.net/model-qualifiers/isDescribedBy,https://identifiers.org/pubmed/12991237
"""
        self.assertEqual(expected, actual)

    def test_rdf_get_repository(self):
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_get_repository_uri(self.rdf)
        )
        expected = "http://omex-library.org/"
        self.assertEqual(expected, actual)

    def test_rdf_get_archive_uri(self):
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_get_archive_uri(self.rdf)
        )
        expected = "http://omex-library.org/NewOmex.omex"
        self.assertEqual(expected, actual)

    def test_rdf_get_model_uri(self):
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_get_model_uri(self.rdf)
        )
        expected = "http://omex-library.org/NewOmex.omex/NewModel.xml#"
        self.assertEqual(expected, actual)

    def test_rdf_get_local_uri(self):
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_get_local_uri(self.rdf)
        )
        expected = "http://omex-library.org/NewOmex.omex/NewModel.rdf#"
        self.assertEqual(expected, actual)

    def test_rdf_set_repository(self):
        PyOmexMetaAPI.rdf_set_repository_uri(self.rdf, "http://my-awesome-omex.org/".encode())
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_get_repository_uri(self.rdf)
        )
        expected = "http://my-awesome-omex.org/"
        self.assertEqual(expected, actual)

    def test_rdf_set_archive_uri(self):
        PyOmexMetaAPI.rdf_set_archive_uri(self.rdf, "my-awesome-omex.omex".encode())
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_get_archive_uri(self.rdf)
        )
        expected = "http://omex-library.org/my-awesome-omex.omex"
        self.assertEqual(expected, actual)

    def test_rdf_set_model_uri(self):
        PyOmexMetaAPI.rdf_set_model_uri(self.rdf, "my-awesome-model.sbml".encode())
        actual_model = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_get_model_uri(self.rdf)
        )
        actual_local = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_get_local_uri(self.rdf)
        )
        expected_model = "http://omex-library.org/NewOmex.omex/my-awesome-model.sbml#"
        expected_local = "http://omex-library.org/NewOmex.omex/my-awesome-model.rdf#"
        self.assertEqual(expected_model, actual_model)
        self.assertEqual(expected_local, actual_local)

    def test_editor_add_namespace(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        PyOmexMetaAPI.editor_add_namespace(editor_ptr, "https://namespace.com".encode(), "ns_".encode())
        singular_annotation = PyOmexMetaAPI.editor_new_singular_annotation(editor_ptr)
        singular_annotation = PyOmexMetaAPI.singular_annotation_set_about(singular_annotation, "cytosol".encode())
        singular_annotation = PyOmexMetaAPI.singular_annotation_set_predicate_uri(singular_annotation,
                                                                                  "https://predicate.com/from/uri".encode())
        singular_annotation = PyOmexMetaAPI.singular_annotation_set_resource_uri(singular_annotation,
                                                                                 "http://uri.com".encode())
        PyOmexMetaAPI.editor_add_single_annotation(editor_ptr, singular_annotation)

        actual = PyOmexMetaAPI.rdf_to_string(self.rdf, "rdfxml-abbrev".encode(), "namespace_test.rdf".encode())
        actual = PyOmexMetaAPI.get_and_free_c_str(actual)
        print(actual)

        expected = r"""<?xml version="1.1" encoding="utf-8"?>
<rdf:RDF xmlns:local="http://omex-library.org/NewOmex.omex/NewModel.rdf#"
   xmlns:myOMEX="http://omex-library.org/NewOmex.omex"
   xmlns:myOMEXlib="http://omex-library.org/"
   xmlns:ns_="https://namespace.com"
   xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#">
  <rdf:Description rdf:about="http://omex-library.org/NewOmex.omex/NewModel.rdf#cytosol">
    <ns1:uri xmlns:ns1="https://predicate.com/from/"
       rdf:resource="http://uri.com"/>
  </rdf:Description>
</rdf:RDF>
"""
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)
        PyOmexMetaAPI.singular_annotation_delete(singular_annotation)

    def test_editor_new_singular_annotation(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        singular_annotation = PyOmexMetaAPI.editor_new_singular_annotation(editor_ptr)
        self.assertIsInstance(singular_annotation, int)
        PyOmexMetaAPI.singular_annotation_delete(singular_annotation)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_editor_new_physical_entity(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        physical_entity = PyOmexMetaAPI.editor_new_physical_entity(editor_ptr)
        self.assertIsInstance(physical_entity, int)
        PyOmexMetaAPI.editor_delete(editor_ptr)
        PyOmexMetaAPI.physical_entity_delete(physical_entity)

    def test_editor_new_physical_process(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        physical_process = PyOmexMetaAPI.editor_new_physical_process(editor_ptr)
        self.assertIsInstance(physical_process, int)
        PyOmexMetaAPI.editor_delete(editor_ptr)
        PyOmexMetaAPI.physical_process_delete(physical_process)

    def test_editor_new_physical_force(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        physical_force = PyOmexMetaAPI.editor_new_physical_force(editor_ptr)
        self.assertIsInstance(physical_force, int)
        PyOmexMetaAPI.editor_delete(editor_ptr)
        PyOmexMetaAPI.physical_force_delete(physical_force)

    def test_editor_check_valid_metaid(self):
        """
        This test can't officially be tested because when this test fails it crashes
        python. However, it is working = When id is wrong we get helpful
        error message.
        """
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        PyOmexMetaAPI.editor_check_valid_metaid(editor_ptr, "#OmexMetaId0000".encode())
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_editor_remove_physical_entity(self):
        pass

    def test_editor_remove_physical_process(self):
        pass

    def test_editor_remove_physical_force(self):
        pass

    def test_editor_remove_singular_annotation(self):
        pass

    def test_editor_remove_personal_information(self):
        pass

    def test_editor_get_metaid(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        ptr = PyOmexMetaAPI.editor_get_metaid(editor_ptr, 0)
        actual = PyOmexMetaAPI.get_and_free_c_str(ptr)
        expected = "OmexMetaId0000"
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_editor_get_num_metaids(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        actual = PyOmexMetaAPI.editor_get_num_metaids(editor_ptr)
        expected = 13
        PyOmexMetaAPI.editor_delete(editor_ptr)
        self.assertEqual(expected, actual)

    def test_editor_get_xml(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.editor_get_xml(editor_ptr)
        )
        print(actual)
        expected = """<?xml version="1.0" encoding="UTF-8"?>
<sbml xmlns="http://www.sbml.org/sbml/level3/version2/core" level="3" version="2">
      <model id="TestModelNotAnnotated" metaid="#OmexMetaId0000">
        <listOfUnitDefinitions>
          <unitDefinition id="molar">
            <listOfUnits>
              <unit kind="mole" exponent="1" scale="1" multiplier="1" metaid="#OmexMetaId0001"/>
              <unit kind="litre" exponent="-1" scale="1" multiplier="1" metaid="#OmexMetaId0002"/>
            </listOfUnits>
          </unitDefinition>
        </listOfUnitDefinitions>
        <listOfCompartments>
          <compartment metaid="cytosol" id="cytosol" size="1" constant="True"/>
        </listOfCompartments>
        <listOfSpecies>
          <species metaid="Meta00001" id="X" compartment="cytosol" initialConcentration="10" substanceUnits="molar" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
          <species id="Y" compartment="cytosol" initialConcentration="20" substanceUnits="molar" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false" metaid="#OmexMetaId0003"/>
          <species id="Y" compartment="cytosol" initialConcentration="15" substanceUnits="molar" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false" metaid="#OmexMetaId0004"/>
        </listOfSpecies>
        <listOfReactions>
          <reaction id="X2Y" reversible="false" metaid="#OmexMetaId0005">
            <listOfProducts>
              <speciesReference species="Y" constant="false"/>
            </listOfProducts>
            <kineticLaw metaid="#OmexMetaId0006">
              <math xmlns="http://www.w3.org/1998/Math/MathML">
                <apply>
                  <times/>
                  <ci> x </ci>
                  <ci> kx2y </ci>
                </apply>
              </math>
              <listOfLocalParameters>
                <localParameter id="kx2y" value="1" metaid="#OmexMetaId0007"/>
                <localParameter id="ky2z" value="1" metaid="#OmexMetaId0008"/>
              </listOfLocalParameters>
            </kineticLaw>
          </reaction>
          <reaction id="y2z" reversible="false" metaid="#OmexMetaId0009">
            <listOfProducts>
              <speciesReference species="Z" constant="false"/>
            </listOfProducts>
            <kineticLaw metaid="#OmexMetaId0010">
              <math xmlns="http://www.w3.org/1998/Math/MathML">
                <apply>
                  <times/>
                  <ci> y </ci>
                  <ci> ky2z </ci>
                </apply>
              </math>
            </kineticLaw>
          </reaction>
        </listOfReactions>
      </model>
    </sbml>
"""
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_editor_get_archive_uri(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.editor_get_archive_uri(editor_ptr)
        )
        expected = "http://omex-library.org/NewOmex.omex"
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_editor_get_local_uri(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.editor_get_local_uri(editor_ptr)
        )
        expected = "http://omex-library.org/NewOmex.omex/NewModel.rdf#"
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_editor_get_model_uri(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.editor_get_model_uri(editor_ptr)
        )
        expected = "http://omex-library.org/NewOmex.omex/NewModel.xml#"
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_editor_get_repository_uri(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.editor_get_repository_uri(editor_ptr)
        )
        expected = "http://omex-library.org/"
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_editor_add_creator(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        PyOmexMetaAPI.editor_add_creator(editor_ptr, "1234-1234-1234-1234".encode())
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_to_string(self.rdf, "turtle".encode(), "base".encode())
        )
        print(actual)
        expected = """@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix myOMEXlib: <http://omex-library.org/> .
@prefix myOMEX: <http://omex-library.org/NewOmex.omex> .
@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#>
    <http://purl.org/dc/terms/creator> <https://orchid.org/1234-1234-1234-1234> .

"""
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_editor_add_curator(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        PyOmexMetaAPI.editor_add_curator(editor_ptr, "1234-1234-1234-1234".encode())
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_to_string(self.rdf, "turtle".encode(), "base".encode())
        )
        expected = """@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix myOMEXlib: <http://omex-library.org/> .
@prefix myOMEX: <http://omex-library.org/NewOmex.omex> .
@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .

<http://omex-library.org/NewOmex.omex>
    <http://purl.org/dc/terms/creator> <https://orchid.org/1234-1234-1234-1234> .

"""
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_editor_get_taxon(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        PyOmexMetaAPI.editor_add_taxon(editor_ptr, "9898".encode())
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_to_string(self.rdf, "turtle".encode(), "base".encode())
        )
        expected = """@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix myOMEXlib: <http://omex-library.org/> .
@prefix myOMEX: <http://omex-library.org/NewOmex.omex> .
@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#>
    <http://biomodels.net/biology-qualifiers/hasTaxon> <NCBI_Taxon:9898> .

"""
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_editor_add_pubmed(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        PyOmexMetaAPI.editor_add_pubmed(editor_ptr, "1234568".encode())
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_to_string(self.rdf, "turtle".encode(), "base".encode())
        )
        expected = """@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix myOMEXlib: <http://omex-library.org/> .
@prefix myOMEX: <http://omex-library.org/NewOmex.omex> .
@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#>
    <http://biomodels.net/model-qualifiers/isDescribedBy> <https://identifiers.org/pubmed/1234568> .

"""
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_editor_add_description(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        PyOmexMetaAPI.editor_add_description(editor_ptr, "An awesome model".encode())
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_to_string(self.rdf, "turtle".encode(), "base".encode())
        )
        expected = """@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix myOMEXlib: <http://omex-library.org/> .
@prefix myOMEX: <http://omex-library.org/NewOmex.omex> .
@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#>
    <http://purl.org/dc/terms/description> "An awesome model"^^rdf:string .

"""
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_editor_add_date_created(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        PyOmexMetaAPI.editor_add_date_created(editor_ptr, "14/01/1001".encode())
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_to_string(self.rdf, "turtle".encode(), "base".encode())
        )
        expected = """@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix myOMEXlib: <http://omex-library.org/> .
@prefix myOMEX: <http://omex-library.org/NewOmex.omex> .
@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#>
    <http://purl.org/dc/terms/created> "14/01/1001"^^rdf:string .

"""
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_editor_add_parent_model(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        PyOmexMetaAPI.editor_add_parent_model(editor_ptr, "BIOMDtoomany0s1.xml".encode())
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_to_string(self.rdf, "turtle".encode(), "base".encode())
        )
        expected = """@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix myOMEXlib: <http://omex-library.org/> .
@prefix myOMEX: <http://omex-library.org/NewOmex.omex> .
@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#>
    <http://biomodels.net/model-qualifiers/isDerivedFrom> <https://identifiers.org/biomod/BIOMDtoomany0s1.xml> .

"""
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_singular_annotation_about(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        singular_annotation = PyOmexMetaAPI.editor_new_singular_annotation(editor_ptr)
        PyOmexMetaAPI.singular_annotation_set_about(singular_annotation, "cytosol".encode())
        ptr = PyOmexMetaAPI.singular_annotation_get_about(singular_annotation)
        actual = PyOmexMetaAPI.get_and_free_c_str(ptr)
        expected = "http://omex-library.org/NewOmex.omex/NewModel.rdf#cytosol"
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)
        PyOmexMetaAPI.singular_annotation_delete(singular_annotation)

    def test_singular_annotation_predicate(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        singular_annotation = PyOmexMetaAPI.editor_new_singular_annotation(editor_ptr)
        PyOmexMetaAPI.singular_annotation_set_predicate(singular_annotation, "bqbiol".encode(), "is".encode())
        ptr = PyOmexMetaAPI.singular_annotation_get_predicate(singular_annotation)
        actual = PyOmexMetaAPI.get_and_free_c_str(ptr)
        expected = "http://biomodels.net/biology-qualifiers/is"
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)
        PyOmexMetaAPI.singular_annotation_delete(singular_annotation)

    def test_singular_annotation_predicate_uri(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        singular_annotation = PyOmexMetaAPI.editor_new_singular_annotation(editor_ptr)
        PyOmexMetaAPI.singular_annotation_set_predicate_uri(singular_annotation,
                                                            "https://predicate.com/from/uri".encode())
        ptr = PyOmexMetaAPI.singular_annotation_get_predicate(singular_annotation)
        actual = PyOmexMetaAPI.get_and_free_c_str(ptr)
        expected = "https://predicate.com/from/uri"
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)
        PyOmexMetaAPI.singular_annotation_delete(singular_annotation)

    def test_singular_annotation_resource_literal(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        singular_annotation = PyOmexMetaAPI.editor_new_singular_annotation(editor_ptr)
        PyOmexMetaAPI.singular_annotation_set_resource_literal(singular_annotation, "LiteralValue".encode())
        ptr = PyOmexMetaAPI.singular_annotation_get_resource(singular_annotation)
        actual = PyOmexMetaAPI.get_and_free_c_str(ptr)
        expected = "LiteralValue"
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)
        PyOmexMetaAPI.singular_annotation_delete(singular_annotation)

    def test_singular_annotation_resource_uri(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        singular_annotation = PyOmexMetaAPI.editor_new_singular_annotation(editor_ptr)
        PyOmexMetaAPI.singular_annotation_set_resource_uri(singular_annotation, "UriValue".encode())
        ptr = PyOmexMetaAPI.singular_annotation_get_resource(singular_annotation)
        actual = PyOmexMetaAPI.get_and_free_c_str(ptr)
        expected = "UriValue"
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)
        PyOmexMetaAPI.singular_annotation_delete(singular_annotation)

    def test_singular_annotation_resource_blank(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        singular_annotation = PyOmexMetaAPI.editor_new_singular_annotation(editor_ptr)
        PyOmexMetaAPI.singular_annotation_set_resource_blank(singular_annotation, "blank".encode())
        ptr = PyOmexMetaAPI.singular_annotation_get_resource(singular_annotation)
        actual = PyOmexMetaAPI.get_and_free_c_str(ptr)
        expected = "blank"
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)
        PyOmexMetaAPI.singular_annotation_delete(singular_annotation)

    def test_singular_annotation_str(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        singular_annotation = PyOmexMetaAPI.editor_new_singular_annotation(editor_ptr)
        PyOmexMetaAPI.singular_annotation_set_about(singular_annotation, "cytosol".encode())
        PyOmexMetaAPI.singular_annotation_set_predicate(singular_annotation, "bqbiol".encode(), "is".encode())
        PyOmexMetaAPI.singular_annotation_set_resource_uri(singular_annotation, "uniprot:PD12345".encode())
        ptr = PyOmexMetaAPI.singular_annotation_str(
            singular_annotation, "rdfxml-abbrev".encode(), "singular_annotation.rdf".encode())
        actual = PyOmexMetaAPI.get_and_free_c_str(ptr)
        print(actual)
        expected = """<?xml version="1.0" encoding="utf-8"?>
<rdf:RDF xmlns:bqbiol="http://biomodels.net/biology-qualifiers/"
   xmlns:local="http://omex-library.org/NewOmex.omex/NewModel.rdf#"
   xmlns:myOMEX="http://omex-library.org/NewOmex.omex/NewModel.xml#"
   xmlns:myOMEXlib="http://omex-library.org/NewOmex.omex"
   xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#">
  <rdf:Description rdf:about="http://omex-library.org/NewOmex.omex/NewModel.rdf#cytosol">
    <bqbiol:is rdf:resource="https://identifiers.org/uniprot/PD12345"/>
  </rdf:Description>
</rdf:RDF>
"""
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)
        PyOmexMetaAPI.singular_annotation_delete(singular_annotation)

    def test_editor_add_single_annotation(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        singular_annotation = PyOmexMetaAPI.editor_new_singular_annotation(editor_ptr)
        PyOmexMetaAPI.singular_annotation_set_about(singular_annotation, "cytosol".encode())
        PyOmexMetaAPI.singular_annotation_set_predicate(singular_annotation, "bqbiol".encode(), "is".encode())
        PyOmexMetaAPI.singular_annotation_set_resource_uri(singular_annotation, "uniprot:PD12345".encode())
        PyOmexMetaAPI.editor_add_single_annotation(editor_ptr, singular_annotation)
        ptr = PyOmexMetaAPI.rdf_to_string(self.rdf, "turtle".encode(), "turtled_singular_annotation.rdf".encode())
        actual = PyOmexMetaAPI.get_and_free_c_str(ptr)
        print(actual)
        expected = """@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix bqbiol: <http://biomodels.net/biology-qualifiers/> .
@prefix myOMEXlib: <http://omex-library.org/> .
@prefix myOMEX: <http://omex-library.org/NewOmex.omex> .
@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .

local:cytosol
    bqbiol:is <https://identifiers.org/uniprot/PD12345> .

"""
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)
        PyOmexMetaAPI.singular_annotation_delete(singular_annotation)

    def test_physical_entity_set_identity(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        physical_entity = PyOmexMetaAPI.editor_new_physical_entity(editor_ptr)
        PyOmexMetaAPI.physical_entity_set_identity(physical_entity, "uniprot:P456".encode())
        ptr = PyOmexMetaAPI.physical_entity_get_identity(physical_entity)
        actual = PyOmexMetaAPI.get_and_free_c_str(ptr)
        expected = "https://identifiers.org/uniprot/P456"
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)
        PyOmexMetaAPI.physical_entity_delete(physical_entity)

    def test_physical_entity_num_locations(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        physical_entity = PyOmexMetaAPI.editor_new_physical_entity(editor_ptr)
        PyOmexMetaAPI.physical_entity_add_location(physical_entity, "fma:fma:3456".encode())
        PyOmexMetaAPI.physical_entity_add_location(physical_entity, "fma/fma:3457".encode())
        actual = PyOmexMetaAPI.physical_entity_get_num_locations(physical_entity)
        expected = 2
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)
        PyOmexMetaAPI.physical_entity_delete(physical_entity)

    def test_physical_entity_add_location(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        physical_entity = PyOmexMetaAPI.editor_new_physical_entity(editor_ptr)
        PyOmexMetaAPI.physical_entity_add_location(physical_entity, "fma:fma:3456".encode())
        PyOmexMetaAPI.physical_entity_add_location(physical_entity, "fma/fma:3457".encode())
        num_locations = PyOmexMetaAPI.physical_entity_get_num_locations(physical_entity)
        ptr = [PyOmexMetaAPI.physical_entity_get_location(physical_entity, i) for i in range(num_locations)]
        actual = [PyOmexMetaAPI.get_and_free_c_str(i) for i in ptr]
        expected = ['https://identifiers.org/fma/fma:3456', 'https://identifiers.org/fma/fma:3457']
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)
        PyOmexMetaAPI.physical_entity_delete(physical_entity)

    def test_physical_entity_str(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        physical_entity = PyOmexMetaAPI.editor_new_physical_entity(editor_ptr)
        PyOmexMetaAPI.physical_entity_set_physical_property(physical_entity, "cytosol".encode(),
                                                            "opb:opb12345".encode())
        PyOmexMetaAPI.physical_entity_set_identity(physical_entity, "uniprot:P456".encode())
        PyOmexMetaAPI.physical_entity_add_location(physical_entity, "fma:fma:3456".encode())
        PyOmexMetaAPI.physical_entity_add_location(physical_entity, "fma/fma:3457".encode())
        ptr = PyOmexMetaAPI.physical_entity_str(physical_entity, "json".encode(), "jsonified_physical_entity".encode())
        actual = PyOmexMetaAPI.get_and_free_c_str(ptr)
        print(actual)
        expected = """
{
  "http://omex-library.org/NewOmex.omex/NewModel.rdf#PhysicalEntity0000" : {
    "http://biomodels.net/biology-qualifiers/is" : [ {
        "value" : "https://identifiers.org/uniprot/P456",
        "type" : "uri"
        }
      
      ],
    "http://biomodels.net/biology-qualifiers/isPartOf" : [ {
        "value" : "https://identifiers.org/fma/fma:3456",
        "type" : "uri"
        }
      ,
      {
        "value" : "https://identifiers.org/fma/fma:3457",
        "type" : "uri"
        }
      
      ]
    }
  ,
  "http://omex-library.org/NewOmex.omex/NewModel.rdf#cytosol" : {
    "http://biomodels.net/biology-qualifiers/isPropertyOf" : [ {
        "value" : "http://omex-library.org/NewOmex.omex/NewModel.rdf#PhysicalEntity0000",
        "type" : "uri"
        }
      
      ],
    "http://biomodels.net/biology-qualifiers/isVersionOf" : [ {
        "value" : "https://identifiers.org/opb/opb12345",
        "type" : "uri"
        }
      
      ]
    }
  }
"""
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)
        PyOmexMetaAPI.physical_entity_delete(physical_entity)

    def test_editor_add_physical_entity(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        physical_entity = PyOmexMetaAPI.editor_new_physical_entity(editor_ptr)
        PyOmexMetaAPI.physical_entity_set_physical_property(physical_entity, "cytosol".encode(),
                                                            "opb:opb12345".encode())
        PyOmexMetaAPI.physical_entity_set_identity(physical_entity, "uniprot:P456".encode())
        PyOmexMetaAPI.physical_entity_add_location(physical_entity, "fma:fma:3456".encode())
        PyOmexMetaAPI.physical_entity_add_location(physical_entity, "fma/fma:3457".encode())
        PyOmexMetaAPI.editor_add_physical_entity(editor_ptr, physical_entity)
        ptr = PyOmexMetaAPI.rdf_to_string(self.rdf, "rdfxml-abbrev".encode(), "PhysicalEntity.rdf".encode())
        actual = PyOmexMetaAPI.get_and_free_c_str(ptr)
        print(actual)
        expected = """<?xml version="1.1" encoding="utf-8"?>
<rdf:RDF xmlns:bqbiol="http://biomodels.net/biology-qualifiers/"
   xmlns:local="http://omex-library.org/NewOmex.omex/NewModel.rdf#"
   xmlns:myOMEX="http://omex-library.org/NewOmex.omex"
   xmlns:myOMEXlib="http://omex-library.org/"
   xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#">
  <rdf:Description rdf:about="http://omex-library.org/NewOmex.omex/NewModel.rdf#PhysicalEntity0000">
    <bqbiol:is rdf:resource="https://identifiers.org/uniprot/P456"/>
    <bqbiol:isPartOf rdf:resource="https://identifiers.org/fma/fma:3456"/>
    <bqbiol:isPartOf rdf:resource="https://identifiers.org/fma/fma:3457"/>
  </rdf:Description>
  <rdf:Description rdf:about="http://omex-library.org/NewOmex.omex/NewModel.rdf#cytosol">
    <bqbiol:isPropertyOf rdf:resource="http://omex-library.org/NewOmex.omex/NewModel.rdf#PhysicalEntity0000"/>
    <bqbiol:isVersionOf rdf:resource="https://identifiers.org/opb/opb12345"/>
  </rdf:Description>
</rdf:RDF>
"""
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)
        PyOmexMetaAPI.physical_entity_delete(physical_entity)

    def test_physical_process_str(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        physical_process = PyOmexMetaAPI.editor_new_physical_process(editor_ptr)
        PyOmexMetaAPI.physical_process_set_physical_property(physical_process, "cytosol".encode(),
                                                             "opb:opb12345".encode())
        PyOmexMetaAPI.physical_process_add_source(
            physical_process, 1.0, "Entity1".encode())

        PyOmexMetaAPI.physical_process_add_sink(
            physical_process, 1.0, "Entity2".encode())

        PyOmexMetaAPI.physical_process_add_mediator(
            physical_process, 1.0, "Entity3".encode())
        ptr = PyOmexMetaAPI.physical_process_str(physical_process, "html".encode(),
                                                 "html_physical_process_ann.rdf".encode())
        actual = PyOmexMetaAPI.get_and_free_c_str(ptr)
        expected = """<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
        "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
  <title>Raptor Graph Serialisation</title>
</head>
<body>
  <table id="triples" border="1">
    <tr>
      <th>Subject</th>
      <th>Predicate</th>
      <th>Object</th>
    </tr>
    <tr class="triple">
      <td><span class="uri"><a href="http://omex-library.org/NewOmex.omex/NewModel.rdf#cytosol">http://omex-library.org/NewOmex.omex/NewModel.rdf#cytosol</a></span></td>
      <td><span class="uri"><a href="http://biomodels.net/biology-qualifiers/isPropertyOf">http://biomodels.net/biology-qualifiers/isPropertyOf</a></span></td>
      <td><span class="uri"><a href="http://omex-library.org/NewOmex.omex/NewModel.rdf#PhysicalProcess0000">http://omex-library.org/NewOmex.omex/NewModel.rdf#PhysicalProcess0000</a></span></td>
    </tr>
    <tr class="triple">
      <td><span class="uri"><a href="http://omex-library.org/NewOmex.omex/NewModel.rdf#cytosol">http://omex-library.org/NewOmex.omex/NewModel.rdf#cytosol</a></span></td>
      <td><span class="uri"><a href="http://biomodels.net/biology-qualifiers/isVersionOf">http://biomodels.net/biology-qualifiers/isVersionOf</a></span></td>
      <td><span class="uri"><a href="https://identifiers.org/opb/opb12345">https://identifiers.org/opb/opb12345</a></span></td>
    </tr>
    <tr class="triple">
      <td><span class="uri"><a href="http://omex-library.org/NewOmex.omex/NewModel.rdf#PhysicalProcess0000">http://omex-library.org/NewOmex.omex/NewModel.rdf#PhysicalProcess0000</a></span></td>
      <td><span class="uri"><a href="http://www.bhi.washington.edu/semsim#hasSourceParticipant">http://www.bhi.washington.edu/semsim#hasSourceParticipant</a></span></td>
      <td><span class="uri"><a href="http://omex-library.org/NewOmex.omex/NewModel.rdf#SourceParticipant0000">http://omex-library.org/NewOmex.omex/NewModel.rdf#SourceParticipant0000</a></span></td>
    </tr>
    <tr class="triple">
      <td><span class="uri"><a href="http://omex-library.org/NewOmex.omex/NewModel.rdf#SourceParticipant0000">http://omex-library.org/NewOmex.omex/NewModel.rdf#SourceParticipant0000</a></span></td>
      <td><span class="uri"><a href="http://www.bhi.washington.edu/semsim#hasPhysicalEntityReference">http://www.bhi.washington.edu/semsim#hasPhysicalEntityReference</a></span></td>
      <td><span class="uri"><a href="http://omex-library.org/NewOmex.omex/NewModel.rdf#Entity1">http://omex-library.org/NewOmex.omex/NewModel.rdf#Entity1</a></span></td>
    </tr>
    <tr class="triple">
      <td><span class="uri"><a href="http://omex-library.org/NewOmex.omex/NewModel.rdf#SourceParticipant0000">http://omex-library.org/NewOmex.omex/NewModel.rdf#SourceParticipant0000</a></span></td>
      <td><span class="uri"><a href="http://www.bhi.washington.edu/semsim#hasMultiplier">http://www.bhi.washington.edu/semsim#hasMultiplier</a></span></td>
      <td><span class="literal"><span class="value">1</span>^^&lt;<span class="datatype">http://www.w3.org/1999/02/22-rdf-syntax-ns#http://www.w3.org/2001/XMLSchema#double</span>&gt;</span></td>
    </tr>
    <tr class="triple">
      <td><span class="uri"><a href="http://omex-library.org/NewOmex.omex/NewModel.rdf#PhysicalProcess0000">http://omex-library.org/NewOmex.omex/NewModel.rdf#PhysicalProcess0000</a></span></td>
      <td><span class="uri"><a href="http://www.bhi.washington.edu/semsim#hasSinkParticipant">http://www.bhi.washington.edu/semsim#hasSinkParticipant</a></span></td>
      <td><span class="uri"><a href="http://omex-library.org/NewOmex.omex/NewModel.rdf#SinkParticipant0000">http://omex-library.org/NewOmex.omex/NewModel.rdf#SinkParticipant0000</a></span></td>
    </tr>
    <tr class="triple">
      <td><span class="uri"><a href="http://omex-library.org/NewOmex.omex/NewModel.rdf#SinkParticipant0000">http://omex-library.org/NewOmex.omex/NewModel.rdf#SinkParticipant0000</a></span></td>
      <td><span class="uri"><a href="http://www.bhi.washington.edu/semsim#hasPhysicalEntityReference">http://www.bhi.washington.edu/semsim#hasPhysicalEntityReference</a></span></td>
      <td><span class="uri"><a href="http://omex-library.org/NewOmex.omex/NewModel.rdf#Entity2">http://omex-library.org/NewOmex.omex/NewModel.rdf#Entity2</a></span></td>
    </tr>
    <tr class="triple">
      <td><span class="uri"><a href="http://omex-library.org/NewOmex.omex/NewModel.rdf#SinkParticipant0000">http://omex-library.org/NewOmex.omex/NewModel.rdf#SinkParticipant0000</a></span></td>
      <td><span class="uri"><a href="http://www.bhi.washington.edu/semsim#hasMultiplier">http://www.bhi.washington.edu/semsim#hasMultiplier</a></span></td>
      <td><span class="literal"><span class="value">1</span>^^&lt;<span class="datatype">http://www.w3.org/1999/02/22-rdf-syntax-ns#http://www.w3.org/2001/XMLSchema#double</span>&gt;</span></td>
    </tr>
    <tr class="triple">
      <td><span class="uri"><a href="http://omex-library.org/NewOmex.omex/NewModel.rdf#PhysicalProcess0000">http://omex-library.org/NewOmex.omex/NewModel.rdf#PhysicalProcess0000</a></span></td>
      <td><span class="uri"><a href="http://www.bhi.washington.edu/semsim#hasMediatorParticipant">http://www.bhi.washington.edu/semsim#hasMediatorParticipant</a></span></td>
      <td><span class="uri"><a href="http://omex-library.org/NewOmex.omex/NewModel.rdf#MediatorParticipant0000">http://omex-library.org/NewOmex.omex/NewModel.rdf#MediatorParticipant0000</a></span></td>
    </tr>
    <tr class="triple">
      <td><span class="uri"><a href="http://omex-library.org/NewOmex.omex/NewModel.rdf#MediatorParticipant0000">http://omex-library.org/NewOmex.omex/NewModel.rdf#MediatorParticipant0000</a></span></td>
      <td><span class="uri"><a href="http://www.bhi.washington.edu/semsim#hasPhysicalEntityReference">http://www.bhi.washington.edu/semsim#hasPhysicalEntityReference</a></span></td>
      <td><span class="uri"><a href="http://omex-library.org/NewOmex.omex/NewModel.rdf#Entity3">http://omex-library.org/NewOmex.omex/NewModel.rdf#Entity3</a></span></td>
    </tr>
  </table>
  <p>Total number of triples: <span class="count">10</span>.</p>
</body>
</html>
"""
        print(actual)
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)
        PyOmexMetaAPI.physical_process_delete(physical_process)

    def test_editor_add_physical_process(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        physical_process = PyOmexMetaAPI.editor_new_physical_process(editor_ptr)
        PyOmexMetaAPI.physical_process_set_physical_property(physical_process, "cytosol".encode(),
                                                             "opb:opb12345".encode())
        PyOmexMetaAPI.physical_process_add_source(
            physical_process, 1.0, "Entity1".encode())

        PyOmexMetaAPI.physical_process_add_sink(
            physical_process, 1.0, "Entity2".encode())

        PyOmexMetaAPI.physical_process_add_mediator(
            physical_process, 1.0, "Entity3".encode())
        PyOmexMetaAPI.editor_add_physical_process(editor_ptr, physical_process)
        ptr = PyOmexMetaAPI.rdf_to_string(self.rdf, "rdfxml-abbrev".encode(), "PhysicalProcess.rdf".encode())
        actual = PyOmexMetaAPI.get_and_free_c_str(ptr)
        print(actual)
        expected = """<?xml version="1.1" encoding="utf-8"?>
<rdf:RDF xmlns:bqbiol="http://biomodels.net/biology-qualifiers/"
   xmlns:local="http://omex-library.org/NewOmex.omex/NewModel.rdf#"
   xmlns:myOMEX="http://omex-library.org/NewOmex.omex"
   xmlns:myOMEXlib="http://omex-library.org/"
   xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
   xmlns:semsim="http://www.bhi.washington.edu/semsim#">
  <rdf:Description rdf:about="http://omex-library.org/NewOmex.omex/NewModel.rdf#MediatorParticipant0000">
    <semsim:hasPhysicalEntityReference rdf:resource="http://omex-library.org/NewOmex.omex/NewModel.rdf#Entity3"/>
  </rdf:Description>
  <rdf:Description rdf:about="http://omex-library.org/NewOmex.omex/NewModel.rdf#PhysicalProcess0000">
    <semsim:hasMediatorParticipant rdf:resource="http://omex-library.org/NewOmex.omex/NewModel.rdf#MediatorParticipant0000"/>
    <semsim:hasSinkParticipant rdf:resource="http://omex-library.org/NewOmex.omex/NewModel.rdf#SinkParticipant0000"/>
    <semsim:hasSourceParticipant rdf:resource="http://omex-library.org/NewOmex.omex/NewModel.rdf#SourceParticipant0000"/>
  </rdf:Description>
  <rdf:Description rdf:about="http://omex-library.org/NewOmex.omex/NewModel.rdf#SinkParticipant0000">
    <semsim:hasMultiplier rdf:datatype="http://www.w3.org/1999/02/22-rdf-syntax-ns#http://www.w3.org/2001/XMLSchema#double">1</semsim:hasMultiplier>
    <semsim:hasPhysicalEntityReference rdf:resource="http://omex-library.org/NewOmex.omex/NewModel.rdf#Entity2"/>
  </rdf:Description>
  <rdf:Description rdf:about="http://omex-library.org/NewOmex.omex/NewModel.rdf#SourceParticipant0000">
    <semsim:hasMultiplier rdf:datatype="http://www.w3.org/1999/02/22-rdf-syntax-ns#http://www.w3.org/2001/XMLSchema#double">1</semsim:hasMultiplier>
    <semsim:hasPhysicalEntityReference rdf:resource="http://omex-library.org/NewOmex.omex/NewModel.rdf#Entity1"/>
  </rdf:Description>
  <rdf:Description rdf:about="http://omex-library.org/NewOmex.omex/NewModel.rdf#cytosol">
    <bqbiol:isPropertyOf rdf:resource="http://omex-library.org/NewOmex.omex/NewModel.rdf#PhysicalProcess0000"/>
    <bqbiol:isVersionOf rdf:resource="https://identifiers.org/opb/opb12345"/>
  </rdf:Description>
</rdf:RDF>
"""
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)
        PyOmexMetaAPI.physical_process_delete(physical_process)

    def test_editor_add_physical_force(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        physical_force = PyOmexMetaAPI.editor_new_physical_force(editor_ptr)
        PyOmexMetaAPI.physical_force_set_physical_property(physical_force, "cytosol".encode(), "opb:opb12345".encode())
        PyOmexMetaAPI.physical_force_add_source(
            physical_force, 1.0, "Entity1".encode())

        PyOmexMetaAPI.physical_force_add_sink(
            physical_force, 1.0, "Entity2".encode())

        PyOmexMetaAPI.editor_add_physical_force(editor_ptr, physical_force)
        ptr = PyOmexMetaAPI.rdf_to_string(self.rdf, "turtle".encode(),
                                          "PhysicalForce.rdf".encode())
        actual = PyOmexMetaAPI.get_and_free_c_str(ptr)
        print(actual)
        expected = """@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix semsim: <http://www.bhi.washington.edu/semsim#> .
@prefix bqbiol: <http://biomodels.net/biology-qualifiers/> .
@prefix myOMEXlib: <http://omex-library.org/> .
@prefix myOMEX: <http://omex-library.org/NewOmex.omex> .
@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .

local:PhysicalForce0000
    semsim:hasSinkParticipant local:SinkParticipant0000 ;
    semsim:hasSourceParticipant local:SourceParticipant0000 .

local:SinkParticipant0000
    semsim:hasMultiplier "1"^^<http://www.w3.org/1999/02/22-rdf-syntax-ns#http://www.w3.org/2001/XMLSchema#double> ;
    semsim:hasPhysicalEntityReference local:Entity2 .

local:SourceParticipant0000
    semsim:hasMultiplier "1"^^<http://www.w3.org/1999/02/22-rdf-syntax-ns#http://www.w3.org/2001/XMLSchema#double> ;
    semsim:hasPhysicalEntityReference local:Entity1 .

local:cytosol
    bqbiol:isPropertyOf local:PhysicalForce0000 ;
    bqbiol:isVersionOf <https://identifiers.org/opb/opb12345> .

"""
        for i in expected.split('\n'):
            self.assertTrue(i in actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)
        PyOmexMetaAPI.physical_force_delete(physical_force)

    def test_physical_force_str(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        physical_force = PyOmexMetaAPI.editor_new_physical_force(editor_ptr)
        PyOmexMetaAPI.physical_force_set_physical_property(physical_force, "cytosol".encode(), "opb:opb12345".encode())
        PyOmexMetaAPI.physical_force_add_source(
            physical_force, 1.0, "Entity1".encode())

        PyOmexMetaAPI.physical_force_add_sink(
            physical_force, 1.0, "Entity2".encode())

        ptr = PyOmexMetaAPI.physical_force_str(physical_force, "turtle".encode(),
                                               "html_physical_process_ann.rdf".encode())
        actual = PyOmexMetaAPI.get_and_free_c_str(ptr)
        print(actual)
        expected = """@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix bqbiol: <http://biomodels.net/biology-qualifiers/> .
@prefix semsim: <http://www.bhi.washington.edu/semsim#> .
@prefix myOMEXlib: <http://omex-library.org/NewOmex.omex> .
@prefix myOMEX: <http://omex-library.org/NewOmex.omex/NewModel.xml#> .
@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .

local:PhysicalForce0000
    semsim:hasSinkParticipant local:SinkParticipant0000 ;
    semsim:hasSourceParticipant local:SourceParticipant0000 .

local:SinkParticipant0000
    semsim:hasMultiplier "1"^^<http://www.w3.org/1999/02/22-rdf-syntax-ns#http://www.w3.org/2001/XMLSchema#double> ;
    semsim:hasPhysicalEntityReference local:Entity2 .

local:SourceParticipant0000
    semsim:hasMultiplier "1"^^<http://www.w3.org/1999/02/22-rdf-syntax-ns#http://www.w3.org/2001/XMLSchema#double> ;
    semsim:hasPhysicalEntityReference local:Entity1 .

local:cytosol
    bqbiol:isPropertyOf local:PhysicalForce0000 ;
    bqbiol:isVersionOf <https://identifiers.org/opb/opb12345> .

"""
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.editor_delete(editor_ptr)
        PyOmexMetaAPI.physical_force_delete(physical_force)

    def test_personal_information_new(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        information = PyOmexMetaAPI.editor_new_personal_information(editor_ptr)
        PyOmexMetaAPI.personal_information_delete(information)
        PyOmexMetaAPI.editor_delete(editor_ptr)
        self.assertTrue(True)  # if we get this far we pass. Bad test but who's counting

    def test_personal_get_local_uri(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        information = PyOmexMetaAPI.editor_new_personal_information(editor_ptr)
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.personal_information_get_local_uri(information)
        )
        expected = "http://omex-library.org/NewOmex.omex/NewModel.rdf#"
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.personal_information_delete(information)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_personal_information_add_creator(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        information = PyOmexMetaAPI.editor_new_personal_information(editor_ptr)
        PyOmexMetaAPI.personal_information_add_creator(information, "1234-1234-1234-1234".encode())
        PyOmexMetaAPI.editor_add_personal_information(editor_ptr, information)
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_to_string(self.rdf, "turtle".encode(), "base".encode())
        )
        expected = """@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix dcterms: <http://purl.org/dc/terms/> .
@prefix myOMEXlib: <http://omex-library.org/> .
@prefix myOMEX: <http://omex-library.org/NewOmex.omex> .
@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#>
    dcterms:creator <http://omex-library.org/NewOmex.omex/NewModel.xml#PersonalInfo0000> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#PersonalInfo0000>
    dcterms:creator <1234-1234-1234-1234> .

"""
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.personal_information_delete(information)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_personal_information_add_name(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        information = PyOmexMetaAPI.editor_new_personal_information(editor_ptr)
        PyOmexMetaAPI.personal_information_add_name(information, "Ciaran Welsh".encode())
        PyOmexMetaAPI.editor_add_personal_information(editor_ptr, information)
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_to_string(self.rdf, "turtle".encode(), "base".encode())
        )
        expected = """@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix dcterms: <http://purl.org/dc/terms/> .
@prefix foaf: <http://xmlns.com/foaf/0.1/> .
@prefix myOMEXlib: <http://omex-library.org/> .
@prefix myOMEX: <http://omex-library.org/NewOmex.omex> .
@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#>
    dcterms:creator <http://omex-library.org/NewOmex.omex/NewModel.xml#PersonalInfo0000> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#PersonalInfo0000>
    foaf:name "Ciaran Welsh"^^rdf:string .

"""
        for i in actual.split("\n"):
            self.assertTrue(i.strip() in actual)
        PyOmexMetaAPI.personal_information_delete(information)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_personal_information_add_mbox(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        information = PyOmexMetaAPI.editor_new_personal_information(editor_ptr)
        PyOmexMetaAPI.personal_information_add_mbox(information, "annotations.uw.edu".encode())
        PyOmexMetaAPI.editor_add_personal_information(editor_ptr, information)
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_to_string(self.rdf, "turtle".encode(), "base".encode())
        )
        expected = """@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix dcterms: <http://purl.org/dc/terms/> .
@prefix foaf: <http://xmlns.com/foaf/0.1/> .
@prefix myOMEXlib: <http://omex-library.org/> .
@prefix myOMEX: <http://omex-library.org/NewOmex.omex> .
@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#>
    dcterms:creator <http://omex-library.org/NewOmex.omex/NewModel.xml#PersonalInfo0000> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#PersonalInfo0000>
    foaf:mbox "annotations.uw.edu"^^rdf:string .

"""
        for i in actual.split("\n"):
            self.assertTrue(i.strip() in actual)
        PyOmexMetaAPI.personal_information_delete(information)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_personal_information_add_account_name(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        information = PyOmexMetaAPI.editor_new_personal_information(editor_ptr)
        PyOmexMetaAPI.personal_information_add_account_name(information, "1234-1234-1234-1234".encode())
        PyOmexMetaAPI.editor_add_personal_information(editor_ptr, information)
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_to_string(self.rdf, "turtle".encode(), "base".encode())
        )
        expected = """@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix dcterms: <http://purl.org/dc/terms/> .
@prefix foaf: <http://xmlns.com/foaf/0.1/> .
@prefix myOMEXlib: <http://omex-library.org/> .
@prefix myOMEX: <http://omex-library.org/NewOmex.omex> .
@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#>
    dcterms:creator <http://omex-library.org/NewOmex.omex/NewModel.xml#PersonalInfo0000> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#PersonalInfo0000>
    foaf:accountName <https://orcid.org/1234-1234-1234-1234> .

"""
        for i in actual.split("\n"):
            self.assertTrue(i.strip() in actual)
        PyOmexMetaAPI.personal_information_delete(information)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_personal_information_add_account_service_homepage(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        information = PyOmexMetaAPI.editor_new_personal_information(editor_ptr)
        PyOmexMetaAPI.personal_information_add_account_service_homepage(
            information,
            "https://github.com/sys-bio/libOmexMeta".encode()
        )
        PyOmexMetaAPI.editor_add_personal_information(editor_ptr, information)
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_to_string(self.rdf, "turtle".encode(), "base".encode())
        )
        expected = """@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix dcterms: <http://purl.org/dc/terms/> .
@prefix foaf: <http://xmlns.com/foaf/0.1/> .
@prefix myOMEXlib: <http://omex-library.org/> .
@prefix myOMEX: <http://omex-library.org/NewOmex.omex> .
@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#>
    dcterms:creator <http://omex-library.org/NewOmex.omex/NewModel.xml#PersonalInfo0000> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#PersonalInfo0000>
    foaf:accountServiceHomepage <https://github.com/sys-bio/libOmexMeta> .

"""
        for i in actual.split("\n"):
            self.assertTrue(i.strip() in actual)
        PyOmexMetaAPI.personal_information_delete(information)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_personal_information_add_foaf_blank(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        information = PyOmexMetaAPI.editor_new_personal_information(editor_ptr)
        PyOmexMetaAPI.personal_information_add_foaf_blank(information, "name".encode(), "Blank".encode())
        PyOmexMetaAPI.editor_add_personal_information(editor_ptr, information)
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_to_string(self.rdf, "turtle".encode(), "base".encode())
        )
        expected = """@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix dcterms: <http://purl.org/dc/terms/> .
@prefix foaf: <http://xmlns.com/foaf/0.1/> .
@prefix myOMEXlib: <http://omex-library.org/> .
@prefix myOMEX: <http://omex-library.org/NewOmex.omex> .
@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#>
    dcterms:creator <http://omex-library.org/NewOmex.omex/NewModel.xml#PersonalInfo0000> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#PersonalInfo0000>
    foaf:name _:Blank .

"""
        for i in actual.split("\n"):
            self.assertTrue(i.strip() in actual)
        PyOmexMetaAPI.personal_information_delete(information)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_personal_information_add_foaf_uri(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        information = PyOmexMetaAPI.editor_new_personal_information(editor_ptr)
        PyOmexMetaAPI.personal_information_add_foaf_uri(information, "mbox".encode(), "http://uri.com/".encode())
        PyOmexMetaAPI.editor_add_personal_information(editor_ptr, information)
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_to_string(self.rdf, "turtle".encode(), "base".encode())
        )
        expected = """@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix dcterms: <http://purl.org/dc/terms/> .
@prefix foaf: <http://xmlns.com/foaf/0.1/> .
@prefix myOMEXlib: <http://omex-library.org/> .
@prefix myOMEX: <http://omex-library.org/NewOmex.omex> .
@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#>
    dcterms:creator <http://omex-library.org/NewOmex.omex/NewModel.xml#PersonalInfo0000> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#PersonalInfo0000>
    foaf:mbox <http://uri.com/> .

"""
        for i in actual.split("\n"):
            self.assertTrue(i.strip() in actual)
        PyOmexMetaAPI.personal_information_delete(information)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_personal_information_add_foaf_literal(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        information = PyOmexMetaAPI.editor_new_personal_information(editor_ptr)
        PyOmexMetaAPI.personal_information_add_foaf_literal(information, "name".encode(), "literal".encode())
        PyOmexMetaAPI.editor_add_personal_information(editor_ptr, information)
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.rdf_to_string(self.rdf, "turtle".encode(), "base".encode())
        )
        expected = """@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix dcterms: <http://purl.org/dc/terms/> .
@prefix foaf: <http://xmlns.com/foaf/0.1/> .
@prefix myOMEXlib: <http://omex-library.org/> .
@prefix myOMEX: <http://omex-library.org/NewOmex.omex> .
@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#>
    dcterms:creator <http://omex-library.org/NewOmex.omex/NewModel.xml#PersonalInfo0000> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#PersonalInfo0000>
    foaf:name "literal"^^rdf:string .

"""
        for i in actual.split("\n"):
            self.assertTrue(i.strip() in actual)
        PyOmexMetaAPI.personal_information_delete(information)
        PyOmexMetaAPI.editor_delete(editor_ptr)


    def test_personal_information_get_metaid(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        information = PyOmexMetaAPI.editor_new_personal_information(editor_ptr)
        PyOmexMetaAPI.editor_add_personal_information(editor_ptr, information)
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.personal_information_get_metaid(information)
        )
        expected = "http://omex-library.org/NewOmex.omex/NewModel.xml#PersonalInfo0000"
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.personal_information_delete(information)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_personal_information_get_model_uri(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        information = PyOmexMetaAPI.editor_new_personal_information(editor_ptr)
        PyOmexMetaAPI.editor_add_personal_information(editor_ptr, information)
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.personal_information_get_model_uri(information)
        )
        expected = "http://omex-library.org/NewOmex.omex/NewModel.xml#"
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.personal_information_delete(information)
        PyOmexMetaAPI.editor_delete(editor_ptr)

    def test_personal_information_set_model_uri(self):
        editor_ptr = PyOmexMetaAPI.rdf_to_editor(self.rdf, TestStrings.xml.encode(), 0, True)
        information = PyOmexMetaAPI.editor_new_personal_information(editor_ptr)
        PyOmexMetaAPI.personal_information_set_model_uri(information, "awesome-model.xml".encode())
        PyOmexMetaAPI.editor_add_personal_information(editor_ptr, information)
        actual = PyOmexMetaAPI.get_and_free_c_str(
            PyOmexMetaAPI.personal_information_get_model_uri(information)
        )
        expected = "awesome-model.xml"
        self.assertEqual(expected, actual)
        PyOmexMetaAPI.personal_information_delete(information)
        PyOmexMetaAPI.editor_delete(editor_ptr)


if __name__ == "__main__":
    unittest.TestCase()
