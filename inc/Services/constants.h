#pragma once

#include <string>

#ifndef PROJECT_DATA_DIR
#define PROJECT_DATA_DIR ""
#endif

const std::string DATA_DIR = std::string(PROJECT_DATA_DIR);

const std::string FILE_NAME_INITIAL_INDIVIDUAL = DATA_DIR + "initial_individual.txt";

const std::string FILE_NAME_INDIVIDUAL = DATA_DIR + "final_individual.txt";
const std::string FILE_NAME_INDIVIDUAL_VALUES = DATA_DIR + "individual_values.csv";

const std::string FILE_NAME_ALGORITHM_SETTINGS = DATA_DIR + "algorithm_settings.txt";

const int LOWER_BOUND = 0;
const int UPPER_BOUND = 1;

const double EPSILON = 0.0000001;
const double EPSILON_STRESS = 10;

const double MINIM_INDIVIDUAL_VALUE = 1.0;