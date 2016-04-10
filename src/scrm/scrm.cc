/*
 * scrm is an implementation of the Sequential-Coalescent-with-Recombination Model.
 * 
 * Copyright (C) 2013, 2014 Paul R. Staab, Sha (Joe) Zhu, Dirk Metzler and Gerton Lunter
 * 
 * This file is part of scrm.
 * 
 * scrm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <iostream>
#include <ctime>
#include <memory>

#include "param.h"
#include "forest.h"
#include "random/random_generator.h"
#include "random/mersenne_twister.h"


#ifndef UNITTEST
int main(int argc, char *argv[]){
  try {
    // Organize output
    std::ostream *output = &std::cout;

    // Parse command line arguments
    Param user_para(argc, argv);
    Model model = user_para.parse();
    output->precision(user_para.precision());

    // Print help if user asked for it
    if (user_para.help()) {
      user_para.printHelp(*output); 
      return EXIT_SUCCESS;
    }
    if (user_para.version()) {
      *output << "scrm " << VERSION << std::endl;
      return EXIT_SUCCESS;
    }

    MersenneTwister rg(user_para.seed_is_set(), user_para.random_seed());
    *output << user_para << std::endl;
    *output << rg.seed() << std::endl;

    if (user_para.print_model()) {
      *output << model << std::endl;
    }

    // Create the forest
    Forest forest = Forest(&model, &rg);

    // Loop over the independent loci/chromosomes
    for (size_t rep_i=0; rep_i < model.loci_number(); ++rep_i) {
      // Mark the start of a new independent sample
      *output << std::endl << "//" << std::endl;

      // Now set up the ARG, and sample the initial tree
      if ( user_para.read_init_genealogy() )
        forest.readNewick ( user_para.init_genealogy[ rep_i % user_para.init_genealogy.size()] );
      else forest.buildInitialTree();
      forest.printSegmentSumStats(*output);

      while (forest.next_base() < model.loci_length()) { 
        // Sample next genealogy
        forest.sampleNextGenealogy();
        forest.printSegmentSumStats(*output);
      }
      assert(forest.next_base() == model.loci_length());

      forest.printLocusSumStats(*output);
      forest.clear();
    }

    return EXIT_SUCCESS;
  }

  catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    std::cerr << "Try 'scrm --help' for more information." << std::endl;
    return EXIT_FAILURE;
  }
}

#endif

