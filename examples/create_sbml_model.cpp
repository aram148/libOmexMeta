# include <semsim/SemSim.h>

# include "sbml/SBMLTypes.h"

# include <iostream>

int main() {
  // create SBML model
  LIBSBML_CPP_NAMESPACE_QUALIFIER SBMLDocument* d =
    new LIBSBML_CPP_NAMESPACE_QUALIFIER SBMLDocument(3, 2);
  LIBSBML_CPP_NAMESPACE_QUALIFIER Model* model = d->createModel();
  model->setId("beta_cell_model");

  LIBSBML_CPP_NAMESPACE_QUALIFIER Compartment* comp = model->createCompartment();
  comp->setId("cytosol");
  comp->setSize(1);
  comp->setConstant(true);

  // create the species for glucose
  LIBSBML_CPP_NAMESPACE_QUALIFIER Species* s = model->createSpecies();
  s->setCompartment("cytosol");
  s->setId("glucose");
  s->setInitialAmount(0);

  // create import reaction for glucose
  LIBSBML_CPP_NAMESPACE_QUALIFIER Reaction* reaction = model->createReaction();
  reaction->setId("glucose_import");
  LIBSBML_CPP_NAMESPACE_QUALIFIER SpeciesReference* sr = reaction->createProduct();
  sr->setSpecies("glucose");
  LIBSBML_CPP_NAMESPACE_QUALIFIER KineticLaw* k = reaction->createKineticLaw();
  LIBSBML_CPP_NAMESPACE_QUALIFIER Parameter* p = k->createParameter();
  p->setId("glucose_import_rate");
  p->setValue(1);
  k->setMath(LIBSBML_CPP_NAMESPACE_QUALIFIER SBML_parseL3FormulaWithModel("glucose_import_rate", model));
  // alternate way to set formula
  // LIBSBML_CPP_NAMESPACE_QUALIFIER ASTNode* a =
  //   new LIBSBML_CPP_NAMESPACE_QUALIFIER ASTNode(LIBSBML_CPP_NAMESPACE_QUALIFIER AST_NAME);
  // a->setName("glucose_import_rate");
  // k->setMath(a);

  LIBSBML_CPP_NAMESPACE_QUALIFIER SBMLWriter sbml_writer;

  std::cerr << "SBML output:\n" << sbml_writer.writeSBMLToString(d) << "\n";

  return 0;
}
