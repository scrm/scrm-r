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

#ifndef scrm_param
#define scrm_param

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <random>

#include "model.h"
#include "summary_statistics/summary_statistic.h"
#include "summary_statistics/tmrca.h"
#include "summary_statistics/seg_sites.h"
#include "summary_statistics/frequency_spectrum.h"
#include "summary_statistics/newick_tree.h"
#include "summary_statistics/oriented_forest.h"

class Param {
 public:
 #ifdef UNITTEST
  friend class TestParam;
 #endif

  // Constructors
  Param() : argc_(0), argv_(NULL) { init(); }
  Param(const std::string &arg);
  Param(int argc, char *argv[], bool directly_called=true) : 
    argc_(argc), argv_(argv), directly_called_(directly_called) {
    init();
  }

  ~Param() {
    for (size_t i = 0; i < argv_vec_.size(); ++i) {
      delete[] argv_vec_[i];
    }
  }
 
  /** Move Operator */
  Param(Param&& other) {
    argc_ = other.argc_;
    argc_i = other.argc_i;
    argv_ = other.argv_;
    random_seed_ = other.random_seed_;  
    directly_called_ = other.directly_called_;
    help_ = other.help_;
    version_ = other.version_;
    std::swap(argv_vec_, other.argv_vec_);
  }

  /** Copy Assignment Operator */
  Param& operator=(Param other) {
    argc_ = other.argc_;
    argc_i = other.argc_i;
    argv_ = other.argv_;
    random_seed_ = other.random_seed_;  
    directly_called_ = other.directly_called_;
    help_ = other.help_;
    version_ = other.version_;
    std::swap(argv_vec_, other.argv_vec_);
    return *this;
  }

  void init() {
    this->seed_set_ = false;
    this->random_seed_ = 0;
    this->set_help(false);
    this->set_version(false);
    this->set_precision(6);
    argc_i = 0;
  }

  // Getters and setters
  bool help() const { return help_; }
  bool version() const { return version_; }
  bool read_init_genealogy() const { return this->read_init_genealogy_; }
  size_t random_seed() const { return random_seed_; }
  void set_random_seed(const size_t seed) { 
    this->random_seed_ = seed;
    this->seed_set_ = true; 
  }
  size_t precision() const { return precision_; }
  void set_precision ( const size_t p ) { this->precision_ = p; }
  bool seed_is_set() const { return this->seed_set_; }

  // Other methods
  void printHelp(std::ostream& stream);

  friend std::ostream& operator<< (std::ostream& stream, const Param& param);

  void parse(Model &model);

  template<class T>
  T readNextInput() {
    ++argc_i;

    if (argc_i >= argc_) {
      throw std::invalid_argument(std::string("Not enough parameters when parsing options: ") + argv_[argc_i-1]);
    }

    T input;
    std::stringstream ss(argv_[argc_i]);
    ss >> input;
    if (ss.fail()) {
      throw std::invalid_argument(std::string("Failed to parse option: ") + argv_[argc_i]);
    }

    return input;
  }

  // Read to double first and then cast to int to support scientific notation
  size_t readNextInt() {
    return readNextInput<double>();
  }


  std::vector < std::string > init_genealogy;

 private:
  Param(const Param &other);
  
  void set_help(const bool help) { this->help_ = help; } 
  void set_version(const bool version) { this->version_ = version; } 

  int argc_;
  int argc_i;
  char * const* argv_;
  size_t seed_set_;
  size_t random_seed_;
  size_t precision_;
  bool directly_called_;
  bool help_;
  bool version_;
  bool read_init_genealogy_;
  std::vector<char*> argv_vec_;
};
#endif
