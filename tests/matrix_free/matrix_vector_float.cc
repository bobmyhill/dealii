//------------------  matrix_vector_float.cc  ------------------------
//    $Id$
//    Version: $Name$
//
//------------------  matrix_vector_float.cc  ------------------------


// Same as matrix_vector_06 for testing correctness of matrix-vector products
// compared with deal.II sparse matrices, but using floats instead of
// doubles. this tests whether everything is alright for longer vector lengths
// as well

#include "../tests.h"

std::ofstream logfile("matrix_vector_float/output");

#include "create_mesh.h"
#include "matrix_vector_common.h"


template <int dim, int fe_degree>
void test ()
{
  Triangulation<dim> tria;
  create_mesh (tria);
  typename Triangulation<dim>::active_cell_iterator
    cell = tria.begin_active (),
    endc = tria.end();
  for (; cell!=endc; ++cell)
    if (cell->center().norm()<1e-8)
      cell->set_refine_flag();
  tria.execute_coarsening_and_refinement();
  cell = tria.begin_active ();
  for (; cell!=endc; ++cell)
    if (cell->center().norm()<0.2)
      cell->set_refine_flag();
  tria.execute_coarsening_and_refinement();
  if (dim < 3 || fe_degree < 2)
    tria.refine_global(1);
  tria.begin(tria.n_levels()-1)->set_refine_flag();
  tria.last()->set_refine_flag();
  tria.execute_coarsening_and_refinement();
  cell = tria.begin_active ();
  for (unsigned int i=0; i<10-3*dim; ++i)
    {
      cell = tria.begin_active ();
      unsigned int counter = 0;
      for (; cell!=endc; ++cell, ++counter)
        if (counter % (7-i) == 0)
          cell->set_refine_flag();
      tria.execute_coarsening_and_refinement();
    }

  FE_Q<dim> fe (fe_degree);
  DoFHandler<dim> dof (tria);
  dof.distribute_dofs(fe);
  ConstraintMatrix constraints;
  DoFTools::make_hanging_node_constraints(dof, constraints);
  constraints.close();

  deallog.threshold_double (5e-6);
  do_test<dim, fe_degree, float> (dof, constraints);
}