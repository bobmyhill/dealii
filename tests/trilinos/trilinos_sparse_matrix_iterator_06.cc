//----------------------------  trilinos_sparse_matrix_iterator_06.cc  ---------------------------
//    $Id$
//    Version: $Name$
//
//    Copyright (C) 2004, 2005, 2012 by the deal.II authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//----------------------------  trilinos_sparse_matrix_iterator_06.cc  ---------------------------

// test setting some elements using a non-const matrix iterator and operator-=,
// and reading them back through the matrix itself

#include "../tests.h"
#include <deal.II/lac/trilinos_sparsity_pattern.h>
#include <deal.II/lac/trilinos_sparse_matrix.h>
#include <fstream>
#include <iomanip>


void test ()
{
  TrilinosWrappers::SparsityPattern sp (5,5,3);
  for (unsigned int i=0; i<5; ++i)
    for (unsigned int j=0; j<5; ++j)
      if (((i+2*j+1) % 3 == 0)
          ||
          (i==j))
        sp.add (i,j);
  sp.compress ();

  TrilinosWrappers::SparseMatrix m(sp);
  for (unsigned int i=0; i<5; ++i)
    for (unsigned int j=0; j<5; ++j)
      if (((i+2*j+1) % 3 == 0)
          ||
          (i==j))
        m.set(i,j,1.);

  TrilinosWrappers::SparseMatrix::iterator i = m.begin();
  for (; i!=m.end(); ++i)
    i->value() -= i->row()*i->column();

  for (unsigned int i=0; i<5; ++i)
    for (unsigned int j=0; j<5; ++j)
      if (((i+2*j+1) % 3 == 0)
          ||
          (i==j))
        {
          deallog << i << ' ' << j << ' ' << m.el(i,j)
                  << std::endl;
          Assert (std::fabs(m.el(i,j)-(1.-i*j)) < 1e-14,
                  ExcInternalError());
        }

  deallog << "OK" << std::endl;
}



int main (int argc, char **argv)
{
  std::ofstream logfile("trilinos_sparse_matrix_iterator_06/output");
  deallog.attach(logfile);
  deallog.depth_console(0);
  deallog.threshold_double(1.e-10);

  Utilities::MPI::MPI_InitFinalize mpi_initialization (argc, argv);

  try
    {
      test ();
    }
  catch (std::exception &exc)
    {
      deallog << std::endl << std::endl
		<< "----------------------------------------------------"
		<< std::endl;
      deallog << "Exception on processing: " << std::endl
		<< exc.what() << std::endl
		<< "Aborting!" << std::endl
		<< "----------------------------------------------------"
		<< std::endl;

      return 1;
    }
  catch (...)
    {
      deallog << std::endl << std::endl
		<< "----------------------------------------------------"
		<< std::endl;
      deallog << "Unknown exception!" << std::endl
		<< "Aborting!" << std::endl
		<< "----------------------------------------------------"
		<< std::endl;
      return 1;
    };
}